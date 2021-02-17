#include "File.h"
#include "LogDump.h"
#include "ZipArch.h"
#include "Window.h"
#include "ScriptDatabase.h"
#include <map>

unsigned int File::FilesOpen = 0;
HANDLE	File::FilesSemaphoreArray[FILE_MAX_ZIP_FILES];
List<StringTuple> File::DirectoryMappingsList;
unsigned int File::AlignmentArray[3] =
{
	1, 1, 4
};
FileWrapper* FileWrapper::ZipFilesArray[FILE_MAX_ZIP_FILES];

ULARGE_INTEGER FileWrapper::GetStorageFreeSpace()
{
	ULARGE_INTEGER spaceAvailable;

	GetDiskFreeSpaceEx(WorkingDirectory.m_szString, &spaceAvailable, nullptr, nullptr);
	return spaceAvailable;
}

extern void GetWorkingDirRelativePath(String* str);
extern void GetGameWorkingDirRelativePath(String* str);

void FileWrapper::SetExecuteAttr(unsigned char _attr)
{
	EnterCriticalSection(&m_CriticalSection);
	m_ExecuteAttribute = _attr;
	LeaveCriticalSection(&m_CriticalSection);
}

time_t File::GetFileTimestamp_Impl(const char* path)
{
	String workingdir = path;
	WIN32_FIND_DATA findData = {};
	HANDLE findHandle = INVALID_HANDLE_VALUE;
	FILETIME fileTime = {};
	SYSTEMTIME fileTimeSystem = {};
	tm time_ = {};

	GetWorkingDirRelativePath(&workingdir);
	findHandle = FindFirstFile(workingdir.m_szString, &findData);

	if (findHandle == INVALID_HANDLE_VALUE && Window::GameDiscFound)
	{
		workingdir = path;
		GetGameWorkingDirRelativePath(&workingdir);
		findHandle = FindFirstFile(workingdir.m_szString, &findData);
	}

	FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
	FileTimeToSystemTime(&fileTime, &fileTimeSystem);
	FindClose(findHandle);

	time_.tm_hour = fileTimeSystem.wHour;
	time_.tm_year = fileTimeSystem.wYear - 1900;
	time_.tm_mday = fileTimeSystem.wDay;
	time_.tm_min = fileTimeSystem.wMinute;
	time_.tm_mon = fileTimeSystem.wMonth - 1;
	time_.tm_isdst = -1;
	time_.tm_sec = fileTimeSystem.wSecond;
	time_.tm_wday = fileTimeSystem.wDayOfWeek;
	time_.tm_yday = NULL;

	return mktime(&time_);
}

bool File::WriteBuffers()
{
	if (m_ReadFromZip)
		return false;

	EnterCriticalSection(&m_FileHandle->m_CriticalSection);

	if (!m_FileHandle->m_Read)
	{
		if (m_FileHandle->m_Buffer != m_FileHandle->m_BufferBegin)
		{
			DWORD numBytesWritten = NULL;
			WriteFile(m_FileHandle->m_File, m_FileHandle->m_Buffer, m_FileHandle->m_BufferBegin - m_FileHandle->m_Buffer, &numBytesWritten, nullptr);
			m_FileHandle->m_BufferEnd = m_FileHandle->m_BufferBegin = m_FileHandle->m_Buffer;
		}
		FlushFileBuffers(m_FileHandle->m_File);
	}

	LeaveCriticalSection(&m_FileHandle->m_CriticalSection);

	return true;
}

int File::_vsnprintf(File* _f, const char* _format, ...)
{
	va_list va;
	char buf[1024];
	memset(&buf, NULL, sizeof(buf));

	va_start(va, _format);
	int written = vsnprintf(buf, sizeof(buf), _format, va);

	for (int ind = 0; ind < written; ++ind)
		_f->_WriteBufferBlockAndInsertNewLine(buf[ind]);

	return written;
}

int File::_scanf(File* _f, const char* _format, ...)
{
	va_list va;
	va_start(va, _format);

	if (m_ReadFromZip)
		return (int)_f;	//	FIXME: wtf?
	else
		return _f->_scanf_impl((char*)_format, (int*)&va);	//	FIXME: wtf?
}

int File::WriteFormattedVarlistDataToBuffer(char* _buf, va_list args)
{
	char buf[1024];
	memset(&buf, NULL, sizeof(buf));

	int written = vsnprintf(buf, sizeof(buf), _buf, args);

	for (int ind = 0; ind < written; ++ind)
		_WriteBufferBlockAndInsertNewLine(buf[ind]);

	return written;
}

