#include <Drawing.h>
#include <YRDDraw.h>
#include <BuildingClass.h>
#include <HouseClass.h>
#include <MouseClass.h>

#include "Body.h"
#include "../TechnoType/Body.h"

#include "../../Misc/Debug.h"

/*
6FB191 = TechnoClass_CreateGap, 8
4D8642 = FootClass_UpdatePosition, 6
6FB4B1 = TechnoClass_DeleteGap_new, 6
6F6B66 = TechnoClass_Remove_DeleteGap, A
6FB723 = TechnoClass_CreateGap_RefreshMap, 6
6FB446 = TechnoClass_CreateGap_RefreshMap, 5
701735 = TechnoClass_ChangeOwnership_OwnerChange, 6
55AFB3 = LogicClass_Update_Gaps, 6
657CE0 = RadarClass_MinimapChanged, 5
65731F = RadarClass_UpdateMinimap_Lock, 6
65757C = RadarClass_UpdateMinimap_Unlock, 8
657CF2 = MapClass_MinimapChanged_Lock1, 6
657D35 = MapClass_MinimapChanged_Unlock1, 7
657D3D = MapClass_MinimapChanged_Lock2, 6
657D8A = MapClass_MinimapChanged_Unlock2, 7

DEFINE_HOOK(6FB306, TechnoClass_CreateGap_Optimize, 6)
{
	GET(CellClass*, pCell, EAX);

	auto nCounter = pCell->ShroudCounter;
	if ((nCounter >= 0) && (nCounter != 1))
	{
		nCounter += 1;
		pCell->ShroudCounter = nCounter;
	}

	++pCell->GapsCoveringThisCell;

	if (nCounter >= 1)
		pCell->CopyFlags &= 0xFFFFFFE7;

	return 0x6FB3BD;
}

DEFINE_HOOK(6FB5F0, TechnoClass_DeleteGap_Optimize, 6)
{
	GET(CellClass*, pCell, EAX);

	auto nGapCover = pCell->GapsCoveringThisCell - 1;
	pCell->GapsCoveringThisCell = nGapCover;

	if (!HouseClass::Player()->SpySatActive || nGapCover > 0)
		return 0x6FB69E;

	--pCell->ShroudCounter;
	if (pCell->ShroudCounter <= 0)
		pCell->CopyFlags |= 0x18u;

	return 0x6FB69E;
}
 
44E2B0 = BuildingClass_Mi_Unload_LargeGap, 6
454BDC = BuildingClass_UpdatePowered_LargeGap, 7
4566D5 = BuildingClass_GetRangeOfRadial_LargeGap, 6

DEFINE_HOOK_AGAIN(6FB4A3, TechnoClass_CreateGap_LargeGap, 7)
DEFINE_HOOK(6FB1B5, TechnoClass_CreateGap_LargeGap, 7)
{
	GET(TechnoClass*, pThis, ESI);
	GET(TechnoTypeClass*, pThisType, EAX);

	//Ares Reimplement this GapRadiusInCells
	pThis->GapRadius = pThisType->GapRadiusInCells;

	return R->Origin() + 0xD;
}


*/

DEFINE_HOOK(65757C, RadarClass_UpdateMinimap_Unlock, 8)
{
	GET(RadarClass*, pThis, ESI);

	pThis->unknown_121C->Unlock();
	pThis->unknown_1220->Unlock();

	return R->AL() ? 0x657584 : 0x6576A5;
}

DEFINE_HOOK(65731F, RadarClass_UpdateMinimap_Lock, 6)
{
	GET(RadarClass*, pThis, ESI);
	pThis->unknown_121C->Lock();
	pThis->unknown_1220->Lock();

	return 0;
}

DEFINE_HOOK(6FB191, TechnoClass_CreateGap, 8)
{
	GET(TechnoClass *, T, ESI);
	bool canGap = false;
	if(BuildingClass * B = specific_cast<BuildingClass *>(T)) {
		canGap = B->IsPowerOnline();
	} else {
		canGap = T->IsAlive && !T->InLimbo && !T->Deactivated;
	}
	return canGap
		? 0x6FB1A1
		: 0x6FB45C
	;
}

DEFINE_HOOK(4D8642, FootClass_UpdatePosition, 6)
{
	GET(FootClass *, F, ESI);
	TechnoTypeClass *Type = F->GetTechnoType();
	if(Type->GapGenerator && Type->GapRadiusInCells > 0) {
		CellStruct curLocation = F->GetMapCoords();
		if(curLocation != F->LastMapCoords) {
			TechnoExt::NeedsRegap = 1;
			F->CreateGap();
			F->DestroyGap();
		}
	}
	return 0;
}

