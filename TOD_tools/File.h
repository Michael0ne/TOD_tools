#pragma once

#include "stdafx.h"
#include "StringsPool.h"
#include "MemoryAllocators.h"

#define FILE_CLASS_SIZE 92

//	NOTE: When performing a Read operation, file is read into m_pBuffer with this size (~128Kb)
#define FILE_BLOCK_SIZE 131072

class File
{
public:
	int m_nUnkInt1;
	int m_nUnkInt2;
	int m_nUnkInt3;
	int m_nUnkInt4;
	int m_nUnkInt5;
	int m_nUnkInt6;
	HANDLE m_hFile;
	String m_WorkingDir;
	String m_GameWorkingDir;	//	NOTE: used only in constructor, if game disc is detected, this value is used.
	int m_nDesiredAccess1;
	int m_nFilePosition;
	int m_nCreationDisposition;
	int m_nDesiredAccess2;
	BYTE m_bCreateIfNotFound;
	BYTE m_ExecuteAttribute;
	BYTE m_bRead;
	BYTE m_bUnkByte3;
	char* m_pBuffer;
	char* m_pBufferBegin;
	char* m_pBufferEnd;

public:
	File(const char* fileName, int dwDesiredAccess, bool createIfNotFound);		//	@438180
	~File();	//	@436F40

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	HANDLE		Create();	//	@4378D0
	const char* GetLastErrorMessage();	//	@437820

	static File* OpenedFiles[8];
	static HANDLE* g_FileSemaphores[8];
	static bool& GameDiscFound;	//	@A35E68
};

static_assert(sizeof(File) == FILE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(File));