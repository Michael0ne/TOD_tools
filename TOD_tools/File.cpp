#include "File.h"
#include "LogDump.h"
#include "ZipArch.h"
#include "Globals.h"

unsigned int File::FilesOpen = 0;
HANDLE	File::FilesSemaphoreArray[FILE_MAX_OPEN_FILES];
File* File::FilesArray[FILE_MAX_OPEN_FILES];

extern void GetWorkingDirRelativePath(String* str);
extern void GetGameWorkingDirRelativePath(String* str);
extern bool IsFileExists(const char* file);

void FileWrapper::SetExecuteAttrib(unsigned char _attr)
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);
	m_ExecuteAttribute = _attr;
	LeaveCriticalSection((LPCRITICAL_SECTION)this);
}

bool File::WriteBuffers()
{
	if (m_ReadFromZip)
		return false;

	EnterCriticalSection((LPCRITICAL_SECTION)m_FileHandle);

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

	LeaveCriticalSection((LPCRITICAL_SECTION)m_FileHandle);

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

int File::_CheckFormattingSymbol(File* _f, const char* _format, ...)
{
	va_list va;
	va_start(va, _format);

	if (m_ReadFromZip)
		return (int)_f;	//	FIXME: wtf?
	else
		return _f->_CheckFormattingSymbol_A((char*)_format, (int*)&va);	//	FIXME: wtf?
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
int File::_CheckFormattingSymbol_A(char* _buf, int* a1)
{
	//	NOTE: this checks input string for formatting symbol and does something with it. Why is it here?
	return (*(int(__thiscall*)(File*, char*, int*))0x42F0A0)(this, _buf, a1);
}

char File::ReadBlock()
{
	if (!m_ReadFromZip)
		return m_FileHandle->ReadBlock_A();

	if (m_SeekPosition >= m_ZipPos)
		return -1;

	WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);
	
	FilesArray[m_ZipSlot]->m_FileHandle->SetExecuteAttrib(m_ExecuteAttribute);
	FilesArray[m_ZipSlot]->Seek(m_ZipIndex + m_SeekPosition);
	char bytesread = FilesArray[m_ZipSlot]->m_FileHandle->ReadBlock_A();
	m_SeekPosition = FilesArray[m_ZipSlot]->GetPosition() - m_ZipIndex;

	ReleaseSemaphore(FilesArray[m_ZipSlot], 1, 0);

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

	if (m_SeekPosition >= m_ZipPos)
		return NULL;

	WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);
	FilesArray[m_ZipSlot]->Seek(m_SeekPosition + m_ZipIndex);
	FilesArray[m_ZipSlot]->m_FileHandle->SetExecuteAttrib(true);

	if (m_ExecuteAttribute)
	{
		int bytesread = FilesArray[m_ZipSlot]->Read(_buffer, ((m_SeekPosition + _numbytestoread) > m_ZipPos) ? m_ZipPos - m_SeekPosition : _numbytestoread);

		m_SeekPosition = FilesArray[m_ZipSlot]->GetPosition() - m_ZipIndex;
		ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

		return bytesread;
	}else{
		int bytesread = NULL;

		for (; bytesread < _numbytestoread; bytesread++)
		{
			if (FilesArray[m_ZipSlot]->GetPosition() >= m_ZipPos + m_ZipIndex)
				break;
			char lastchar = FilesArray[m_ZipSlot]->ReadBlock();
			if (lastchar == '\n' ||
				lastchar == '\r')
			{
				char lastchar_2 = FilesArray[m_ZipSlot]->ReadBlock();
				if ((lastchar_2 == '\n' || lastchar_2 == '\r') && lastchar == lastchar_2)
					FilesArray[m_ZipSlot]->ReadBlockDecreasePosition();
				lastchar = '\n';
			}
			((char*)(_buffer))[bytesread] = lastchar;
		}

		m_SeekPosition = FilesArray[m_ZipSlot]->GetPosition() - m_ZipIndex;
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

	EnterCriticalSection((LPCRITICAL_SECTION)m_FileHandle);
	
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

	LeaveCriticalSection((LPCRITICAL_SECTION)m_FileHandle);
	return true;
}

char File::WriteFromBuffer()
{
	if (!m_ReadFromZip)
		return m_FileHandle->WriteFromBufferAndSetToEnd();

	m_SeekPosition = m_ZipPos;

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

	FilesArray[m_ZipSlot]->Seek(m_SeekPosition + m_ZipIndex);
	FilesArray[m_ZipSlot]->m_FileHandle->SetExecuteAttrib(m_ExecuteAttribute);
	FilesArray[m_ZipSlot]->ReadBlockDecreasePosition();
	m_SeekPosition = FilesArray[m_ZipSlot]->GetPosition() - m_ZipIndex;

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

const char* File::GetFileName()
{
	return m_FileName.m_szString;
}

#pragma message(TODO_IMPLEMENTATION)
File::File(const char* _filename, int _desiredaccess, bool _createifnotfound)
{
	int _zipfileinfo[2] = { NULL, NULL };

	m_FileName.Set(_filename);	//	TODO: call to 'GetPathFromDirectoryMappings' should be here instead.
	m_FileHandle = nullptr;
	m_ReadFromZip = false;
	m_ZipSlot = -1;
	m_FileReadAttribute = (_desiredaccess >> 7) & 1;

	//	Try and read this file from open zip archive.
	if (_desiredaccess & 1 &&
		ZipArch::SlotId > 0 &&
		ZipArch::FindFile(_filename, _zipfileinfo, &m_ZipSlot))
	{
		m_ReadFromZip = true;
		m_ZipPos = _zipfileinfo[1];
		m_ZipIndex = _zipfileinfo[0];
		m_SeekPosition = NULL;
		m_ExecuteAttribute = (_desiredaccess >> 5) & 1;
		m_ZipFileHandle = nullptr;
	}else
		m_FileHandle = new FileWrapper(m_FileName.m_szString, _desiredaccess, _createifnotfound);

	if ((m_ReadFromZip && m_ZipIndex >= NULL) ||
		(m_FileHandle->m_File != NULL))
		++FilesOpen;

	MESSAGE_CLASS_CREATED(File);
}

File::~File()
{
	if (m_ReadFromZip)
		if (m_ZipFileHandle)
			delete m_ZipFileHandle;

	if ((m_ReadFromZip && m_ZipIndex >= 0) || (m_FileHandle->m_File != nullptr))
		--FilesOpen;

	if (m_FileHandle)
		delete m_FileHandle;
}

bool File::IsFileOpen()
{
	return (m_ReadFromZip && m_ZipIndex >= 0) || m_FileHandle->m_File != nullptr;
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

FileWrapper::FileWrapper(const char* _filename, int _desiredaccess, bool _createifnotfound)
{
	InitializeCriticalSection((LPCRITICAL_SECTION)this);

	m_File = nullptr;
	m_CreateIfNotFound = _createifnotfound;
	m_DesiredAccess_1 = _desiredaccess;

	if (!_filename)
		return;

	GetWorkingDirRelativePath(&m_WorkingDir);
	GetGameWorkingDirRelativePath(&m_GameWorkingDir);

	if (_desiredaccess & FILE_READ_EA)
		IsFileExists(m_GameWorkingDir.m_szString);

	m_DesiredAccess_2 = NULL;

	if (_desiredaccess & FILE_READ_DATA)
	{
		m_DesiredAccess_2 = 0x80000000;
		m_Read = true;
	}else
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
		Create();

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

HANDLE FileWrapper::Create()
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);

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
			LeaveCriticalSection((LPCRITICAL_SECTION)this);
			return m_File;
		}

		if (GameDiscFound)
		{
			m_WorkingDir = m_GameWorkingDir;
			m_File = CreateFile(m_WorkingDir.m_szString, m_DesiredAccess_2, FILE_SHARE_READ, NULL, m_CreationDisposition, FILE_FLAG_WRITE_THROUGH, NULL);

			if (m_File == INVALID_HANDLE_VALUE)
			{
				m_File = nullptr;
				LeaveCriticalSection((LPCRITICAL_SECTION)this);
				return m_File;
			}
		}
	}

	if (m_File)
		m_Buffer = m_BufferBegin = m_BufferEnd = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(FILE_BUFFER_SIZE, NULL, NULL);

	LeaveCriticalSection((LPCRITICAL_SECTION)this);

	return m_File;
}

