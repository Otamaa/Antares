#include "Body.h"
#include "../Techno/Body.h"
#include <EBolt.h>
#include <ParticleSystemTypeClass.h>
#include <ParticleSystemClass.h>

// Ares replace all the look-up with global var 
// to minimize lookup 
// will follow later
DEFINE_HOOK(6FD469, TechnoClass_FireEBolt, 9)
{
	//GET(TechnoClass*, pThis, EDI);
	GET_STACK(WeaponTypeClass*, pWeapon, STACK_OFFS(0x30, -0x8));

	R->EAX(WeaponTypeExt::CreateBolt(pWeapon));
	R->ESI(0);

	return 0x6FD480;
}

DEFINE_HOOK(4C2951, EBolt_DTOR, 5)
{
	GET(EBolt*, Bolt, ECX);
	WeaponTypeExt::BoltExt.erase(Bolt);
	return 0;
}

DEFINE_HOOK(4C1F2F, EBolt_Draw_Colors, B)
{
	GET(EBolt*, Bolt, ECX);
	GET_BASE(int, nIndex, 0x20);

	auto nDraw = FileSystem::PALETTE_PAL();
	WeaponTypeExt::DecidedBoltColor1 = nDraw->MidPoint[nIndex];
	WeaponTypeExt::DecidedBoltColor2 = nDraw->MidPoint[nIndex];
	WeaponTypeExt::DecidedBoltColor3 = nDraw->MidPoint[15];

	if (auto pData = WeaponTypeExt::BoltExt.get_or_default(Bolt))
	{
		auto& clr1 = pData->Bolt_Color1;
		auto& clr2 = pData->Bolt_Color2;
		auto& clr3 = pData->Bolt_Color3;

		if (clr1.isset())
			WeaponTypeExt::DecidedBoltColor1 = Drawing::Color16bit(clr1);
		if (clr2.isset())
			WeaponTypeExt::DecidedBoltColor2 = Drawing::Color16bit(clr2);
		if (clr3.isset())
			WeaponTypeExt::DecidedBoltColor3 = Drawing::Color16bit(clr3);
	}

	return 0;
}

DEFINE_HOOK(4C24BE, EBolt_Draw_Color1, 5)
{
	R->EAX(WeaponTypeExt::DecidedBoltColor1);
	return 0x4C24E4;
}

DEFINE_HOOK(4C25CB, EBolt_Draw_Color2, 5)
{
	R->Stack<int>(0x18, WeaponTypeExt::DecidedBoltColor2);
	return 0x4C25FD;
}

DEFINE_HOOK(4C26CF, EBolt_Draw_Color3, 5)
{
	R->EBX(R->EBX() - 2);
	R->EAX(WeaponTypeExt::DecidedBoltColor3);
	return 0x4C26EE;
}

DEFINE_HOOK(4C2AFF, EBolt_Fire_Particles, 5)
{
	GET(EBolt*, Bolt, ESI);

	auto ParticleSys = RulesClass::Instance->DefaultSparkSystem;
	if (auto pData = WeaponTypeExt::BoltExt.get_or_default(Bolt))
	{
		if (pData->Bolt_ParticleSystem.isset())
			ParticleSys = pData->Bolt_ParticleSystem.Get();
	}

	if (ParticleSys)
		GameCreate<ParticleSystemClass>(ParticleSys, Bolt->Point2, nullptr , Bolt->Owner , CoordStruct::Empty, Bolt->Owner ? Bolt->Owner->GetOwningHouse() : nullptr);

	// causing EIP 0000000 crash wtf 
	//return 0x4C2B35;

	R->EAX<void*>(nullptr); // skip 
	return 0x4C2B0C;
}