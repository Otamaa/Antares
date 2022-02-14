void TechnoExt::CalculateBounty(TechnoClass* pKiller, TechnoClass* pVictim)
{
	if (pVictim && pKiller)
	{
		pVictim = pVictim->SpawnOwner || pVictim->GetTechnoType()->MissileSpawn ? pVictim->SpawnOwner : pVictim;

		if (!pVictim)
			return;

		auto pVictimTypeExt = TechnoTypeExt::ExtMap.Find(pVictim->GetTechnoType());

		if (!pVictimTypeExt->Bounty)
			return;

		auto pKillerOwner = pKiller->Owner;
		auto pVictimOwner = pVictim->Owner;

		bool ShowMessage = false;
		bool VictimOwner_Deplet = true;

		if (!pKillerOwner || !pVictimOwner || pKillerOwner->IsAlliedWith(pVictimOwner))
			return;

		auto nCost = pVictim->GetTechnoType()->GetCost();

		if (nCost == 0)
			return;

		pKillerOwner->TransactMoney(nCost);

		if (VictimOwner_Deplet)
		{
			pVictimOwner->TransactMoney(nCost * -1);
		}

		if (ShowMessage)
		{
			//draw message here ?
		}
	}
}