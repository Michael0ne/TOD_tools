#pragma once

#include "stdafx.h"
#include "KapowStringsPool.h"

//	NOTE: When performing a Read operation, file is read into m_pBuffer with this size (~128Kb)
#define FILE_BLOCK_SIZE 131072

class File
{
	//	Size is 92 (0x5C) bytes.
private:
	int m_nUnkInt1;
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
	char *m_pBuffer;
	char *m_pBufferBegin;
	char *m_pBufferEnd;

public:
	File();
};