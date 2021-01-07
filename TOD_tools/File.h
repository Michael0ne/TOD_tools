#pragma once

#include "List.h"

#define FILE_CLASS_SIZE 52
#define FILEWRAPPER_CLASS_SIZE 92
#define FILE_MAX_OPEN_FILES 8
#define FILE_BUFFER_SIZE (1024 * 128)
#define FILE_ZIP_MAGIC_HEADER 0x16ED5B50

class FileWrapper
{
	friend class File;
protected:
	CRITICAL_SECTION m_CriticalSection;
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

public:
	void			SetFileHandle(HANDLE _hnd);	//	@436D90
	void			ReleaseFileHandle();	//	@436DC0
	void			_436FF0(HANDLE);	//	@436FF0
	HANDLE			CreateFile_();	//	@4378D0

protected:
	int				GetPosition();	//	@436DF0
	void			SetExecuteAttr(unsigned char);	//	@436E10
	void			_436E30();	//	@436E30
	void			_436E70();	//	@436E70
	void			ReadBlock_Internal();	//	@436EA0
	void			ReadBlockFromOffset();	//	@436EE0
	char			ReadBlock();	//	@4370C0
	char			_WriteBufferBlockAndInsertNewLine(char);	//	@437170
	int				Read(LPVOID, int);	//	@437230
	int				WriteBuffer(const char*, int);	//	@437430
	int				Seek(int);	//	@437560
	char			WriteFromBufferAndSetToEnd();	//	@437690
	char			ReadBlockDecreasePosition();	//	@437720
	char			_437790();	//	@437790
	void			GetLastErrorMessage(char*);	//	@437820

	static FileWrapper*	FilesArray[FILE_MAX_OPEN_FILES];	//	@A35DB8
};

class File
{
	friend class FileWrapper;
protected:
	String			m_FileName;
public:
	FileWrapper*	m_FileHandle;
protected:
	unsigned char	m_FileReadAttribute;
	unsigned char	m_ReadFromZip;
	int				m_SeekPosition;
	int				m_ZipIndex;	//	NOTE: not sure how to name this one and below, data is coming from zip slot.
	int				m_ZipPos;
	int				m_ZipSlot;
	bool			m_ExecuteAttribute;
	FileWrapper*	m_ZipFileHandle;

public:
	virtual File*	scalar_destructor(bool) { return nullptr; };
	virtual bool	WriteBuffers();	//	@417D20
	virtual int		_vsnprintf(File*, const char* _format, ...);	//	@42EFC0
	virtual int		_scanf(File*, const char* _format, ...);	//	@417960
	virtual int		WriteFormattedVarlistDataToBuffer(char* _buf, va_list args);	//	@42F040
	virtual int		_scanf_impl(char* format, int* outArgs);	//	@42F0A0
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
	virtual const char* GetFileName() const;	//	@419C40

	File(const char* _filename, int _desiredaccess, bool _createifnotfound);	//	@418E30
	~File();	//	@417E20

	bool			IsFileOpen() const;	//	@417CF0
	char			ReadString(void* outStr);	//	@4180C0

	static void		AddDirectoryMappingsListEntry(const char* str1, const char* str2);	//	@418F90
	static String*	GetPathFromDirectoryMappings(String* outStr, const char* path);	//	@41A360
	static bool		IsFileExists(const char* file);	//	@437E90
	static void		ExtractFilePath(const char* inFilePath, char* outDirectory, char* outFileName, char* outFileExtension);	//	@4088E0
	static String*	ExtractFileDir(String& outStr, const char* path);	//	@409360
	static String*	ExtractFileName(String& outStr, const char* path);	//	@4093B0
	static bool		FindFileEverywhere(const char* path);	//	@4182A0
	static void		OpenZip(const char* szZipPath);	//	@419100
	static void		ReadZipDirectories(const char* szFileSystem);	//	@419550

	static unsigned int FilesOpen;	//	@A35DD8
	static HANDLE	FilesSemaphoreArray[FILE_MAX_OPEN_FILES];	//	@A35D98
	static List<StringTuple> DirectoryMappingsList;	//	@A35DE4
};

static_assert(sizeof(File) == FILE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(File));
static_assert(sizeof(FileWrapper) == FILEWRAPPER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(FileWrapper));