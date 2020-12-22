#include "SavePoint.h"

int SavePoint::OpenFilesCount = *(int*)0xA35EA8;	//	@A35EA8

SavePoint::SavePoint(SaveSlot* dirInfo, const char* saveDir, const char* saveSlotId, unsigned int bufferSize)
{
	MESSAGE_CLASS_CREATED(SavePoint);

	m_SaveDir = String(saveDir);
	m_SlotIdStr = String(saveSlotId);
	m_SlotDir = String();
	m_SaveSlot = dirInfo;
	m_LastError = eSavePointStatus::E_UNKNOWN_3;
	m_SaveFile = nullptr;
}

SavePoint::~SavePoint()
{
	MESSAGE_CLASS_DESTROYED(SavePoint);

	if (m_SaveFile) {
		--OpenFilesCount;
		delete m_SaveFile;
		m_SaveFile = nullptr;
	}

	m_LastError = eSavePointStatus::E_UNKNOWN_3;
}