char FileWrapper::ReadBlock_A()
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);
	
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
		LeaveCriticalSection((LPCRITICAL_SECTION)this);

		return -1;
	}

	++m_FilePosition;
	char endsym = *m_BufferBegin;
	m_BufferBegin += 1;

	if (m_ExecuteAttribute || endsym != '\r')
	{
		LeaveCriticalSection((LPCRITICAL_SECTION)this);

		return endsym;
	}else{
		if (m_BufferBegin == m_BufferEnd)
			ReadBlock();

		m_BufferBegin++;
		m_FilePosition++;

		LeaveCriticalSection((LPCRITICAL_SECTION)this);

		return '\n';
	}
}

int FileWrapper::GetPosition()
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);
	int filepos = m_FilePosition;
	LeaveCriticalSection((LPCRITICAL_SECTION)this);

	return filepos;
}

int FileWrapper::Seek(int _pos)
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);

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

	LeaveCriticalSection((LPCRITICAL_SECTION)this);

	return m_FilePosition;
}

void FileWrapper::GetLastErrorMessage(char* _buf)
{
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), NULL, _buf, NULL, nullptr);
}

void FileWrapper::ReadBlock()
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
	EnterCriticalSection((LPCRITICAL_SECTION)this);
	if (!m_CreateIfNotFound)
		m_File = _hnd;
	LeaveCriticalSection((LPCRITICAL_SECTION)this);
}

