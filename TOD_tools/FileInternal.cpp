#include "FileInternal.h"

void FileInternal::Open(const char* szFileName, unsigned int dwDesiredAccess, bool bCreateIfNotFound)
{
	(*(void(__thiscall*)(FileInternal*, const char*, unsigned int, bool))0x418E30)(this, szFileName, dwDesiredAccess, bCreateIfNotFound);
}

bool FileInternal::IsFileOpen()
{
	if (m_bReadFromZip)
		return m_nFilenameStrLen >= 0;
	else
		return m_pFileHandle->m_hFile != 0;
}

void FileInternal::Close()
{
	(*(void(__thiscall*)(FileInternal*))0x417E20)(this);
}

char FileInternal::ReadString(String* outStr)
{
	return (*(char(__thiscall*)(FileInternal*, String*))0x4180C0)(this, outStr);
}

int& FileInternal::FilesOpen = *(int*)0xA35DD8;