#include "CSFLoader.h"
#include <cstdio>

#include "../Ares.h"

int CSFLoader::CSFCount = 0;
int CSFLoader::NextValueIndex = 0;

void CSFLoader::LoadAdditionalCSF(const char *pFileName)
{
	//The main stringtable must have been loaded (memory allocation)
	//To do that, use StringTable::LoadFile.
	if(StringTable::is_Loaded() && pFileName && *pFileName) {
		CCFileClass* pFile;
		GAME_ALLOC(CCFileClass, pFile, pFileName);
		if(pFile->Exists(NULL) && pFile->Open(eFileMode::Read)) {
			CSFHeader header;

			if(pFile->ReadBytes(&header, sizeof(CSFHeader)) == sizeof(CSFHeader)) {
				if(header.Signature == CSF_SIGNATURE &&
					header.CSFVersion >= 2 &&
					header.Language == StringTable::get_Language()) //should stay in one language
				{
					++CSFCount;
					StringTable::ReadFile(pFileName); //must be modified to do the rest ;)

					qsort(
						StringTable::get_Labels(),
						StringTable::get_LabelCount(),
						sizeof(CSFLabel),
						(int (__cdecl *)(const void *,const void *))_strcmpi);
				}
			}
		}
		GAME_DEALLOC(pFile);
	}
};

//0x7346D0
DEFINE_HOOK(0x7346D0, CSF_LoadBaseFile, 0x6)
{
	StringTable::set_Loaded(true);
	CSFLoader::CSFCount = 0;

	return 0;
}

//0x734823
DEFINE_HOOK(0x734823, CSF_AllocateMemory, 0x6)
{
	//aaaah... finally, some serious hax :)
	//we don't allocate memory by the amount of labels in the base CSF,
	//but enough for exactly CSF_MAX_ENTRIES entries.
	//We're assuming we have only one value for one label, which is standard.

	CSFLabel* pLabels;
	GAME_ALLOC_ARR(CSFLabel, CSF_MAX_ENTRIES, pLabels);
	wchar_t** pValues;
	GAME_ALLOC_ARR(wchar_t*, CSF_MAX_ENTRIES, pValues);
	char** pExtraValues;
	GAME_ALLOC_ARR(char*, CSF_MAX_ENTRIES, pExtraValues);

	for(int i = 0; i < CSF_MAX_ENTRIES; i++) {
		*pLabels[i].Name = 0;
		pLabels[i].NumValues = 0;
		pLabels[i].FirstValueIndex = 0;

		pValues[i] = NULL;
		pExtraValues[i] = NULL;
	}

	StringTable::set_Labels(pLabels);
	StringTable::set_Values(pValues);
	StringTable::set_ExtraValues(pExtraValues);

	return 0x7348BC;
}

//0x734A5F, 5
DEFINE_HOOK(0x734A5F, CSF_AddOrOverrideLabel, 0x5)
{
	if(CSFLoader::CSFCount > 0)
	{
		CSFLabel* pLabel = (CSFLabel*)bsearch(
			(const char*)0xB1BF38, //label buffer, char[4096]
			StringTable::get_Labels(),
			StringTable::get_LabelCount(),
			sizeof(CSFLabel),
			(int (__cdecl *)(const void *,const void *))_strcmpi);

		if(pLabel)
		{
			//Label already exists - override!

			//If you study the CSF format deeply, you'll call this method suboptimal,
			//because it assumes that we have only one value assigned to one label.
			//This is always the case for RA2, but in no way a limit!
			//Just adding this as a note.
			int idx = pLabel->FirstValueIndex;
			CSFLoader::NextValueIndex = idx;

			wchar_t** pValues = StringTable::get_Values();
			if(pValues[idx])
			{
				GAME_DEALLOC(pValues[idx]);
				pValues[idx] = NULL;
			}

			char** pExtraValues = StringTable::get_ExtraValues();
			if(pExtraValues[idx])
			{
				GAME_DEALLOC(pExtraValues[idx]);
				pExtraValues[idx] = NULL;
			}

			auto ix = pLabel - StringTable::get_Labels();
			R->EBP(ix * sizeof(CSFLabel));
		}
		else
		{
			//Label doesn't exist yet - add!
			int idx = StringTable::get_ValueCount();
			CSFLoader::NextValueIndex = idx;
			StringTable::set_ValueCount(idx + 1);
			StringTable::set_LabelCount(StringTable::get_LabelCount() + 1);

			R->EBP(idx * sizeof(CSFLabel)); //set the index
		}
	}
	return 0;
}

//0x734A97
DEFINE_HOOK(0x734A97, CSF_SetIndex, 0x6)
{
	R->EDX(StringTable::get_Labels());

	if(CSFLoader::CSFCount > 0) {
		R->ECX(CSFLoader::NextValueIndex);
	} else {
		R->ECX(R->Stack32(0x18));
	}

	return 0x734AA1;
}

DEFINE_HOOK(0x6BD886, CSF_LoadExtraFiles, 0x5)
{
	CSFLoader::LoadAdditionalCSF("ares.csf");
	char fname[32];
	for(int idx = 0; idx < 100; ++idx) {
		_snprintf(fname, 32, "stringtable%02d.csf", idx);
		CSFLoader::LoadAdditionalCSF(fname);
	}
	R->AL(1);
	return 0x6BD88B;
}

DEFINE_HOOK(0x734E83, CSF_LoadString_1, 0x6)
{
	GET(char *, Name, EBX);
	if(strlen(Name) >= 6 && !strncmp(Name, "NOSTR:", 6)) {
		CSFString *NewString;
		GAME_ALLOC(CSFString, NewString);
		wsprintfW(NewString->Text, L"%hs", &Name[6]);

		NewString->PreviousEntry = StringTable::LastLoadedString;
		StringTable::LastLoadedString = NewString;

		R->EAX(NewString->Text);

		return 0x734F0F;
	}
	return 0;
}

DEFINE_HOOK(0x734EC2, CSF_LoadString_2, 0x7)
{
	GET(char *, Name, EBX);
	CSFString *NewString;
	GAME_ALLOC(CSFString, NewString);

	wsprintfW(NewString->Text, L"MISSING:'%hs'", Name);

	NewString->PreviousEntry = StringTable::LastLoadedString;
	StringTable::LastLoadedString = NewString;

	R->EAX(NewString->Text);

	return 0x734F0F;
}
