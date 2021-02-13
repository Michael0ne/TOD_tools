#pragma once
#include "StringsPool.h"
#include <map>

#define ZIPSLOT_STRUCT_SIZE 16

struct ZipSlotInfo
{
	struct FileInfo
	{
		unsigned int	m_Checksum;
		unsigned int	m_FileSize;
	};
public:
	int					m_OffsetTableSize;
	struct SlotFlags
	{
		unsigned char	field_0;
		unsigned char	field_1;
		unsigned char	field_2;
		unsigned char	field_3;
	}					m_Flags;
	unsigned int*		m_OffsetTable;
	FileInfo*			m_FilesInfo;	//	NOTE: size is 'OffsetTableSize' * sizeof(FileInfo).

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

	FileInfo*			FindFileByCRC(unsigned int* checksum);	//	@4198F0
	void				AddFileInfo(const std::map<unsigned int, FileInfo>& fileInfo);	//	@41A5F0
};

class ZipArch
{
public:
	static bool			FindFile(const char* inPathStr, ZipSlotInfo::FileInfo*, int* outZipSlot);	//	@41A500
	
	static String		ZipNames[];	//	@A085A8
	static ZipSlotInfo	SlotInfo[];	//	@A08628
	static unsigned int	SlotId;	//	@A35DDC
};

static_assert(sizeof(ZipSlotInfo) == ZIPSLOT_STRUCT_SIZE, MESSAGE_WRONG_CLASS_SIZE(ZipSlotInfo));