char FileWrapper::_WriteBufferBlockAndInsertNewLine(char _sym)
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);

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

		LeaveCriticalSection((LPCRITICAL_SECTION)this);

		return true;
	}else{
		*m_BufferBegin = '\r';
		m_BufferBegin++;

		if (m_BufferBegin - m_Buffer == FILE_BUFFER_SIZE)
			_436E30();

		*m_BufferBegin = '\n';
		++m_BufferBegin;
		m_FilePosition += 2;

		LeaveCriticalSection((LPCRITICAL_SECTION)this);

		return true;
	}
}

int FileWrapper::WriteBuffer(const char* _buf, int _len)
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);

	if (_len <= FILE_BUFFER_SIZE)
	{
		if (_len > 0)
		{
			char* _buf_tmp = (char*)_buf;
			for (int ind = _len; ind; ind--)
				_WriteBufferBlockAndInsertNewLine(*_buf_tmp++);
		}
		LeaveCriticalSection((LPCRITICAL_SECTION)this);

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

		LeaveCriticalSection((LPCRITICAL_SECTION)this);

		return numBytesWritten;
	}
}

char FileWrapper::ReadBlockDecreasePosition()
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);

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

			LeaveCriticalSection((LPCRITICAL_SECTION)this);
			return true;
		}

		m_BufferBegin -= 1;
		--m_FilePosition;
	}

	LeaveCriticalSection((LPCRITICAL_SECTION)this);
	return true;
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

int FileWrapper::Read(void* _buf, int _numbytestoread)
{
	//	NOTE: this is ridiculous.
	EnterCriticalSection((LPCRITICAL_SECTION)this);

	if (_numbytestoread <= FILE_BUFFER_SIZE)
	{
		if (m_ExecuteAttribute)
		{
			if ((m_BufferEnd - m_BufferBegin) >= _numbytestoread)
			{
				memcpy(_buf, m_BufferBegin, _numbytestoread);
				m_FilePosition += _numbytestoread;
				m_BufferBegin = &m_BufferBegin[_numbytestoread];

				LeaveCriticalSection((LPCRITICAL_SECTION)this);
				return _numbytestoread;
			}

			memcpy(_buf, m_BufferBegin, m_BufferEnd - m_BufferBegin);
			m_BufferBegin = m_BufferEnd;

			ReadBlock();

			if (m_BufferBegin == m_BufferEnd)
			{
				m_FilePosition += m_BufferEnd - m_BufferBegin;

				LeaveCriticalSection((LPCRITICAL_SECTION)this);
				return m_BufferEnd - m_BufferBegin;
			}

			int _len = (m_BufferEnd - m_BufferBegin);
			if (_len >= (_numbytestoread - _len))
				_len = _numbytestoread - _len;
			memcpy((void*)((m_BufferEnd - m_BufferBegin) + (int)_buf), m_BufferBegin, _len);

			m_BufferBegin = &m_BufferBegin[_len];
			m_FilePosition += _len * 2;

			LeaveCriticalSection((LPCRITICAL_SECTION)this);
			return _len * 2;
		}else{
			if (_numbytestoread > 0)
			{
				int ind = 0;
				while (true)
				{
					char _sym = ReadBlock_A();
					if (_sym == -1)
						break;

					((char*)_buf)[ind] = _sym;
					if (ind >= _numbytestoread)
					{
						LeaveCriticalSection((LPCRITICAL_SECTION)this);
						return ind;
					}
				}
			}else{
				LeaveCriticalSection((LPCRITICAL_SECTION)this);
				return NULL;
			}
		}
	}

	if (m_Read)
	{
		if (m_BufferBegin != m_BufferEnd)
			SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, NULL, FILE_CURRENT);
		m_BufferEnd = m_BufferBegin = m_Buffer;
	}

	DWORD numBytesRead = NULL;
	ReadFile(m_File, _buf, _numbytestoread, &numBytesRead, nullptr);
	m_FilePosition += numBytesRead;

	if (m_ExecuteAttribute)
	{
		LeaveCriticalSection((LPCRITICAL_SECTION)this);
		return numBytesRead;
	}

	int bytesread = NULL, bytesread_ = NULL;
	if (numBytesRead)
	{
		for (unsigned int ind = 0; ind < numBytesRead; ind++, bytesread_++)
		{
			if (!ind && *(char*)_buf == '\n')
			{
				ind = 1;
				if (numBytesRead <= 1)
					break;
			}

			if (((char*)_buf)[ind] == '\r')
			{
				((char*)_buf)[bytesread_] = '\n';
				++ind;
			}else
				((char*)_buf)[bytesread_] = ((char*)_buf)[ind];
		}
	}

	LeaveCriticalSection((LPCRITICAL_SECTION)this);
	return bytesread;
}

char FileWrapper::WriteFromBufferAndSetToEnd()
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);

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

	LeaveCriticalSection((LPCRITICAL_SECTION)this);

	return true;
}