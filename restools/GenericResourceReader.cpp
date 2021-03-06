#include "GenericResourceReader.h"
#include <direct.h>
#include <fileapi.h>

bool GenericResourceReader::OpenFirstResourceFile()
{
	if (m_ResourceName.empty())
		return false;

	m_FindHandle = FindFirstFileA(m_ResourceName.c_str(), &m_FindInfo);
	if (m_FindHandle == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if (m_FindInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		m_LastOpenFiles.push_back(m_FindInfo.cFileName);
	} while (FindNextFileA(m_FindHandle, &m_FindInfo) != NULL);

	FindClose(m_FindHandle);

	if (m_LastOpenFiles.empty())
		return false;

	m_ResourceName = m_LastOpenFiles.back();
	m_LastOpenFiles.pop_back();

	return OpenResourceFile();
}

GenericResourceReader::~GenericResourceReader()
{
	if (m_FilePtr)
		fclose(m_FilePtr);
}

void GenericResourceReader::DumpData() const
{
	printf("Dump of this resource type is not implemented!\nSorry :(\n");
}

void GenericResourceReader::PrintFileInfo()
{
	printf("******************\n");
	printf("\tFilename: %s\n", m_ResourceName.c_str());
	printf("\tFilesize: %d (%d Kb)\n", m_ResourceFileSize, m_ResourceFileSizeKb);
	printf("******************\n");
}

bool GenericResourceReader::OpenResourceFile()
{
	if (m_ResourceName.empty())
		return false;

	if (m_ResourceName.c_str()[0] == '*')
		return OpenFirstResourceFile();

	m_LastErrorCode = fopen_s(&m_FilePtr, m_ResourceName.c_str(), "rb");

	if (!m_FilePtr)
		return false;

	fseek(m_FilePtr, NULL, SEEK_END);
	m_ResourceFileSize = ftell(m_FilePtr);
	m_ResourceFileSizeKb = m_ResourceFileSize / 1024;
	rewind(m_FilePtr);

	return true;
}

bool GenericResourceReader::OpenNextResourceFile()
{
	if (m_LastOpenFiles.empty())
		return false;

	m_ResourceName = m_LastOpenFiles.back();
	m_LastOpenFiles.pop_back();

	return OpenResourceFile();
}

inline errno_t GenericResourceReader::GetLastErrorCode() const
{
	return m_LastErrorCode;
}

void GenericResourceReader::SetCurrentWorkingDir(const char* const dir)
{
	m_LastErrorCode = _chdir(dir);
}

void GenericResourceReader::PrintError() const
{
	if (m_LastErrorCode)
	{
		printf("******************\n");
		printf("\tERROR: %d\n", m_LastErrorCode);
		printf("\t\"%s\"\n", strerror(m_LastErrorCode));
		printf("******************\n");
	}
}

void GenericResourceReader::SetPlatform(PlatformDefinition platform)
{
	m_WorkingPlatform = platform;
}