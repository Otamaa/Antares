#include "Body.h"

#include <AircraftClass.h>
#include <BuildingClass.h>
#include <UnitClass.h>

#include <Ext/TechnoType/Body.h>
#include <Ext/BuildingType/Body.h>
// NOTE! enabling this hook breaks tank bunker. (#1155833)
//A_FINE_HOOK(6FFF03, TechnoClass_GetCursorOverObject, A)
//{
//	enum {
//		Cursor_Select = 0x6FFF7E,
//		Cursor_None = 0x7005E6,
//		Cursor_ToggleSelect = 0x700104,
//		Cursor_NotMyProblem = 0x6FFF8D
//	} Result = Cursor_NotMyProblem;
//
//	GET(TechnoClass *, pThis, ESI);
//
//	if(generic_cast<FootClass *>(pThis)) {
//		// BuildingClass::IsControllable only permits factories and conyards, derp herp
//		if(pThis->Owner->ControlledByPlayer() && !pThis->IsControllable()) {
//			if(pThis->CanBeSelected()) {
//				// all four derivate classes have special cased this value and will never attempt to override it
//				Result = Cursor_ToggleSelect;
//			} else {
//				Result = Cursor_None;
//			}
//		}
//	}
//
//	return Result;
//}

// skip the check for UnitRepair, as it does not play well with UnitReload and
// Factory=AircraftType at all. in fact, it's prohibited, and thus docking to
// other structures was never allowed.
DEFINE_HOOK(417E16, AircraftClass_GetActionOnObject_Dock, 6)
{
	// target is known to be a building
	GET(AircraftClass* const, pThis, ESI);
	GET(BuildingClass* const, pBuilding, EDI);

	// enter and no-enter cursors only if aircraft can dock
	if(pThis->Type->Dock.FindItemIndex(pBuilding->Type) != -1) {
		return 0x417E4B;
	}

	// select cursor
	return 0x417E7D;
}

DEFINE_HOOK(70055D, TechnoClass_GetActionOnObject_AttackCursor, 8)
{
	GET(TechnoClass*, pThis, ESI);
	GET_STACK(int, nWeapon, STACK_OFFS(0x1C, 0x8));

	auto nCursor = TechnoTypeExt::GetCursorByWeapon(pThis, nWeapon, false);
	CursorType::Insert((int)nCursor, Action::Attack, false);

	return 0;
}

DEFINE_HOOK(700AA8, TechnoClass_GetActionOnCell_AttackCursor, 8)
{
	GET(TechnoClass*, pThis, ESI);
	GET(int, nWeapon, EBP);

	auto nCursor = TechnoTypeExt::GetCursorByWeapon(pThis, nWeapon, false);
	CursorType::Insert((int)nCursor, Action::Attack, false);

	return 0;
}

DEFINE_HOOK(6FFEC0, TechnoClass_GetActionOnObject_Cursors, 5)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(ObjectClass*, pTarget, 0x4);

	auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	CursorType::Insert(pThisExt->Move_Cursor.Get(), Action::Move, false);
	CursorType::Insert(pThisExt->NoMove_Cursor.Get(), Action::NoMove, false);

	if (auto pTargetType = pTarget->GetTechnoType())
	{
		auto const& pTargetExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

		CursorType::Insert(pTargetExt->Enter_Cursor.Get(), Action::Repair, false);
		CursorType::Insert(pTargetExt->Enter_Cursor.Get(), Action::Enter, false);
		CursorType::Insert(pTargetExt->NoEnter_Cursor.Get(), Action::NoEnter, false);
	}

	return 0;
}

DEFINE_HOOK(700600, TechnoClass_GetActionOnCell_Cursors, 5)
{
	GET(TechnoClass*, pThis, ECX);

	auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	CursorType::Insert(pThisExt->Move_Cursor.Get(), Action::Move, false);
	CursorType::Insert(pThisExt->NoMove_Cursor.Get(), Action::NoMove, false);

	return 0;
}

DEFINE_HOOK(7000CD, TechnoClass_GetActionOnObject_SelfDeployCursor, 6)
{
	GET(TechnoClass*, pThis, ESI);

	auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	CursorType::Insert(pThisExt->Deploy_Cursor.Get(), Action::AreaAttack, false);
	CursorType::Insert(pThisExt->Deploy_Cursor.Get(), Action::Self_Deploy, false);
	CursorType::Insert(pThisExt->NoDeploy_Cursor.Get(), Action::NoDeploy, false);

	return 0;
}

DEFINE_HOOK(7400F0, UnitClass_GetActionOnObject_SelfDeployCursor_Bunker, 6)
{
	GET(UnitClass*, pThis, ESI);

	if (pThis->BunkerLinkedItem)
	{
		auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
		CursorType::Insert(pThisExt->Deploy_Cursor.Get(), Action::Self_Deploy, false);
		return 0x73FFE6;
	}

	return pThis->unknown_bool_6AC ? 0x7400FA : 0x740115;
}