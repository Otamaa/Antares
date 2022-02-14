#include "Body.h"

#include "../BuildingType/Body.h"

#include <MouseClass.h>

//DEFINE_HOOK(45EC90, Foundations_GetFoundationWidth, 6)
DEFINE_HOOK(45EC90, BuildingTypeClass_GetFoundationWidth, 6)
{
	GET(BuildingTypeClass*, pThis, ECX);
	BuildingTypeExt::ExtData* pData = BuildingTypeExt::ExtMap.Find(pThis);

	if(pData->IsCustom) {
		R->EAX(pData->CustomWidth);
		return 0x45EC9D;
	}

	return 0;
}

//DEFINE_HOOK(45ECA0, Foundations_GetFoundationHeight, 6)
DEFINE_HOOK(45ECA0, BuildingTypeClass_GetFoundationHeight, 6)
{
	GET(BuildingTypeClass*, pThis, ECX);
	BuildingTypeExt::ExtData* pData = BuildingTypeExt::ExtMap.Find(pThis);

	if(pData->IsCustom) {
		bool bIncludeBib = (R->Stack8(0x4) != 0);

		int fH = pData->CustomHeight;
		if(bIncludeBib && pThis->Bib) {
			++fH;
		}

		R->EAX(fH);
		return 0x45ECDA;
	}

	return 0;
}

//DEFINE_HOOK(656584, MapClass_GetFoundationShape, 6)
DEFINE_HOOK(656584, RadarClass_GetFoundationShape, 6)
{
	GET(RadarClass*, pThis, ECX);
	GET(BuildingTypeClass*, pType, EAX);

	auto fnd = pType->Foundation;
	DynamicVectorClass<Point2D>* ret = nullptr;

	if(fnd >= fnd_1x1 && fnd <= fnd_0x0) {
		// in range of default foundations
		ret = &pThis->FoundationTypePixels[fnd];
	} else if(auto pExt = BuildingTypeExt::ExtMap.Find(pType)) {
		// custom foundation
		ret = &pExt->FoundationRadarShape;
	} else {
		// default if everything fails
		ret = &pThis->FoundationTypePixels[fnd_2x2];
	}

	R->EAX(ret);
	return 0x656595;
}

DEFINE_HOOK(6563B0, RadarClass_UpdateFoundationShapes_Custom, 5)
{
	// update each building type foundation
	for(auto const& pType : *BuildingTypeClass::Array) {
		if(auto pExt = BuildingTypeExt::ExtMap.Find(pType)) {
			pExt->UpdateFoundationRadarShape();
		}
	}

	return 0;
}

DEFINE_HOOK(568411, MapClass_AddContentAt_Foundation_P1, 0)
{
	GET(BuildingClass *, pThis, EDI);

	R->EBP(pThis->GetFoundationData(false));

	return 0x568432;
}

DEFINE_HOOK(568565, MapClass_AddContentAt_Foundation_OccupyHeight, 5)
{
	GET(BuildingClass *, pThis, EDI);
	GET(int, ShadowHeight, EBP);
	GET_STACK(CellStruct *, MainCoords, 0x8B4);

	auto const& AffectedCells = BuildingExt::GetCoveredCells(
		pThis, *MainCoords, ShadowHeight);

	auto &Map = MapClass::Instance;

	for(auto const& cell : AffectedCells) {
		if(auto pCell = Map->TryGetCellAt(cell)) {
			++pCell->OccupyHeightsCoveringMe;
		}
	}

	return 0x568697;
}

DEFINE_HOOK(568841, MapClass_RemoveContentAt_Foundation_P1, 0)
{
	GET(BuildingClass *, pThis, EDI);

	R->EBP(pThis->GetFoundationData(false));

	return 0x568862;
}

DEFINE_HOOK(568997, MapClass_RemoveContentAt_Foundation_OccupyHeight, 5)
{
	GET(BuildingClass *, pThis, EDX);
	GET(int, ShadowHeight, EBP);
	GET_STACK(CellStruct *, MainCoords, 0x8B4);

	auto const& AffectedCells = BuildingExt::GetCoveredCells(
		pThis, *MainCoords, ShadowHeight);

	auto &Map = MapClass::Instance;

	for(auto const& cell : AffectedCells) {
		if(auto const pCell = Map->TryGetCellAt(cell)) {
			if(pCell->OccupyHeightsCoveringMe > 0) {
				--pCell->OccupyHeightsCoveringMe;
			}
		}
	}

	return 0x568ADC;
}


