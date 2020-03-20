#pragma once

#include "stdafx.h"
#include "StringsPool.h"

class FileInternal;

struct FileInternal__vtable
{
	void(__thiscall* Release)(FileInternal*, bool releasememory);
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
	FileInternal__vtable*	lpVtbl;
	String					m_sFileName;
	File*					m_pFileHandle;
	char					field_18;
	char					m_b19;				//	Unknown flag. If set - m_pFileHandle is ignored.
	short					field_1A;
	int						m_nSeekPosition;
	int						field_20;
	int						field_24;
	int						m_nZipSlotIndex;	//	When file is open using this class's method, it is placed in memory buffer to be referenced by this index.
	char					m_bExecute;
	char					field_30[3];
	File*					m_pFile;			//	I dunno what's difference between m_pFileHandle and this.
};

extern FileInternal* g_pFileInternal;

static_assert(sizeof(FileInternal) == 0x34, MESSAGE_WRONG_CLASS_SIZE("FileInternal"));