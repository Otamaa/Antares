#include "Body.h"

#include <UnitClass.h>
#include <Ext/TechnoType/Body.h>
#include <BuildingClass.h>
#include <InfantryClass.h>
#include <BuildingTypeClass.h>

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

// ToDo : Tunnel Check 
namespace Funct_
{
	void PopulateMap(TechnoClass* pThis, TechnoTypeClass* pType)
	{
		int nPassangersTotal = pType->Passengers;
		auto pTypeExt = TechnoTypeExt::ExtMap.Find(pType);
		auto pExt = TechnoExt::ExtMap.Find(pThis);
		auto& nData = pExt->PassangersPipData;
		nData.clear();
		auto nSlotLeft = nPassangersTotal;
		auto pPassenger = pThis->Passengers.GetFirstPassenger();

		for (int i = 0; i < pThis->Passengers.NumPassengers; ++i)
		{
			if (!pPassenger)
				break;

			auto nSize = (int)(pPassenger->GetTechnoType()->Size - 1.0);
			if (nSize <= 0)
				nSize = 1;

			int nPip = 1;
			if (auto const pInf = specific_cast<InfantryClass*>(pPassenger))
				nPip = (int)pInf->Type->Pip;
			else if (pPassenger->WhatAmI() == AbstractType::Unit)			
				nPip = 5;		

			if (auto pBuilding = specific_cast<BuildingClass*>(pThis))
			{
				int nSizeCount = pBuilding->Absorber() ? 1 : nSize;
				nSlotLeft -= nSizeCount;
				nData[i] = { nPip , nSizeCount, true };
			}
			else
			{
				nSlotLeft -= nSize;
				nData[i] = { nPip  , pTypeExt->Passengers_BySize.Get() ? nSize : 1 , true };
			}

			pPassenger = generic_cast<FootClass*>(pPassenger->NextObject);
		}

		if (nSlotLeft > 0)
			nData[pThis->Passengers.NumPassengers] = { 0 , nSlotLeft , true };
	}

};

struct DrawPipDataStruct
{
	DWORD Array; int Y; SHPStruct* Shape; int Number;
};

DEFINE_HOOK(709D38, TechnoClass_DrawPipscale_Passengers, 7)
{
	GET(TechnoClass* const , pThis, EBP);
	GET(TechnoTypeClass*, pType, EAX);

	if (pType->PipScale != PipScale::Passengers)
		return 0x70A083;

	GET(int, nBracketPosDeltaY, ESI);
	//GET_STACK(SHPStruct*, pShp, 0x7);
	GET_STACK(DrawPipDataStruct, nDataBundle, STACK_OFFS(0x74, 0x60));
	GET_STACK(RectangleStruct*, pRect, STACK_OFFS(0x74, -0xC));
	GET_STACK(Point2D, nPoint, STACK_OFFS(0x74, 0x24));
	GET_STACK(int, nBracketPosDeltaX, STACK_OFFS(0x74, 0x1C));

	auto pExt = TechnoExt::ExtMap.Find(pThis);

	Point2D nPos = { nPoint.X ,nPoint.Y };

	Funct_::PopulateMap(pThis, pType);

	if (!pExt->PassangersPipData.empty())
	{
		for (auto const& nDataItems : pExt->PassangersPipData)
		{
			if (!nDataItems.second.IsActive)
				pExt->PassangersPipData.erase(nDataItems.first);

			for (int s = 0; s < nDataItems.second.DrawCount; ++s)
			{
				DSurface::Temp->DrawSHP
				(FileSystem::PALETTE_PAL,
					nDataBundle.Shape,
					nDataItems.second.PipIdx,
					&nPos,
					pRect,
					BlitterFlags(0x600),
					0,
					0,
					ZGradient::Ground,
					1000,
					0,
					0,
					0,
					0,
					0
				);

				auto nX = nBracketPosDeltaX + nPoint.X;
				auto nY = nBracketPosDeltaY + nPoint.Y;
				nPos.X += nBracketPosDeltaX;
				nPos.Y += nBracketPosDeltaY;

				if (pType->Gunner)
				{
					nPos.X += nX + nBracketPosDeltaX;
					nPos.Y += nY + nBracketPosDeltaY;
				}
			}
		}
	}

	return 0x70A4EC;
}