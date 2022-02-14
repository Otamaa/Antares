#include "Body.h"

#include <algorithm>

DEFINE_HOOK(53CC6E, IonBlastClass_Update, 6)
{
	GET(IonBlastClass *, IB, EBX);
	return (!WarheadTypeExt::IonExt.contains(IB))
		? 0
		: 0x53CE0A
	;
}
//53CB91 = IonBlastClass_DTOR, 6
//53CC63 = IonBlastClass_Update_Beam, 6
DEFINE_HOOK(53CC0D, IonBlastClass_Update_DTOR, 5)
{
	GET(IonBlastClass *, IB, EBX);
	WarheadTypeExt::IonExt.erase(IB);
	return 0;
}

DEFINE_HOOK(53CBF5, IonBlastClass_Update_Duration, 5)
{
	GET(IonBlastClass *, IB, EBX);

	int Ripple_Radius = 79;
	if(auto pData = WarheadTypeExt::IonExt.get_or_default(IB)) {
		Ripple_Radius = std::min(Ripple_Radius, pData->Ripple_Radius + 1);
	}

	if(IB->Lifetime < Ripple_Radius) {
//		++IB->Lifetime;
		return 0x53CC3A;
	} else {
		return 0x53CBFA;
	}
}
