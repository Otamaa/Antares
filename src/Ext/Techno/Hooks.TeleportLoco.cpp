#include "Body.h"
#include <TeleportLocomotionClass.h>

DEFINE_HOOK(719CCE, TeleportLocomotionClass_Load_TimeLeft, 7)
{
	if (Debug::bLog)
	{
		GET(TeleportLocomotionClass*, pThis, ESI);
		GET_STACK(void*, pCaller, 0xC);

		Debug::Log(__FUNCTION__ ": pTele = %X, TimeLeft = %d, caller = %X\n"

			, pThis, pThis->Timer.GetTimeLeft(), pCaller
		);
	}

	//return 0x719CD5;
	return 0;
}