// lolhax
CoordStruct GapCoordBuffer;

DEFINE_HOOK(6FB4B1, TechnoClass_DeleteGap_new, 6)
{
	GET(FootClass *, F, ESI);
	if(F->WhatAmI() == AbstractType::Building) {
		R->EDX<CoordStruct *>(&F->Location);
	} else {
		GapCoordBuffer = MapClass::Instance->GetCellAt(F->LastMapCoords)->GetCoords();
		R->EDX<CoordStruct *>(&GapCoordBuffer);
	}
	return 0x6FB4B7;
}

DEFINE_HOOK(6F6B66, TechnoClass_Remove_DeleteGap, A)
{
	GET(TechnoClass *, T, ESI);
	if(T->WhatAmI() == AbstractType::Building) {
		T->DestroyGap();
	} else {
		TechnoExt::NeedsRegap = 1;
		FootClass *F = reinterpret_cast<FootClass *>(T);
		CellStruct buffer = F->LastMapCoords;
		F->LastMapCoords = F->CurrentMapCoords;
		F->DestroyGap();
		F->LastMapCoords = buffer;
	}
	return 0x6F6B70;
}

DEFINE_HOOK(6FB446, TechnoClass_CreateGap_RefreshMap, 5)
{
	return TechnoExt::NeedsRegap
		? 0x6FB45C
		: 0
	;
}

DEFINE_HOOK(6FB723, TechnoClass_DeleteGap_RefreshMap, 5)
{
	return TechnoExt::NeedsRegap
		? 0x6FB739
		: 0
	;
}

DEFINE_HOOK(55AFB3, LogicClass_Update_Gaps, 6)
{
	if(TechnoExt::NeedsRegap && !(Unsorted::CurrentFrame % 8)) {
		TechnoExt::NeedsRegap = 0;
		MapClass::Instance->sub_657CE0();
		MapClass::Instance->RedrawSidebar(2);
	}
	return 0;
}

static void StartTimer() {
	reinterpret_cast<DSurface *>(MouseClass::Instance->unknown_121C)->Lock(0, 0);
	reinterpret_cast<DSurface *>(MouseClass::Instance->unknown_1220)->Lock(0, 0);
}

static void EndTimer() {
	reinterpret_cast<DSurface *>(MouseClass::Instance->unknown_1220)->Unlock();
	reinterpret_cast<DSurface *>(MouseClass::Instance->unknown_121C)->Unlock();
}

DEFINE_HOOK(657CF2, MapClass_MinimapChanged_1S, 6)
{
	StartTimer();
	return 0;
}

DEFINE_HOOK(657D35, MapClass_MinimapChanged_1E, 7)
{
	EndTimer();
	return 0;
}


DEFINE_HOOK(657D3D, MapClass_MinimapChanged_2S, 6)
{
	StartTimer();
	return 0;
}

DEFINE_HOOK(657D8A, MapClass_MinimapChanged_2E, 7)
{
	EndTimer();
	return 0;
}

DEFINE_HOOK(655DFF, MapClass_UpdateMinimapPixel_1, 6)
{
	GET(TechnoTypeClass *, Type, EAX);
	return (Type->GapGenerator)
		? 0x655E66
		: 0
	;
}

DEFINE_HOOK(6561E4, MapClass_UpdateMinimapForCrates_2, 6)
{
	GET(TechnoClass *, Techno, ESI);
	return (Techno->GetTechnoType()->GapGenerator)
		? 0x656292
		: 0
	;
}

DEFINE_HOOK(6FB306, TechnoClass_CreateGap_TryOptimize, 6)
{
	GET(CellClass *, Cell, EAX);
	const auto ShroudCounter = Cell->ShroudCounter;
	if(ShroudCounter >= 0 && ShroudCounter != 1) {
		++Cell->ShroudCounter;
	}
	++Cell->GapsCoveringThisCell;
	if(ShroudCounter >= 1) {
		Cell->CopyFlags &= ~(0x18);
	}
	return 0x6FB3BD;
}

DEFINE_HOOK(6FB5F0, TechnoClass_DeleteGap_TryOptimize, 6)
{
	GET(CellClass *, Cell, EAX);
	--Cell->GapsCoveringThisCell;
	const auto remainingGaps = static_cast<signed int>(Cell->GapsCoveringThisCell);
	if(HouseClass::Player->SpySatActive) {
		if(remainingGaps <= 0) {
			--Cell->ShroudCounter;
			const auto ShroudCounter = Cell->ShroudCounter;
			if(ShroudCounter <= 0) {
				Cell->CopyFlags |= 0x18;
			}
		}
	}
	return 0x6FB69E;
}