#pragma message(TODO_IMPLEMENTATION)
int File::_scanf_impl(char* format, int* outArgs)
{
	//	NOTE: this checks input string for formatting symbol and does something with it. Why is it here?
	return (*(int(__thiscall*)(File*, const char*, int*))0x42F0A0)(this, format, outArgs);
}

char File::ReadBlock()
{
	if (!m_ReadFromZip)
		return m_FileHandle->ReadBlock();

	if (m_SeekPosition >= m_FileSize)
		return -1;

	WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);
	
	FileWrapper::ZipFilesArray[m_ZipSlot]->SetExecuteAttr(m_ExecuteAttribute);
	FileWrapper::ZipFilesArray[m_ZipSlot]->Seek(m_FileChecksum + m_SeekPosition);
	char bytesread = FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlock();
	m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_FileChecksum;

	ReleaseSemaphore(FileWrapper::ZipFilesArray[m_ZipSlot], 1, 0);

	return bytesread;
}

int File::ReadBlockAndGetPosition()
{
	int bytesread = ReadBlock();
	ReadBlockDecreasePosition();

	return bytesread;
}

char File::_WriteBufferBlockAndInsertNewLine(char _newlinesym)
{
	return m_FileHandle->_WriteBufferBlockAndInsertNewLine(_newlinesym);
}

int File::Read(void* _buffer, int _numbytestoread)
{
	if (!m_ReadFromZip)
		return m_FileHandle->Read(_buffer, _numbytestoread);

	if (m_SeekPosition >= m_FileSize)
		return NULL;

	WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);
	FileWrapper::ZipFilesArray[m_ZipSlot]->Seek(m_SeekPosition + m_FileChecksum);
	FileWrapper::ZipFilesArray[m_ZipSlot]->SetExecuteAttr(true);

	if (m_ExecuteAttribute)
	{
		int bytesread = FileWrapper::ZipFilesArray[m_ZipSlot]->Read(_buffer, ((m_SeekPosition + _numbytestoread) > m_FileSize) ? m_FileSize - m_SeekPosition : _numbytestoread);

		m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_FileChecksum;
		ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

		return bytesread;
	}else{
		int bytesread = NULL;

		for (; bytesread < _numbytestoread; bytesread++)
		{
			if (FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() >= m_FileSize + m_FileChecksum)
				break;
			char lastchar = FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlock();
			if (lastchar == '\n' ||
				lastchar == '\r')
			{
				char lastchar_2 = FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlock();
				if ((lastchar_2 == '\n' || lastchar_2 == '\r') && lastchar == lastchar_2)
					FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlockDecreasePosition();
				lastchar = '\n';
			}
			((char*)(_buffer))[bytesread] = lastchar;
		}

		m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_FileChecksum;
		ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

		return bytesread;
	}
}

int File::WriteBuffer(const char* _buf)
{
	return m_FileHandle->WriteBuffer(_buf, strlen(_buf));
}

int File::WriteBufferWithSize(const char* _buf, int _size)
{
	return m_FileHandle->WriteBuffer(_buf, _size);
}

int File::Seek(int _pos)
{
	if (!m_ReadFromZip)
		return m_FileHandle->Seek(_pos);

	return m_SeekPosition = _pos;
}

char File::_WriteBufferAndSetToStart()
{
	if (m_ReadFromZip)
	{
		m_SeekPosition = NULL;
		return true;
	}

	EnterCriticalSection(&m_FileHandle->m_CriticalSection);
	
	if (m_FileHandle->m_File)
	{
		if (m_FileHandle->m_Buffer != m_FileHandle->m_BufferBegin &&
			!m_FileHandle->m_Read)
		{
			DWORD numBytesRead = NULL;
			WriteFile(m_FileHandle->m_File, m_FileHandle->m_Buffer, m_FileHandle->m_BufferBegin - m_FileHandle->m_Buffer, &numBytesRead, nullptr);
			m_FileHandle->m_BufferEnd = m_FileHandle->m_BufferBegin = m_FileHandle->m_Buffer;
		}

		if (m_FileHandle->m_Read)
		{
			if (m_FileHandle->m_BufferBegin != m_FileHandle->m_BufferEnd)
				SetFilePointer(m_FileHandle->m_File, m_FileHandle->m_BufferBegin - m_FileHandle->m_BufferEnd, NULL, FILE_CURRENT);
			m_FileHandle->m_BufferEnd = m_FileHandle->m_BufferBegin = m_FileHandle->m_Buffer;
		}

		m_FileHandle->m_FilePosition = SetFilePointer(m_FileHandle->m_File, NULL, NULL, FILE_BEGIN);
	}

	LeaveCriticalSection(&m_FileHandle->m_CriticalSection);
	return true;
}

