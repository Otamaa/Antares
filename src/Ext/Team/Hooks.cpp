//#include "Body.h"

#include "../Rules/Body.h"
#include <Ext/HouseType/Body.h>

#include <AircraftClass.h>
#include <YRPPGlobal.h>

// #895225: make the AI smarter. this code was missing from YR.
// it clears the targets and assigns the attacker the team's current focus.
DEFINE_HOOK(6EB432, TeamClass_AttackedBy_Retaliate, 9)
{
	GET(TeamClass*, pThis, ESI);
	GET(AbstractClass*, pAttacker, EBP);

	// get ot if global option is off
	if(!RulesExt::Global()->TeamRetaliate) {
		return 0x6EB47A;
	}

	auto pFocus = abstract_cast<TechnoClass*>(pThis->Focus);
	auto pSpawn = pThis->SpawnCell;

	if(!pFocus || !pFocus->GetWeapon(0)->WeaponType || !pSpawn || pFocus->IsCloseEnoughToAttackCoords(pSpawn->GetCoords())) {
		// disallow aircraft, or units considered as aircraft, or stuff not on map like parasites
		if(pAttacker->WhatAmI() != AircraftClass::AbsID) {
			if(auto pAttackerFoot = abstract_cast<FootClass*>(pAttacker)) {
				if(pAttackerFoot->InLimbo || pAttackerFoot->GetTechnoType()->ConsideredAircraft) {
					return 0x6EB47A;
				}
			}

			pThis->Focus = pAttacker;

			// this is the original code, but commented out because it's responsible for switching
			// targets when the team is attacked by two or more opponents. Now, the team should pick
			// the first target, and keep it. -AlexB
			//for(NextTeamMember i(pThis->FirstUnit); i; ++i) {
			//	if(i->IsAlive && i->Health && (Unsorted::IKnowWhatImDoing || !i->InLimbo)) {
			//		if(i->IsTeamLeader || i->WhatAmI() == AircraftClass::AbsID) {
			//			i->SetTarget(nullptr);
			//			i->SetDestination(nullptr, true);
			//		}
			//	}
			//}
		}
	}

	return 0x6EB47A;
}

DEFINE_HOOK(6EFB69, TeamClass_GatherAtFriendlyBase_Distance, 6)
{
	GET_BASE(ScriptActionNode*, pActionNode, 0x8);
	R->EDX(pActionNode->Argument + RulesGlobal->AISafeDistance);
	return 0x6EFB6F;
}

DEFINE_HOOK(6EF8A1, TeamClass_GatherAtEnemyBase_Distance, 6)
{
	GET_BASE(ScriptActionNode*, pActionNode, 0x8);
	R->EDX(pActionNode->Argument + RulesGlobal->AISafeDistance);
	return 0x6EF8A7;
}

DEFINE_HOOK(65DBB3, TeamTypeClass_CreateInstance_Plane, 5)
{
	GET(FootClass*, pReinforcement, EBP);

	auto pHouseTypeExt = HouseTypeExt::ExtMap.Find(pReinforcement->Owner->Type);
	R->ECX(pHouseTypeExt->GetParadropPlane());
	++Unsorted::IKnowWhatImDoing;
	return 0x65DBD0;
}

