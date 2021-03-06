#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <direct.h>
#include <string>
#include <vector>
#include <map>

#include "Utils.h"

//	NOTE: abstract reader. Doesn't do anything and relies on implementations of derived classes.
class GenericResourceReader
{
protected:
	FILE*				m_FilePtr = nullptr;
	WIN32_FIND_DATA		m_FindInfo = {};
	HANDLE				m_FindHandle = INVALID_HANDLE_VALUE;
	errno_t				m_LastErrorCode = NULL;
	std::string			m_CurrentWorkingDirectory;
	std::string			m_ResourceName;
	std::string			m_ResourceExtension;
	long				m_ResourceFileSize = NULL;
	long				m_ResourceFileSizeKb = NULL;
	std::vector<std::string>	m_LastOpenFiles;
	PlatformDefinition	m_WorkingPlatform = PLATFORM_PC;

	bool				OpenFirstResourceFile();

public:
	GenericResourceReader() {};
	~GenericResourceReader();

	virtual void		ReadInfo() = 0;
	virtual void		PrintInfo() const = 0;
	virtual void		DumpData() const;

	void				PrintFileInfo();
	bool				OpenResourceFile();
	bool				OpenNextResourceFile();
	errno_t				GetLastErrorCode() const;
	void				SetCurrentWorkingDir(const char* const dir);
	void				PrintError() const;
	void				SetPlatform(PlatformDefinition);
};