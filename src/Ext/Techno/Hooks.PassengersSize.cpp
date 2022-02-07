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
/*
#include <BuildingClass.h>
#include <InfantryClass.h>
#include <Helpers/Enumerators.h>

namespace PipManager
{
	//	DynamicVectorClass<FootClass*>& GetTunnelContents(BuildingClass* pThis)
	//	{
	//		DynamicVectorClass<FootClass*> nContens;
	//		return nContens;
	//	}

	// ToDo : -Handle building
	std::tuple<int*, int, bool, bool> GetPipResult(TechnoClass* pThis)
	{
		auto const nPipMax = pThis->GetTechnoType()->GetPipMax();
		auto pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
		auto nArr = GameCreateArray<int>((size_t)nPipMax);
		auto nCargoCount = pThis->Passengers.GetTotalSize() - 1;

		if (nCargoCount >= nPipMax)
		{
			GameDeleteArray(nArr, (size_t)nPipMax);
			return std::make_tuple(nullptr, 0, false, true);
		}

		auto pPassenger = pThis->Passengers.GetFirstPassenger();

		for (int i = 0; i < pThis->Passengers.NumPassengers; ++i)
		{
			if (!pPassenger)
				break;


			if (auto pInf = specific_cast<InfantryClass*>(pPassenger))
			{
				auto nSizei =  (int)(pPassenger->GetTechnoType()->Size - 1.0);
				if (nSizei > 0)
				{
					auto nAccessed_inf = &nArr[nCargoCount];
					nCargoCount -= nSizei;
					do
					{
						*nAccessed_inf-- = 3;
					} while (nSizei - 1);
				}

				nArr[nCargoCount] = (int)pInf->Type->Pip;

			}
			else
				if (pPassenger->WhatAmI() == AbstractType::Unit)
				{
					auto nSize = (int)(pPassenger->GetTechnoType()->Size - 1.0);
					if (nSize > 0)
					{
						auto nAccessed_u = &nArr[nCargoCount];
						nCargoCount -= nSize;
						do
						{
							*nAccessed_u-- = 3;
						} while (nSize - 1);
					}
					nArr[nCargoCount] = 5;
				}
				else
				{
					nArr[nCargoCount] = 1;
				}

			pPassenger = generic_cast<FootClass*>(pPassenger->NextObject);
			--nCargoCount;
		}

		return std::make_tuple(nArr, nPipMax, pThis->GetTechnoType()->Gunner, false);
	}
	
	//void Test(TechnoClass* pThis)
	//{
	//	auto pType = pThis->GetTechnoType();
	//
	//	auto nRemaining = 
	//}

};

DEFINE_HOOK(709D38, TechnoClass_DrawPipscale_Passengers, 7)
{
	struct DrawPipDataStruct
	{
		int* Array; int Y; SHPStruct* Shape; int Number;
	};

	GET(TechnoClass*, pThis, EBP);
	GET(TechnoTypeClass*, pType, EAX);
	GET(int, nBracketPosDeltaY, ESI);
	GET_STACK(DrawPipDataStruct, nDataBundle, STACK_OFFS(0x74, 0x60));
	GET_STACK(RectangleStruct*, pRect, STACK_OFFS(0x74, -0xC));
	GET_STACK(Point2D, nPoint, STACK_OFFS(0x74, 0x24));
	GET_STACK(int, nBracketPosDeltaX, STACK_OFFS(0x74, 0x1C));

	if (pType->PipScale != PipScale::Passengers)
		return 0x70A083;

	auto nPipResut = PipManager::GetPipResult(pThis);

	if (std::get<3>(nPipResut))
		return 0x70A083;

	auto pExt = TechnoExt::ExtMap.Find(pThis);
	auto& nArr = std::get<0>(nPipResut);
	auto nArr_size = std::get<1>(nPipResut);
	bool IsGunner = std::get<2>(nPipResut);

	Point2D nPos
	{
		nPoint.X,
		nPoint.Y
	};

	auto nArr_iterator = make_iterator(nArr, (size_t)nArr_size);
	for (auto const& nData : nArr_iterator)
	{
		DSurface::Temp->DrawSHP
		(FileSystem::PALETTE_PAL,
			nDataBundle.Shape,
			nData,
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
		if (IsGunner)
		{
			nPos.X += nX + nBracketPosDeltaX;
			nPos.Y += nY + nBracketPosDeltaY;
		}

	}

	return 0x70A4EC;
}*/