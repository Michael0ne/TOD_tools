#include "File.h"
#include "Globals.h"

File* File::g_FilesArray[8] = {
	(File*)0xA35DB8,
	(File*)0xA35DBC,
	(File*)0xA35DC0,
	(File*)0xA35DC4,
	(File*)0xA35DC8,
	(File*)0xA35DCC,
	(File*)0xA35DD0,
	(File*)0xA35DD4
};

HANDLE* File::g_FileSemaphores[8] = {
	(HANDLE*)0xA35D98,
	(HANDLE*)0xA35D9C,
	(HANDLE*)0xA35DA0,
	(HANDLE*)0xA35DA4,
	(HANDLE*)0xA35DA8,
	(HANDLE*)0xA35DAC,
	(HANDLE*)0xA35DB0,
	(HANDLE*)0xA35DB8
};

bool& File::GameDiscFound = *(bool*)0xA35E68;	//	@A35E68

File::File(const char* fileName, int dwDesiredAccess, bool createIfNotFound)
{
	MESSAGE_CLASS_CREATED(File);

	m_WorkingDir = String();
	m_GameWorkingDir = String();

	InitializeCriticalSection((LPCRITICAL_SECTION)this);

	m_hFile = nullptr;
	m_bCreateIfNotFound = createIfNotFound;
	m_nDesiredAccess1 = dwDesiredAccess;

	if (!fileName)
		return;

	String sFileName(fileName);

	Utils::GetWorkingDirRelativePath(&sFileName);
	Utils::GetGameWorkingDirRelativePath(&sFileName);

	if (dwDesiredAccess & FILE_READ_EA)
		Utils::ReadFileAttributes(sFileName.m_szString);

	m_nDesiredAccess2 = NULL;

	if (dwDesiredAccess & FILE_READ_DATA)
	{
		m_nDesiredAccess2 = 0x80000000;
		m_bRead = true;
	}else
		if (dwDesiredAccess & FILE_WRITE_DATA)
		{
			m_nDesiredAccess2 = 0x40000000;
			m_bRead = false;
		}

	if (dwDesiredAccess & FILE_READ_DATA)
		m_nCreationDisposition = OPEN_EXISTING;
	else
		if (dwDesiredAccess & FILE_WRITE_EA &&
			dwDesiredAccess & FILE_WRITE_DATA)
			m_nCreationDisposition = OPEN_ALWAYS;
		else
			m_nCreationDisposition = CREATE_ALWAYS;

	m_ExecuteAttribute = (dwDesiredAccess >> 5) & 1;
	m_nFilePosition = NULL;
	m_pBuffer = nullptr;

	if (createIfNotFound)
		Create();
}

File::~File()
{
	MESSAGE_CLASS_DESTROYED(File);

	if (m_hFile)
	{
		if (m_pBuffer != m_pBufferBegin && !m_bRead)
		{
			DWORD bytesWritten = 0;
			WriteFile(m_hFile, m_pBuffer, m_pBufferBegin - m_pBuffer, &bytesWritten, NULL);
			m_pBufferEnd = m_pBufferBegin = m_pBuffer;
		}
	}

	if (m_pBuffer)
		delete m_pBuffer;

	if (m_hFile && m_bCreateIfNotFound)
		CloseHandle(m_hFile);

	//	NOTE: destructors for strings are called here implicitly.
}

HANDLE File::Create()
{
	EnterCriticalSection((LPCRITICAL_SECTION)this);

	m_hFile = CreateFile(m_WorkingDir.m_szString, m_nDesiredAccess2, FILE_SHARE_READ, NULL, m_nCreationDisposition, FILE_FLAG_WRITE_THROUGH, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE) {
		if (m_nDesiredAccess1 & FILE_WRITE_DATA) {
			debug("Could not open file for writing: %s\n", GetLastErrorMessage());
			m_hFile = nullptr;

			LeaveCriticalSection((LPCRITICAL_SECTION)m_hFile);
			return m_hFile;
		}

		if (GameDiscFound)
		{
			m_WorkingDir = m_GameWorkingDir;
			m_hFile = CreateFile(m_WorkingDir.m_szString, m_nDesiredAccess2, FILE_SHARE_READ, NULL, m_nCreationDisposition, FILE_FLAG_WRITE_THROUGH, NULL);
		}

		if (m_hFile == INVALID_HANDLE_VALUE)
			m_hFile = nullptr;
	}

	if (m_hFile)
		m_pBuffer = m_pBufferBegin = m_pBufferEnd = new char[FILE_BLOCK_SIZE];

	LeaveCriticalSection((LPCRITICAL_SECTION)m_hFile);

	return m_hFile;
}

const char* File::GetLastErrorMessage()
{
	char* msgBuf = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), FORMAT_MESSAGE_FROM_STRING, msgBuf, NULL, NULL);

	return msgBuf;
}