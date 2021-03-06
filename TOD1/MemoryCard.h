#pragma once

#include "Globals.h"

enum MemoryCardIndex
{
	SAVE_SLOT_0 = 0,
	SAVE_SLOT_1,
	SAVE_SLOT_2,
	SAVE_SLOT_3,
	SAVE_SLOT_4,
	SAVE_SLOT_5,
	SAVE_SLOT_6,
	SAVE_SLOT_7,
	SAVE_SLOT_8,
	SAVE_SLOT_LAST
};

//	TODO: move this to 'MemoryCards' class.
class MemoryCard
{
public:
	MemoryCardIndex	m_MemoryCardIndex;	//	NOTE: for PC save directory information, this is set to 8.
	String			m_SaveFolderPath;
	bool			m_Formatted;
	unsigned char	field_16;
	unsigned char	field_17;

public:
	MemoryCard(MemoryCardIndex index);
	virtual ~MemoryCard();	//	@93F480

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	bool			IsFormatted() const;	//	@43B0B0
	bool			FormatCard();	//	@928840
	bool			IsSaveDirPresent(const char* const directory) const;	//	@928E40
	bool			IsSaveFilePresent(const char* const directory, const char* const slot) const;	//	@928DC0
	unsigned int	GetSavePointFileSize(const char* const directory, const char* const slotfilename) const;	//	@928F00
	String&			GetFullSaveFolderPath(String& outStr, const char* const directory, const char* const slot) const;	//	@43B160
	bool			DeleteSavePointFile(const char* const savedir, const char* const slotindstr);	//	@928D00
	bool			IsSavePointFileExists(const char* const savedir, const char* const slotindstr) const;	//	@928DC0
	bool			CreateSaveDirectory(const char* const savedir);	//	@928AA0
};

extern MemoryCard* MemoryCardInfo[SAVE_SLOT_LAST];	//	@A35E84

ASSERT_CLASS_SIZE(MemoryCard, 28);