#include "Body.h"
#include "../BuildingType/Body.h"
#include "../Techno/Body.h"
#include "../TechnoType/Body.h"

/* #633 - spy building infiltration */
// wrapper around the entire function
DEFINE_HOOK(0x4571E0, BuildingClass_Infiltrate, 0x5)
{
	GET(BuildingClass *, EnteredBuilding, ECX);
	GET_STACK(HouseClass *, Enterer, 0x4);

	BuildingExt::ExtData *pBuilding = BuildingExt::ExtMap.Find(EnteredBuilding);

	return (pBuilding->InfiltratedBy(Enterer))
		? 0x4575A2
		: 0
	;
}

// #814: force sidebar repaint for standard spy effects
DEFINE_HOOK_AGAIN(0x4574D2, BuildingClass_Infiltrate_Standard, 0x6)
DEFINE_HOOK(0x457533, BuildingClass_Infiltrate_Standard, 0x6)
{
	MouseClass::Instance->SidebarNeedsRepaint();
	return R->get_Origin() + 6;
}

// check before drawing the tooltip
DEFINE_HOOK(0x43E7EF, BuildingClass_DrawVisible_P1, 0x5)
{
	GET(BuildingClass *, B, ESI);
	BuildingTypeExt::ExtData *pType = BuildingTypeExt::ExtMap.Find(B->Type);
	return (pType->RevealProduction && B->DisplayProductionTo.Contains(HouseClass::Player))
		? 0x43E80E
		: 0x43E832
	;
}

// check before drawing production cameo
DEFINE_HOOK(0x43E832, BuildingClass_DrawVisible_P2, 0x6)
{
	GET(BuildingClass *, B, ESI);
	BuildingTypeExt::ExtData *pType = BuildingTypeExt::ExtMap.Find(B->Type);
	return (pType->RevealProduction && B->DisplayProductionTo.Contains(HouseClass::Player))
		? 0x43E856
		: 0x43E8EC
	;
}

// fix palette for spied factory production cameo drawing
DEFINE_HOOK(0x43E8D1, BuildingClass_DrawVisible_P3, 0x8)
{
	GET(TechnoTypeClass *, Type, EAX);
	TechnoTypeExt::ExtData *pData = TechnoTypeExt::ExtMap.Find(Type);
	R->EAX<SHPStruct *>(Type->Cameo);
	R->EDX<ConvertClass *>(pData->CameoPal.Convert ? pData->CameoPal.Convert : FileSystem::CAMEO_PAL);
	return 0x43E8DF;
}

// if this is a radar, change the owner's house bitfields responsible for radar reveals
DEFINE_HOOK(0x44161C, BuildingClass_Destroy_OldSpy1, 0x6)
{
	GET(BuildingClass *, B, ESI);
	B->DisplayProductionTo.Clear();
	BuildingExt::UpdateDisplayTo(B);
	return 0x4416A2;
}

// if this is a radar, change the owner's house bitfields responsible for radar reveals
DEFINE_HOOK(0x448312, BuildingClass_ChangeOwnership_OldSpy1, 0xa)
{
	GET(HouseClass *, newOwner, EBX);
	GET(BuildingClass *, B, ESI);

	if(B->DisplayProductionTo.Contains(newOwner)) {
		B->DisplayProductionTo.Remove(newOwner);
		BuildingExt::UpdateDisplayTo(B);
	}
	return 0x4483A0;
}

// if this is a radar, drop the new owner from the bitfield
DEFINE_HOOK(0x448D95, BuildingClass_ChangeOwnership_OldSpy2, 0x8)
{
	GET(HouseClass *, newOwner, EDI);
	GET(BuildingClass *, B, ESI);

	if(B->DisplayProductionTo.Contains(newOwner)) {
		B->DisplayProductionTo.Remove(newOwner);
	}

	return 0x448DB9;
}

DEFINE_HOOK(0x44F7A0, BuildingClass_UpdateDisplayTo, 0x0)
{
	GET(BuildingClass *, B, ECX);
	BuildingExt::UpdateDisplayTo(B);
	return 0x44F813;
}

DEFINE_HOOK(0x509303, HouseClass_AllyWith_unused, 0x0)
{
	GET(HouseClass *, pThis, ESI);
	GET(HouseClass *, pThat, EAX);

	pThis->RadarVisibleTo.Add(pThat);
	return 0x509319;
}

DEFINE_HOOK(0x56757F, MapClass_RevealArea0_DisplayTo, 0x0)
{
	GET(HouseClass *, pThis, EDI);
	GET(HouseClass *, pThat, EAX);

	return pThis->RadarVisibleTo.Contains(pThat)
		? 0x567597
		: 0x56759D
	;
}

DEFINE_HOOK(0x567AC1, MapClass_RevealArea1_DisplayTo, 0x0)
{
	GET(HouseClass *, pThis, EBX);
	GET(HouseClass *, pThat, EAX);

	return pThis->RadarVisibleTo.Contains(pThat)
		? 0x567AD9
		: 0x567ADF
	;
}
