#include <TemporalClass.h>
#include <Ext/Building/Body.h>
#include <Ext/Techno/Body.h>

#include <UnitClass.h>

namespace CanThisWarped
{
	bool IsFresh(TechnoClass* pThis, bool bIncludeNaval = true)
	{
		// if this is a newly produced unit that still is in its
		// weapons factory, this skips it.
		if (pThis)
		{
			if (pThis->WhatAmI() == AbstractType::Unit) {
				if (auto const pLinkBld = abstract_cast<BuildingClass*>(pThis->GetNthLink(0))) {
					if (pThis->GetCell()->GetBuilding() == pLinkBld) {
						if (pLinkBld->Type->WeaponsFactory || (bIncludeNaval && pLinkBld->Type->Naval)) {
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	bool Answer(TechnoClass* pTarget)
	{
		if (!pTarget)
			return false;

		if (pTarget->IsIronCurtained())
			return false;

		if (!pTarget->GetTechnoType()->Warpable)
			return false;

		// check ability here !
		// return false

		if (auto pBuilding = specific_cast<BuildingClass*>(pTarget))
			if (auto pExt = BuildingExt::ExtMap.Find(pBuilding))
				if (pExt->AboutToChronoshift)
					return false;

		if (IsFresh(pTarget, true))
			return false;

		return true;
	}
};

DEFINE_HOOK(71AE50, TemporalClass_CanWarpTarget, 8)
{
	GET_STACK(TechnoClass*, pTarget, 0x4);
	R->EAX(CanThisWarped::Answer(pTarget));
	return 0x71AF19;
}

/*
// removed ? -Otamaa
// temporal per-slot
DEFINE_HOOK(71AB30, TemporalClass_GetHelperDamage, 5)
{
	GET(TemporalClass *, Temp, ESI);
	TechnoClass *T = Temp->Owner;
	TechnoExt::ExtData *pData = TechnoExt::ExtMap.Find(T);
	WeaponStruct *W = T->GetWeapon(pData->idxSlot_Warp);
	WarheadTypeExt::Temporal_WH = W->WeaponType->Warhead;
	R->EAX<WeaponStruct *>(W);
	return 0x71AB47;
}

*/
namespace WarpPerStep
{
	int Get(TemporalClass* pThis, int nStep)
	{
		int nAddStep = 0;

		if (!pThis)
			return 0;

		do
		{
			if (nStep >= 50)
				break;

			++nStep;
			auto pOwner = pThis->Owner;
			auto pOwnerExt = TechnoExt::ExtMap.Find(pOwner);
			auto pWeapon = pOwner->GetWeapon(pOwnerExt->idxSlot_Warp)->WeaponType;
			nAddStep += pWeapon->Damage;
			pThis->WarpPerStep = pWeapon->Damage;
			pThis = pThis->PrevTemporal;

		} while (pThis);

		return nAddStep;
	}
};

DEFINE_HOOK(71AB10, TemporalClass_GetWarpPerStep, 6)
{
	GET_STACK(int, nStep, 0x4);
	GET(TemporalClass*, pThis, ESI);
	R->EAX(WarpPerStep::Get(pThis, nStep));
	return 0x71AB57;
}