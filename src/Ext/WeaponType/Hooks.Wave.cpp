#include "Body.h"
#include <Ext/Techno/Body.h>

#include <HouseClass.h>
#include <WaveClass.h>
#include <WarheadTypeClass.h>

// Partially done , i guess 
// some may not function properly atm 
// let this go and work on something else atm -Otamaa
namespace AresCreateWave
{
	WaveClass* Create(CoordStruct nFrom, CoordStruct nTo, TechnoClass* pOwner, WaveType nType, AbstractClass* pTarget, byte nWeaponIdx, WeaponTypeClass* pWeapon)
	{
		auto const pData = WeaponTypeExt::ExtMap.Find(pWeapon);
		auto const pWave = GameCreate<WaveClass>(nFrom, nTo, pOwner, nType, pTarget);
		TechnoExt::ExtMap.Find(pOwner)->idxSlot_Wave = nWeaponIdx;
		WeaponTypeExt::WaveExt[pWave] = pData;
		return pWave;
	}
}

// custom beam styles
// 6FF5F5, 6
DEFINE_HOOK(6FF5F5, TechnoClass_Fire_OtherWaves, 6)
{
	GET(WeaponTypeClass* const, pSource, EBX);
	auto const pData = WeaponTypeExt::ExtMap.Find(pSource);

	//none of these , return 
	if (!pSource->IsMagBeam && !pData->Wave_IsLaser && !pData->Wave_IsBigLaser)
		return 0x6FF656;
	GET(TechnoClass* const, pThis, ESI);

	if(pThis->Wave)
		return 0x6FF656;

	GET(TechnoClass* const, pTarget, EDI);
	GET_BASE(byte, idxWeapon, 0xC);
	REF_STACK(CoordStruct const, crdSrc, 0x44);
	REF_STACK(CoordStruct const, crdTgt, 0x88);

	auto type = WaveType::Magnetron;

	if (!pSource->IsMagBeam)
	{
		type = pData->Wave_IsBigLaser
			? WaveType::BigLaser : WaveType::Laser;
	}

	pThis->Wave = AresCreateWave::Create(crdSrc, crdTgt, pThis, type, pTarget, idxWeapon, pSource);;
	return 0x6FF656;
}

DEFINE_HOOK(6FF449, TechnoClass_Fire_SonicWave, 5)
{
	GET(TechnoClass* const, pThis, ESI);

	if (pThis->Wave)
		return 0x6FF48A;

	GET(WeaponTypeClass* const, pSource, EBX);

	GET_BASE(byte, idxWeapon, 0xC);
	GET_BASE(AbstractClass*, pTarget, 0x8);

	REF_STACK(CoordStruct const, crdSrc, 0x44);
	REF_STACK(CoordStruct const, crdTgt, 0x88);

	pThis->Wave = AresCreateWave::Create(crdSrc, crdTgt, pThis, WaveType::Sonic, pTarget, idxWeapon, pSource);
	return 0x6FF48A;
}

// 763226, 6
DEFINE_HOOK(763226, WaveClass_DTOR, 6)
{
	GET(WaveClass*, Wave, EDI);
	WeaponTypeExt::WaveExt.erase(Wave);
	return 0;
}

// 760F50, 6
// complete replacement for WaveClass::Update
DEFINE_HOOK(760F50, WaveClass_Update, 6)
{
	GET(WaveClass*, pThis, ECX);

	auto pData = WeaponTypeExt::WaveExt.get_or_default(pThis);
	const WeaponTypeClass* Weap = pData->OwnerObject();

	if (!Weap) {
		return 0;
	}

	int Intensity;

	if (Weap->AmbientDamage) {
		CoordStruct coords;
		for (int i = 0; i < pThis->Cells.Count; ++i) {
			CellClass* Cell = pThis->Cells.GetItem(i);
			pThis->DamageArea(*Cell->Get3DCoords3(&coords));
		}
	}

	switch (pThis->Type)
	{
	case WaveType::Sonic:
	case WaveType::Magnetron:
		pThis->WaveAI();
		Intensity = pThis->WaveIntensity;
		--Intensity;
		pThis->WaveIntensity = Intensity;
		if (Intensity < 0) {
			pThis->UnInit();
		}
		else {
			SET_REG32(ECX, pThis);
			CALL(0x5F3E70); // ObjectClass::Update
		}
		break;
	case WaveType::BigLaser:
	case WaveType::Laser:
		Intensity = pThis->LaserIntensity;
		Intensity -= 6;
		pThis->LaserIntensity = Intensity;
		if (Intensity < 32) {
			pThis->UnInit();
		}
		break;
	default:
		break;
	}

	return 0x76101A;
}

DEFINE_HOOK(760BC2, WaveClass_Draw2, 9)
{
	GET(WaveClass*, Wave, EBX);
	GET(WORD*, dest, EBP);

	return (WeaponTypeExt::ModifyWaveColor(*dest, *dest, Wave->LaserIntensity, WeaponTypeExt::WaveColorTemp))
		? 0x760CAFu
		: 0u
		;
}

