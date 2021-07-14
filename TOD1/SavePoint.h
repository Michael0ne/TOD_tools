#pragma once

#include "File.h"
#include "MemoryCard.h"

#define SAVEPOINT_CLASS_SIZE 64
#define SAVEPOINT_SAVE_SIZE 8092
#define SAVEPOINT_MAX_OPEN_FILES 3

enum SavePointStatus {
	STATUS_SUCCESS = 0,
	STATUS_1 = 1,
	STATUS_2 = 2,
	STATUS_CLOSED = 3,
	STATUS_FAILED = 4
};

//	NOTE: this is actually derived from FileInternal class, so methods are same.
class SavePoint
{
	friend class SceneSaveLoad;
protected:
	MemoryCard*		m_SaveMemoryCard;
	String			m_SaveDir;
	String			m_SlotIdStr;
	File*			m_SaveFile;
	SavePointStatus	m_Status;
	String			m_SlotDir;

public:
	virtual ~SavePoint();	//	@86BFE0, @86BF20
	virtual bool SaveBuffersDataToFile() { return false; };	//	@86C3D0
	virtual int vsnprintf(const char*, ...) { return NULL; };	//	@42EFC0
	virtual int _42F020(const char*, ...) { return NULL; };	//	@42F020
	virtual int WriteFormattedVarlistDataToBuffer(const char*, va_list) { return NULL; };	//	@42F040
	virtual void CheckFormattingSymbol(const char*, int*) {};	//	@42F0A0
	virtual int ReadBlock() { return NULL; };	//	@86C480
	virtual int ReadBlockAndGetPosition() { return NULL; };	//	@86C4E0
	virtual bool WriteBufferBlockAndInsertNewLine(int unk) { return false; };	//	@86C3A0
	virtual int Read(void* buffer, int size) { return NULL; };	//	@86C420
	virtual int WriteBuffer(int unk) { return NULL; };	//	@86C540
	virtual int WriteBufferWithSize(const char* buffer, int size) { return NULL; };	//	@86C340 NOTE: returns number of bytes written.
	virtual int Seek(int position) { return NULL; };	//	@86C570
	virtual bool RewindFileToBeginning() { return false; };	//	@86BFC0
	virtual bool IsFileOpen() { return false; };	//	@86C700
	virtual int GetPosition() { return NULL; };	//	@86C5D0
	virtual bool ReadBlockAndDecreasePosition() { return false; };	//	@86C630
	virtual bool ReadBlockIfFailed() { return false; };	//	@86C680
	virtual const char* GetSaveSlotDir() { return nullptr; };	//	@86C250

	SavePoint(MemoryCard* dirInfo, const char* saveDir, const char* saveSlotId, unsigned int bufferSize);	//	@86C160

	void* operator new(size_t size)
	{
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::ReleaseMemory(ptr, false);
	}

	time_t			GetTime() const;	//	@928920
	bool			Open(SavePointStatus mode);	//	@43B240
	int				CalculateChecksum(SavePoint* savepoint, int pos);	//	@874180
	bool			CloseFile();	//	@869D90

	static bool		VerifyFileChecksum(SavePoint*);	//	@874230

	static int		OpenFilesCount;	//	@A35EA8
};

static_assert(sizeof(SavePoint) == SAVEPOINT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SavePoint));