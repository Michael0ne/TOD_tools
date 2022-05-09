#pragma once
#include "MemoryManager.h"
#include "EntityType.h"
#include "TransactionBuffer.h"
#include "Scene.h"

#include "zlib121/zlib.h"

// NOTE: saveslot data is compressed using deflate (version 1.2.1, stream size = 56).
struct SaveInfo
{
    TransactionBuffer* m_TransactionBuffer;
    class Folder_*  m_LoadedFolders[6];
    int     field_1C; // NOTE: when reading a save file, 'BAADF00D' written here.
    class Node*   m_MemorySummaryNode;
    int     m_SavedGameTimeMs;
    int     m_SavedFrameNumber;
};

#define SAVEPOINT_FILE_VERSION 9
#define SAVEPOINT_FILE_BUFFERS 6

struct SaveFile
{
    unsigned int m_Version; // NOTE: default is 9.
    unsigned int m_PropertyListCRC;
    unsigned int m_EngineBuildVersion; // NOTE: default is 1925.
    unsigned int m_HeaderCRC;
    unsigned int field_10;
    unsigned int m_BuffersCount; // TODO: or maybe 'BlocksCount' - how many 'blocks' are there. Default is 6, original code only reads 6 of these.
    unsigned int field_18;
    unsigned int m_SummaryDataSize;
    char*   m_Summary; // NOTE: size is 'SummaryDataSize'.
    unsigned int m_BuffersOffsets[7]; // TODO: blocks id's or smth, dunno.
    unsigned int m_SavedGameTimeMs;
    unsigned int m_SavedFrameNumber;
    unsigned int field_44;
    unsigned int m_DeflateBufferSize;
    char*   m_DeflateBuffer; // NOTE: size is 'DeflateBufferSize' * 4.
};

class SceneSaveLoad
{
private:
    int                *field_0;    //  NOTE: pointer to the end of the SaveInfo transaction buffer.
    SaveInfo            m_SaveInfo; // NOTE: this is used when WRITING savepoint data.
    Scene::PlayMode     m_SavedPlayMode;    //  NOTE: 1 - when saving/loading, 2 - anything else
    Scene::PlayMode     m_CurrentPlayMode;
    char               *m_RewindDataBuffer;
    int                 m_RewindDataBufferSize;
    SavePoint          *m_SavePoint;
    bool                m_RewindDataBufferFull;
    SaveInfo            m_SaveInfo_1; // NOTE: this is used when READING savepoint data.
    int                 m_TransactionBufferSize;

public:
    SceneSaveLoad(); // @874510
    ~SceneSaveLoad();

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    void                _874940(); // @874940
    void                ResetSavedPlayMode(); // @873B90
    bool                LoadSaveSummaryToBuffer(class SavePoint* savepoint, SaveInfo* saveinfo); // @874570
    bool                LoadSaveSummary(class SavePoint* savepoint, const class Node* summarynode); // @874910
    bool                LoadSavePointData(class SavePoint*, EntityType*, class Node* readFinishedCb); // @874F40
    bool                CompressAndWriteSaveData(class SavePoint*, EntityType*); // @874A00
    bool                ResetGame(class Node** loadedBlocksArray); // @874D00
    int*                GetEntityDataBuffer(const unsigned int nodeid); // @873BA0
    void                SaveEntityToDataBuffer(const unsigned int, const int* a2); // @873C00
    void                Adapt(); // @873D30
    bool                LoadSaveInformation(SavePoint* sp, SaveInfo* outSaveInfo, char** outDeflatedBufferPtr, int* outDeflatedBufferSize); //  @8747F0
    
    static bool         WriteDummySavePointData(class SavePoint* savepoint, unsigned int); // @8743F0
    static bool         WriteSavePointFileData(class SavePoint* savepoint, SaveInfo* savedata); // @873DA0
    static int          UncompressSaveDataBuffer(char* outBuffer, const int size);  //  @873B40

    static z_stream     BufferStream; // @A3D748
    static char         CompressedRewindData[2048]; // @A3CF48
};

extern SceneSaveLoad* g_SceneSaveLoad;

ASSERT_CLASS_SIZE(SceneSaveLoad, 120);