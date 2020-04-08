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
		lpVtbl = nullptr;
		field_4 = 0;
		m_sSaveFolderPath = String();
		m_bFormatted = false;
		field_16 = 0;
		field_17 = 0;

		debug("SavesDirectoriesInformation created at %X\n", this);
	}

	~SavesDirectoriesInformation()
	{
		debug("SavesDirectoriesInformation destroyed!\n");
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(size);
	}

	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	void	Init(unsigned int index);

	void	SetSaveFolderPath(const char* szPath)
	{
		m_sSaveFolderPath = String(szPath);
	}

	//	TODO: implementation!
	bool	IsFormatted()	//	@43B0B0
	{
		bool(__thiscall * _IsFormatted)(SavesDirectoriesInformation * _this) = (bool(__thiscall*)(SavesDirectoriesInformation*))0x43B0B0;

		return _IsFormatted(this);
	}

	//	TODO: implementation!
	void	FormatCard()	//	@928840
	{
		void(__thiscall * _FormatCard)(SavesDirectoriesInformation * _this) = (void(__thiscall*)(SavesDirectoriesInformation*))0x928840;

		_FormatCard(this);
	}
};

extern SavesDirectoriesInformation* g_SavesDirsInfo[3];

static_assert(sizeof(SavesDirectoriesInformation) == SAVESDIRINF_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("SavesDirectoriesInformation"));