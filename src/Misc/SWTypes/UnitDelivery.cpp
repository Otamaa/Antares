#include "UnitDelivery.h"
#include "../../Ext/Techno/Body.h"

void SW_UnitDelivery::Initialize(SWTypeExt::ExtData *pData, SuperWeaponTypeClass *pSW)
{
	pData->SW_AITargetingType = SuperWeaponAITargetingMode::ParaDrop;
}

void SW_UnitDelivery::LoadFromINI(
	SWTypeExt::ExtData *pData, SuperWeaponTypeClass *pSW, CCINIClass *pINI)
{
	const char * section = pSW->ID;

	if(!pINI->GetSection(section)) {
		return;
	}

	if(pINI->ReadString(section, "Deliver.Types", "", Ares::readBuffer, Ares::readLength)) {
		pData->SW_Deliverables.Clear();
		for(char *cur = strtok(Ares::readBuffer, ","); cur && *cur; cur = strtok(NULL, ",")) {
			TechnoTypeClass * Type = InfantryTypeClass::Find(cur);
			if(!Type) {
				Type = UnitTypeClass::Find(cur);
			}
			if(!Type) {
				Type = AircraftTypeClass::Find(cur);
			}
			if(!Type) {
				Type = BuildingTypeClass::Find(cur);
			}
			if(!Type) {
				Debug::INIParseFailed(section, "Deliver.Types", cur, "Expected valid TechnoType ID.");
			}
			if(Type) {
				pData->SW_Deliverables.AddItem(Type);
			}
		}
	}

	INI_EX exINI(pINI);
	pData->SW_DeliverBuildups.Read(&exINI, section, "Deliver.Buildups");
}

bool SW_UnitDelivery::Launch(SuperClass* pThis, CellStruct* pCoords, byte IsPlayer)
{
	this->newStateMachine(150, *pCoords, pThis);

	return 1;
}

void UnitDeliveryStateMachine::Update() {
	switch(this->TimePassed()) {
	case 1:
		// play anim
		break;
	case 20:
		this->PlaceUnits();
		break;
	case 100:
		// write message
		break;
	};
}

// Replaced my own implementation with AlexB's shown below

//This function doesn't skip any placeable items, no matter how
//they are ordered. Infantry is grouped. Units are moved to the
//center as close as they can. There is an additional fix for a
//glitch: previously, even if a unit could be placed, it would
//have been unloaded again, when it was at index 100.

void UnitDeliveryStateMachine::PlaceUnits() {
	int unitIdx = 0;
	SWTypeExt::ExtData *pData = this->FindExtData();

	if(!pData) {
		return;
	}

	while(unitIdx < pData->SW_Deliverables.Count) {
		TechnoTypeClass * Type = pData->SW_Deliverables[unitIdx];
		TechnoClass * Item = generic_cast<TechnoClass *>(Type->CreateObject(this->Super->Owner));
		BuildingClass * ItemBuilding = specific_cast<BuildingClass *>(Item);

		if(ItemBuilding && pData->SW_DeliverBuildups) {
			ItemBuilding->QueueMission(mission_Construction, false);
		}

		int cellIdx = 0;
		bool Placed = false;
		do {
			CellStruct tmpCell = CellSpread::GetCell(cellIdx) + this->Coords;
			if(MapClass::Instance->CellExists(&tmpCell)) {
				CellClass *cell = MapClass::Instance->GetCellAt(&tmpCell);
				CoordStruct XYZ;
				cell->GetCoordsWithBridge(&XYZ);

				bool validCell = true;
				if(cell->OverlayTypeIndex != -1) {
					// disallow placing on rocks, rubble and walls
					OverlayTypeClass *Overlay = OverlayTypeClass::Array->GetItem(cell->OverlayTypeIndex);
					validCell = !Overlay->Wall && !Overlay->IsARock && !Overlay->IsRubble;
				}
				if(AircraftClass * ItemAircraft = specific_cast<AircraftClass *>(Item)) {
					// for aircraft: cell must be empty: non-water, non-cliff, non-shore, non-anything
					validCell &= !cell->GetContent() && !cell->Tile_Is_Cliff()
						&& !cell->Tile_Is_DestroyableCliff() && !cell->Tile_Is_Shore()
						&& !cell->Tile_Is_Water() && !cell->ContainsBridge();
				}
				if(Type->Naval && validCell) {
					// naval types look stupid on bridges
					validCell = (!cell->ContainsBridge() && cell->LandType != lt_Road)
						|| Type->SpeedType == st_Hover;
				}

				if(validCell) {
					Item->OnBridge = cell->ContainsBridge();
					if((Placed = Item->Put(&XYZ, (cellIdx & 7))) == true) {
						if(ItemBuilding) {
							if (pData->SW_DeliverBuildups) {
								ItemBuilding->UpdateOwner(this->Super->Owner);
								ItemBuilding->unknown_bool_6DD = 1;
							}
						} else {
							if(Type->BalloonHover || Type->JumpJet) {
								Item->Scatter(0xB1CFE8, 1, 0);
							}
						}
						if(TechnoExt::ExtData* pItemData = TechnoExt::ExtMap.Find(Item)) {
							if(!pItemData->IsPowered() || !pItemData->IsOperated()) {
								Item->Deactivate();
								if(ItemBuilding) {
									Item->Owner->ShouldRecheckTechTree = true; 
								}
							}
						}
					}
				}
			}

			++cellIdx;
			if(cellIdx >= 100) { // 100 cells should be enough for any sane delivery
				cellIdx = 0;
				if(!Placed) {
					Item->UnInit();
				}
				break;
			}
		} while(!Placed);
		++unitIdx;
	}
}
