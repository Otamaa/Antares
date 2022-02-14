#include "Body.h"

#include <HouseClass.h>
#include <YRPPGlobal.h>

//ToDo: this
DEFINE_HOOK(6F47A0, TechnoClass_GetBuildTime, 5)
{
	GET(TechnoClass*, pThis, ECX);

	auto pType = pThis->GetTechnoType();
	auto nWhatImi = pThis->WhatAmI();

	bool nCountbool = true;
	if (nWhatImi != AbstractType::Unit || !pType->Naval)
		nCountbool = false;

	double nFinalSpeed = 0.01;
	double nFinalSpeed_Pure = 0.01;

	if (auto pOwner = pThis->Owner)
	{
		auto nSpeed = pType->GetBuildSpeed();

		auto nBuildMult = pOwner->GetBuildTimeMult(pType);
			 nSpeed = int(nBuildMult * nSpeed);

		auto nBuildTimeMult = pType->BuildTimeMultiplier;
			 nSpeed = int(nBuildTimeMult * nSpeed);

		auto nPowerPercentage = (float)pOwner->GetPowerPercentage();
		auto nLowPowerPenalty = RulesGlobal->LowPowerPenaltyModifier;
		auto nMinLowPoweProductionSpeed = RulesGlobal->MinLowPowerProductionSpeed;
		auto nMaxLowPowerProductionSpeed = RulesGlobal->MaxLowPowerProductionSpeed;
		float nPowerSpeed = 1.0f - (1.0f - nPowerPercentage) * nLowPowerPenalty;
		

		if (nMinLowPoweProductionSpeed > nPowerSpeed)
			nPowerSpeed = nMinLowPoweProductionSpeed;

		if (nPowerPercentage >= 1.0 || nPowerSpeed <= nMaxLowPowerProductionSpeed)
			nMaxLowPowerProductionSpeed = nPowerSpeed;

		if (nMaxLowPowerProductionSpeed <= 0.01f)
			nMaxLowPowerProductionSpeed = 0.01f;

		nFinalSpeed = nSpeed / nMaxLowPowerProductionSpeed;
		nFinalSpeed_Pure = nFinalSpeed;

		if (RulesGlobal->MultipleFactory > 0.0)
		{
			for (int i = (pOwner->FactoryCount(nWhatImi, nCountbool) - 1); i > 0; --i)
			{
				nFinalSpeed *= RulesGlobal->MultipleFactory;
			}
		}
	}

	// BuildTime.Speed
	bool bHasBuildSpeed = true;
	if (nWhatImi == AbstractType::Building && !bHasBuildSpeed && static_cast<BuildingTypeClass*>(pType)->Wall)
		nFinalSpeed = nFinalSpeed_Pure * RulesGlobal->WallBuildSpeedCoefficient;

	R->EAX(nFinalSpeed);
	return 0x6F4955;
}

//ToDo: this
DEFINE_HOOK(711EE0, TechnoTypeClass_GetBuildSpeed, 6)
{
	GET(TechnoTypeClass*, pThis, ECX);
	// BuildTime.Speed ,BuildTime.Cost
	R->EAX(RulesGlobal->BuildSpeed * pThis->Cost / 1000.0 * 900.0);
	return 0x711EDE;
}
