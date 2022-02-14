#include "Body.h"

#include <HouseClass.h>

#include <Ext/TechnoType/Body.h>

DEFINE_HOOK(70AA60, TechnoClass_DrawExtraInfo, 6)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(Point2D*, pPoint, 0x8);
	//	GET_STACK(unsigned int , nFrame, 0x4);
	GET_STACK(RectangleStruct*, pRect, 0xC);

	if (auto pBuilding = specific_cast<BuildingClass*>(pThis))
	{
		auto const pType = pBuilding->Type;
		auto const pOwner = pBuilding->Owner;
		auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType);

		if (!pType || !pOwner)
			return 0x70AD4C;

		auto DrawTheStuff = [&pPoint, &pOwner, &pRect](const wchar_t* pFormat)
		{
			auto nPoint = *pPoint;
			//DrawingPart
			RectangleStruct nTextDimension;
			Drawing::GetTextDimensions(&nTextDimension, pFormat, nPoint, TextPrintType::Center | TextPrintType::FullShadow | TextPrintType::Efnt, 4, 2);
			auto nIntersect = Drawing::Intersect(nTextDimension, *pRect);
			auto nColorInt = pOwner->Color.ToInit();//0x63DAD0

			DSurface::Temp->FillRect(&nIntersect, (COLORREF)0);
			DSurface::Temp->DrawRect(&nIntersect, (COLORREF)nColorInt);
			Point2D nRet;
			CommonFunction::Simple_Text_Print_Wide(&nRet, pFormat, DSurface::Temp.get(), pRect, &nPoint, (COLORREF)nColorInt, (COLORREF)0, TextPrintType::Center | TextPrintType::FullShadow | TextPrintType::Efnt, true);
		};

		bool IsAlly = pOwner->IsAlliedWith(HouseClass::Player);;
		bool IsObserver = HouseClass::Observer || HouseClass::IsPlayerObserver();
		bool isFake = pTypeExt->Is_Fake.Get();
		bool bReveal = pThis->DisplayProductionTo.Contains(HouseClass::Player);

		if (IsAlly || IsObserver || bReveal)
		{
			if (isFake)
				DrawTheStuff(StringTable::LoadStringA("TXT_FAKE"));

			if (pType->PowerBonus > 0)
			{
				auto pDrainFormat = StringTable::LoadStringA("TXT_POWER_DRAIN2");
				wchar_t pOutDraimFormat[0x80];
				auto pDrain = (int)pOwner->Power_Drain();
				auto pOutput = (int)pOwner->Power_Output();
				swprintf_s(pOutDraimFormat, pDrainFormat, pOutput, pDrain);

				DrawTheStuff(pOutDraimFormat);
			}

			if (pType->Storage > 0)
			{
				auto pMoneyFormat = StringTable::LoadStringA("TXT_MONEY_FORMAT_1");
				wchar_t pOutMoneyFormat[0x80];
				auto nMoney = pOwner->Available_Money();
				swprintf_s(pOutMoneyFormat, pMoneyFormat, nMoney);

				DrawTheStuff(pOutMoneyFormat);
			}

			if (pThis->IsPrimaryFactory)
				DrawTheStuff(StringTable::LoadStringA((pType->GetFoundationWidth() != 1) ? "TXT_PRIMARY" : "TXT_PRI"));
		}
	}

	return 0x70AD4C;
}

DEFINE_HOOK(4AE670, DisplayClass_GetToolTip_EnemyUIName, 8)
{
	enum { SetUIName = 0x4AE678 };

	GET(ObjectClass*, pObject, ECX);

	auto pDecidedUIName = pObject->GetUIName();
	auto pTechno = generic_cast<TechnoClass*>(pObject);
	auto pTechnoType = pObject->GetTechnoType();

	if (pTechno && pTechnoType && !pObject->IsDisguised())
	{
		bool bReveal = (pTechno->WhatAmI() == AbstractType::Building) && pTechno->DisplayProductionTo.Contains(HouseClass::Player);
		bool IsAlly = true;
		bool IsCivilian = false;
		bool IsObserver = HouseClass::Observer || HouseClass::IsPlayerObserver();

		if (auto pOwnerHouse = pTechno->GetOwningHouse())
		{
			IsAlly = pOwnerHouse->IsAlliedWith(HouseClass::Player);
			IsCivilian = (pOwnerHouse == HouseClass::FindCivilianSide()) || pOwnerHouse->IsNeutral();
		}

		if (!IsAlly && !IsCivilian && !IsObserver)
		{
			if (!bReveal)
			{
				auto pTechnoTypeExt = TechnoTypeExt::ExtMap.Find(pTechnoType);
				if (auto pEnemyUIName = pTechnoTypeExt->EnemyUIName.Get().Text)
				{
					pDecidedUIName = pEnemyUIName;
				}
			}
		}
	}

	R->EAX(pDecidedUIName);
	return SetUIName;
}