#include <BombListClass.h>

#include "Body.h"
#include "../Techno/Body.h"

#include <Helpers/Iterators.h>
#include <BulletClass.h>
#include <WarheadTypeClass.h>
#include <BuildingClass.h>
#include <HouseClass.h>

// custom ivan bomb attachment
// bugfix #385: Only InfantryTypes can use Ivan Bombs
DEFINE_HOOK(438E86, BombListClass_Plant_AllTechnos, 5)
{
	GET(TechnoClass*, Source, EBP);
	switch (Source->WhatAmI()) {
	case AbstractType::Aircraft:
	case AbstractType::Infantry:
	case AbstractType::Unit:
	case AbstractType::Building:
		return 0x438E97;
	default:
		return 0x439022;
	}
}

DEFINE_HOOK(438FD7, BombListClass_Plant_AttachSound, 7)
{
	return 0x439022;
}

DEFINE_HOOK(438A00, BombClass_GetCurrentFrame, 6)
{
	GET(BombClass*, pThis, ECX);

	auto pData = WeaponTypeExt::BombExt.get_or_default(pThis);
	if (!pData) {
		return 0;
	}

	SHPStruct* pSHP = pData->Ivan_Image.Get(RulesClass::Instance->BOMBCURS_SHP);
	int frame = 0;

	if (pSHP->Frames >= 2) {
		if (pThis->Type == BombType::NormalBomb) {
			// -1 so that last iteration has room to flicker. order is important
			int delay = pData->Ivan_Delay.Get(RulesClass::Instance->IvanTimedDelay);
			int lifetime = (Unsorted::CurrentFrame - pThis->PlantingFrame);
			frame = lifetime / (delay / (pSHP->Frames - 1));

			// flicker over a time period
			int rate = pData->Ivan_FlickerRate.Get(RulesClass::Instance->IvanIconFlickerRate);
			int period = 2 * rate;
			if (Unsorted::CurrentFrame % period >= rate) {
				++frame;
			}

			if (frame >= pSHP->Frames) {
				frame = pSHP->Frames - 1;
			}
			else if (frame == pSHP->Frames - 1) {
				--frame;
			}
		}
		else {
			// DeathBombs (that don't exist) use the last frame
			frame = pSHP->Frames - 1;
		}
	}

	R->EAX(frame);
	return 0x438A62;
}

// 6F523C, 5
// custom ivan bomb drawing
DEFINE_HOOK(6F523C, TechnoClass_DrawExtras_IvanBombImage, 5)
{
	GET(TechnoClass*, pThis, EBP);
	auto pBomb = pThis->AttachedBomb;

	auto pData = WeaponTypeExt::BombExt.get_or_default(pBomb);

	if (SHPStruct* pImage = pData->Ivan_Image.Get(RulesClass::Instance->BOMBCURS_SHP)) {
		R->ECX(pImage);
		return 0x6F5247;
	}
	return 0;
}

// 6FCBAD, 6
// custom ivan bomb disarm 1
DEFINE_HOOK(6FCBAD, TechnoClass_CanFire_IvanBomb, 6)
{
	GET(TechnoClass*, Target, EBP);
	GET(WarheadTypeClass*, Warhead, EDI);
	if (Warhead->BombDisarm) {
		if (BombClass* Bomb = Target->AttachedBomb) {
			auto pData = WeaponTypeExt::BombExt.get_or_default(Bomb);
			if (!pData->Ivan_Detachable) {
				return 0x6FCBBE;
			}
		}
	}
	return 0;
}

// 51E488, 5
DEFINE_HOOK(51E488, InfantryClass_GetActionOnObject2, 5)
{
	GET(TechnoClass*, Target, ESI);
	BombClass* Bomb = Target->AttachedBomb;

	auto pData = WeaponTypeExt::BombExt.get_or_default(Bomb);
	if (!pData->Ivan_Detachable) {
		return 0x51E49E;
	}
	return 0;
}

// 438799, 6
// custom ivan bomb detonation 1
DEFINE_HOOK(438799, BombClass_Detonate1, 6)
{
	GET(BombClass*, Bomb, ESI);

	auto pData = WeaponTypeExt::BombExt.get_or_default(Bomb);

	R->Stack<WarheadTypeClass*>(0x4, pData->Ivan_WH.Get(RulesClass::Instance->IvanWarhead));
	R->EDX(pData->Ivan_Damage.Get(RulesClass::Instance->IvanDamage));
	return 0x43879F;
}

// 438843, 6
// custom ivan bomb detonation 2
DEFINE_HOOK(438843, BombClass_Detonate2, 6)
{
	GET(BombClass*, Bomb, ESI);

	auto pData = WeaponTypeExt::BombExt.get_or_default(Bomb);

	R->EDX<WarheadTypeClass*>(pData->Ivan_WH.Get(RulesClass::Instance->IvanWarhead));
	R->ECX(pData->Ivan_Damage.Get(RulesClass::Instance->IvanDamage));
	return 0x438849;
}