char File::WriteFromBuffer()
{
	if (!m_ReadFromZip)
		return m_FileHandle->WriteFromBufferAndSetToEnd();

	m_SeekPosition = m_FileSize;

	return true;
}

int File::GetPosition()
{
	return m_ReadFromZip ? m_SeekPosition : m_FileHandle->GetPosition();
}

char File::ReadBlockDecreasePosition()
{
	if (!m_ReadFromZip)
		return m_FileHandle->ReadBlockDecreasePosition();

	WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);

	FileWrapper::ZipFilesArray[m_ZipSlot]->Seek(m_SeekPosition + m_FileChecksum);
	FileWrapper::ZipFilesArray[m_ZipSlot]->SetExecuteAttr(m_ExecuteAttribute);
	FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlockDecreasePosition();
	m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_FileChecksum;

	ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

	return true;
}

char File::ReadIfNotEOF()
{
	if (ReadBlock() == -1)
		return true;

	ReadBlockDecreasePosition();

	return false;
}

const char* File::GetFileName() const
{
	return m_FileName.m_szString;
}

File::File(const char* _filename, int _desiredaccess, bool _createifnotfound)
{
	ZipSlotInfo::FileInfo finfo;
	String fname;

	GetPathFromDirectoryMappings(&fname, _filename);

	m_FileName = fname.m_szString;
	m_FileHandle = nullptr;
	m_ReadFromZip = false;
	m_ZipSlot = -1;
	m_FileReadAttribute = (_desiredaccess >> 7) & 1;
	m_ExecuteAttribute = NULL;
	m_SeekPosition = NULL;
	m_ZipFileHandle = nullptr;
	m_FileChecksum = NULL;
	m_FileSize = NULL;

	if (_desiredaccess & 1 &&
		ZipArch::SlotId > 0 &&
		ZipArch::FindFile(_filename, &finfo, &m_ZipSlot))
	{
		m_ReadFromZip = true;
		m_FileSize = finfo.m_FileSize;
		m_FileChecksum = finfo.m_Checksum;
		m_SeekPosition = NULL;
		m_ExecuteAttribute = (_desiredaccess >> 5) & 1;
		m_ZipFileHandle = nullptr;
	}else
		m_FileHandle = new FileWrapper(m_FileName.m_szString, _desiredaccess, _createifnotfound);

	if ((m_ReadFromZip && m_FileChecksum) || (m_FileHandle && m_FileHandle->m_File != NULL))
		++FilesOpen;

	MESSAGE_CLASS_CREATED(File);
}

File::~File()
{
	MESSAGE_CLASS_DESTROYED(File);

	if (m_ReadFromZip)
		if (m_ZipFileHandle)
			delete m_ZipFileHandle;

	if ((m_ReadFromZip && m_FileChecksum >= 0) || (m_FileHandle->m_File != nullptr))
		--FilesOpen;

	if (m_FileHandle)
		delete m_FileHandle;
}

unsigned int File::GetSize()
{
	if (m_ReadFromZip)
		return m_FileSize;

	unsigned int currPos = m_FileHandle->GetPosition();
	m_FileHandle->WriteFromBufferAndSetToEnd();
	unsigned int fsize = m_FileHandle->GetPosition();
	m_FileHandle->Seek(currPos);

	return fsize;
}

bool File::IsFileOpen() const
{
	return (m_ReadFromZip && m_FileChecksum >= 0) || m_FileHandle->m_File != nullptr;
}

char File::ReadString(void* outStr)
{
	if (ReadIfNotEOF())
		return false;

	int _charread = ReadBlock();

	if (_charread == -1)
		return true;

	while (true)
	{
		if ((char)_charread == '\n')
		{
			char _endsym = ReadBlock();
			if (_endsym != '\r' && _endsym != -1)
				ReadBlockDecreasePosition();

			return true;
		}

		if ((char)_charread == '\r')
			break;

		((String*)outStr)->Append((const char*)_charread);

		if (ReadBlock() == -1)
			return true;
	}

	char _endsym = ReadBlock();
	if (_endsym != '\n' && _endsym != -1)
		ReadBlockDecreasePosition();

	return true;
}

void File::SetPosAligned(unsigned char alignind)
{
	Seek(~(AlignmentArray[alignind] - 1) & (AlignmentArray[alignind] + GetPosition() - 1));
}

void File::AddDirectoryMappingsListEntry(const char* str1, const char* str2)
{
	StringTuple _tmp(str1, str2);

	DirectoryMappingsList.AddElement(&_tmp);
}

