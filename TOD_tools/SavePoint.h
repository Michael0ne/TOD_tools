#pragma once

#include "StringsPool.h"
#include "SavesDirectoriesInformation.h"
#include "MemoryAllocators.h"
#include "FileInternal.h"

class SavePoint;

#define SAVEPOINT_CLASS_SIZE 64
#define SAVEPOINT_FILE_VERSION 9
#define ENGINE_VERSION 1925
#define SAVEPOINT_FILE_BUFFERS 6

struct SavePoint_File {
	unsigned int	FileVersion;
	unsigned int	GlobalPropertiesListCRC;
	unsigned int	BuildVersion;
	unsigned int	HeaderChecksum;
	unsigned int	HeaderChecksum_1;	//	NOTE: this value is ignored.
	unsigned int	BuffersInFile;	//	NOTE: initialized with 6 and doesn't change.
	unsigned int	RewindBuffer_PtrToStruc;	//	NOTE: initialized with pointer to unknown struct from RewindBuffer.
	//	NOTE: next thing is unknown RewindBuffer Entity's script properties written to a file.
};

struct SavePoint__vtable {
	void(__thiscall* Release)(SavePoint* _this, bool);	//	@86BFE0
	char(__thiscall* SaveBuffersDataToFile)(SavePoint* _this);	//	@86C3D0
	int(__cdecl* vsnprintf)(const char*, ...);	//	@42EFC0
	int(__cdecl* _42F020)(const char*, ...);	//	@42F020
	int(__thiscall* WriteFormattedVarlistDataToBuffer)(SavePoint* _this, const char*, va_list);	//	@42F040
	void(__thiscall* CheckFormattingSymbol)(SavePoint* _this, const char*, int*);	//	@42F0A0
	int(__thiscall* ReadBlock)(SavePoint* _this);	//	@86C480
	int(__thiscall* ReadBlockAndGetPosition)(SavePoint* _this);	//	@86C4E0
	char(__thiscall* WriteBufferBlockAndInsertNewLine)(SavePoint* _this, int unk);	//	@86C3A0
	int(__thiscall* Read)(SavePoint* _this, void* buffer, int size);	//	@86C420
	int(__thiscall* WriteBuffer)(SavePoint* _this, int unk);	//	@86C540
	int(__thiscall* WriteBufferWithSize)(SavePoint* _this, const char* buffer, int size);	//	@86C340 NOTE: returns number of bytes written.
	int(__thiscall* Seek)(SavePoint* _this, int position);	//	@86C570
	bool(__thiscall* RewindFileToBeginning)(SavePoint* _this);	//	@86BFC0
	bool(__thiscall* IsFileOpen)(SavePoint* _this);	//	@86C700
	int(__thiscall* GetPosition)(SavePoint* _this);	//	@86C5D0
	bool(__thiscall* ReadBlockAndDecreasePosition)(SavePoint* _this);	//	@86C630
	bool(__thiscall* ReadBlockIfFailed)(SavePoint* _this);	//	@86C680
	const char* (__thiscall* GetSaveSlotDir)(SavePoint* _this);	//	@86C250
};

enum eSavePointStatus {
	E_SUCCESS = 0,
	E_UNKNOWN_1 = 1,
	E_UNKNOWN_2 = 2,
	E_UNKNOWN_3 = 3
};

//	NOTE: this is actually derived from FileInternal class, so methods are same.
class SavePoint
{
private:
	SavePoint__vtable* lpVtbl;
	SavesDirectoriesInformation* m_pSavesDirInfo;
	String m_sSaveDir;
	String m_sSlotId;
	FileInternal* m_pSaveFileHandle;
	int m_nLastError;
	String m_sSaveSlotDir;

public:
	SavePoint(SavesDirectoriesInformation* dirInfo, const char* saveDir, const char* saveSlotId, unsigned int bufferSize);	//	@86C160
	~SavePoint();	//	@86BF20

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, false);
	}

	static bool		WriteSavePointFileData(const SavePoint& savepoint, const struct RewindBuffer& rewbuff);	//	@873DA0
	static bool		VerifyFileChecksum(SavePoint*);	//	@874230

	static int		ms_FilesOpen;	//	@A35EA8
};

static_assert(sizeof(SavePoint) == SAVEPOINT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SavePoint));