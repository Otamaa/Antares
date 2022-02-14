#include "Body.h"
#include <Ext/Building/Body.h>
#include <Ext/TechnoType/Body.h>

#include <VocClass.h>

DEFINE_HOOK(4FB2FD, HouseClass_UnitFromFactory_BuildingSlam, 6)
{
	GET(TechnoClass*, pThis, ESI);
	auto pTechnoTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	VocClass::PlayGlobal(pTechnoTypeExt->Slam_Sound.Get(RulesClass::Instance->BuildingSlam), 0x2000, 1.0);
	return 0x4FB319;
}