String* File::GetPathFromDirectoryMappings(String* outStr, const char* path)
{
	if (DirectoryMappingsList.m_CurrIndex <= NULL)
		return (*outStr = path, outStr);

	for (unsigned int ind = 0; ind < DirectoryMappingsList.m_CurrIndex; ind++)
		if (String::EqualIgnoreCase(path, DirectoryMappingsList.m_Elements[ind]->m_String_1.m_szString, DirectoryMappingsList.m_Elements[ind]->m_String_1.m_nLength))
		{
			*outStr = DirectoryMappingsList.m_Elements[ind]->m_String_2.m_szString;
			outStr->Append(&path[DirectoryMappingsList.m_Elements[ind]->m_String_1.m_nLength]);

			return outStr;
		}

	return (*outStr = path, outStr);
}

String* File::ExtractFileDir(String& outStr, const char* path)
{
	char buf[1024] = {};
	char buf_[1024] = {};
	ExtractFilePath(path, buf, buf_, buf_);

	outStr = buf;
	return &outStr;
}

String* File::ExtractFileName(String& outStr, const char* path)
{
	char buf[1024] = {};
	char buf_[1024] = {};

	if (path && *path)
	{
		ExtractFilePath(path, buf_, buf, buf_);
		outStr = buf;
		return &outStr;
	}else
		return NULL;
}

bool File::FindFileEverywhere(const char* path)
{
	if (!path || *path == NULL)
		return false;

	String _tmp;
	int zipSlot;

	if (ZipArch::SlotId > NULL)
		if (ZipArch::FindFile(GetPathFromDirectoryMappings(&_tmp, path)->m_szString, nullptr, &zipSlot))
			return true;

	return IsFileExists(GetPathFromDirectoryMappings(&_tmp, path)->m_szString);
}

time_t File::GetFileTimestamp(const char* filename)
{
	String tempstr;
	int zipslot = NULL;

	if (ZipArch::SlotId <= 0 ||
		!ZipArch::FindFile(GetPathFromDirectoryMappings(&tempstr, filename)->m_szString, nullptr, &zipslot))
		return GetFileTimestamp_Impl(GetPathFromDirectoryMappings(&tempstr, filename)->m_szString);
	else
		return GetFileTimestamp_Impl(FileWrapper::ZipFilesArray[0]->m_WorkingDir.m_szString);
}

bool File::IsFileExists(const char* file)
{
	String temp(file);
	GetWorkingDirRelativePath(&temp);

	if (GetFileAttributes(temp.m_szString) == INVALID_FILE_ATTRIBUTES)
	{
		if (!Window::GameDiscFound)
			return false;

		GetGameWorkingDirRelativePath(&temp);

		if (GetFileAttributes(temp.m_szString) == INVALID_FILE_ATTRIBUTES)
			return false;
	}

	return true;
}

void File::ExtractFilePath(const char* inFilePath, char* outDirectory, char* outFileName, char* outFileExtension)
{
	char buff[256];
	memset(&buff, NULL, sizeof(buff));

	char* dotPos = strchr(buff, '.');
	if (dotPos)
	{
		*dotPos = NULL;
		if (outFileExtension)
			while (true)
				if (*dotPos == NULL) break; else *(outFileExtension++) = *(dotPos++);
		else
			*outFileExtension = NULL;
	}

	char* slashPos = strrchr(buff, '/');
	slashPos = slashPos == nullptr ? strrchr(buff, '\\') : slashPos;

	if (slashPos)
	{
		if (outFileName)
			strcpy(outFileName, slashPos + 1);
		*(slashPos + 1) = NULL;
		if (outDirectory)
			strcpy(outDirectory, buff);
	}
	else {
		if (outDirectory)
			*outDirectory = NULL;
		if (outFileName)
			strcpy(outFileName, buff);
	}
}

FileWrapper::FileWrapper(const char* _filename, int _desiredaccess, bool _createifnotfound)
{
	m_File = NULL;
	m_CreateIfNotFound = _createifnotfound;
	m_DesiredAccess_1 = _desiredaccess;
	m_Buffer = m_BufferBegin = m_BufferEnd = nullptr;
	m_CreationDisposition = NULL;
	m_DesiredAccess_2 = NULL;
	m_ExecuteAttribute = NULL;
	m_FilePosition = NULL;
	m_Read = false;

	InitializeCriticalSection(&m_CriticalSection);

	if (!_filename)
		return;

	m_WorkingDir = _filename;
	m_GameWorkingDir = _filename;
	GetWorkingDirRelativePath(&m_WorkingDir);
	GetGameWorkingDirRelativePath(&m_GameWorkingDir);

	if (_desiredaccess & FILE_READ_EA)
		File::IsFileExists(m_GameWorkingDir.m_szString);

	m_DesiredAccess_2 = NULL;

	if (_desiredaccess & FILE_READ_DATA)
	{
		m_DesiredAccess_2 = 0x80000000;
		m_Read = true;
	}
	else
		if (_desiredaccess & FILE_WRITE_DATA)
		{
			m_DesiredAccess_2 = 0x40000000;
			m_Read = false;
		}

	if (_desiredaccess & FILE_READ_DATA)
		m_CreationDisposition = OPEN_EXISTING;
	else
		if (_desiredaccess & FILE_WRITE_DATA && _desiredaccess & FILE_WRITE_EA)
			m_CreationDisposition = OPEN_ALWAYS;
		else
			m_CreationDisposition = CREATE_ALWAYS;

	m_ExecuteAttribute = (_desiredaccess >> 5) & 1;
	m_FilePosition = NULL;
	m_Buffer = nullptr;

	if (_createifnotfound)
		CreateFile_();

	MESSAGE_CLASS_CREATED(FileWrapper);
}

