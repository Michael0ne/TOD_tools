#pragma once

#include "stdafx.h"

#include "StringsPool.h"
#include "MemoryAllocators.h"

#define SAVESDIRINF_CLASS_SIZE 28

class SavesDirectoriesInformation
{
private:
	struct __vtable {
		void(__thiscall* Release)(SavesDirectoriesInformation* _this, bool releasememory);	//	@93F480
	} *lpVtbl;
	int field_4;
	String m_sSaveFolderPath;
	byte m_bFormatted;
	byte field_16;
	byte field_17;

public:
	SavesDirectoriesInformation()
	{
		MESSAGE_CLASS_CREATED(SavesDirectoriesInformation);

		lpVtbl = nullptr;
		field_4 = 0;
		m_sSaveFolderPath = String();
		m_bFormatted = false;
		field_16 = 0;
		field_17 = 0;
	}
	~SavesDirectoriesInformation()
	{
		MESSAGE_CLASS_DESTROYED(SavesDirectoriesInformation);
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void	Init(unsigned int index);

	void	SetSaveFolderPath(const char* szPath)
	{
		m_sSaveFolderPath = String(szPath);
	}

	bool	IsFormatted();	//	@43B0B0
	bool	FormatCard();	//	@928840
};

extern SavesDirectoriesInformation* g_SavesDirsInfo[3];

static_assert(sizeof(SavesDirectoriesInformation) == SAVESDIRINF_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("SavesDirectoriesInformation"));