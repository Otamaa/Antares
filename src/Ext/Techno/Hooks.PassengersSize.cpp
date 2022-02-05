#include "Body.h"

#include <UnitClass.h>
#include <Ext/TechnoType/Body.h>

DEFINE_HOOK(73762B, UnitClass_ReceivedRadioCommand_BySize1, 6)
{
	GET(UnitClass*, pThis, ESI);

	auto pType = pThis->Type;
	auto pExt = TechnoTypeExt::ExtMap.Find(pType);

	if (pExt->Passengers_BySize.Get())
		return 0;

	return pThis->Passengers.NumPassengers < pType->Passengers ?
		0x737677 : 0x73780F;
}

DEFINE_HOOK(73778F, UnitClass_ReceivedRadioCommand_BySize2, 6)
{
	GET(UnitClass*, pThis, ESI);

	auto pType = pThis->Type;
	auto pExt = TechnoTypeExt::ExtMap.Find(pType);

	if (pExt->Passengers_BySize.Get())
		return 0;

	return pThis->Passengers.NumPassengers == pType->Passengers ?
		0x7377AA : 0x7377C9;
}

DEFINE_HOOK(73782F, UnitClass_ReceivedRadioCommand_BySize3, 6)
{
	GET(UnitClass*, pThis, ESI);

	auto pType = pThis->Type;
	auto pExt = TechnoTypeExt::ExtMap.Find(pType);

	if (pExt->Passengers_BySize.Get())
		return 0;

	return pThis->Passengers.NumPassengers < pType->Passengers ?
		0x737877 : 0x73780F;
}

DEFINE_HOOK(737994, UnitClass_ReceivedRadioCommand_BySize4, 6)
{
	GET(UnitClass*, pThis, ESI);

	auto pType = pThis->Type;
	auto pExt = TechnoTypeExt::ExtMap.Find(pType);

	if (pExt->Passengers_BySize.Get())
		return 0;

	return pThis->Passengers.NumPassengers < pType->Passengers ?
		0x7379E8 : 0x737AFC;
}
