namespace WHVoiceDie
{
	bool oVerrideVoiceDie = false;
	int oVerridedVoiceDie = -1;

	bool oVerrideDieSound = false;
	int oVerridedDieSound = -1;
};

DEFINE_HOOK(702185, TechnoClass_ReceiveDamage_OverrideVoiceDie, 6)
{
	//GET_STACK(WarheadTypeClass* const, pWarhead, 0xD0);
	if (WHVoiceDie::oVerrideVoiceDie)
	{
		GET(TechnoClass*, pThis, ESI);
		auto nWhere = pThis->GetCoords();
		VocClass::PlayAt(WHVoiceDie::oVerridedVoiceDie, nWhere);
		return 0x702190;
	}

	return 0;
}
DEFINE_HOOK(7021F5, TechnoClass_ReceiveDamage_OverrideDieSound, 6)
{
	//GET_STACK(WarheadTypeClass* const, pWarhead, 0xD0);
	if (WHVoiceDie::oVerrideDieSound)
	{
		GET(TechnoClass*, pThis, ESI);
		auto nWhere = pThis->GetCoords();
		VocClass::PlayAt(WHVoiceDie::oVerridedDieSound, nWhere);
		return 0x702200;
	}
	return 0;
}