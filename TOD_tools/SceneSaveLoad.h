#pragma once

#include "MemoryAllocators.h"
#include "ScriptTypes.h"
#include "RewindBuffer.h"

#define SCENESAVELOAD_CLASS_SIZE 120

enum ScenePlayMode
{
	MODE_NONE = 0,
	MODE_1 = 1,
	MODE_2 = 2,
	MODE_3 = 3
};

//	NOTE: saveslot data is compressed using inflate (version 1.2.1, stream size = 56).
struct SaveInfo
{
	RewindBuffer* field_0;
	int* field_4[6];	//	NOTE: when reading a save file, this seems like it holds pointers to 'Blocks'. (?)
	int field_1C;	//	NOTE: when reading a save file, 'BAADF00D' written here.
	ScriptType_Entity* field_20;
	int m_SavedGameTimeMs;
	int m_SavedFrameNumber;
};

#define SAVEPOINT_FILE_VERSION 9
#define ENGINE_VERSION 1925
#define SAVEPOINT_FILE_BUFFERS 6

struct SaveFile
{
	unsigned int	m_Version;	//	NOTE: default is 9.
	unsigned int	m_PropertyListCRC;
	unsigned int	m_EngineBuildVersion;	//	NOTE: default is 1925.
	unsigned int	m_HeaderCRC;
	unsigned int	field_10;
	unsigned int	m_BuffersCount;	//	TODO: or maybe 'BlocksCount' - how many 'blocks' are there. Default is 6, original code only reads 6 of these.
	unsigned int	field_18;
	unsigned int	m_SummaryDataSize;
	char*			m_Summary;	//	NOTE: size is 'SummaryDataSize'.
	unsigned int	m_BuffersOffsets[7];	//	TODO: blocks id's or smth, dunno.
	unsigned int	m_SavedGameTimeMs;
	unsigned int	m_SavedFrameNumber;
	unsigned int	field_44;
	unsigned int	m_DeflateBufferSize;
	char*			m_DeflateBuffer;	//	NOTE: size is 'DeflateBufferSize' * 4.
};

class SceneSaveLoad
{
private:
	int* field_0;
	SaveInfo	m_SaveInfo;	//	NOTE: this is used when WRITING savepoint data.
	ScenePlayMode m_SavedPlayMode;
	int field_34;
	int** field_38;
	int field_3C;
	int field_40;
	char field_44;
	SaveInfo m_SaveInfo_1;	//	NOTE: this is used when READING savepoint data.
	int field_74;

public:
	SceneSaveLoad();	//	@874510
	~SceneSaveLoad();

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void		_874940();	//	@874940
	void		ResetSavedPlayMode();	//	@873B90
	bool		LoadSavePointData(class SavePoint*, ScriptType_Entity*, class Node* readFinishedCb);	//	@874F40
	bool		CompressAndWriteSaveData(class SavePoint*, ScriptType_Entity*);	//	@874A00
};

extern SceneSaveLoad* g_SceneSaveLoad;

static_assert(sizeof(SceneSaveLoad) == SCENESAVELOAD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SceneSaveLoad));