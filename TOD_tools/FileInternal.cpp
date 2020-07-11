#include "FileInternal.h"
#include "Globals.h"
#include "ZipArch.h"

int& FileInternal::FilesOpen = *(int*)0xA35DD8;	//	@A35DD8

FileInternal::FileInternal(const char* szFileName, unsigned int dwDesiredAccess, bool bCreateIfNotFound)
{
	MESSAGE_CLASS_CREATED(FileInternal);

	String sTemp;

	m_sFileName = String(Utils::GetPathFromDirectoryMappings(&sTemp, szFileName));
	m_pFileHandle = nullptr;
	m_bReadFromZip = false;
	m_nZipSlotIndex = -1;
	m_FileReadAttribute = (dwDesiredAccess >> 7 & 1);

	if (dwDesiredAccess & (FILE_READ_DATA || FILE_LIST_DIRECTORY) &&
		ZipArch::SlotId > 0 &&
		ZipArch::FindFile(szFileName, &sTemp, &m_nZipSlotIndex))
	{
		m_bReadFromZip = true;

		strcpy_s(m_szFilenameStr, sTemp.m_nLength, sTemp.m_szString);

		m_nFilenameStrLen = sTemp.m_nLength;
		m_nSeekPosition = 0;
		m_ExecuteAttribute = (dwDesiredAccess >> 5) & 1;
		m_pFile = nullptr;
	}else
		m_pFileHandle = new File(szFileName, dwDesiredAccess, bCreateIfNotFound);

	if ((m_bReadFromZip && m_nFilenameStrLen >= 0) || (m_pFileHandle->m_hFile))
		FilesOpen++;
}

bool FileInternal::IsFileOpen()
{
	if (m_bReadFromZip)
		return m_nFilenameStrLen >= 0;
	else
		return m_pFileHandle->m_hFile != 0;
}

FileInternal::~FileInternal()
{
	MESSAGE_CLASS_DESTROYED(FileInternal);

	if (m_bReadFromZip)
		delete m_pFile;

	if ((m_bReadFromZip && m_nFilenameStrLen >= 0) || m_pFileHandle->m_hFile)
		--FilesOpen;

	if (m_pFileHandle)
		delete m_pFileHandle;

	//	NOTE: String class destructor called automatically here.
}

char FileInternal::ReadString(String* outStr)
{
	return (*(char(__thiscall*)(FileInternal*, String*))0x4180C0)(this, outStr);
}