// 760DE2, 6
DEFINE_HOOK(760DE2, WaveClass_Draw3, 9)
{
	GET(WaveClass*, Wave, EBX);
	GET(WORD*, dest, EDI);

	return (WeaponTypeExt::ModifyWaveColor(*dest, *dest, Wave->LaserIntensity, WeaponTypeExt::WaveColorTemp))
		? 0x760ECBu
		: 0u
		;
}

// 75EE57, 7
DEFINE_HOOK(75EE57, WaveClass_Draw_Sonic, 7)
{
	//GET_STACK(WaveClass *, Wave, 0x4);
	GET(WORD*, src, EDI);
	GET(DWORD, offset, ECX);

	return (WeaponTypeExt::ModifyWaveColor(src[offset], *src, R->ESI(), WeaponTypeExt::WaveColorTemp))
		? 0x75EF1Cu
		: 0u
		;
}

// 7601FB, 0B
DEFINE_HOOK(7601FB, WaveClass_Draw_Magnetron2, 0B)
{
	//GET_STACK(WaveClass *, Wave, 0x8);
	GET(WORD*, src, EBX);
	GET(DWORD, offset, ECX);

	return (WeaponTypeExt::ModifyWaveColor(src[offset], *src, R->EBP(), WeaponTypeExt::WaveColorTemp))
		? 0x760285u
		: 0u
		;
}

// 760286, 5
DEFINE_HOOK(760286, WaveClass_Draw_Magnetron3, 5)
{
	return 0x7602D3;
}


// 75F38F, 6
DEFINE_HOOK(75F38F, WaveClass_DamageCell, 6)
{
	GET(WaveClass*, Wave, EBP);
	auto pData = WeaponTypeExt::WaveExt.get_or_default(Wave);
	R->EDI(R->EAX());
	R->EBX(pData->OwnerObject());
	return 0x75F39D;
}

DEFINE_HOOK_AGAIN(7601C7, WaveClass_Draw_Magnetron, 8)
DEFINE_HOOK(75EE2E, WaveClass_Draw_Green, 8)
{
	GET(int, Q, EDX);
	if (Q > 0x15F90) {
		Q = 0x15F90;
	}
	R->EDX(Q);
	return 0;
}

DEFINE_HOOK(76110B, WaveClass_RecalculateAffectedCells_Clear, 5)
{
	GET(DynamicVectorClass<CellClass*>*, pCell, EBP);
	pCell->Clear();
	pCell->Count = 0;
	return 0x761110;
}

DEFINE_HOOK(7609E3, WaveClass_Draw_NodLaser_Details, 5)
{
	R->EAX(2);
	return 0x7609E8;
}

//set color here to reduce lookup
DEFINE_HOOK(75FA29, WaveClass_Draw_Colors, 6)
{
	GET(WaveClass*, pThis, ESI);

	WeaponTypeExt::WaveColorTemp = static_cast<DWORD>(WeaponTypeExt::SelectColor(pThis));
	return 0;
}

DEFINE_HOOK(75F46E, WaveClass_DamageCell_Wall, 6)
{
	GET(WeaponTypeClass*, pWeapon, EBX);
	return pWeapon->Warhead->Wall ? 0x0 : 0x75F47C;
}

DEFINE_HOOK(762B62, WaveClass_Update_Beam, 6)
{
	GET(WaveClass*, Wave, ESI);

	TechnoClass* Firer = Wave->Owner;
	TechnoClass* Target = Wave->Target;

	if (!Target || !Firer || Wave->WaveIntensity == (1.0 / 0.050000001) || Firer->Target != Target){
		return 0x762C40;
	}

	int weaponIdx = TechnoExt::ExtMap.Find(Firer)->idxSlot_Wave;
	auto pData = WeaponTypeExt::WaveExt.get_or_default(Wave);
		 
	if (!pData) {
		return 0;
	}

	if (Wave->Type == WaveType::Magnetron) {

		if (!Firer->IsCloseEnough(Target, weaponIdx)) {
			return 0x762C40;
		}
	}
	else {
		auto nFirerCoord = Firer->GetCoords();
		auto nTargetCoord = Target->GetCoords();
		if ((pData->OwnerObject()->Range) <
			(int)(nFirerCoord.DistanceFrom(nTargetCoord) / 1.414213562373095)) {
			return 0x762C40;
		}
	}

	CoordStruct xyzSrc = Firer->GetFLH(weaponIdx, CoordStruct::Empty);
	CoordStruct xyzTgt = Target->GetAltCoords(); // not GetCoords() !
	bool reversed = pData->IsWaveReversedAgainst(Target);

	if (Wave->Type == WaveType::Magnetron) {
		reversed
			? Wave->DrawMag(xyzTgt, xyzSrc)
			: Wave->DrawMag(xyzSrc, xyzTgt);
	}
	else {
		reversed
			? Wave->DrawNonMag(xyzTgt, xyzSrc)
			: Wave->DrawNonMag(xyzSrc, xyzTgt);
	}

	return 0x762D57;
}