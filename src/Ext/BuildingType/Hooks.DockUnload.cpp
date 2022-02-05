#include "Body.h"

#include <UnitClass.h>
#include <BuildingClass.h>

/*	Hook pack for DockUnload , put them onto one hook file for easy diagnostic later on -Otamaa */

namespace Get
{

	DirStruct UnloadFacing(UnitClass* pThis)
	{
		int nFacingRet = 0x4000;
		if (pThis->HasAnyLink())
		{
			// fetch very fist item on RadIoLinks item most likely
			// equal to pThis->RadioLinks[0] 
			if (auto pLink = *pThis->RadioLinks.Items)
			{
				if (auto pBld = specific_cast<BuildingClass*>(pLink))
				{
					auto const &pBldExt = BuildingTypeExt::ExtMap.Find(pBld->Type);
					if (pBldExt && pBldExt->DockUnload_Facing.isset())
						nFacingRet = pBldExt->DockUnload_Facing.Get() << 11;
				}
			}
		}

		return DirStruct(nFacingRet);
	}

	CellStruct UnloadCell(BuildingClass* pThis)
	{
		CellStruct nBuff = { 3, 1 };
		if (auto const& pBldExt = BuildingTypeExt::ExtMap.Find(pThis->Type))
		{
			if (pBldExt->DockUnload_Cell.isset())
				nBuff = pBldExt->DockUnload_Cell.Get();
		}

		return nBuff;
	}
}

DEFINE_HOOK(7376D9, UnitClass_ReceivedRadioCommand_DockUnload_Facing, 5)
{
	GET(UnitClass*, pUnit, ESI);
	GET(DirStruct*, nCurrentFacing, EAX);

	auto nValue = nCurrentFacing->value();
	auto nDecidedFacing = Get::UnloadFacing(pUnit);

	if (nValue == nDecidedFacing.value())
		return 0x73771B;

	pUnit->Locomotor->Do_Turn(nDecidedFacing);

	return 0x73770C;
}

DEFINE_HOOK(73DF66, UnitClass_Mi_Unload_DockUnload_Facing, 5)
{
	GET(UnitClass*, pUnit, ESI);
	GET(DirStruct*, nCurrentFacing, EAX);

	auto nValue = nCurrentFacing->value();
	auto nDecidedFacing = Get::UnloadFacing(pUnit);

	if (nValue == nDecidedFacing.value() || pUnit->IsRotating)
		return 0x73DFBD;

	pUnit->Locomotor->Do_Turn(nDecidedFacing);

	return 0x73DFB0;
}

DEFINE_HOOK(43CA80, BuildingClass_ReceivedRadioCommand_DockUnloadCell, 7)
{
	GET(CellStruct*, pCell, EAX);
	GET(BuildingClass*, pThis, ESI);

	auto nBuff = Get::UnloadCell(pThis);
	R->DX(pCell->X + nBuff.X);
	R->AX(pCell->Y + nBuff.Y);

	return 0x43CA8D;
}