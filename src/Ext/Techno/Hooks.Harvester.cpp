#include "Body.h"
#include <YRPPGlobal.h>

DEFINE_HOOK(6AF748, SlaveManagerClass_UpdateSlaves_SlaveScan, 6)
{
	//GET(InfantryClass*, pSlave, ESI);
	//GET(SlaveManagerClass*, pThis, EDI);

	R->EAX(RulesGlobal->SlaveMinerSlaveScan);
	return 0x6AF74E;
}

DEFINE_HOOK_AGAIN(6B026C, SlaveManagerClass_UpdateMiner_ShortScan, 6)
DEFINE_HOOK(6B006D, SlaveManagerClass_UpdateMiner_ShortScan, 6)
{
	//	GET(TechnoClass*, pSlaveOwner, ECX);

	R->EAX(RulesGlobal->SlaveMinerShortScan);
	return R->Origin() + 0x6;
}

DEFINE_HOOK(6B01A3, SlaveManagerClass_UpdateMiner_ScanCorrection, 6)
{
	//GET(SlaveManagerClass*, pThis, ESI);

	R->EAX(RulesGlobal->SlaveMinerScanCorrection);
	return 0x6B01A9;
}

DEFINE_HOOK_AGAIN(6B02CC, SlaveManagerClass_UpdateMiner_LongScan, 6)
DEFINE_HOOK_AGAIN(6B00BD, SlaveManagerClass_UpdateMiner_LongScan, 6)
DEFINE_HOOK(6AFDFC, SlaveManagerClass_UpdateMiner_LongScan, 6)
{
	//	GET(TechnoClass*, pSlaveOwner, ECX);
	R->EAX(RulesGlobal->SlaveMinerLongScan);
	return R->Origin() + 0x6;
}

DEFINE_HOOK(6B1065, SlaveManagerClass_ShouldWakeUp_ShortScan, 5)
{
	GET(SlaveManagerClass*, pThis, ESI);

	auto nKickFrameDelay = RulesGlobal->SlaveMinerKickFrameDelay;

	if (nKickFrameDelay < 0 ||
		nKickFrameDelay + pThis->LastScanFrame >= Unsorted::CurrentFrame
		)
		return 0x6B10C6;

	R->EAX(RulesGlobal->SlaveMinerShortScan);
	return 0x6B1085;
}

// no Ext yet here !
DEFINE_HOOK(73EC0E, UnitClass_Mi_Harvest_TooFarDistance1, 6)
{
	//GET(UnitClass*, pThis, EBP);
	R->EDX(RulesGlobal->HarvesterTooFarDistance);
	return 0x73EC14;
}

DEFINE_HOOK(73EE40, UnitClass_Mi_Harvest_TooFarDistance2, 6)
{
	//GET(UnitClass*, pThis, EBP);

	R->EDX(RulesGlobal->ChronoHarvTooFarDistance);
	return 0x73EE46;
}

DEFINE_HOOK(73E66D, UnitClass_Mi_Harvest_SkipDock, 6)
{
	return 0x73E6CF;
}

DEFINE_HOOK_AGAIN(73EAC6, UnitClass_Mi_Harvest_ShortScan, 6)
DEFINE_HOOK_AGAIN(73EAA6, UnitClass_Mi_Harvest_ShortScan, 6)
DEFINE_HOOK_AGAIN(73EA17, UnitClass_Mi_Harvest_ShortScan, 6)
DEFINE_HOOK(73E9F1, UnitClass_Mi_Harvest_ShortScan, 6)
{
	//GET(UnitClass*, pThis, EBP);

	R->EAX(RulesGlobal->TiberiumShortScan);
	return R->Origin() + 6;
}

DEFINE_HOOK_AGAIN(73E772, UnitClass_Mi_Harvest_LongScan, 6)
DEFINE_HOOK(73E851, UnitClass_Mi_Harvest_LongScan, 6)
{
	//GET(UnitClass*, pThis, EBP);
	R->EAX(RulesGlobal->TiberiumLongScan);
	return R->Origin() + 6;
}

DEFINE_HOOK(74081F, UnitClass_Mi_Guard_KickFrameDelay, 5)
{
	GET(UnitClass*, pThis, ESI);
	auto nFrame = RulesGlobal->SlaveMinerKickFrameDelay;

	return(nFrame < 0 || nFrame + pThis->CurrentMissionStartTime >= Unsorted::CurrentFrame) ?
		0x740854 : 0x74083B;
}

DEFINE_HOOK(74410D, UnitClass_Mi_AreaGuard_KickFrameDelay, 5)
{
	GET(UnitClass*, pThis, ESI);
	auto nFrame = RulesGlobal->SlaveMinerKickFrameDelay;

	return(nFrame < 0 || nFrame + pThis->CurrentMissionStartTime >= Unsorted::CurrentFrame) ?
		0x74416C : 0x744129;
}