FileWrapper::~FileWrapper()
{
	if (m_File)
	{
		if (m_Buffer != m_BufferBegin &&
			!m_Read)
		{
			DWORD numBytesWritten = NULL;
			WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, nullptr);
			m_BufferEnd = m_BufferBegin = m_Buffer;
		}
	}

	if (m_Buffer)
		Allocators::ReleaseMemory(m_Buffer, false);

	if (m_File && m_CreateIfNotFound)
		CloseHandle(m_File);

	MESSAGE_CLASS_DESTROYED(FileWrapper);
}

HANDLE FileWrapper::CreateFile_()
{
	EnterCriticalSection(&m_CriticalSection);

	m_File = CreateFile(m_WorkingDir.m_szString, m_DesiredAccess_2, FILE_SHARE_READ, NULL, m_CreationDisposition, FILE_FLAG_WRITE_THROUGH, NULL);

	if (m_File == INVALID_HANDLE_VALUE)
	{
		if (m_DesiredAccess_1 & FILE_WRITE_DATA)
		{
			char errbuf[512];
			memset(&errbuf, NULL, sizeof(errbuf));

			GetLastErrorMessage(errbuf);
			LogDump::LogA("Could not open file for writing: %s\n", errbuf);

			m_File = nullptr;
			LeaveCriticalSection(&m_CriticalSection);
			return m_File;
		}

		if (Window::GameDiscFound)
		{
			m_WorkingDir = m_GameWorkingDir;
			m_File = CreateFile(m_WorkingDir.m_szString, m_DesiredAccess_2, FILE_SHARE_READ, NULL, m_CreationDisposition, FILE_FLAG_WRITE_THROUGH, NULL);

			if (m_File == INVALID_HANDLE_VALUE)
			{
				m_File = nullptr;
				LeaveCriticalSection(&m_CriticalSection);
				return m_File;
			}
		}
	}

	if (m_File)
		m_Buffer = m_BufferBegin = m_BufferEnd = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(FILE_BUFFER_SIZE, NULL, NULL);

	LeaveCriticalSection(&m_CriticalSection);

	return m_File;
}

char FileWrapper::ReadBlock()
{
	EnterCriticalSection(&m_CriticalSection);
	
	if (m_BufferBegin == m_BufferEnd &&
		m_Read)
	{
		DWORD numBytesRead = NULL;
		ReadFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesRead, nullptr);
		m_BufferEnd = &m_Buffer[numBytesRead];
		m_BufferBegin = m_Buffer;
	}

	if (m_BufferBegin = m_BufferEnd)
	{
		LeaveCriticalSection(&m_CriticalSection);

		return -1;
	}

	++m_FilePosition;
	char endsym = *m_BufferBegin;
	m_BufferBegin += 1;

	if (m_ExecuteAttribute || endsym != '\r')
	{
		LeaveCriticalSection(&m_CriticalSection);

		return endsym;
	}else{
		if (m_BufferBegin == m_BufferEnd)
			ReadBlock_Internal();

		m_BufferBegin++;
		m_FilePosition++;

		LeaveCriticalSection(&m_CriticalSection);

		return '\n';
	}
}

int FileWrapper::GetPosition()
{
	EnterCriticalSection(&m_CriticalSection);
	int filepos = m_FilePosition;
	LeaveCriticalSection(&m_CriticalSection);

	return filepos;
}

