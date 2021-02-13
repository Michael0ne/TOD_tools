#include "ZipArch.h"
#include "Globals.h"

String ZipArch::ZipNames[8];
ZipSlotInfo ZipArch::SlotInfo[8];
unsigned int ZipArch::SlotId = NULL;

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