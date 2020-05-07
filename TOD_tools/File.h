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
	void* m_nUnkInt1;
	int m_nUnkInt2;
	int m_nUnkInt3;
	int m_nUnkInt4;
	int m_nUnkInt5;
	int m_nUnkInt6;
	HANDLE m_hFile;
	String m_sFilePath;
	String m_sFilePath2;
	int m_nDesiredAccess1;
	int m_nFilePosition;
	int m_nCreationDisposition;
	int m_nDesiredAccess2;
	BYTE m_bCreateIfNotFound;
	BYTE m_bExecute;
	BYTE m_bRead;
	BYTE m_bUnkByte3;
	char* m_pBuffer;
	char* m_pBufferBegin;
	char* m_pBufferEnd;

public:
	File()
	{
		debug("File created at %X\n", this);
	}

	~File()
	{
		debug("File destroyed!\n");
	}

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	void	Open(const char* szFileName, int nMode, bool bCreateIfNotFound)	//	@A085A8
	{
		void(__thiscall * _Open)(File * _this, const char* _fn, int _mode, bool _create) = (void(__thiscall*)(File*, const char*, int, bool))0x438180;

		_Open(this, szFileName, nMode, bCreateIfNotFound);
	}
	bool	WriteFromBufferOrSetToBegin()	//	@437690
	{
		bool(__thiscall * _Reset)(File * _this) = (bool(__thiscall*)(File*))0x437690;

		return _Reset(this);
	}
	int		GetPosition()	//	@436DF0
	{
		int(__thiscall * _GetPos)(File * _this) = (int(__thiscall*)(File*))0x436DF0;

		return _GetPos(this);
	}
	void	Seek(int nPosition)	//	@437560
	{
		void(__thiscall * _Seek)(File * _this, int _pos) = (void(__thiscall*)(File*, int))0x437560;

		_Seek(this, nPosition);
	}
	int		Read(void* pBuffer, int nNumberOfBytesToRead)	//	@437230
	{
		int(__thiscall * _Read)(File * _this, void* _buf, int _toread) = (int(__thiscall*)(File*, void*, int))0x437230;

		return _Read(this, pBuffer, nNumberOfBytesToRead);
	}

	static File* g_FilesArray[8];
	static HANDLE* g_FileSemaphores[8];
};

extern File * g_File;

static_assert(sizeof(File) == FILE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("File"));