int FileWrapper::Seek(int _pos)
{
	EnterCriticalSection(&m_CriticalSection);

	if (_pos != m_FilePosition)
	{
		if (m_Buffer != m_BufferBegin &&
			!m_Read)
		{
			DWORD numBytesRead = NULL;
			WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesRead, nullptr);
			m_BufferEnd = m_BufferBegin = m_Buffer;
		}

		if (m_Read)
		{
			if (m_BufferBegin != m_BufferEnd)
				SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, NULL, FILE_CURRENT);
			m_BufferEnd = m_BufferBegin = m_Buffer;
		}

		m_FilePosition = SetFilePointer(m_File, _pos, NULL, FILE_BEGIN);
	}

	LeaveCriticalSection(&m_CriticalSection);

	return m_FilePosition;
}

void FileWrapper::GetLastErrorMessage(char* _buf)
{
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), NULL, _buf, NULL, nullptr);
}

void FileWrapper::ReadBlock_Internal()
{
	if (m_Read)
	{
		DWORD numBytesRead = NULL;
		ReadFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesRead, nullptr);
		m_BufferEnd = &m_Buffer[numBytesRead];
		m_BufferBegin = m_Buffer;
	}
}

void FileWrapper::_436E30()
{
	if (m_Buffer != m_BufferBegin && m_Read)
	{
		DWORD numBytesWritten = NULL;
		WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, NULL);
		m_BufferEnd = m_BufferBegin = m_Buffer;
	}
}

void FileWrapper::_436E70()
{
	if (m_Read)
	{
		if (m_BufferBegin != m_BufferEnd)
			SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, NULL, FILE_CURRENT);
		m_BufferEnd = m_BufferBegin = m_Buffer;
	}
}

void FileWrapper::SetFileHandle(HANDLE _hnd)
{
	EnterCriticalSection(&m_CriticalSection);
	if (!m_CreateIfNotFound)
		m_File = _hnd;
	LeaveCriticalSection(&m_CriticalSection);
}

void FileWrapper::ReleaseFileHandle()
{
	EnterCriticalSection(&m_CriticalSection);
	if (!m_CreateIfNotFound)
		m_File = nullptr;
	LeaveCriticalSection(&m_CriticalSection);
}

void FileWrapper::_436FF0(HANDLE)
{
	EnterCriticalSection(&m_CriticalSection);

	if (m_File && m_Buffer != m_BufferBegin)
	{
		DWORD numBytesWritten = NULL;
		WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, nullptr);
		m_BufferEnd = m_BufferBegin = m_Buffer;
	}

	delete m_Buffer;

	if (m_File)
		CloseHandle(m_File);

	LeaveCriticalSection(&m_CriticalSection);
}

char FileWrapper::_WriteBufferBlockAndInsertNewLine(char _sym)
{
	EnterCriticalSection(&m_CriticalSection);

	if (m_BufferBegin - m_Buffer == FILE_BUFFER_SIZE &&
		m_Buffer != m_BufferBegin &&
		!m_Read)
	{
		DWORD numBytesWritten;
		WriteFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesWritten, nullptr);
		m_BufferEnd = m_BufferBegin = m_Buffer;
	}

	if (m_ExecuteAttribute ||
		_sym != '\n')
	{
		*m_BufferBegin = _sym;
		++m_BufferBegin;
		m_FilePosition++;

		LeaveCriticalSection(&m_CriticalSection);

		return true;
	}else{
		*m_BufferBegin = '\r';
		m_BufferBegin++;

		if (m_BufferBegin - m_Buffer == FILE_BUFFER_SIZE)
			_436E30();

		*m_BufferBegin = '\n';
		++m_BufferBegin;
		m_FilePosition += 2;

		LeaveCriticalSection(&m_CriticalSection);

		return true;
	}
}

int FileWrapper::WriteBuffer(const char* _buf, int _len)
{
	EnterCriticalSection(&m_CriticalSection);

	if (_len <= FILE_BUFFER_SIZE)
	{
		if (_len > 0)
		{
			char* _buf_tmp = (char*)_buf;
			for (int ind = _len; ind; ind--)
				_WriteBufferBlockAndInsertNewLine(*_buf_tmp++);
		}
		LeaveCriticalSection(&m_CriticalSection);

		return _len;
	}else{
		if (m_Buffer != m_BufferBegin && !m_Read)
		{
			DWORD numBytesWritten = NULL;
			WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, nullptr);
			m_BufferEnd = m_BufferBegin = m_Buffer;
		}

		int ind = NULL;
		char* buf = nullptr;
		if (m_ExecuteAttribute)
		{
			buf = (char*)_buf;
			ind = _len;
		}else{
			int ind_ = NULL;
			for (buf = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(_len * 2, NULL, NULL); ind_ < _len; ++ind_)
			{
				if (_buf[ind_] == '\n')
				{
					buf[ind++] = '\r';
					buf[ind] = '\n';
				}else
					buf[ind] = _buf[ind_];

				++ind;
			}
		}
		DWORD numBytesWritten = NULL;
		WriteFile(m_File, buf, ind, &numBytesWritten, nullptr);
		m_FilePosition += numBytesWritten;

		LeaveCriticalSection(&m_CriticalSection);

		return numBytesWritten;
	}
}

