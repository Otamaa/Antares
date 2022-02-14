/*
43C326 = BuildingClass_ReceivedRadioCommand_QueryCanEnter_Tunnel, A
43C716 = BuildingClass_ReceivedRadioCommand_RequestCompleteEnter_Tunnel, 6
44351A = BuildingClass_ActionOnObject_Tunnel, 6
44731C = BuildingClass_GetActionOnObject_Tunnel, 6
44D8A7 = BuildingClass_Mi_Unload_Tunnel, 6
442DF2 = BuildingClass_Demolish_Tunnel, 6
44A37F = BuildingClass_Mi_Selling_Tunnel, 6
//require tunnel vector that hold FootClass inside then clearing it
71A995 = TemporalClass_Update_Tunnel, 5

DEFINE_HOOK(73F606, UnitClass_IsCellOccupied_Tunnel, 6)
{
	GET(BuildingClass*, pBuilding, ESI);

	bool bTunnel = false;

	return pBuilding->Type->UnitAbsorb || bTunnel ? 0x73F616 : 0x73F628;
}

741CE5 = UnitClass_SetDestination_Tunnel, 6
73A23F = UnitClass_UpdatePosition_Tunnel, 6
51ED8E = InfantryClass_GetActionOnObject_Tunnel, 6
51A2AD = InfantryClass_UpdatePosition_Tunnel, 9
*/