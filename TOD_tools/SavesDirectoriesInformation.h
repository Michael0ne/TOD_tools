#pragma once

#include "stdafx.h"
#include "StringsPool.h"

class SavesDirectoriesInformation
{
private:
	void* lpVtbl;
	int field_4;
	String m_sSaveFolderPath;
	byte m_bFormatted;
	byte field_16;
	byte field_17;
	byte field_18[4];
};

static_assert(sizeof(SavesDirectoriesInformation) == 0x20, MESSAGE_WRONG_CLASS_SIZE("SavesDirectoriesInformation"));