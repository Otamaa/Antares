#include "Body.h"
#include <Ext/BuildingType/Body.h>

namespace AI
{
	bool IsBaseNormal(BuildingClass* pThis)
	{
		auto pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
		auto pExt = BuildingExt::ExtMap.Find(pThis);

		if (pExt->DeliveredFromSW)
			return false;

		if (pTypeExt->AI_BaseNormal.isset())
			return pTypeExt->AI_BaseNormal.Get();

		if ((pThis->Type->UndeploysInto && pThis->Type->ResourceGatherer) || pThis->IsStrange())
			return false;

		return true;
	}
}

DEFINE_HOOK(4A8FF5, MapClass_CanBuildingTypeBePlacedHere_Ignore, 5)
{
	GET(BuildingClass*, pBuilding, ESI);
	auto const pExt = BuildingExt::ExtMap.Find(pBuilding);
	return pExt->DeliveredFromSW ? 0x4A8FFA : 0x0;
}

DEFINE_HOOK(440C08, BuildingClass_Put_AIBaseNormal, 6)
{
	GET(BuildingClass*, pThis, ESI);
	R->AL(!AI::IsBaseNormal(pThis));
	return 0x440C2C;
}

DEFINE_HOOK(445A72, BuildingClass_Remove_AIBaseNormal, 6)
{
	GET(BuildingClass*, pThis, ESI);
	R->AL(!AI::IsBaseNormal(pThis));
	return 0x445A94;
}

DEFINE_HOOK(456370, BuildingClass_UnmarkBaseSpace_AIBaseNormal, 6)
{
	GET(BuildingClass*, pThis, ESI);
	R->AL(!AI::IsBaseNormal(pThis));
	return 0x456394;
}