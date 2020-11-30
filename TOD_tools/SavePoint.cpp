#include "SavePoint.h"

int SavePoint::ms_FilesOpen = *(int*)0xA35EA8;	//	@A35EA8

SavePoint::SavePoint(SavesDirectoriesInformation* dirInfo, const char* saveDir, const char* saveSlotId, unsigned int bufferSize)
{
	MESSAGE_CLASS_CREATED(SavePoint);

	m_sSaveDir = String(saveDir);
	m_sSlotId = String(saveSlotId);
	m_sSaveSlotDir = String();
	m_pSavesDirInfo = dirInfo;
	m_nLastError = eSavePointStatus::E_UNKNOWN_3;
	m_pSaveFileHandle = nullptr;
}

SavePoint::~SavePoint()
{
	MESSAGE_CLASS_DESTROYED(SavePoint);

	if (m_pSaveFileHandle) {
		--ms_FilesOpen;
		m_pSaveFileHandle->scalar_destructor(true);
		m_pSaveFileHandle = nullptr;
	}

	m_nLastError = eSavePointStatus::E_UNKNOWN_3;

	//	NOTE: implicit calls to destructors for string objects are here.
}