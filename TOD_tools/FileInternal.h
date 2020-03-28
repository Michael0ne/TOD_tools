#pragma once

#include "stdafx.h"
#include "StringsPool.h"

class FileInternal;

struct FileInternal__vtable
{
	void(__stdcall* Release)(bool releasememory);
	bool(__thiscall* WriteBuffers)(FileInternal*);
	int (*vsnprintf)(FileInternal*, char* str, ...);
	int (*stub3)(FileInternal*, const char* str, ...);
	int(__thiscall* WriteFormattedVarlistDataToBuffer)(FileInternal*, char* str, va_list arguments);
	void(__thiscall* _CheckFormattingSymbol)(FileInternal*, char* str, int* unk);
	int(__thiscall* _ReadAndGetNewLineSymbol)(FileInternal*);
	void(__thiscall* stub7)(FileInternal*, int);
	void(__thiscall* snprintf)(FileInternal*, int);
	void(__thiscall* stub9)(FileInternal*, int);
	void(__thiscall* Read)(FileInternal*, char* buffer, int numofbytestoread);
	int(__thiscall* WriteBuffer)(FileInternal*, const char*);
	int(__thiscall* WriteBufferWithSize)(FileInternal*, const char* buffer, signed int size);
	void(__thiscall* Seek)(FileInternal*, int position);
	char(__thiscall* WriteBufferAndSetToStart)(FileInternal*);
	void(__thiscall* stub15)(FileInternal*, int);
	int(__thiscall* GetPosition)(FileInternal*);
	void(__thiscall* stub17)(FileInternal*, int);
	void(__thiscall* stub18)(FileInternal*, int);
	char* (__thiscall* GetFileName)(FileInternal*);
};


class FileInternal
{
private:
	FileInternal__vtable* lpVtbl;
	String m_sFileName;
	File* m_pFileHandle;
	byte m_b18;
	byte m_bReadFromZip;
	__int16 field_1A;
	int m_nSeekPosition;
	int m_nFilenameStrLen;
	char* m_szFilenameStr;
	int m_nZipSlotIndex;
	byte m_bExecute;
	byte m_b30[3];
	File* m_pFile;
};

extern FileInternal* g_pFileInternal;

static_assert(sizeof(FileInternal) == 0x34, MESSAGE_WRONG_CLASS_SIZE("FileInternal"));