#include <InfantryClass.h>
#include <IonBlastClass.h>
#include <ScenarioClass.h>
#include <WeaponTypeClass.h>
#include <HouseTypeClass.h>
#include <HouseClass.h>
#include <SideClass.h>
#include "Body.h"
#include "../Techno/Body.h"
#include "../Bullet/Body.h"
#include "../WeaponType/Body.h"
#include "../../Enum/ArmorTypes.h"

// feature #384: Permanent MindControl Warheads + feature #200: EMP Warheads
// attach #407 here - set TechnoClass::Flashing.Duration // that doesn't exist, according to yrpp::TechnoClass.h::struct FlashData
// attach #561 here, reuse #407's additional hooks for colouring
DEFINE_HOOK(46920B, BulletClass_Detonate, 6) {
	GET(BulletClass* const, pThis, ESI);
	GET_BASE(const CoordStruct* const, pCoordsDetonation, 0x8);

	auto const pWarhead = pThis->WH;
	auto const pWHExt = WarheadTypeExt::ExtMap.Find(pWarhead);

	auto const pOwnerHouse = pThis->Owner ? pThis->Owner->Owner : nullptr;

	// this snapping stuff does not belong here. it should go into BulletClass::Fire
	auto coords = *pCoordsDetonation;
	auto snapped = false;

	static auto const SnapDistance = 64;
	if(pThis->Target && pThis->DistanceFrom(pThis->Target) < SnapDistance) {
		coords = pThis->Target->GetCoords();
		snapped = true;
	}

	// these effects should be applied no matter what happens to the target
	pWHExt->applyRipples(coords);

	bool targetStillOnMap = true;
	if(snapped) {
		if(auto const pWeaponExt = WeaponTypeExt::ExtMap.Find(pThis->WeaponType)) {
			targetStillOnMap = !pWeaponExt->conductAbduction(pThis);
		}
	}

	// if the target gets abducted, there's nothing there to apply IC, EMP, etc. to
	// mind that conductAbduction() neuters the bullet, so if you wish to change
	// this check, you have to fix that as well
	if(targetStillOnMap) {
		auto const damage = pThis->WeaponType ? pThis->WeaponType->Damage : 0;
		pWHExt->applyIronCurtain(coords, pOwnerHouse, damage);
		pWHExt->applyEMP(coords, pThis->Owner);
		pWHExt->applyAttachedEffect(coords, pThis->Owner);

		if(snapped) {
			WarheadTypeExt::applyOccupantDamage(pThis);
			pWHExt->applyKillDriver(pThis->Owner, pThis->Target);
		}
	}

	return pWHExt->applyPermaMC(pOwnerHouse, pThis->Target) ? 0x469AA4u : 0u;
}

// issue 472: deglob WarpAway
DEFINE_HOOK(71A900, TemporalClass_Update_WarpAway, 6) {
	auto pData = WarheadTypeExt::ExtMap.Find(WarheadTypeExt::Temporal_WH);
	R->EDX<AnimTypeClass *>(pData->Temporal_WarpAway.Get(RulesClass::Global()->WarpAway));
	return 0x71A906;
}

DEFINE_HOOK(517FC1, InfantryClass_ReceiveDamage_DeployedDamage, 6) {
	GET(InfantryClass *, I, ESI);
	bool IgnoreDefenses = R->BL() != 0;

	if (!I->IsDeployed() || IgnoreDefenses) {
		return 0;
	}
	GET(WarheadTypeClass *, WH, EBP);
	GET(int *, Damage, EDI);

	WarheadTypeExt::ExtData *pData = WarheadTypeExt::ExtMap.Find(WH);

	*Damage = static_cast<int>(*Damage * pData->DeployedDamage);

	// yes, let's make sure the pointer's safe AFTER we've dereferenced it... Failstwood!
	return WH ? 0x517FF9u : 0x518016u;
}
/*
 * Fixing issue #722
 */

DEFINE_HOOK(7384BD, UnitClass_ReceiveDamage_OreMinerUnderAttack, 6)
{
	GET_STACK(WarheadTypeClass *, WH, STACK_OFFS(0x44, -0xC));

	auto pData = WarheadTypeExt::ExtMap.Find(WH);
	return !pData->Malicious ? 0x738535u : 0u;
}

//

DEFINE_HOOK(442974, BuildingClass_ReceiveDamage_Malicious, 6)
{
	GET(BuildingClass*, pBuilding, ESI);
	GET_STACK(WarheadTypeClass*, pWarhead, 0xA8);

	WarheadTypeExt::Malicious_WH = pWarhead;
	pBuilding->BuildingUnderAttack();
	WarheadTypeExt::Malicious_WH = nullptr;

	return 0x442980;
}

