#include "Body.h"
#include <InfantryClass.h>
#include <WWKeyboardClass.h>
#include <WeaponTypeClass.h>
#include <Ext/WarheadType/Body.h>
#include <Enum/CursorTypes.h>

/*
6FA361 = TechnoClass_Update_LoseTarget, 5
6F8F1F = TechnoClass_FindTargetType_Heal, 6
6F8EE3 = TechnoClass_FindTargetType_Heal, 6
6F7FC5 = TechnoClass_CanAutoTargetObject_Heal, 7 
51C913 = InfantryClass_CanFire_Heal, 7
520731 = InfantryClass_UpdateFiringState_Heal, 5

736E8E = UnitClass_UpdateFiringState_Heal, 6
741113 = UnitClass_CanFire_Heal, A
*/

DEFINE_HOOK(51E710 ,InfantryClass_GetActionOnObject_Heal, 7)
{
	enum {
		ActionGueardArea = 0x51E748,
		NextCheck = 0x51E757,
		NextCheck2 = 0x51E7A6,
		DoActionHeal = 0x51E739
	};

	GET(InfantryClass*, pThis, EDI);
	GET(ObjectClass*, pThat, ESI);

	if (pThis == pThat)
		return ActionGueardArea;

	if(!pThat || ((pThat->AbstractFlags & AbstractFlags::Techno) == AbstractFlags::None))
		return NextCheck;

	bool bKeyPressed_1 = WWKeyboardClass::Instance->IsDown(*reinterpret_cast<int*>(0xA8EBF8));
	bool bKeyPressed_2 = WWKeyboardClass::Instance->IsDown(*reinterpret_cast<int*>(0xA8EBFC));
	auto pWeapon = pThis->GetWeapon(pThis->SelectWeapon(pThat))->WeaponType;
	auto pWHExt = WarheadTypeExt::ExtMap.Find(pWeapon->Warhead);
	auto const& Verses = pWHExt->GetVerses(pThat->GetTechnoType()->Armor).Verses;
	auto nHP = RulesClass::Instance->ConditionGreen <= pThat->GetHealthPercentage();

	if (nHP || bKeyPressed_1 || bKeyPressed_2 || Verses <= 0.0)
	{
		if (auto pBuilding = specific_cast<BuildingClass*>(pThat))
			if (pBuilding->Type->Grinding)
				return NextCheck2;

		return NextCheck;
	}

	int nCursor = (pThat->GetTechnoType()->Organic || pThat->WhatAmI() == AbstractType::Infantry) ? 90:91;
	CursorType::Insert(nCursor, Action::Heal, false);
	return DoActionHeal;
}

DEFINE_HOOK(73FDBD, UnitClass_GetActionOnObject_Heal, 5)
{
	GET(UnitClass*, pThis, EDI);
	GET(ObjectClass*, pThat, ESI);
	GET(Action, nAct, EBX);

	if (nAct == Action::GuardArea)
		return 0x73FE48;

	bool bKeyPressed_1 = WWKeyboardClass::Instance->IsDown(*reinterpret_cast<int*>(0xA8EBF8));
	bool bKeyPressed_2 = WWKeyboardClass::Instance->IsDown(*reinterpret_cast<int*>(0xA8EBFC));
	auto nHP = RulesClass::Instance->ConditionGreen <= pThat->GetHealthPercentage();

	if(bKeyPressed_1 ||  bKeyPressed_2 || !pThat ||
	((pThat->AbstractFlags & AbstractFlags::Techno) == AbstractFlags::None) ||
		nHP || !pThat->IsSurfaced())
	{
		return 0x73FE3B;
	}

	if (auto pAir = specific_cast<AircraftClass*>(pThat))
	{
		if(pAir->GetCell()->GetBuilding())
			return 0x73FE3B;
	}

	auto pWeapon = pThis->GetWeapon(pThis->SelectWeapon(pThat))->WeaponType;
	auto pWHExt = WarheadTypeExt::ExtMap.Find(pWeapon->Warhead);
	auto const& Verses = pWHExt->GetVerses(pThat->GetTechnoType()->Armor).Verses;

	if(Verses <= 0.0)
		return 0x73FE3B;

	int nCursor = (pThat->GetTechnoType()->Organic || pThat->WhatAmI() == AbstractType::Infantry) ? 90 : 91;
	CursorType::Insert(nCursor, Action::GRepair, false);

	return 0x73FE08;
}