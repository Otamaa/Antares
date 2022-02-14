/*
4368C9 = BuildingLightClass_Update_Trigger, 5
5F57B5 = ObjectClass_ReceiveDamage_Trigger, 6
744745 = UnitClass_RegisterDestruction_Trigger, 5
*/

/* New Trigger Event is added !
DEFINE_HOOK(7032B0 ,TechnoClass_RegisterLoss_Trigger, 6)
{
	GET(TechnoClass*, pThis, ESI);
	GET(TechnoClass*, pAttacker, EDI);

	if (pThis->IsAlive)
	{
		if (pAttacker)
		{
			if (auto pTag = pThis->AttachedTag)
			{
				pTag->RaiseEvent(static_cast<TriggerEvent>(0x55), pThis, CellStruct::Empty, false, pAttacker);
			}
		}
	}

	return 0;
}


DEFINE_HOOK(702DD6 ,TechnoClass_RegisterDestruction_Trigger, 6)
{
	GET(TechnoClass*, pThis, ESI);
	GET(TechnoClass*, pAttacker, EDI);

	if (pThis->IsAlive)
	{
		if (pAttacker)
		{
			if (auto pTag = pThis->AttachedTag)
			{
				pTag->RaiseEvent(static_cast<TriggerEvent>(0x55), pThis, CellStruct::Empty, false, pAttacker);
			}
		}
	}

	return 0;
}
*/