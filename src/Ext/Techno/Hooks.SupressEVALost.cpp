#include "Body.h"

#include <Ext/TechnoType/Body.h>
#include <Ext/WarheadType/Body.h>

#include <BuildingClass.h>
#include <HouseClass.h>
#include <VoXClass.h>

/*
* https://ares-developers.github.io/Ares-docs/new/warheads/unitlost.html?highlight=suppress
*/
DEFINE_HOOK(44D760, BuildingClass_Destroyed_UnitLost, 7)
{
	GET(BuildingClass*, pThis, ECX);

	auto pType = pThis->Type;

	if (!pType->Insignificant && !pType->DontScore)
	{
		auto pOwner = pThis->Owner;
		if (pOwner && pOwner->IsPlayerControl())
		{
			auto pTechnoTypeExt = TechnoTypeExt::ExtMap.Find(pType);
			auto pTechnoExt = TechnoExt::ExtMap.Find(pThis);
			int lostIdx = pTechnoTypeExt->EVA_UnitLost.Get();

			if (lostIdx >= 0 && !pTechnoExt->SupressLostEva)
			{
				VoxClass::PlayIndex(lostIdx);
			}
		}
	}

	return 0;
}

// [WH] UnitLost.Suppress=
DEFINE_HOOK(702050, TechnoClass_ReceiveDamage_SuppressUnitLost, 6)
{
	GET(TechnoClass*, pThis, ESI);
	GET_STACK(WarheadTypeClass*, pWarhead, 0xD0);

	auto pWarheadExt = WarheadTypeExt::ExtMap.Find(pWarhead);
	auto pTechExt = TechnoExt::ExtMap.Find(pThis);

	if (pWarheadExt->Supress_LostEva.Get())
		pTechExt->SupressLostEva = true;

	return 0x0;
}

// bugfix #379: Temporal friendly kills give veterancy
// bugfix #1266: Temporal kills gain double experience
DEFINE_HOOK(71A917, TemporalClass_Update_Erase, 5)
{
	GET(TemporalClass*, pThis, ESI);
	auto pOwner = pThis->Owner;
	auto pOwnerExt = TechnoExt::ExtMap.Find(pOwner);
	auto pWarhead = pOwner->GetWeapon(pOwnerExt->idxSlot_Warp)->WeaponType->Warhead;
	auto pWarheadExt = WarheadTypeExt::ExtMap.Find(pWarhead);

	if (pWarheadExt->Supress_LostEva.Get())
		pOwnerExt->SupressLostEva = true;

	return 0x71A97D;
}