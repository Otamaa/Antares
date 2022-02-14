#include "Body.h"

// need to implement new SW stuffs 
///DynamicVectorClass<BuildingTypeClass*> SW_Batt_OverPower;
///DynamicVectorClass<BuildingTypeClass*> SW_Batt_KeepOnline;
/// 
/*
static void Activate(BuildingClass* pThis)
{
    auto Iter = make_iterator(SW_Batt_OverPower);
    if (Iter.contains(pThis->Type))
    {
        pThis->IsOverpowered = true;
    }
}

static void GetPowerFromBattery(BuildingClass* pThis)
{

}
//44019D = BuildingClass_Update_Battery, 6

{
  BuildingClass *v1; // esi
  unsigned int Ext; // ecx
  BuildingTypeClass **v3; // eax
  BuildingTypeClass **v4; // ecx

  v1 = a1->_ESI.data;
  Ext = v1->Owner->HouseClassExt;
  v3 = *(Ext + 0x80); //vec.end()
  v4 = *(Ext + 0x7C); //++vec.first()
  if ( v4 != v3 )
  {
    while ( *v4 != v1->Type )
    {
      if ( ++v4 == v3 )
        return 0;
    }
    v1->IsOverpowered = 1;
  }
  return 0;
}
*/

//4555D5 = BuildingClass_IsPowerOnline_KeepOnline, 5
/*
{
  BuildingClass *v1; // edx
  unsigned int houseExt; // ecx
  BuildingTypeClass **v3; // eax
  BuildingTypeClass **v4; // ecx
  BuildingTypeClass *v5; // edx
  bool v6; // zf
  __int64 result; // rax

  v1 = a1->_ESI.data;
  houseExt = v1->Owner->HouseClassExt;
  v3 = *(houseExt + 116);
  v4 = *(houseExt + 112);
  if ( v4 != v3 )
  {
    v5 = v1->Type;
    do
    {
      if ( *v4 == v5 )
        break;
      ++v4;
    }
    while ( v4 != v3 );
  }
  v6 = v4 == v3;

  a1->_EDI.data = v6 ? 0:2;

  return 4555D5;
}
508C7F = HouseClass_UpdatePower_Auxiliary, 6
508E66 = HouseClass_UpdateRadar_Battery, 8
*/