char FileWrapper::ReadBlockDecreasePosition()
{
	EnterCriticalSection(&m_CriticalSection);

	if (m_BufferBegin == m_Buffer)
		ReadBlockFromOffset();
	else
		m_BufferBegin -= 1;

	--m_FilePosition;

	if (*m_BufferBegin == '\n' &&
		!m_ExecuteAttribute)
	{
		if (m_BufferBegin == m_Buffer)
		{
			ReadBlockFromOffset();
			--m_FilePosition;

			LeaveCriticalSection(&m_CriticalSection);
			return true;
		}

		m_BufferBegin -= 1;
		--m_FilePosition;
	}

	LeaveCriticalSection(&m_CriticalSection);
	return true;
}

char FileWrapper::_437790()
{
	if (ReadBlock() == -1)
		return 1;

	ReadBlockDecreasePosition();

	return 0;
}

void FileWrapper::ReadBlockFromOffset()
{
	SetFilePointer(m_File, m_Buffer - m_BufferEnd - 20, NULL, FILE_CURRENT);

	if (m_Read)
	{
		DWORD numBytesRead = NULL;
		ReadFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesRead, nullptr);
		m_BufferEnd = &m_Buffer[numBytesRead];
		m_BufferBegin = m_Buffer;
	}

	m_BufferBegin = m_Buffer + 19;
}

#pragma message(TODO_IMPLEMENTATION)
int FileWrapper::Read(LPVOID _buf, int _numbytestoread)
{
	return (*(int(__thiscall*)(FileWrapper*, LPVOID, int))0x437230)(this, _buf, _numbytestoread);
}

char FileWrapper::WriteFromBufferAndSetToEnd()
{
	EnterCriticalSection(&m_CriticalSection);

	if (m_File)
	{
		if (m_Buffer != m_BufferBegin && !m_Read)
		{
			DWORD numBytesRead = NULL;
			WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesRead, nullptr);
			m_BufferEnd = m_BufferBegin = m_Buffer;
		}

		if (m_Read)
		{
			if (m_BufferBegin != m_BufferEnd)
				SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, NULL, FILE_CURRENT);
			m_BufferEnd = m_BufferBegin = m_Buffer;
		}

		m_FilePosition = SetFilePointer(m_File, NULL, NULL, FILE_END);
	}

	LeaveCriticalSection(&m_CriticalSection);

	return true;
}

void File::ReadZipDirectories(const char* szFileSystem)
{
	if (szFileSystem && *szFileSystem) {
		int nCurrentArchiveIndex = 0;
		char szZipName[255];

		memset(&szZipName, 0, sizeof(szZipName));

		while (true) {
			int nCharIndex = 0;
			do {
				char cCurrentChar = szFileSystem[nCurrentArchiveIndex];

				if (!cCurrentChar)
					break;
				if (cCurrentChar == ' ' || cCurrentChar == ',')
					break;

				++nCurrentArchiveIndex;
				szZipName[nCharIndex++] = cCurrentChar;
			} while (nCharIndex < 255);

			szZipName[nCharIndex] = 0;

			while (true) {
				char cCurrentChar = szFileSystem[nCurrentArchiveIndex];

				if (cCurrentChar != ' ' && cCurrentChar != ',')
					break;

				++nCurrentArchiveIndex;
			}

			OpenZip(szZipName);

			char sZipNameLocalised[32];
			memset(sZipNameLocalised, NULL, sizeof(sZipNameLocalised));

			strcpy(sZipNameLocalised, Script::LanguageMode.m_szString);
			strcat(sZipNameLocalised, "_");
			strcat(sZipNameLocalised, szZipName);

			OpenZip(sZipNameLocalised);

			if (!szFileSystem[nCurrentArchiveIndex])
				break;

			memset(&szZipName, 0, sizeof(szZipName));
		}
	}
}

ULARGE_INTEGER File::GetStorageFreeSpace()
{
	return FileWrapper::GetStorageFreeSpace();
}

