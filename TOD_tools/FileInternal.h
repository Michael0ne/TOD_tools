#pragma once

#include "stdafx.h"
#include "File.h"
#include "StringsPool.h"

#define FILEINTERNAL_CLASS_SIZE 52

class FileInternal
{
	friend class SavePoint;
protected:
	String			m_sFileName;
	File*			m_pFileHandle;
	char			m_FileReadAttribute;
	char			m_bReadFromZip;
	short			field_1A;
	int				m_nSeekPosition;
	int				m_nFilenameStrLen;
	char*			m_szFilenameStr;
	int				m_nZipSlotIndex;
	char			m_ExecuteAttribute;
	File*			m_pFile;

	virtual void	scalar_destructor(bool);	//	@41A030
	virtual bool	WriteBuffers();	//	@417D20
	virtual int		_vsnprintf(const char*, ...);	//	@42EFC0
	virtual int		_CheckFormattingSymbolsList(const char*, ...);	//	@417960
	virtual int		WriteFormattedVarlistDataToBuffer(char* str, va_list arguments);	//	@42F040
	virtual signed int _CheckFormattingSymbol(char* str, int* unk);	//	@42F0A0
	virtual bool	ReadBlock();	//	@417980
	virtual void	ReadBlockAndGetPosition(int);	//	@419BD0
	virtual bool	WriteBufferBlockAndInsertNewLine(char newlinesym);	//	@419BF0
	virtual int		Read(void* buffer, int bytestoread);	//	@417A30
	virtual signed int WriteBuffer(const char* buffer);	//	@419C10
	virtual signed int WriteBufferWithSize(const char* buffer, signed int size);	//	@419C00
	virtual void	Seek(int position);	//	@417B90
	virtual char	WriteBufferAndSetToStart();	//	@417BB0
	virtual char	WriteFromBuffer();	//	@417BD0
	virtual int		GetPosition();	//	@417BF0
	virtual void	ReadBlockDecreasePosition();	//	@417C50
	virtual char	ReadBlockIfFailed();	//	@417850
	virtual const char* GetFilename();	//	@419C40

public:
	FileInternal(const char* fileName, unsigned int dwDesiredAccess, bool createIfNotFound);	//	@418E30
	~FileInternal();	//	@417E20

	bool	IsFileOpen();	//	@417CF0
	char	ReadString(String* outStr);	//	@4180C0

	static int& FilesOpen;	//	@A35DD8
};

static_assert(sizeof(FileInternal) == FILEINTERNAL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(FileInternal));