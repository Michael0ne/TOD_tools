#pragma once

#include "stdafx.h"
#include "StringsPool.h"
#include "SavesDirectoriesInformation.h"

class FileInternal;

class SavePoint
{
private:
	void* lpVtbl;
	SavesDirectoriesInformation* m_pSavesDirInfo;
	String m_sSaveDir;
	String m_sSlotId;
	FileInternal* m_pSaveFileHandle;
	int m_nUnkState;
	String m_sUnkStr3;
};

static_assert(sizeof(SavePoint) == 0x40, MESSAGE_WRONG_CLASS_SIZE("SavePoint"));