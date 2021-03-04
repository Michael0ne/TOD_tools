#include "ZipArch.h"
#include "Globals.h"

String ZipArch::ZipNames[ZIP_MAX_SLOTS] = {};
ZipSlotInfo ZipArch::SlotInfo[ZIP_MAX_SLOTS] = {};
unsigned int ZipArch::SlotId = NULL;

ZipSlotInfo::ZipSlotInfo()
	: m_OffsetTableSize(0), m_OffsetTable(nullptr), m_FilesInfo(nullptr)
{
	MESSAGE_CLASS_CREATED(ZipSlotInfo);

	m_Flags.field_2 = 1;
	m_Flags.field_0 = 79;
}

ZipSlotInfo::~ZipSlotInfo()
{
	MESSAGE_CLASS_DESTROYED(ZipSlotInfo);

	delete m_OffsetTable;
	delete m_FilesInfo;
}

ZipSlotInfo::FileInfo* ZipSlotInfo::FindFileByCRC(unsigned int* checksum)
{
	unsigned int size = m_OffsetTableSize;

	if (m_OffsetTableSize)
	{
		unsigned int i = 0;
		do 
		{
			if (m_OffsetTable[(i + (size - i) / 2)] < *checksum)
				i = (i + (size - i) / 2) + 1;
			else
				size = i + (size - i) / 2;
		} while (i != size);
	}

	return (size >= m_OffsetTableSize || *checksum < m_OffsetTable[size]) ? nullptr : &m_FilesInfo[size];
}

#pragma message(TODO_IMPLEMENTATION)
void ZipSlotInfo::AddFileInfo(const std::map<unsigned int, FileInfo>& fileInfo)
{
}

bool ZipArch::FindFile(const char* inPathStr, ZipSlotInfo::FileInfo* outFileInfo, int* outZipSlot)
{
	String::ToLowerCase((char*)(inPathStr + 1));
	unsigned int checksum = Utils::CalcCRC32(inPathStr + 1, strlen(inPathStr) - 1);

	if (ZipArch::SlotId <= NULL)
		return false;

	unsigned int slotIndex = NULL;
	ZipSlotInfo::FileInfo* fileInfo = nullptr;
	while (true)
	{
		fileInfo = ZipArch::SlotInfo[slotIndex].FindFileByCRC(&checksum);
		if (fileInfo)
			break;
		
		slotIndex++;

		if (slotIndex >= ZipArch::SlotId)
			return false;
	}

	if (outFileInfo)
	{
		outFileInfo->m_Checksum = fileInfo->m_Checksum;	//	TODO: maybe don't need to copy values and just assign pointer?
		outFileInfo->m_FileSize = fileInfo->m_FileSize;
	}

	*outZipSlot = slotIndex;
	return true;
}