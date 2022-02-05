#include "CursorTypes.h"

#include <Utilities/TemplateDef.h>

#include <UnitClass.h>

#include <Ext/TechnoType/Body.h>
#include <Ext/BuildingType/Body.h>

Enumerable<CursorType>::container_t Enumerable<CursorType>::Array;
std::map<Action, int> CursorType::ActionCursor_Map;
MouseCursor* CursorType::TempCursor;

const char* Enumerable<CursorType>::GetMainSection()
{
	return "MouseCursors";
}

//init this somewhere more early than rules , because game also need to access the cursor !
void CursorType::AddDefaults()
{
	FindOrAllocate_("Default", MouseCursor::DefaultCursors()[0]);

#pragma region MoveCursor
	FindOrAllocate_("MoveN", MouseCursor::DefaultCursors()[1]);
	FindOrAllocate_("MoveNE", MouseCursor::DefaultCursors()[2]);
	FindOrAllocate_("MoveE", MouseCursor::DefaultCursors()[3]);
	FindOrAllocate_("MoveSE", MouseCursor::DefaultCursors()[4]);
	FindOrAllocate_("MoveS", MouseCursor::DefaultCursors()[5]);
	FindOrAllocate_("MoveSW", MouseCursor::DefaultCursors()[6]);
	FindOrAllocate_("MoveW", MouseCursor::DefaultCursors()[7]);
	FindOrAllocate_("MoveNW", MouseCursor::DefaultCursors()[8]);
#pragma endregion

#pragma region NoMoveCursor
	FindOrAllocate_("NoMoveN", MouseCursor::DefaultCursors()[9]);
	FindOrAllocate_("NoMoveNE", MouseCursor::DefaultCursors()[10]);
	FindOrAllocate_("NoMoveE", MouseCursor::DefaultCursors()[11]);
	FindOrAllocate_("NoMoveSE", MouseCursor::DefaultCursors()[12]);
	FindOrAllocate_("NoMoveS", MouseCursor::DefaultCursors()[13]);
	FindOrAllocate_("NoMoveSW", MouseCursor::DefaultCursors()[14]);
	FindOrAllocate_("NoMoveW", MouseCursor::DefaultCursors()[15]);
	FindOrAllocate_("NoMoveNW", MouseCursor::DefaultCursors()[16]);
#pragma endregion

	FindOrAllocate_("Select", MouseCursor::DefaultCursors()[17]);

	FindOrAllocate_("Move", MouseCursor::DefaultCursors()[18]);
	FindOrAllocate_("NoMove", MouseCursor::DefaultCursors()[19]);

	FindOrAllocate_("Attack", MouseCursor::DefaultCursors()[20]);
	FindOrAllocate_("AttackOutOfRange", MouseCursor::DefaultCursors()[21]);

	FindOrAllocate_("AreaGuard", MouseCursor::DefaultCursors()[22]);

	FindOrAllocate_("Game_Tote", MouseCursor::DefaultCursors()[23]);
	FindOrAllocate_("NoTote", MouseCursor::DefaultCursors()[24]);

	FindOrAllocate_("Enter", MouseCursor::DefaultCursors()[25]);
	FindOrAllocate_("NoEnter", MouseCursor::DefaultCursors()[26]);

	FindOrAllocate_("Deploy", MouseCursor::DefaultCursors()[27]);
	FindOrAllocate_("NoDeploy", MouseCursor::DefaultCursors()[28]);
	FindOrAllocate_("Undeploy", MouseCursor::DefaultCursors()[29]);

	FindOrAllocate_("Sell", MouseCursor::DefaultCursors()[30]);
	FindOrAllocate_("SellUnit", MouseCursor::DefaultCursors()[31]);
	FindOrAllocate_("NoSell", MouseCursor::DefaultCursors()[32]);

	FindOrAllocate_("Repair", MouseCursor::DefaultCursors()[33]);
	FindOrAllocate_("EngineerRepair", MouseCursor::DefaultCursors()[34]);
	FindOrAllocate_("NoRepair", MouseCursor::DefaultCursors()[35]);

	FindOrAllocate_("Waypoint", MouseCursor::DefaultCursors()[36]);
	FindOrAllocate_("Disguise", MouseCursor::DefaultCursors()[37]);

	FindOrAllocate_("IvanBomb", MouseCursor::DefaultCursors()[38]);
	FindOrAllocate_("MindControl", MouseCursor::DefaultCursors()[39]);
	FindOrAllocate_("RemoveSquid", MouseCursor::DefaultCursors()[40]);

	FindOrAllocate_("Crush", MouseCursor::DefaultCursors()[41]);

	FindOrAllocate_("SpyTech", MouseCursor::DefaultCursors()[42]);
	FindOrAllocate_("SpyPower", MouseCursor::DefaultCursors()[43]);

	FindOrAllocate_("Cursor_2C", MouseCursor::DefaultCursors()[44]);

	FindOrAllocate_("GIDeploy", MouseCursor::DefaultCursors()[45]);
	FindOrAllocate_("Ripple", MouseCursor::DefaultCursors()[46]);

	FindOrAllocate_("ParaDrop", MouseCursor::DefaultCursors()[47]); 

	FindOrAllocate_("Rallypoint", MouseCursor::DefaultCursors()[48]);
	FindOrAllocate_("CloseWaypoint", MouseCursor::DefaultCursors()[49]);

	FindOrAllocate_("LightningStorm", MouseCursor::DefaultCursors()[50]);
	FindOrAllocate_("Detonate", MouseCursor::DefaultCursors()[51]);
	FindOrAllocate_("Demolish", MouseCursor::DefaultCursors()[52]);
	FindOrAllocate_("Nuke", MouseCursor::DefaultCursors()[53]);

	FindOrAllocate_("BlueCursor", MouseCursor::DefaultCursors()[54]);
	FindOrAllocate_("Power", MouseCursor::DefaultCursors()[55]);
	FindOrAllocate_("NoBlueCursor", MouseCursor::DefaultCursors()[56]);

	FindOrAllocate_("IronCurtain", MouseCursor::DefaultCursors()[57]);
	FindOrAllocate_("Chronosphere", MouseCursor::DefaultCursors()[58]);
	FindOrAllocate_("Disarm", MouseCursor::DefaultCursors()[59]);
	FindOrAllocate_("Disallowed", MouseCursor::DefaultCursors()[60]);

	FindOrAllocate_("Scroll", MouseCursor::DefaultCursors()[61]);
	FindOrAllocate_("ScrollESW", MouseCursor::DefaultCursors()[62]);
	FindOrAllocate_("ScrollSW", MouseCursor::DefaultCursors()[63]);
	FindOrAllocate_("ScrollNSW", MouseCursor::DefaultCursors()[64]);
	FindOrAllocate_("ScrollNW", MouseCursor::DefaultCursors()[65]);
	FindOrAllocate_("ScrollNEW", MouseCursor::DefaultCursors()[66]);
	FindOrAllocate_("ScrollNE", MouseCursor::DefaultCursors()[67]);
	FindOrAllocate_("ScrollNES", MouseCursor::DefaultCursors()[68]);
	FindOrAllocate_("ScrollES", MouseCursor::DefaultCursors()[69]);

	FindOrAllocate_("Protect2", MouseCursor::DefaultCursors()[70]);
	FindOrAllocate_("AttackOutOfRange2", MouseCursor::DefaultCursors()[71]);
	FindOrAllocate_("Deployinh", MouseCursor::DefaultCursors()[72]);
	FindOrAllocate_("InfantryAbsorb", MouseCursor::DefaultCursors()[73]);

	FindOrAllocate_("NoMindControl", MouseCursor::DefaultCursors()[74]);
	FindOrAllocate_("NoMindControlAlly", MouseCursor::DefaultCursors()[75]);

	FindOrAllocate_("SmallGuard", MouseCursor::DefaultCursors()[76]);
	FindOrAllocate_("SmallAttack", MouseCursor::DefaultCursors()[77]);

	FindOrAllocate_("Beacon", MouseCursor::DefaultCursors()[78]);

	FindOrAllocate_("ForceShield", MouseCursor::DefaultCursors()[79]);
	FindOrAllocate_("NoForceShield", MouseCursor::DefaultCursors()[80]);

	FindOrAllocate_("GeneticMutator", MouseCursor::DefaultCursors()[81]);
	FindOrAllocate_("AirStrike", MouseCursor::DefaultCursors()[82]);
	FindOrAllocate_("PsychicDominator", MouseCursor::DefaultCursors()[83]);
	FindOrAllocate_("PsychicReveal", MouseCursor::DefaultCursors()[84]);
	FindOrAllocate_("SpyPlane", MouseCursor::DefaultCursors()[85]);

	// additional default
	FindOrAllocate_("Tote", { 329,10,4,-1,-1,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //86
	FindOrAllocate_("EngineerDamage", { 299,10,4,-1,-1,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //87
	FindOrAllocate_("TogglePower", { 339,6,0,-1,-1,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //88
	FindOrAllocate_("NoTogglePower", { 384,1,0,-1,-1,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //89
	FindOrAllocate_("InfantryHeal", { 355,1,0,-1,-1,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //90
	FindOrAllocate_("UnitRepair", { 150,20,4,-1,-1,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //91
	FindOrAllocate_("TakeVehicle", { 89,10,4,100,10,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //92
	FindOrAllocate_("Sabotage", { 89,10,4,100,10,MouseHotSpotX::Center,MouseHotSpotY::Middle }); //93
	FindOrAllocate_("RepairTrench", { 150,20,4,-1,-1,MouseHotSpotX::Center,MouseHotSpotY::Middle });//94

	Debug::Log("MouseCursorType added [%d] , default !\n", Array.size());
}

// should i check section ? , i dunno
// lets keep this as it is atm -Otamaa
void CursorType::LoadFromINI(CCINIClass* pINI)
{
	const char* pName = this->Name.data();

	if (!_strcmpi(pName, "Default"))
		return;

	INI_EX exINI(pINI);
	this->Cursor.Read(exINI, Enumerable<CursorType>::GetMainSection(), pName, false);	
}

void CursorType::LoadFromINIList(CCINIClass* pINI)
{
	const char* pSection = GetMainSection();
	for (int i = 0; i < pINI->GetKeyCount(pSection); ++i)
	{
		const char* pKey = pINI->GetKeyName(pSection, i);

		if (CCINIClass::IsBlank(pKey))
			continue;

		if (auto pAlloc = FindOrAllocate(pKey))
			pAlloc->LoadFromINI(pINI);

		Debug::Log("Allocating CursorType with Name[%s] at [%d] \n", pKey, i);
	}

	{
		for (size_t i = 0; i < Array.size(); ++i)
		{
			auto nArr = Array[i].get();
			auto nCur = nArr->Cursor.Get();
			Debug::Log("Cursor [%d=%s] <Frame %d,Count %d,Interval %d,MiniFrame %d,MiniCount %d,HotX %d,HotY %d> \n", i,
				nArr->Name.data(),
				nCur.Frame,
				nCur.Count,
				nCur.Interval,
				nCur.MiniFrame,
				nCur.MiniCount,
				nCur.HotX,
				nCur.HotY
			);
		}
	}
}

template <typename T>
void CursorType::Serialize(T& Stm)
{
	Stm
		.Process(this->Cursor)
		;
}

void CursorType::LoadFromStream(AresStreamReader& Stm)
{
	this->Serialize(Stm);
}

void CursorType::SaveToStream(AresStreamWriter& Stm)
{
	this->Serialize(Stm);
}

bool CursorType::LoadGlobals(AresStreamReader& Stm)
{
	Clear();

	size_t Count = 0;
	if (!Stm.Load(Count)) {
		return false;
	}

	for (size_t i = 0; i < Count; ++i) {
		void* oldPtr = nullptr;
		decltype(Name) name;
		if (!Stm.Load(oldPtr) || !Stm.Load(name)) {
			return false;
		}

		auto newPtr = FindOrAllocate(name);
		AresSwizzle::Instance.RegisterChange(oldPtr, newPtr);

		newPtr->LoadFromStream(Stm);
	}

	
	return Stm
		.Process(TempCursor)
		.Process(ActionCursor_Map)
		.Success()
		;
}

bool CursorType::SaveGlobals(AresStreamWriter& Stm)
{
	Stm.Save(Array.size());

	for (const auto& item : Array) {
		// write old pointer and name, then delegate
		Stm.Save(item.get());
		Stm.Save(item->Name);
		item->SaveToStream(Stm);
	}

	return Stm
		.Process(TempCursor)
		.Process(ActionCursor_Map)
		.Success()
		;
}

DEFINE_HOOK(5BDC8C, MouseClass_UpdateCursor, 7)
{
	GET(size_t, nCursor, EAX);
	GET(MouseClass*, pMouse, EBP);
	GET_STACK(bool, bMini, 0x24);

	MouseCursor* pDecided = 
		CursorType::GetCursor(nCursor, reinterpret_cast<MouseCursor*>(nCursor));

	auto nMini = bMini && pDecided->MiniFrame >= 0;
	if (MouseClass::ShapeOverride() &&
		(!MouseClass::ShapeFile() || (pMouse->MouseCursorIndex == (MouseCursorType)nCursor && pMouse->MouseCursorIsMini == nMini)))
		return 0x5BDCD8;

	CursorType::TempCursor = pDecided;
	R->ESI(pDecided);
	R->BL(nMini);

	return 0x5BDCE3;
}

DEFINE_HOOK(5BDBC4, MouseClass_GetCursorCurrentFrame, 7)
{
	GET(size_t, nCursor, EAX);
	R->EAX(CursorType::GetCursor(nCursor, reinterpret_cast<MouseCursor*>(nCursor)));
	return 0x5BDBD4;
}

DEFINE_HOOK(5BE974, MouseClass_GetCursorFirstFrame, 7)
{
	GET(size_t, nCursor, EAX);
	R->EAX(CursorType::GetCursor(nCursor, reinterpret_cast<MouseCursor*>(nCursor))->Count);
	return 0x5BE9A4;
}

DEFINE_HOOK(5BDB90, MouseClass_GetCursorFirstFrame_Minimap, B)
{
	GET_STACK(bool, wSmall, 0x8);
	GET_STACK(size_t, nCursor, 0x4);

	MouseCursor* pDecided = CursorType::GetCursor(nCursor, reinterpret_cast<MouseCursor*>(nCursor));

	R->EAX((!wSmall || pDecided->MiniFrame < 0) ? pDecided->Frame : pDecided->MiniFrame);
	return 0x5BDBB6;
}

DEFINE_HOOK(5BE994, MouseClass_GetCursorFrameCount, 7)
{
	GET(size_t, nCursor, EAX);
	R->EAX(CursorType::GetCursor(nCursor, reinterpret_cast<MouseCursor*>(nCursor))->Count);
	return 0x5BE9A4;
}

DEFINE_HOOK(5BDC1B, MouseClass_GetCursorHotSpot, 7)
{
	GET(size_t, nCursor, EAX);
	MouseCursor* pDecided = CursorType::GetCursor(nCursor, reinterpret_cast<MouseCursor*>(nCursor));
	R->ESI(pDecided);
	R->ECX(pDecided->HotX);
	return 0x5BDC29;
}

DEFINE_HOOK(5BDADF, MouseClass_UpdateCursorMinimapState_UseCursor, 0)
{
	GET(size_t, nCursor, EAX);

	R->EBP(CursorType::GetCursor(nCursor, reinterpret_cast<MouseCursor*>(nCursor)));

	return R->DL()
		? 0x5BDAEC
		: 0x5BDAFA
		;
}

DEFINE_HOOK(5BDDC8, MouseClass_Update_AnimateCursor, 6)
{
	GET(MouseClass*, pThis, ESI);

	MouseCursor* pDecided =
		CursorType::GetCursor((size_t)pThis->MouseCursorIndex, reinterpret_cast<MouseCursor*>((size_t)pThis->MouseCursorIndex));

	R->EBX(pDecided);

	return (pDecided->Interval == 0)
		? 0x5BDF13  //no animation
		: 0x5BDDED
		;
}

DEFINE_HOOK(5BDE64, MouseClass_Update_AnimateCursor2, 6)
{
	GET(MouseClass*, pThis, ESI);

	MouseCursor* pDecided =
		CursorType::GetCursor((size_t)pThis->MouseCursorIndex, reinterpret_cast<MouseCursor*>((size_t)pThis->MouseCursorIndex));

	R->ECX(pDecided);

	return (pThis->MouseCursorIsMini)
		? 0x5BDE84 //minimap
		: 0x5BDE92
		;
}

DEFINE_HOOK(5BDDC0, MouseClass_Actions_Reset, 5)
{
	CursorType::TempCursor = nullptr;
	CursorType::ActionCursor_Map.clear();
	return 0;
}

DEFINE_HOOK(653CA6, RadarClass_GetMouseAction_AllowMinimap, 5)
{
	if (MouseCursor* pDecided = CursorType::TempCursor)
		return (pDecided->MiniFrame >= 0) ? 0x653CC0 : 0x653CBA;

	return (R->EAX<size_t>() > 0x47) ? 0x653CBA: 0x653CAB;
}

DEFINE_HOOK(4AB35A, DisplayClass_SetAction_CustomCursor, 6)
{
	GET(DisplayClass*, pThis, ESI);
	GET(Action, nAction, EAX);
	GET_STACK(bool, bMini, STACK_OFFS(0x20, -0x14));//0x34
	GET_STACK(AbstractClass*, pTarget, STACK_OFFS(0x20, 0x8)); //0x18
	GET_STACK(bool, bShadow, 0x28);

	MouseCursorType nType = MouseCursorType::Default;

	auto SetAttackCursorOrRetAction = [&pTarget, &nType](Action& nAction)
	{
		auto const& nObjectVect = *ObjectClass::CurrentObjects();

		if (nAction == Action::Attack)
		{
			if (pTarget && (nObjectVect.Count == 1))
			{
				if (auto T = generic_cast<TechnoClass*>(nObjectVect[0]))
				{
					auto nWeaponIdx = T->SelectWeapon(pTarget);
					if (!T->IsCloseEnough(pTarget, nWeaponIdx))
					{
						nType = TechnoTypeExt::GetCursorByWeapon(T,nWeaponIdx);
					}
				}
			}
			
				nAction = Action::Harvest;
		}
	};

	if (bShadow)
	{
		switch (nAction)
		{
		case Action::NoMove:
		{
			auto const& nObjDvc = *ObjectClass::CurrentObjects(); //current object with cursor
			if (nObjDvc.Count)
			{
				if (auto T = generic_cast<TechnoClass*>(nObjDvc[0]))
				{
					if (T->GetTechnoType()->MoveToShroud)
					{
						nAction = Action::Move;
						break;
					}
				}
			}

			nAction = Action::NoMove;
		}

		break;
		case Action::Enter:
		case Action::Self_Deploy:
		case Action::Harvest:
		case Action::Select:
		case Action::ToggleSelect:
		case Action::Capture:
		case Action::Repair:
		case Action::Sabotage:
		case Action::DontUse2:
		case Action::DontUse3:
		case Action::DontUse4:
		case Action::DontUse5:
		case Action::DontUse6:
		case Action::DontUse7:
		case Action::DontUse8:
		case Action::Damage:
		case Action::GRepair:
		case Action::EnterTunnel:
		case Action::DragWaypoint:
		case Action::AreaAttack:
		case Action::IvanBomb:
		case Action::NoIvanBomb:
		case Action::Detonate:
		case Action::DetonateAll:
		case Action::DisarmBomb:
		case Action::SelectNode:
		case Action::AttackSupport:
		case Action::Demolish:
		case Action::Airstrike:
			nAction = Action::None;
			//check custom action here
			break;
		case Action::Eaten:
		case Action::NoGRepair:
		case Action::NoRepair:
			nAction = Action::NoRepair;
			break;
		case Action::SellUnit:
		case Action::Sell:
		case Action::NoSell:
			nAction = Action::NoSell;
			break;
		case Action::TogglePower:
			nAction = (Action)34;
			break;
		case Action::NoEnterTunnel:
			nAction = (Action)31;
			break;
		case Action::ChronoWarp:
			nAction = (Action)39;
			break;
		case Action::SelectBeacon:
			nAction = (Action)7;
			break;
		case Action::ParaDrop:
		case Action::AmerParaDrop:
			nAction = (Action)41;
			break;
		default:
			SetAttackCursorOrRetAction(nAction);
			break;
		}
	}
	else
	{
		SetAttackCursorOrRetAction(nAction);
	}

	if (CursorType::IsMapcontains(nAction))
		nType = (MouseCursorType)CursorType::ActionCursor_Map[nAction];

	//if default cursor is still present 
	//re-check the cases and set the cursor type
	if (nType == MouseCursorType::Default)
	{
		switch (nAction)
		{
		case Action::Move:
			nType = MouseCursorType::Move;
			break;
		case Action::NoMove:
		case Action::NoIvanBomb:
			nType = MouseCursorType::NoMove;
			break;
		case Action::Enter:
		case Action::Capture:
		case Action::Repair:
		case Action::EnterTunnel:
			nType = MouseCursorType::Enter;
			break;
		case Action::Self_Deploy:
		case Action::AreaAttack:
			nType = MouseCursorType::Deploy;
			break;
		case Action::Attack:
			nType = MouseCursorType::Attack;
			break;
		case Action::Harvest:
			nType = MouseCursorType::AttackOutOfRange;
			break;
		case Action::Select:
		case Action::ToggleSelect:
		case Action::SelectBeacon:
			nType = MouseCursorType::Select;
			break;
		case Action::Eaten:
			nType = MouseCursorType::EngineerRepair;
			break;
		case Action::Sell:
			nType = MouseCursorType::Sell;
			break;
		case Action::SellUnit:
			nType = MouseCursorType::SellUnit;
			break;
		case Action::NoSell:
			nType = MouseCursorType::NoSell;
			break;
		case Action::NoRepair:
		case Action::NoGRepair:
			nType = MouseCursorType::NoRepair;
			break;
		case Action::Sabotage:
		case Action::Demolish:
			nType = MouseCursorType::Demolish;
			break;
		case Action::Tote:
			nType = MouseCursorType::PsychicReveal | MouseCursorType::Move_NE;
			break;
		case Action::Nuke:
			nType = MouseCursorType::Nuke;
			break;
		case Action::GuardArea:
			nType = MouseCursorType::Protect;
			break;
		case Action::Heal:
		case Action::PlaceWaypoint:
		case Action::TibSunBug:
		case Action::EnterWaypointMode:
		case Action::FollowWaypoint:
		case Action::SelectWaypoint:
		case Action::LoopWaypointPath:
		case Action::AttackWaypoint:
		case Action::EnterWaypoint:
		case Action::PatrolWaypoint:
			nType = MouseCursorType::Disallowed;
			break;
		case Action::GRepair:
			nType = MouseCursorType::Repair;
			break;
		case Action::NoDeploy:
			nType = MouseCursorType::NoDeploy;
			break;
		case Action::NoEnterTunnel:
		case Action::NoEnter:
			nType = MouseCursorType::NoEnter;
			break;
		case Action::TogglePower:
			nType = MouseCursorType::NoForceShield | MouseCursorType::Move_NW;
			break;
		case Action::NoTogglePower:
			nType = MouseCursorType::GeneticMutator | MouseCursorType::Move_NW;
			break;
		case Action::IronCurtain:
			nType = MouseCursorType::IronCurtain;
			break;
		case Action::LightningStorm:
			nType = MouseCursorType::LightningStorm;
			break;
		case Action::ChronoSphere:
		case Action::ChronoWarp:
			nType = MouseCursorType::Chronosphere;
			break;
		case Action::ParaDrop:
		case Action::AmerParaDrop:
			nType = MouseCursorType::ParaDrop;
			break;
		case Action::IvanBomb:
			nType = MouseCursorType::IvanBomb;
			break;
		case Action::Detonate:
		case Action::DetonateAll:
			nType = MouseCursorType::Detonate;
			break;
		case Action::DisarmBomb:
			nType = MouseCursorType::Disarm;
			break;
		case Action::PlaceBeacon:
			nType = MouseCursorType::Beacon;
			break;
		case Action::AttackMoveNav:
		case Action::AttackMoveTar:
			nType = MouseCursorType::AttackOutOfRange2;
			break;
		case Action::PsychicDominator:
			nType = MouseCursorType::PsychicDominator;
			break;
		case Action::SpyPlane:
			nType = MouseCursorType::SpyPlane;
			break;
		case Action::GeneticConverter:
			nType = MouseCursorType::GeneticMutator;
			break;
		case Action::ForceShield:
			nType = MouseCursorType::ForceShield;
			break;
		case Action::NoForceShield:
			nType = MouseCursorType::NoForceShield;
			break;
		case Action::Airstrike:
			nType = MouseCursorType::AirStrike;
			break;
		case Action::PsychicReveal:
			nType = MouseCursorType::PsychicReveal;
			break;
		default:
			break;
		}
	}

	pThis->SetCursor(nType, bMini);
	return 0x4AB78F;
}

DEFINE_HOOK(70055D, TechnoClass_GetActionOnObject_AttackCursor, 8)
{
	GET(TechnoClass*, pThis, ESI);
	GET_STACK(int, nWeapon, STACK_OFFS(0x1C, 0x8));

	auto nCursor = TechnoTypeExt::GetCursorByWeapon(pThis, nWeapon, false);
	CursorType::Insert((int)nCursor, Action::Attack, false);

	return 0;
}

DEFINE_HOOK(700AA8, TechnoClass_GetActionOnCell_AttackCursor, 8)
{
	GET(TechnoClass*, pThis, ESI);
	GET(int, nWeapon, EBP);

	auto nCursor = TechnoTypeExt::GetCursorByWeapon(pThis, nWeapon, false);
	CursorType::Insert((int)nCursor, Action::Attack, false);

	return 0;
}

DEFINE_HOOK(6FFEC0, TechnoClass_GetActionOnObject_Cursors, 5)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(ObjectClass*, pTarget, 0x4);

	auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	CursorType::Insert(pThisExt->Move_Cursor.Get(), Action::Move, false);
	CursorType::Insert(pThisExt->NoMove_Cursor.Get(), Action::NoMove, false);

	if (auto pTargetType = pTarget->GetTechnoType())
	{
		auto const& pTargetExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

		CursorType::Insert(pTargetExt->Enter_Cursor.Get(), Action::Repair, false);
		CursorType::Insert(pTargetExt->Enter_Cursor.Get(), Action::Enter, false);
		CursorType::Insert(pTargetExt->NoEnter_Cursor.Get(), Action::NoEnter, false);
	}

	return 0;
}

DEFINE_HOOK(700600, TechnoClass_GetActionOnCell_Cursors, 5)
{
	GET(TechnoClass*, pThis, ECX);

	auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	CursorType::Insert(pThisExt->Move_Cursor.Get(), Action::Move, false);
	CursorType::Insert(pThisExt->NoMove_Cursor.Get(), Action::NoMove, false);

	return 0;
}

DEFINE_HOOK(7000CD, TechnoClass_GetActionOnObject_SelfDeployCursor, 6)
{
	GET(TechnoClass*, pThis, ESI);

	auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	CursorType::Insert(pThisExt->Deploy_Cursor.Get(), Action::AreaAttack, false);
	CursorType::Insert(pThisExt->Deploy_Cursor.Get(), Action::Self_Deploy, false);
	CursorType::Insert(pThisExt->NoDeploy_Cursor.Get(), Action::NoDeploy, false);

	return 0;
}

DEFINE_HOOK(7400F0, UnitClass_GetActionOnObject_SelfDeployCursor_Bunker, 6)
{
	GET(UnitClass*, pThis, ESI);

	if (pThis->BunkerLinkedItem)
	{
		auto const& pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
		CursorType::Insert(pThisExt->Deploy_Cursor.Get(), Action::Self_Deploy, false);
		return 0x73FFE6;
	}

	return pThis->unknown_bool_6AC ? 0x7400FA : 0x740115;
}