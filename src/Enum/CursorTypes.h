#pragma once
#include <MouseClass.h>

#include <Enum/_Enumerator.hpp>
#include <Utilities/Template.h>
#include <Utilities/Constructs.h>
#include <map>

class CursorType final : public Enumerable<CursorType>
{
public:

	Valueable<MouseCursor> Cursor;
	CursorType(const char* const pTitle) : Enumerable<CursorType>(pTitle)
		, Cursor{ }
	{ }

	CursorType(const char* const pTitle, MouseCursor nDefault) :
		Enumerable<CursorType>(pTitle)
		, Cursor{ nDefault }
	{ }

	virtual ~CursorType() override = default;
	static void AddDefaults();

	virtual void LoadFromINI(CCINIClass* pINI) override;
	virtual void LoadFromStream(AresStreamReader& Stm);
	virtual void SaveToStream(AresStreamWriter& Stm);

	static void LoadFromINIList(CCINIClass* pINI);
	static bool LoadGlobals(AresStreamReader& Stm);
	static bool SaveGlobals(AresStreamWriter& Stm);

	static CursorType* FindOrAllocate_(const char* Title, MouseCursor nDefault)
	{
		if (CursorType* find = Find(Title))
			return find;

		Array.push_back(std::make_unique<CursorType>(Title, nDefault));
		return Array.back().get();
	}

	static const Action SuperWeaponAllowed = static_cast<Action>(0x7F);
	static const Action SuperWeaponDisallowed = static_cast<Action>(0x7E);
	static MouseCursor* TempCursor;

	static std::map<Action, int> ActionCursor_Map;

	static void Clear() {} //do nothing 
	static void CleanUp() 
	{
		//not really sure
		Array.clear();
		TempCursor = nullptr;
		ActionCursor_Map.clear();
	}
														// mostly unused ?
	static void Insert(int nCursorIndex, Action nAct, bool bAllowShrouded)
	{ ActionCursor_Map[nAct] = nCursorIndex; }

	static MouseCursor* GetCursor(size_t nIdx , MouseCursor* pDefault = nullptr)
	{
		if (nIdx < CursorType::Array.size())
			return &Array[nIdx].get()->Cursor;

		return pDefault;
	}

private:
	template <typename T>
	void Serialize(T& Stm);
};