//DEFINE_HOOK(4A8C77, MapClass_ProcessFoundation1_UnlimitBuffer, 5)
DEFINE_HOOK(4A8C77, DisplayClass_ProcessFoundation1_UnlimitBuffer, 5)
{
	GET_STACK(CellStruct const*, Foundation, 0x18);
	GET(DisplayClass *, Display, EBX);

	DWORD Len = BuildingExt::FoundationLength(Foundation);

	BuildingExt::TempFoundationData1.assign(Foundation, Foundation + Len);

	Display->CurrentFoundation_Data = BuildingExt::TempFoundationData1.data();

	auto const bounds = Display->FoundationBoundsSize(
		BuildingExt::TempFoundationData1.data());

	R->Stack<CellStruct>(0x18, bounds);
	R->EAX<CellStruct *>(R->lea_Stack<CellStruct *>(0x18));

	return 0x4A8C9E;
}

//DEFINE_HOOK(4A8DD7, MapClass_ProcessFoundation2_UnlimitBuffer, 5)
DEFINE_HOOK(4A8DD7, Display_ProcessFoundation2_UnlimitBuffer, 5)
{
	GET_STACK(CellStruct const*, Foundation, 0x18);
	GET(DisplayClass *, Display, EBX);

	DWORD Len = BuildingExt::FoundationLength(Foundation);

	BuildingExt::TempFoundationData2.assign(Foundation, Foundation + Len);

	Display->CurrentFoundationCopy_Data = BuildingExt::TempFoundationData2.data();

	auto const bounds = Display->FoundationBoundsSize(
		BuildingExt::TempFoundationData2.data());

	R->Stack<CellStruct>(0x18, bounds);
	R->EAX<CellStruct *>(R->lea_Stack<CellStruct *>(0x18));

	return 0x4A8DFE;
}
// Ares 3.0 add this for custom foundation -Otamaa
// Hook offset-ed 
DEFINE_HOOK_AGAIN(6D5579, sub_6D5030_CustomFoundation, 6)
DEFINE_HOOK(6D5100, sub_6D5030_CustomFoundation, 5)
{
	//static constexpr reference<CellStruct*, 0x880964u> CursorSize{};
	//static constexpr reference<CellStruct*, 0x880974u> CursorSizeSecond{};
	auto nCursorSize = (R->Origin() == 0x6D5579) ? R->EDX<CellStruct*>() : R->EAX<CellStruct*>();
	auto nReturn = (R->Origin() == 0x6D5579) ? 0x6D5589 : 0x6D5110;

	constexpr auto nFoundation_Rect_Translated = [](CellStruct* a2)
	{
		if (!a2)
			return RectangleStruct{ 0,0,0,0 };
		/*
		struct wCell
		{
			short width;
			short height;
		};*/

		int min_x = 512;
		int max_x = -512;
		int min_y = 512;
		int max_y = -512;
		auto v3 = a2;

		while (1)
		{
			auto p_X = v3->X;
			if (v3->X == 0x7FFF && v3->Y == 0x7FFF)
			{
				break;
			}
			auto p_Y = v3->Y;
			if (max_x <= p_X)
			{
				max_x = p_X;
			}

			if (min_x >= p_X)
			{
				min_x = p_X;
			}

			if (max_y <= p_Y)
			{
				max_y = v3->Y;
			}

			if (min_y >= p_Y)
			{
				min_y = v3->Y;
			}
			++v3;
		}

		return  RectangleStruct{ min_x ,min_y ,max_x,max_y };
	};

	auto nFoundation = nFoundation_Rect_Translated(nCursorSize);
	
	auto v5 = nFoundation.Y;

	short v6 = 0;
	if (nFoundation.Width - nFoundation.X >= 0)
		v6 = (short)(nFoundation.Width - nFoundation.X);

	short v7 = 0;
	if (nFoundation.Height - v5 >= 0)
		v7 = (short)(nFoundation.Height - v5);

	CellStruct v9 = { v6 + 1 ,v7 + 1 };
	CellStruct v11 = { (short)nFoundation.X ,(short)nFoundation.Y };

	R->EAX(&v9);
	R->Stack(STACK_OFFS(0x54, 0x40), v11);

	return nReturn;
}