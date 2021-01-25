#include "SavePoint.h"

int SavePoint::OpenFilesCount;

SavePoint::SavePoint(SaveSlot* dirInfo, const char* saveDir, const char* saveSlotId, unsigned int bufferSize)
{
	MESSAGE_CLASS_CREATED(SavePoint);

	m_SaveDir = saveDir;
	m_SlotIdStr = saveSlotId;
	m_SlotDir;
	m_SaveSlot = dirInfo;
	m_Status = STATUS_SUCCESS;
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

	m_Status = STATUS_CLOSED;
}