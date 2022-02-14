#include "Body.h"
#include <TunnelLocomotionClass.h>
#include <FootClass.h>
#include <YRPPGlobal.h>

//ToDo: all of these
namespace AresHadleTunnelLocoStuffs
{
	void Handle(FootClass* pOwner, bool bDigIn = false, bool PlayAnim = false)
	{
		//Differentiate DigIn and DigOut Sound
		//not sure if these on correct position atm ! -Otamaa
		auto nSound = bDigIn ? RulesGlobal->DigSound : RulesGlobal->DigSound;
		auto nLoc = pOwner->Location;

		VocClass::PlayAt(nSound, nLoc);

		if (PlayAnim)
		{
			if (auto pAnimType = RulesGlobal->Dig)
			{
				if (auto pAnim = GameCreate<AnimClass>(pAnimType, nLoc))
				{
					pAnim->Owner = pOwner->Owner;
				}
			}
		}

	}

}

DEFINE_HOOK(728EF0, TunnelLocomotionClass_ILocomotion_Process_Dig, 5)
{
	GET(FootClass*, pFoot, EAX);

	AresHadleTunnelLocoStuffs::Handle(pFoot, true, true);
	return 0x728F74;
}

DEFINE_HOOK(7292CF, TunnelLocomotionClass_sub_7291F0_Dig, 8)
{
	GET(RepeatableTimerStruct*, pTimer, EDX);
	GET(TunnelLocomotionClass*, pThis, ESI);
	GET(int, nTimeLeft, EAX);

	pTimer->Start(nTimeLeft);
	AresHadleTunnelLocoStuffs::Handle(pThis->LinkedTo, true, true);
	return 0x729365;

}

DEFINE_HOOK(7293DA, TunnelLocomotionClass_sub_729370_Dig, 6)
{
	GET(FootClass*, pFoot, ECX);

	AresHadleTunnelLocoStuffs::Handle(pFoot, true, true);
	return 0x72945E;
}

DEFINE_HOOK(7297C4, TunnelLocomotionClass_sub_729580_Dig, 6)
{
	GET(FootClass*, pFoot, EAX);

	AresHadleTunnelLocoStuffs::Handle(pFoot, false, false);
	return 0x7297F3;
}

DEFINE_HOOK(7299A9, TunnelLocomotionClass_sub_7298F0_Dig, 5)
{
	GET(TunnelLocomotionClass*, pThis, ESI);

	AresHadleTunnelLocoStuffs::Handle(pThis->LinkedTo, false, true);
	return 0x729A34;
}

DEFINE_HOOK(72920C, TunnelLocomotionClass_Turning, 9)
{
	GET(TunnelLocomotionClass*, pThis, ESI);

	if (pThis->_CoordsNow.X || pThis->_CoordsNow.Y || pThis->_CoordsNow.Z)
		return 0;

	pThis->__ZAdjust = 7;
	return 0x729369;
}