/*
namespace AresKillDriverWithoutWH
{
	bool IsDriverKillable(TechnoClass* pVictim, double nMinHealthTresh)
	{
		if (pVictim && pVictim->Health > 0 && pVictim->IsAlive && pVictim->IsOnMap && !pVictim->InLimbo)
		{
			if (auto pVictimUnit = generic_cast<FootClass*>(pVictim))
			{
				if (!Helpers::Alex::is_any_of(pVictim->WhatAmI(), AbstractType::Unit, AbstractType::Aircraft))
					return false;

				auto pVictimExt = TechnoExt::ExtMap.Find(pVictim);

				if (pVictimExt->DriverKilled)
					return false;

				auto pType = pVictimUnit->GetTechnoType();

				if (auto pUnitType = specific_cast<UnitTypeClass*>(pType))
				{
					if (pUnitType->CrateGoodie)
						return false;
				}

				if (auto const pAircraftType = abstract_cast<AircraftTypeClass*>(pType))
				{
					if (pAircraftType->AirportBound)
					{
						return false;
					}
				}

				if (pType->Dock.Count)
					return false;

				if (pVictim->BeingWarpedOut || pVictim->IsIronCurtained() || pVictim->TemporalTargetingMe)
					return false;

				if (auto pCell = pVictim->GetCell())
				{
					if (auto pBuildingBelowMe = pCell->GetBuilding())
					{
						if (pBuildingBelowMe = specific_cast<BuildingClass*>(pVictim->GetNthLink()))
						{
							auto pBuildingBelowMeType = pBuildingBelowMe->Type;
							if (pBuildingBelowMeType->WeaponsFactory && !pBuildingBelowMeType->Naval)
								return false;
						}
					}
				}

				auto pVictimTypeExt = TechnoTypeExt::ExtMap.Find(pType);
				auto const maxKillHealth = pVictimTypeExt->ProtectedDriver ? 0.0 : nMinHealthTresh;

				//Ability Check for KillDriver is exist !

				if (!pType->Natural && !pType->Organic
					&& pVictimUnit->GetHealthPercentage() <= maxKillHealth)
				{
					return true;
				}
			}
		}

		return false;
	}


	bool KillTheDriver(FootClass* pVictim, TechnoClass* pDestroyer, HouseClass* pHouseAfter)
	{
		auto pVictimExt = TechnoExt::ExtMap.Find(pVictim);
		auto const passive = pHouseAfter->Type->MultiplayPassive;
		pVictimExt->DriverKilled = passive;

		// exit if owner would not change
		if (pVictim->Owner == pHouseAfter)
		{
			return false;
		}

		auto pType = pVictim->GetTechnoType();
		auto pVictimTypeExt = TechnoTypeExt::ExtMap.Find(pType);
		auto pTarget = pVictim;
		auto pSource = pDestroyer;

		// If this vehicle uses Operator=, we have to take care of actual "physical" drivers, rather than theoretical ones
		if (pVictimTypeExt->IsAPromiscuousWhoreAndLetsAnyoneRideIt && pVictim->Passengers.GetFirstPassenger())
		{
			// kill first passenger
			auto const pPassenger = pVictim->RemoveFirstPassenger();
			pPassenger->RegisterDestruction(pDestroyer);
			pPassenger->UnInit();

		}
		else if (auto const pOperatorType = pVictimTypeExt->Operator)
		{
			// find the driver cowardly hiding among the passengers, then kill him
			for (NextObject passenger(pVictim->Passengers.GetFirstPassenger()); passenger; ++passenger)
			{
				auto const pPassenger = static_cast<FootClass*>(*passenger);
				if (pPassenger->GetTechnoType() == pOperatorType)
				{
					pVictim->RemovePassenger(pPassenger);
					pPassenger->RegisterDestruction(pDestroyer);
					pPassenger->UnInit();
					break;
				}
			}
		}

		// if passengers remain in the vehicle, operator-using or not, they should leave
		if (pVictim->Passengers.GetFirstPassenger())
		{
			TechnoExt::EjectPassengers(pVictim, -1);
		}

		pTarget->HijackerInfantryType = -1;

		// If this unit is driving under influence, we have to free it first
		if (auto const pController = pTarget->MindControlledBy)
		{
			if (auto const pCaptureManager = pController->CaptureManager)
			{
				pCaptureManager->FreeUnit(pTarget);
			}
		}
		pTarget->MindControlledByAUnit = false;
		pTarget->MindControlledByHouse = nullptr;

		// remove the mind-control ring anim
		if (pTarget->MindControlRingAnim)
		{
			pTarget->MindControlRingAnim->UnInit();
			pTarget->MindControlRingAnim = nullptr;
		}

		// If this unit mind controls stuff, we should free the controllees, since they still belong to the previous owner
		if (pTarget->CaptureManager)
		{
			pTarget->CaptureManager->FreeAll();
		}

		// This unit will be freed of its duties
		if (auto const pFoot = abstract_cast<FootClass*>(pTarget))
		{
			pFoot->LiberateMember();
		}

		// If this unit spawns stuff, we should kill the spawns, since they still belong to the previous owner
		if (auto const pSpawnManager = pTarget->SpawnManager)
		{
			pSpawnManager->KillNodes();
			pSpawnManager->ResetTarget();
		}

		// If this unit enslaves stuff, we should free the slaves, since they still belong to the previous owner
				// <DCoder> SlaveManagerClass::Killed() sets the manager's Owner to NULL
				// <Renegade> okay, does Killed() also destroy the slave manager, or just unlink it from the unit?
				// <DCoder> unlink
				// <Renegade> so on principle, I could just re-link it?
				// <DCoder> yes you can
		if (auto const pSlaveManager = pTarget->SlaveManager)
		{
			pSlaveManager->Killed(pSource);
			pSlaveManager->ZeroOutSlaves();
			pSlaveManager->Owner = pTarget;
			if (passive)
			{
				pSlaveManager->SuspendWork();
			}
			else
			{
				pSlaveManager->ResumeWork();
			}
		}

		// Hand over to a different house
		pTarget->SetOwningHouse(pHouseAfter);

		if (passive)
		{
			pTarget->QueueMission(Mission::Harmless, true);
		}

		pTarget->SetTarget(nullptr);
		pTarget->SetDestination(nullptr, false);
		return true;


		//Raise Tag event for Driver killed !
	}
};

namespace TeamExt
{
	bool Handled(TeamClass* pThis, ScriptActionNode* pNode, int nSomething)
	{
		bool Handled = false;
		auto nActionInt = (int)pNode->Action;
		auto DoGarrison = [&pThis, &nActionInt]()
		{
			if (auto pUnit = pThis->FirstUnit)
			{
				do
				{
					auto pUnitExt = TechnoExt::ExtMap.Find(pUnit);
					pUnitExt->TakeVehicle = nActionInt == 67;
					if (pUnit->GarrisonStructure())
					{
						pUnit->LiberateMember();
					}

					pUnit = pUnit->NextTeamMember;

				} while (pUnit);
			}

			pThis->StepCompleted = true;
			return nActionInt == 67;
		};

		if (nActionInt > 64)
		{
			switch (nActionInt)
			{
			case 65:// Auxiliary Power
			{
				auto pOwner = pThis->Owner;
				auto pOwnerExt = HouseExt::ExtMap.Find(pOwner);
				pOwnerExt->TeamPoweAdd += pNode->Argument;
				pOwner->RecheckPower = true;
				pThis->StepCompleted = true;
				Handled = true;
			}
			break;
			case 66: //Kill Driver 
			{
				if (auto pUnit = pThis->FirstUnit)
				{
					do
					{
						if (AresKillDriverWithoutWH::IsDriverKillable(pUnit, 1.0))
						{
							AresKillDriverWithoutWH::KillTheDriver(pUnit, nullptr, HouseClass::FindSpecial());
						}

						pUnit = pUnit->NextTeamMember;
					} while (pUnit);
				}

				pThis->StepCompleted = true;
				Handled = true;
			}
			break;
			case 67: //Take Vehicles 
			{
				Handled = DoGarrison();
			}
			break;
			case 68: //type change 
			{
				if (auto pUnit = pThis->FirstUnit)
				{
					do
					{
						// Duh not done yet !
						pUnit = pUnit->NextTeamMember;

					} while (pUnit);
				}

				pThis->StepCompleted = true;
				Handled = true;
			}
			break;
			case 69: //SonarReveal 
			{
				if (auto pUnit = pThis->FirstUnit)
				{
					do
					{
						auto pUnitExt = TechnoExt::ExtMap.Find(pUnit);
						int nSonarDur = pNode->Argument;
						if (nSonarDur > 0)
						{
							auto const delay = Math::max(pUnitExt->CloakSkipTimer.GetTimeLeft(), nSonarDur);
							pUnitExt->CloakSkipTimer.Start(delay);

							// this part may implemented on TechnoClass::Ai or somewhere to decloak the unit , hmm 
							if (pUnit->CloakState != CloakState::Uncloaked)
							{
								pUnit->Uncloak(true);
								pUnit->NeedsRedraw = true;
							}
						}
						pUnit = pUnit->NextTeamMember;

					} while (pUnit);
				}

				pThis->StepCompleted = true;
				Handled = true;
			}
			break;
			case 70: //DisableWeapon 
			{
				if (auto pUnit = pThis->FirstUnit)
				{
					do
					{
						auto pUnitExt = TechnoExt::ExtMap.Find(pUnit);
						int nDisableWeaponDuer = pNode->Argument;
						if (nDisableWeaponDuer > 0)
						{
							auto const delay = Math::max(pUnitExt->DisableWeaponTimer.GetTimeLeft(), nDisableWeaponDuer);
							pUnitExt->DisableWeaponTimer.Start(delay);
						}
						pUnit = pUnit->NextTeamMember;

					} while (pUnit);
				}

				pThis->StepCompleted = true;
				Handled = true;
			}
			break;
			default:
				break;
			}
		}
		else
			if (nActionInt == (int)TeamMissionType::YR_garrison_building)
			{
				Handled = DoGarrison();
			}

		return Handled;
	}

	void FireSuperWeaponOverride(TeamClass* pThis, ScriptActionNode* pNode, int nSomething)
	{
		if (auto nLeader = pThis->FetchLeader())
		{
			if (auto pOwner = nLeader->Owner)
			{
				bool bAllowFire = false;
				auto nDrain = pOwner->PowerDrain;
				bool bHadPower = !nDrain || pOwner->PowerOutput >= nDrain;
				SuperWeaponTypeClass* pSWTypeToFire = nullptr;

				for (auto const& pSuperType : *SuperWeaponTypeClass::Array())
				{
					auto pSuperTypeExt = SWTypeExt::ExtMap.Find(pSuperType);
					auto bEligibleTargetingType = pSuperTypeExt->SW_AITargetingType == SuperWeaponAITargetingMode::IronCurtain;
					auto bEligibleGroup = pSuperTypeExt->SW_Group.Get() == pNode->Argument;
					if (!bAllowFire && bEligibleTargetingType && bEligibleGroup)
					{
						auto pSuper = pOwner->Supers[pSuperType->ArrayIndex];
						if (pSuper->IsCharged && bHadPower || !pSuper->IsPowered())
						{
							pSWTypeToFire = pSuperType;
							bAllowFire = true;
							break;
						}

						if (!pSWTypeToFire && pSuper->Granted)
						{
							auto nRechargeTime = pSuper->GetRechargeTime();
							auto nSuperTime = pSuper->RechargeTimer.GetTimeLeft();
							if ((nRechargeTime - nSuperTime) >= RulesGlobal->AIMinorSuperReadyPercent * nRechargeTime)
							{
								pSWTypeToFire = pSuperType;
								bAllowFire = false;
								break;
							}
						}
					}
				}

				if (!pSWTypeToFire)
					goto CompleteStep;

				if (bAllowFire)
				{
					auto nCoords = pThis->SpawnCell->GetCoords();
					auto nCellStruct = CellClass::Coord2Cell(nCoords);
					pOwner->Fire_SW(pSWTypeToFire->ArrayIndex, nCellStruct);
					goto CompleteStep;
				}

			}
		}
	CompleteStep:
		pThis->StepCompleted = true;
	}

}

DEFINE_HOOK(6EFC70, TeamClass_IronCurtain, 5)
{
	GET(TeamClass*, pThis, ECX);
	GET_STACK(ScriptActionNode*, pActionNode, 0x4);
	GET_STACK(int, nSomething, 0x8);

	TeamExt::FireSuperWeaponOverride(pThis, pActionNode, nSomething);
	return 0x6EFE4F;
}

DEFINE_HOOK(6E9443, TeamClass_Update, 8)
{
	GET(TeamClass*, pThis, ESI);
	GET(ScriptActionNode*, nAction, EAX);
	GET_STACK(int, nSomething, 0x10);

	return TeamExt::Handled(pThis, nAction, nSomething) ? 0x6E95AB : 0x0;
}*/