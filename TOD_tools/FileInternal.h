#pragma once

#include "stdafx.h"

#include "File.h"

#include "StringsPool.h"

#define FILEINTERNAL_CLASS_SIZE 52

class FileInternal;

struct FileInternal__vtable
{
	void(__thiscall* Release)(FileInternal* _this, bool releasememory);	//	@41A030
	bool(__thiscall* WriteBuffers)(FileInternal* _this);	//	@417D20
	int (*vsnprintf)(FileInternal*, const char* str, ...);	//	@42EFC0
	int (* _CheckFormattingSymbolsList)(FileInternal* _this, const char* str, ...);	//	@417960
	int(__thiscall* WriteFormattedVarlistDataToBuffer)(FileInternal*, char* str, va_list arguments);	//	@42F040
	signed int (__thiscall* _CheckFormattingSymbol)(FileInternal* _this, char* str, int* unk);	//	@42F0A0
	bool(__thiscall* ReadBlock)(FileInternal* _this);	//	@417980
	void(__thiscall* ReadBlockAndGetPosition)(FileInternal* _this, int);	//	@419BD0
	bool(__thiscall* WriteBufferblockAndInsertNewLine)(FileInternal* _this, char newlinesym);	//	@419BF0
	int(__thiscall* Read)(FileInternal* _this, void* buffer, int bytestoread);	//	@417A30
	signed int (__thiscall* WriteBuffer)(FileInternal* _this, const char* buffer);	//	@419C10
	signed int(__thiscall* WriteBufferWithSize)(FileInternal* _this, const char* buffer, signed int size);	//	@419C00
	void(__thiscall* Seek)(FileInternal* _this, int position);	//	@417B90
	char(__thiscall* WriteBufferAndSetToStart)(FileInternal* _this);	//	@417BB0
	char(__thiscall* WriteFromBuffer)(FileInternal* _this);	//	@417BD0
	int(__thiscall* GetPosition)(FileInternal* _this);	//	@417BF0
	void(__thiscall* ReadBlockDecreasePosition)(FileInternal* _this);	//	@417C50
	char(__thiscall* ReadBlockIfFailed)(FileInternal* _this);	//	@417850
	const char* (__thiscall* GetFileName)(FileInternal* _this);	//	@419C40
};


class FileInternal
{
public:
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

public:
	void	Open(const char* szFileName, unsigned int dwDesiredAccess, bool bCreateIfNotFound);	//	@418E30
	bool	IsFileOpen();	//	@417CF0
	void	Close();	//	@417E20
	char	ReadString(String* outStr);	//	@4180C0

	static int& FilesOpen;	//	@A35DD8
};

extern FileInternal* g_FileInternal;

static_assert(sizeof(FileInternal) == FILEINTERNAL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(FileInternal));