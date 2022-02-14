#include "Body.h"
/*
; \Ext\TechnoType\Hooks.Weapons.cpp
7128C0 = TechnoTypeClass_LoadFromINI_Weapons1, 6
715B1F = TechnoTypeClass_LoadFromINI_Weapons2, 6
7177C0 = TechnoTypeClass_GetWeapon, B
7177E0 = TechnoTypeClass_GetEliteWeapon, B
717890 = TechnoTypeClass_SetWeaponTurretIndex, 8
7178B0 = TechnoTypeClass_GetWeaponTurretIndex, b
5F865F = ObjectTypeClass_Load3DArt_Turrets, 6
5F887B = ObjectTypeClass_Load3DArt_Barrels, 6
5F8084 = ObjectTypeClass_UnloadTurretArt, 6
73BD15 = UnitClass_DrawVXL_Turrets, 7
*/

namespace AresTurrents
{
	VoxelStruct get(TechnoTypeClass* pThis, int nIdx)
	{
		if (nIdx < 18)
		{
			return pThis->ChargerTurrets[nIdx];
		}
		//else
		// GetTurrentIdexFromAres
	}

	VoxelStruct get_ChargerBarrels(TechnoTypeClass* pThis, int nIdx)
	{
		if (nIdx < 18)
		{
			return pThis->ChargerBarrels[nIdx];
		}
		//else
		// GetChargerBarrelsFromAres
	}
};

DEFINE_HOOK(73BD15, UnitClass_DrawVXL_Turrets, 7)
{
	GET(UnitClass*, pThis, EBX);
	GET(int, nIdx, ESI);

	R->ECX(&AresTurrents::get(pThis->GetTechnoType(), nIdx));

	return 0x73BD1C;
}

DEFINE_HOOK(73B90E, UnitClass_DrawVXL_Barrels1, 7)
{
	GET(UnitClass*, pThis, EBX);
	GET(int, nIdx, EAX);
	GET(float, nRow, ESI);

	R->Stack(0x2C, nRow);
	auto nBarrel = &AresTurrents::get_ChargerBarrels(pThis->Type, nIdx);

	return (!nBarrel->VXL || !nBarrel->HVA) ? 0x73B94A : 0x73B928;
}
DEFINE_HOOK(73BCCD, UnitClass_DrawVXL_Barrels2, 7)
{
	GET(UnitClass*, pThis, EBX);
	GET(int, nIdx, ECX);

	R->EDX(&AresTurrents::get_ChargerBarrels(pThis->Type, nIdx));
	return 0x73BCD4;
}

DEFINE_HOOK(73BD6A, UnitClass_DrawVXL_Barrels3, 7)
{
	GET(UnitClass*, pThis, EBX);
	GET(int, nIdx, ESI);

	R->ECX(&AresTurrents::get_ChargerBarrels(pThis->Type, nIdx));
	return 0x73BD71;
}

namespace GetIdx
{
	int CurrentWeapon(TechnoTypeClass* pThis, int nIdx = 0)
	{

		if (nIdx < 18)
		{
			nIdx = pThis->TurretWeapon[nIdx];
		}

		//Ares trying to get the index from somewhere , uh ?
		//else
		//{
		//	nIdx = 
		//}

		return nIdx;
	}
}

DEFINE_HOOK(70DC70, TechnoClass_SwitchGunner, 6)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(int, nIndedx, 0x4);

	auto const pType = pThis->GetTechnoType();

	if (!pType->IsChargeTurret)
	{
		nIndedx = nIndedx < 0 || nIndedx >= pType->WeaponCount ? 0 : nIndedx;
		pThis->CurrentTurretNumber = GetIdx::CurrentWeapon(pType, nIndedx);
		pThis->CurrentWeaponNumber = nIndedx;
	}

	return 0x70DCDB;
}