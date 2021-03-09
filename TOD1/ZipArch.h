#pragma once
#include "StringsPool.h"
#include <map>

#define ZIP_MAX_SLOTS 8

class ZipArch
{
public:
	struct FileInfo
	{
		unsigned int	m_OffsetInArch;
		unsigned int	m_FileSize;

		FileInfo(unsigned int offset, unsigned int filesize)
			: m_OffsetInArch(offset), m_FileSize(filesize)
		{};

		FileInfo()
			: m_OffsetInArch(0), m_FileSize(0)
		{};
	};

	static bool			FindFile(const char* inPathStr, FileInfo& outFileInfo, int* outZipSlotIndex);	//	@41A500
	
	static String		ZipNames[];	//	@A085A8
	static std::map<unsigned int, FileInfo>	SlotInfo[];	//	@A08628
	static unsigned int	SlotId;	//	@A35DDC
};