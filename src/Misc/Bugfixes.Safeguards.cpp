#include <IsometricTileTypeClass.h>
#include <TriggerTypeClass.h>
#include <HouseTypeClass.h>

#include "Debug.h"
#include "../Ares.h"

DEFINE_HOOK(547043, IsometricTileTypeClass_ReadFromFile, 6)
{
	GET(int, FileSize, EBX);
	GET(IsometricTileTypeClass *, pTileType, ESI);

	if(FileSize == 0) {
		const char * tile = pTileType->ID;
		if(strlen(tile) > 9) {
			Debug::FatalErrorAndExit("Maximum allowed length for tile names, excluding the extension, is 9 characters.\n"
					"The tileset using filename '%s' exceeds this limit - the game cannot proceed.", tile);
		} else {
			Debug::FatalErrorAndExit("The tileset '%s' contains a file that could not be loaded for some reason - make sure the file exists.", tile);
		}
	}
	return 0;
}

// Ares 3.o -Otamaa
DEFINE_HOOK(41088D, AbstractTypeClass_CTOR_IDTooLong, 6)
{
	GET(const char*, ID, EAX);

	if (strlen(ID) > 25)
		Debug::FatalErrorAndExit("Tried to create a type with ID '%s' which is longer than the maximum length of 24 .", ID);

	return 0;
}

DEFINE_HOOK(7272B5, TriggerTypeClass_LoadFromINI_House, 6)
{
	GET(int const, index, EBX);
	GET(TriggerTypeClass* const, pTrig, EBP);
	GET(const char*, pHouse, ESI);

	if (index < 0)
	{
		Debug::FatalError("TriggerType '%s' refers to a house named '%s', which does not exist. In case no house is needed, use '<none>' explicitly.", pTrig->ID, pHouse);
		R->EDX<HouseTypeClass*>(nullptr);
	}
	else
	{
		R->EDX(HouseTypeClass::Find(pHouse));
	}

	return 0x7272C1;
}