#pragma once

#include "Globals.h"

#define SAVESDIRINF_CLASS_SIZE 28

enum SaveSlotIndex
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
class SaveSlot
{
private:
	SaveSlotIndex	m_MemoryCardIndex;	//	NOTE: for PC save directory information, this is set to 8.
public:
	String			m_SaveFolderPath;
private:
	bool			m_Formatted;
	unsigned char	field_16;
	unsigned char	field_17;

public:
	virtual void scalar_destructor(bool) {};	//	@93F480

	SaveSlot(SaveSlotIndex index);
	~SaveSlot();

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	bool	IsFormatted();	//	@43B0B0
	bool	FormatCard();	//	@928840
};

static SaveSlot* SaveSlots[SAVE_SLOT_LAST] = {
   nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};	//	@A35E84

static_assert(sizeof(SaveSlot) == SAVESDIRINF_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SaveSlot));