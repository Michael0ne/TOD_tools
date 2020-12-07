#pragma once

#include "StringsPool.h"

#define FILE_CLASS_SIZE 52
#define FILEWRAPPER_CLASS_SIZE 92
#define FILE_MAX_OPEN_FILES 8
//	NOTE: when writing to file, we're using 128 KiB blocks.
#define FILE_BUFFER_SIZE (1024 * 128)

class FileWrapper
{
	friend class File;
protected:
	int				field_0;
	int				field_4;
	int				field_8;
	int				field_C;
	int				field_10;
	int				field_14;
	HANDLE			m_File;
	String			m_WorkingDir;
	String			m_GameWorkingDir;
	int				m_DesiredAccess_1;
	int				m_FilePosition;
	int				m_CreationDisposition;
	int				m_DesiredAccess_2;
	bool			m_CreateIfNotFound;
	bool			m_ExecuteAttribute;
	bool			m_Read;
	char*			m_Buffer;
	char*			m_BufferBegin;
	char*			m_BufferEnd;

protected:
	FileWrapper(const char* _filename, int _desiredaccess, bool _createifnotfound);	//	@438180
	~FileWrapper();	//	@436F40

	HANDLE			Create();	//	@4378D0
	char			ReadBlock_A();	//	@4370C0
	int				GetPosition();	//	@436DF0
	int				Seek(int _pos);	//	@437560

private:
	void			GetLastErrorMessage(char* _buf);	//	@437820
	void			ReadBlock();	//	@436EA0
	void			_436E30();	//	@436E30
	void			_436E70();	//	@436E70	//	NOTE: this is unused.
	void			SetFileHandle(HANDLE);	//	@436D90
	char			_WriteBufferBlockAndInsertNewLine(char _sym);	//	@437170
	int				WriteBuffer(const char* _buf, int _len);	//	@437430
	char			ReadBlockDecreasePosition();	//	@437720
	void			ReadBlockFromOffset();	//	@436EE0
	int				Read(void* _buf, int _numbytestoread);	//	@437230
	char			WriteFromBufferAndSetToEnd();	//	@437690
};

class File
{
	friend class FileWrapper;
protected:
	String			m_FileName;
	FileWrapper*	m_FileHandle;
	unsigned char	m_FileReadAttribute;
	unsigned char	m_ReadFromZip;
	int				m_SeekPosition;
	int				m_ZipIndex;	//	NOTE: not sure how to name this one and below, data is coming from zip slot.
	int				m_ZipPos;
	int				m_ZipSlot;
	bool			m_ExecuteAttribute;
	FileWrapper*	m_ZipFileHandle;

	void			SetExecuteAttrib(bool);	//	@436E10

public:
	virtual void	scalar_destructor(bool) {};
	virtual bool	WriteBuffers();	//	@417D20
	virtual int		_vsnprintf(File*, const char* _format, ...);	//	@42EFC0
	virtual int		_CheckFormattingSymbol(File*, const char* _format, ...);	//	@417960
	virtual int		WriteFormattedVarlistDataToBuffer(char* _buf, va_list args);	//	@42F040
	virtual int		_CheckFormattingSymbol_A(char* _buf, int* a1);	//	@42F0A0
	virtual char	ReadBlock();	//	@417980
	virtual int		ReadBlockAndGetPosition();	//	@419BD0
	virtual char	_WriteBufferBlockAndInsertNewLine(char _newlinesym);	//	@419BF0	//	NOTE: this is a thunk function.
	virtual int		Read(void* _buffer, int _numbytestoread);	//	@417A30
	virtual int		WriteBuffer(const char* _buf);	//	@419C10	//	NOTE: this is a thunk function.
	virtual int		WriteBufferWithSize(const char* _buf, int _size);	//	@419C00	//	NOTE: this is a thunk function.
	virtual int		Seek(int _pos);	//	@417B90
	virtual char	_WriteBufferAndSetToStart();	//	@417BB0
	virtual char	WriteFromBuffer();	//	@417BD0
	virtual int		GetPosition();	//	@417BF0
	virtual char	ReadBlockDecreasePosition();	//	@417C50
	virtual char	ReadIfNotEOF();	//	@417850
	virtual const char* GetFileName();	//	@419C40

	File(const char* _filename, int _desiredaccess, bool _createifnotfound);	//	@418E30
	~File();	//	@417E20

	bool			IsFileOpen();	//	@417CF0
	char			ReadString(void* outStr);	//	@4180C0

	static unsigned int FilesOpen;	//	@A35DD8
	static HANDLE	FilesSemaphoreArray[FILE_MAX_OPEN_FILES];	//	@A35D98
	static File*	FilesArray[FILE_MAX_OPEN_FILES];	//	@A35DB8
};

static_assert(sizeof(File) == FILE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(File));
static_assert(sizeof(FileWrapper) == FILEWRAPPER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(FileWrapper));