#pragma message(TODO_IMPLEMENTATION)
void File::OpenZip(const char* szZipPath)
{
	if (!IsFileExists(szZipPath))
		return;

	int slotId = ZipArch::SlotId++;
	LogDump::LogA("Opening zip <%s> into slot %i\n", szZipPath, ZipArch::SlotId);

	ZipArch::SlotInfo[slotId].m_Flags.field_0 = 15;
	ZipArch::ZipNames[slotId] = szZipPath;

	FileWrapper::ZipFilesArray[slotId] = new FileWrapper(szZipPath, 33, true);
	FileWrapper::ZipFilesArray[slotId]->WriteFromBufferAndSetToEnd();

	if (FileWrapper::ZipFilesArray[slotId]->GetPosition())
	{
		FilesSemaphoreArray[slotId] = CreateSemaphore(NULL, 1, 1, nullptr);
		FileWrapper::ZipFilesArray[slotId]->Seek(FileWrapper::ZipFilesArray[slotId]->GetPosition() - 22);

		unsigned char	EndOfCentralDirectoryRecord[20] = {};	//	TODO: maybe this can become actual struct from ZIP library.
		bool			ValidZipFile = false;

		FileWrapper::ZipFilesArray[slotId]->Read(EndOfCentralDirectoryRecord, sizeof(EndOfCentralDirectoryRecord));
		if (EndOfCentralDirectoryRecord[0] +
			((EndOfCentralDirectoryRecord[1] + ((EndOfCentralDirectoryRecord[2] + (EndOfCentralDirectoryRecord[3] << 8)) << 8)) << 8) == FILE_ZIP_MAGIC_HEADER)
			ValidZipFile = true;

		unsigned int FilesInArchive = EndOfCentralDirectoryRecord[10] + (EndOfCentralDirectoryRecord[11] << 8);
		unsigned int CentralDirectorySize = EndOfCentralDirectoryRecord[12] + ((EndOfCentralDirectoryRecord[13] + ((EndOfCentralDirectoryRecord[14] + (EndOfCentralDirectoryRecord[15] << 8)) << 8)) << 8);
		unsigned int CentralDirectoryStartOffset = EndOfCentralDirectoryRecord[16] + ((EndOfCentralDirectoryRecord[17] + ((EndOfCentralDirectoryRecord[18] + (EndOfCentralDirectoryRecord[19] << 8)) << 8)) << 8);

		FileWrapper::ZipFilesArray[slotId]->Seek(CentralDirectoryStartOffset);

		unsigned char* FileHeaderArray = new unsigned char[CentralDirectorySize];
		FileWrapper::ZipFilesArray[slotId]->Read(FileHeaderArray, CentralDirectorySize);

		std::map<unsigned int, ZipSlotInfo::FileInfo> FilesMap;

		if (FilesInArchive > 0)
		{
			for (unsigned int i = FilesInArchive; i; i--)
			{
				String filePath;

				if (ValidZipFile)
					FileHeaderArray -= 8;

				char			fileNameBuffer[MAX_PATH] = {};
				char*			fileNameBufferPtr = &fileNameBuffer[0];
				unsigned int	fileOffset = FileHeaderArray[24] + ((FileHeaderArray[25] + ((FileHeaderArray[26] + (FileHeaderArray[27] << 8)) << 8)) << 8);
				unsigned int	filePathStrlen = FileHeaderArray[28] + (FileHeaderArray[29] << 8);
				unsigned short	nextOffset_1 = FileHeaderArray[30] + (FileHeaderArray[31] << 8);
				unsigned short	nextOffset_2 = FileHeaderArray[32] + (FileHeaderArray[33] << 8);
				unsigned int	nextOffset_3 = FileHeaderArray[42] + ((FileHeaderArray[43] + ((FileHeaderArray[44] + (FileHeaderArray[45] << 8)) << 8)) << 8);

				if (ValidZipFile)
					FileHeaderArray += 8;

				if (filePathStrlen > NULL)
				{
					unsigned char* filenameStartOffset = FileHeaderArray + 46;

					for (unsigned int j = filePathStrlen; j != 1; j--)
					{
						if (ValidZipFile)
							*fileNameBufferPtr++ = (*filenameStartOffset << 2) | (*filenameStartOffset >> 6);
						else
							*fileNameBufferPtr++ = *filenameStartOffset;
						
						filenameStartOffset++;
					}
				}

				FileHeaderArray += filePathStrlen + nextOffset_1 + nextOffset_2 + 46;
				unsigned int FileChecksum = nextOffset_1 + nextOffset_3 + filePathStrlen + 30;

				String::ConvertBackslashes(fileNameBuffer);
				String::ToLowerCase(fileNameBuffer);

				unsigned int checksum = Utils::CalcCRC32(fileNameBuffer, strlen(fileNameBuffer));
				std::pair<unsigned int, ZipSlotInfo::FileInfo> tmp = { checksum, { FileChecksum, fileOffset } };
				FilesMap.insert(tmp);
			}
		}

		ZipArch::SlotInfo[slotId].AddFileInfo(FilesMap);

		delete[] FileHeaderArray;
	}
}