DEFINE_HOOK(44266B, BuildingClass_ReceiveDamage_Destroyed, 6)
{
	GET(BuildingClass*, pBuilding, ESI);
	GET(TechnoClass*, pDestroyer, EBP);

	pBuilding->Destroyed(pDestroyer);
	return 0;
}

DEFINE_HOOK(4F94A5, HouseClass_BuildingUnderAttack, 6)
{
	if (auto pWarhead = WarheadTypeExt::Malicious_WH)
		if (auto pData = WarheadTypeExt::ExtMap.Find(pWarhead))
			if (!pData->Malicious)
				return 0x4F95D4;

	return 0;
}

/*
DEFINE_HOOK(4F94A5, HouseClass_BuildingUnderAttack, 6)
{
	GET_STACK(DWORD, Caller, 0x14);
	if(Caller == 0x442980) {
		//Debug::DumpStack(R, 0xF0, 0xA0);
		GET_STACK(WarheadTypeClass *, WH, 0x14 + 0xA4 + 0xC);
		if(auto pData = WarheadTypeExt::ExtMap.Find(WH)) {
			if(!pData->Malicious) {
				return 0x4F95D4;
			}
		}
	}
	return 0;
}*/

DEFINE_HOOK(702669, TechnoClass_ReceiveDamage_SuppressDeathWeapon, 9)
{
	GET(TechnoClass* const, pThis, ESI);
	GET_STACK(WarheadTypeClass* const, pWarhead, STACK_OFFS(0xC4, -0xC));

	auto const pExt = WarheadTypeExt::ExtMap.Find(pWarhead);
	auto const abs = pThis->WhatAmI();

	auto const suppressed =
		(abs == AbstractType::Unit && pExt->SuppressDeathWeapon_Vehicles)
		|| (abs == AbstractType::Infantry && pExt->SuppressDeathWeapon_Infantry)
		|| pExt->SuppressDeathWeapon.Contains(pThis->GetTechnoType());
	
	if(!suppressed) {
		pThis->FireDeathWeapon(0);
	}

	return 0x702672;
}

//ToDo: this
DEFINE_HOOK(701A5C, TechnoClass_ReceiveDamage_IronCurtainFlash, 7)
{
	GET_STACK(WarheadTypeClass* const, pWarhead, 0xD0);
	GET(TechnoClass*, pThis, ESI);

	bool ICFlash = true; //control thru [AudioVisual] / WH itself

	if (!pWarhead || !ICFlash)
		return 0x701A98;

	return pThis->ForceShielded ? 0x701A65 : 0x701A69;
}

/*ToDo: this
DEFINE_HOOK(71AFB2, TemporalClass_Fire_HealthFactor, 5)
{
	GET(TechnoClass*, pTarget, ECX);
	GET(TemporalClass*, pThis, ESI);
	GET(int, nStreght, EAX);

	auto pOwner = pThis->Owner;
	auto pOwnerExt = TechnoExt::ExtMap.Find(pOwner);
	auto pWarhead = pOwner->GetWeapon(pOwnerExt->idxSlot_Warp)->WeaponType->Warhead;
	auto pWarheadExt = WarheadTypeExt::ExtMap.Find(pWarhead);

	auto nCalc = (1.0 - pTarget->Health / pTarget->GetTechnoType()->Strength) * pWarheadExt->Temporal_HealthFactor.Get();
	auto nCalc_b = (1.0 - nCalc) * (10 * nStreght) + nCalc * 0.0;

	R->EAX(nCalc_b <= 1.0 ? 1 : static_cast<int>(nCalc_b));
	return 0x71AFB7;
}
*/

/*
5F53E5 = ObjectClass_ReceiveDamage_Relative, 6
4892BE = DamageArea_NullDamage, 6
489E9F = DamageArea_BridgeAbsoluteDestroyer, 5
489FD8 = DamageArea_BridgeAbsoluteDestroyer2, 6
48A15D = DamageArea_BridgeAbsoluteDestroyer3, 6
48A229 = DamageArea_BridgeAbsoluteDestroyer4, 6
48A283 = DamageArea_BridgeAbsoluteDestroyer5, 6
629BC0 = ParasiteClass_UpdateSquiddy_Culling, 8
5F5456 = ObjectClass_ReceiveDamage_Culling, 6
*/