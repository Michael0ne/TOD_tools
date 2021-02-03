#pragma once

#include "StringsPool.h"

#define ZIPSLOT_STRUCT_SIZE 16

struct ZipSlotInfo
{
public:
	int					m_ChecksumsListSize;
	union
	{
		unsigned char	_0 : 1;
		unsigned char	_1 : 1;
		unsigned char	_2 : 1;
	}					field_4;
	unsigned int*		m_ChecksumsList;
	unsigned int*		field_C;	//	NOTE: pointer to some array, possible, structure could be 'offset:size'.

	int*				_4198F0(const unsigned int* hashStr);	//	@4198F0
	void				_41A5F0(unsigned int*);	//	@41A5F0
public:
	ZipSlotInfo();
	~ZipSlotInfo();

	void*				operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	}
	void				operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, false);
	}
};

class ZipArch
{
public:
	static bool			FindFile(const char* inPathStr, int* outPath, int* outZipSlot);	//	@41A500
	
	static String		ZipNames[];	//	@A085A8
	static ZipSlotInfo	SlotInfo[];	//	@A08628
	static unsigned int	SlotId;	//	@A35DDC
};

static_assert(sizeof(ZipSlotInfo) == ZIPSLOT_STRUCT_SIZE, MESSAGE_WRONG_CLASS_SIZE(ZipSlotInfo));