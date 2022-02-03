#include "Actions.h"
#include "../Misc/Debug.h"
#include <HouseClass.h>
#include <TechnoClass.h>

#include <Helpers\Macro.h>

//4D7524
DEFINE_HOOK(4D7524, FootClass_ActionOnObject_Allow, 9)
{
	//overwrote the ja, need to replicate it
	GET(Action, CursorIndex, EBP);
	if(CursorIndex == Action::None || CursorIndex > Action::Airstrike) {
		if(CursorIndex == CursorType::SuperWeaponAllowed || CursorIndex == CursorType::SuperWeaponDisallowed) {
			return 0x4D769F;
		} else {
			return 0x4D7CC0;
		}
	} else {
		return 0x4D752D;
	}
}

DEFINE_HOOK(6929FC, DisplayClass_ChooseAction_CanSell, 7)
{
	GET(TechnoClass *, Target, ESI);
	switch(Target->WhatAmI()) {
		case AbstractType::Aircraft:
		case AbstractType::Unit:
			R->Stack(0x10, Action::SellUnit);
			return 0x692B06;
		case AbstractType::Building:
			R->Stack(0x10, Target->IsStrange() ? Action::NoSell : Action::Sell);
			return 0x692B06;
		default:
			return 0x692AFE;
	}
}

DEFINE_HOOK(4ABFBE, DisplayClass_LeftMouseButtonUp_ExecPowerToggle, 7)
{
	GET(TechnoClass *, Target, ESI);
	return (Target && Target->Owner->ControlledByHuman() && Target->WhatAmI() == AbstractType::Building)
	 ? 0x4ABFCE
	 : 0x4AC294
	;
}
