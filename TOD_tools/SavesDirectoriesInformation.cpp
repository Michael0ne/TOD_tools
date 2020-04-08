#include "SavesDirectoriesInformation.h"

SavesDirectoriesInformation* g_SavesDirsInfo[3] = {
	nullptr, nullptr, nullptr
};

void SavesDirectoriesInformation::Init(unsigned int index)
{
	if (index == 8)
		//	PC.
		patch(0xA35EA4, this, 4);
	else
		//	PS2 emulation.
		patch(0xA35E84 + index, this, 4);

	lpVtbl = (SavesDirectoriesInformation::__vtable*)0x9D79D4;
	field_4 = index;
	m_bFormatted = true;
}