// 438879, 6
// custom ivan bomb detonation 3
DEFINE_HOOK(438879, BombClass_Detonate3, 6)
{
	GET(BombClass*, Bomb, ESI);

	auto pData = WeaponTypeExt::BombExt.get_or_default(Bomb);
	return pData->Ivan_KillsBridges ? 0 : 0x438989;
}

// 4393F2, 5
// custom ivan bomb cleanup
DEFINE_HOOK(4393F2, BombClass_SDDTOR, 5)
{
	GET(BombClass*, Bomb, ECX);
	WeaponTypeExt::BombExt.erase(Bomb);
	return 0;
}

/* this is a wtf: it unsets target if the unit can no longer affect its current target.
 * Makes sense, except Aircraft that lose the target so crudely in the middle of the attack
 * (i.e. ivan bomb weapon) go wtfkerboom with an IE
 */
DEFINE_HOOK(6FA4C6, TechnoClass_Update_ZeroOutTarget, 5)
{
	GET(TechnoClass*, T, ESI);
	return (T->WhatAmI() == AbstractType::Aircraft) ? 0x6FA4D1 : 0;
}

DEFINE_HOOK(46934D, BulletClass_DetonateAt_IvanBombs, 6)
{
	GET(BulletClass*, pBullet, ESI);

	if (TechnoClass* pOwner = generic_cast<TechnoClass*>(pBullet->Owner)) {
		if (auto pExt = WeaponTypeExt::ExtMap.Find(pBullet->GetWeaponType())) {

			// single target or spread switch
			if (pBullet->WH->CellSpread < 0.5f) {

				// single target
				if (auto pTarget = generic_cast<TechnoClass*>(pBullet->Target)) {
					pExt->PlantBomb(pOwner, pTarget);
				}
			}
			else {

				// cell spread
				int Spread = static_cast<int>(pBullet->WH->CellSpread);

				CoordStruct tgtCoords = pBullet->GetTargetCoords();

				CellStruct centerCoords = MapClass::Instance->GetCellAt(tgtCoords)->MapCoords;

				CellSpreadIterator<TechnoClass>{}(centerCoords, Spread,
					[pOwner, pExt](TechnoClass* pTechno)
					{
						if (pTechno != pOwner && !pTechno->AttachedBomb) {
							pExt->PlantBomb(pOwner, pTechno);
						}
						return true;
					});
			}
		}
		else {
			Debug::Log(Debug::Severity::Warning, "IvanBomb bullet without attached WeaponType.\n");
		}
	}

	return 0x469AA4;
}

static bool Detonatebomb(TechnoClass* pVictim)
{
	if (auto pBomb = pVictim->AttachedBomb)
	{
		auto const& pWeapon = WeaponTypeExt::BombExt[pBomb];
		auto const pOwner = pBomb->OwnerHouse;

		if (pOwner && pOwner->IsPlayerControl())
		{
			auto const nDeath = pWeapon->Ivan_CanDetonateDeathBomb.Get(RulesClass::Instance()->CanDetonateDeathBomb);
			auto const nTime = pWeapon->Ivan_CanDetonateTimeBomb.Get(RulesClass::Instance()->CanDetonateTimeBomb);

			if (pBomb->Type == BombType::DeathBomb ? nDeath : nTime)
				return true;
		}
	}

	return false;
}

DEFINE_HOOK(6FFEC0, TechnoClass_GetActionOnObject_IvanBombsA, 5)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(ObjectClass*, pTarget, 0x4);

	if ((pThis == pTarget) && (ObjectClass::CurrentObjects()->Count == 1))
	{
		if (Detonatebomb(pThis))
		{
			R->EAX(Action::Detonate);
			return 0x7005EF;
		}
	}
	return 0x0;
}

DEFINE_HOOK(6FFF9E, TechnoClass_GetActionOnObject_IvanBombsB, 8) {
	return 0x700006;
}

DEFINE_HOOK(51F1D8, InfantryClass_ActionOnObject_IvanBombs, 6) {
	return 0x51F1EA;
}

DEFINE_HOOK(7388EB, UnitClass_ActionOnObject_IvanBombs, 6) {
	return 0x7388FD;
}

// #896027: do not announce pointers as expired to bombs
// if the pointed to object is staying in-game.
DEFINE_HOOK(725961, AnnounceInvalidPointer_BombCloak, 6) {
	GET(bool, remove, EDI);
	return remove ? 0 : 0x72596C;
}


DEFINE_HOOK(4471D5, BuildingClass_Sell_DetonateNoBuildup, 6) {
	GET(BuildingClass*, pStructure, ESI);
	if (auto Bomb = pStructure->AttachedBomb) {
		Bomb->Detonate();
	}

	return 0;
}

DEFINE_HOOK(44A1FF, BuildingClass_Mi_Selling_DetonatePostBuildup, 6) {
	GET(BuildingClass*, pStructure, EBP);
	if (auto Bomb = pStructure->AttachedBomb) {
		Bomb->Detonate();
	}

	return 0;
}

DEFINE_HOOK(4D9F7B, FootClass_Sell_Detonate, 6) {
	GET(FootClass*, pSellee, ESI);
	if (auto Bomb = pSellee->AttachedBomb) {
		Bomb->Detonate();
	}
	return 0;
}