#pragma once

#include "stdafx.h"

class KapowFile
{
	//	Possible size is 92 (0x5C) bytes.
private:
	int m_nUnkInt1;
	int m_nUnkInt2;
	int m_nUnkInt3;
	int m_nUnkInt4;
	int m_nUnkInt5;
	int m_nUnkInt6;
	HANDLE m_hFile;
	int m_nUnkInt8;
	char *m_szFileName;
	int m_nUnkInt10;
	int m_nUnkCurrentFileIndex;
	int m_nUnkInt12;
	int m_nUnkInt13;
	int m_nUnkInt14;
	int m_nUnkInt15;
	int m_nUnkInt16;
	int m_nUnkInt17;
	int m_dwCreationDisposition;
	int m_dwDesiredAccess;
	BYTE m_bCreateIfNotFound;
	BYTE m_bUnkByte1;
	BYTE m_bUnkByte2;
	BYTE m_bUnkByte3;
	char *m_szBuffer;
	int m_pUnkAllocatedSpace2;
	char *m_pUnkAllocatedSpace3;

public:
	KapowFile();
};