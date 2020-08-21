#include "FileInternal.h"
#include "Globals.h"
#include "ZipArch.h"

int& FileInternal::FilesOpen = *(int*)0xA35DD8;	//	@A35DD8

void FileInternal::scalar_destructor(bool freememory)
{
	if (freememory)
		Allocators::ReleaseMemory(this, false);
}

bool FileInternal::WriteBuffers()
{
	if (m_ReadFromZip)
		return false;

	EnterCriticalSection((LPCRITICAL_SECTION)m_FileHandle);

	if (!m_FileHandle->m_bRead)
	{
		if (m_FileHandle->m_pBuffer != m_FileHandle->m_pBufferBegin)
		{
			DWORD byteswritten = NULL;
			WriteFile(m_FileHandle->m_hFile, m_FileHandle->m_pBuffer, m_FileHandle->m_pBufferBegin - m_FileHandle->m_pBuffer, &byteswritten, NULL);
			m_FileHandle->m_pBufferEnd = m_FileHandle->m_pBufferBegin = m_FileHandle->m_pBuffer;
		}
		FlushFileBuffers(m_FileHandle->m_hFile);
	}

	LeaveCriticalSection((LPCRITICAL_SECTION)m_FileHandle);

	return true;
}

int FileInternal::_vsnprintf(const char* format, ...)
{
	va_list va;
	char buffer[1024] = {};

	va_start(va, format);
	int unk = vsnprintf(buffer, sizeof(buffer), format, va);

	for (int i = 0; i < unk; ++i)
		WriteBufferBlockAndInsertNewLine(buffer[i]);

	return unk;
}

void* FileInternal::_CheckFormattingSymbolsList(FileInternal* file, const char* format, ...)
{
	va_list va;

	va_start(va, format);

	if (m_ReadFromZip)
		return file;
	else
		return (void*)_CheckFormattingSymbol(format, va);
}

int FileInternal::WriteFormattedVarlistDataToBuffer(const char* str, va_list arguments)
{
	char buffer[1024] = {};

	int unk = vsnprintf(buffer, sizeof(buffer), str, arguments);
	for (int i = 0; i < unk; ++i)
		WriteBufferBlockAndInsertNewLine(buffer[i]);

	return unk;
}

//	TODO: implementation!
signed int FileInternal::_CheckFormattingSymbol(const char* str, ...)
{
	va_list va;
	va_start(str, va);

	return (*(signed int(__thiscall*)(FileInternal*, const char*, va_list))0x42F0A0)(this, str, va);
}

//	TODO: implementation!
bool FileInternal::ReadBlock()
{
	return (*(bool(__thiscall*)(FileInternal*))0x417980)(this);
}

int FileInternal::ReadBlockAndGetPosition(int)
{
	int data = ReadBlock();
	ReadBlockDecreasePosition();

	return data;
}

//	TODO: implementation!
bool FileInternal::WriteBufferBlockAndInsertNewLine(char newlinesym)
{
	return (*(bool(__thiscall*)(FileInternal*, char))0x419BF0)(this, newlinesym);
}

//	TODO: implementation!
int FileInternal::Read(void* buffer, int bytestoread)
{
	return (*(int(__thiscall*)(FileInternal*, void*, int))0x417A30)(this, buffer, bytestoread);
}

//	TODO: implementation!
signed int FileInternal::WriteBuffer(const char* buffer)
{
	return (*(signed int(__thiscall*)(FileInternal*, const char*))0x419C10)(this, buffer);
}

//	TODO: implementation!
signed int FileInternal::WriteBufferWithSize(const char* buffer, signed int size)
{
	return (*(signed int(__thiscall*)(FileInternal*, const char*, signed int))0x419C00)(this, buffer, size);
}

//	TODO: implementation!
void FileInternal::Seek(int position)
{
	(*(void(__thiscall*)(FileInternal*, int))0x417B90)(this, position);
}

//	TODO: implementation!
char FileInternal::WriteBufferAndSetToStart()
{
	return (*(char(__thiscall*)(FileInternal*))0x417BB0)(this);
}

//	TODO: implementation!
char FileInternal::WriteFromBuffer()
{
	return (*(char(__thiscall*)(FileInternal*))0x417BD0)(this);
}

//	TODO: implementation!
int FileInternal::GetPosition()
{
	return (*(int(__thiscall*)(FileInternal*))0x417BF0)(this);
}

//	TODO: implementation!
void FileInternal::ReadBlockDecreasePosition()
{
	(*(void(__thiscall*)(FileInternal*))0x417C50)(this);
}

char FileInternal::ReadBlockIfFailed()
{
	if ((char)ReadBlock() == (char)-1)
		return 1;
	ReadBlockDecreasePosition();

	return NULL;
}

File* FileInternal::GetFileHandle()
{
	return m_FileHandle;
}

FileInternal::FileInternal(const char* szFileName, unsigned int dwDesiredAccess, bool bCreateIfNotFound)
{
	MESSAGE_CLASS_CREATED(FileInternal);

	String sTemp;

	m_FileName = String(Utils::GetPathFromDirectoryMappings(&sTemp, szFileName));
	m_FileHandle = nullptr;
	m_ReadFromZip = false;
	m_ZipSlotIndex = -1;
	m_FileReadAttribute = (dwDesiredAccess >> 7 & 1);

	if (dwDesiredAccess & (FILE_READ_DATA || FILE_LIST_DIRECTORY) &&
		ZipArch::SlotId > 0 &&
		ZipArch::FindFile(szFileName, &sTemp, &m_ZipSlotIndex))
	{
		m_ReadFromZip = true;

		strcpy_s(m_FilenameStr, sTemp.m_nLength, sTemp.m_szString);

		m_FilenameStrLen = sTemp.m_nLength;
		m_SeekPosition = 0;
		m_ExecuteAttribute = (dwDesiredAccess >> 5) & 1;
		m_File = nullptr;
	}else
		m_FileHandle = new File(szFileName, dwDesiredAccess, bCreateIfNotFound);

	if ((m_ReadFromZip && m_FilenameStrLen >= 0) || (m_FileHandle->m_hFile))
		FilesOpen++;
}

bool FileInternal::IsFileOpen()
{
	if (m_ReadFromZip)
		return m_FilenameStrLen >= 0;
	else
		return m_FileHandle->m_hFile != 0;
}

FileInternal::~FileInternal()
{
	MESSAGE_CLASS_DESTROYED(FileInternal);

	if (m_ReadFromZip)
		delete m_File;

	if ((m_ReadFromZip && m_FilenameStrLen >= 0) || m_FileHandle->m_hFile)
		--FilesOpen;

	if (m_FileHandle)
		delete m_FileHandle;

	//	NOTE: String class destructor called automatically here.
}

char FileInternal::ReadString(String* outStr)
{
	return (*(char(__thiscall*)(FileInternal*, String*))0x4180C0)(this, outStr);
}