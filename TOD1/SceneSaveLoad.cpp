#include "SceneSaveLoad.h"
#include "TransactionBuffer.h"
#include "StreamedSoundBuffers.h"
#include "SavePoint.h"
#include "LogDump.h"
#include "ScriptDatabase.h"
#include "Scene.h"

SceneSaveLoad* g_SceneSaveLoad = nullptr;
z_stream SceneSaveLoad::BufferStream;
char SceneSaveLoad::CompressedRewindData[2048];

SceneSaveLoad::SceneSaveLoad()
{
    MESSAGE_CLASS_CREATED(SceneSaveLoad);

    m_SavedPlayMode = Scene::PlayMode::MODE_INGAME;
    m_SavePoint = nullptr;
    m_RewindDataBuffer = nullptr;
    m_RewindDataBufferSize = NULL;
    m_SaveInfo_1.m_TransactionBuffer = new TransactionBuffer(0x96000);
}

#pragma message(TODO_IMPLEMENTATION)
void SceneSaveLoad::_874940()
{
    // NOTE: possible name is 'CurrentUndo'.
}

void SceneSaveLoad::ResetSavedPlayMode()
{
    m_SavedPlayMode = Scene::PlayMode::MODE_INGAME;
}

#pragma message(TODO_IMPLEMENTATION)
bool SceneSaveLoad::LoadSaveSummaryToBuffer(SavePoint* savepoint, SaveInfo* saveinfo)
{
    unsigned int saveVersion = NULL;

    if (savepoint->Read(&saveVersion, sizeof(saveVersion)) != sizeof(saveVersion))
        return false;

    if (saveVersion != SAVEPOINT_FILE_VERSION)
    {
        LogDump::LogA("Save point version mismatch (current is %d, file is %d)\n", SAVEPOINT_FILE_VERSION, saveVersion);
        return false;
    }

    unsigned int propertyChecksum = NULL;

    if (savepoint->Read(&propertyChecksum, sizeof(propertyChecksum)) != sizeof(propertyChecksum))
        return false;

    unsigned int propertyChecksumDefined = GetGlobalPropertyListChecksum();

    if (propertyChecksum != propertyChecksumDefined)
    {
        LogDump::LogA("Property checksum mismatch (current is %d, saved is %d)\n", propertyChecksumDefined, propertyChecksum);
        return false;
    }

    unsigned int saveEngineVersion = NULL;

    if (savepoint->Read(&saveEngineVersion, sizeof(saveEngineVersion)) != sizeof(saveEngineVersion))
        return false;

    if (saveEngineVersion != Utils::GetEngineVersionBuild())
    {
        LogDump::LogA("Savepoint created with a different engine (Current build version = %d, saved = %d)\n", Utils::GetEngineVersionBuild(), saveEngineVersion);
        return false;
    }

    unsigned int saveHeaderChecksum = NULL;

    if (savepoint->Read(&saveHeaderChecksum, sizeof(saveHeaderChecksum)) != sizeof(saveHeaderChecksum))
        return false;

    unsigned int saveHeaderEndPos = NULL;

    if (savepoint->Read(&saveHeaderEndPos, sizeof(saveHeaderEndPos)) != sizeof(saveHeaderEndPos))
        return false;

    const unsigned int saveCurrPos = savepoint->GetPosition();
    if (saveHeaderChecksum != savepoint->CalculateChecksum(savepoint, saveHeaderEndPos))
    {
        LogDump::LogA("Header checksum failed\n");
        return false;
    }

    savepoint->RewindFileToBeginning();
    savepoint->Seek(saveCurrPos);

    unsigned int saveBuffersCount = NULL;

    if (savepoint->Read(&saveBuffersCount, sizeof(saveBuffersCount)) != sizeof(saveBuffersCount))
        return false;

    if (saveBuffersCount != SAVEPOINT_FILE_BUFFERS)
    {
        LogDump::LogA("Save point number of buffers mismatch\n");
        return false;
    }

    if (savepoint->Read(&saveinfo->field_1C, sizeof(saveinfo->field_1C)) != sizeof(saveinfo->field_1C))
        return false;

    // TODO: complete this!
    return false;
}

bool SceneSaveLoad::LoadSaveSummary(class SavePoint* savepoint, const class Node* summarynode)
{
    if (!savepoint)
        return false;

    m_SaveInfo_1.m_MemorySummaryNode = (Node*)summarynode;
    return LoadSaveSummaryToBuffer(savepoint, &m_SaveInfo_1);
}

#pragma message(TODO_IMPLEMENTATION)
bool SceneSaveLoad::LoadSavePointData(class SavePoint* sp, EntityType* ent, class Node* readFinishedCb)
{
    return false;
}

#pragma message(TODO_IMPLEMENTATION)
bool SceneSaveLoad::ResetGame(Node** loadedBlocksArray)
{
    return false;
}

int* SceneSaveLoad::_873BA0(const unsigned int nodeid)
{
    if (!Scene::SceneInstance)
        return nullptr;

    if (Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_INGAME || Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_PAUSED)
    {
        switch (m_SavedPlayMode)
        {
        case 2:
            return ((nodeid >> 20) & 7) == 1 ? field_0 : nullptr;
        case 3:
            return (int*)&m_RewindDataBuffer[m_RewindDataBufferSize];
        case 4:
            return (int*)&m_SaveInfo_1.m_TransactionBuffer->m_Buffer[m_SaveInfo_1.m_TransactionBuffer->m_Size + m_TransactionBufferSize];
        }
    }

    return nullptr;
}

void SceneSaveLoad::_873C00(const unsigned int, const int* a2)
{
    switch (m_SavedPlayMode)
    {
    case Scene::PlayMode::MODE_REWIND:
    {
        m_RewindDataBufferSize = ((char*)a2 - (char*)m_RewindDataBuffer) >> 2;
        BufferStream.avail_in = m_RewindDataBufferSize * 4;
        BufferStream.next_in = (Bytef*)m_RewindDataBuffer;

        if (m_RewindDataBufferSize * 4 >= 8192)
        {
            m_RewindDataBufferSize = 0;
            while (BufferStream.avail_in)
            {
                BufferStream.avail_out = 2048;
                BufferStream.next_out = (Bytef*)CompressedRewindData;
                deflate(&BufferStream, 0);

                if (m_SavePoint->WriteBufferWithSize(CompressedRewindData, 2048 - BufferStream.avail_out) != 2048 - BufferStream.avail_out)
                    m_RewindDataBufferFull = 0;
            }
        }
        break;
    }
    case Scene::PlayMode::MODE_4:
        m_TransactionBufferSize += (char*)&a2[-m_TransactionBufferSize - m_SaveInfo_1.m_TransactionBuffer->m_Size] - m_SaveInfo_1.m_TransactionBuffer->m_Buffer;
        break;
    default:
        field_0 = (int*)a2;
        break;
    }
}

void SceneSaveLoad::Adapt()
{
    for (unsigned int i = 1; i < 6; ++i)
    {
        Folder_* fldr = Scene::SceneInstance->GetLoadedBlockByIndex(i - 1);
        if (m_SaveInfo.m_LoadedFolders[i] != fldr)
        {
            m_SaveInfo.m_LoadedFolders[i] = fldr;
            LogDump::LogA("Adapt: CurrentUndo loaded blocks %d = %s\n", i, fldr && fldr->m_Name ? fldr->m_Name : "<null>");
        }
    }
}

bool SceneSaveLoad::WriteDummySavePointData(class SavePoint* savepoint, unsigned int pad)
{
    if (savepoint->m_Status != STATUS_CLOSED)
    {
        if (savepoint->m_SaveFile)
        {
            --SavePoint::OpenFilesCount;
            delete savepoint->m_SaveFile;
        }

        savepoint->m_Status = STATUS_CLOSED;
    }

    if (savepoint->Open(STATUS_2))
    {
        savepoint->IsFileOpen(); // NOTE: why?
        unsigned int filePos = savepoint->GetPosition();
        LogDump::LogA("pad: %x, %x\n", filePos, pad);

        if (filePos <= pad)
        {
            char dummybuffer[9216] = {};
            if (pad - filePos >= sizeof(dummybuffer))
                for (unsigned int i = pad - filePos; i >= sizeof(dummybuffer); i--, filePos += sizeof(dummybuffer))
                    savepoint->WriteBufferWithSize(dummybuffer, sizeof(dummybuffer));
            savepoint->WriteBufferWithSize(dummybuffer, pad - filePos);

            return savepoint->CloseFile() != false;
        }
        else
            return true;
    }

    return false;
}

bool SceneSaveLoad::WriteSavePointFileData(class SavePoint* savepoint, SaveInfo* savedata)
{
    const unsigned int savefileversion = 9;
    const unsigned int propertychecksum = GetGlobalPropertyListChecksum();
    const unsigned int enginebuild = Utils::GetEngineVersionBuild();
    const unsigned int dummynull = NULL;
    const unsigned int folderstosave = 6;

    if (savepoint->WriteBufferWithSize((const char*)&savefileversion, sizeof(savefileversion) != sizeof(savefileversion)))
        return false;

    if (savepoint->WriteBufferWithSize((const char*)&propertychecksum, sizeof(propertychecksum)) != sizeof(propertychecksum))
        return false;

    if (savepoint->WriteBufferWithSize((const char*)&enginebuild, sizeof(enginebuild)) != sizeof(enginebuild))
        return false;

    const int checksumfilepos = savepoint->GetPosition();

    if (savepoint->WriteBufferWithSize((const char*)&dummynull, sizeof(dummynull)) != sizeof(dummynull))
        return false;

    if (savepoint->WriteBufferWithSize((const char*)&dummynull, sizeof(dummynull)) != sizeof(dummynull))
        return false;

    SaveFileHelper savefilehelper;

    if (savefilehelper.WriteBufferWithSize((const char*)&folderstosave, sizeof(folderstosave)) != sizeof(folderstosave))
        return false;

    if (savefilehelper.WriteBufferWithSize((const char*)&savedata->field_1C, sizeof(savedata->field_1C)) != sizeof(savedata->field_1C))
        return false;

    if (savedata->m_MemorySummaryNode)
    {
        const int filepos = savefilehelper.GetPosition();
        if (savefilehelper.WriteBufferWithSize((const char*)&dummynull, sizeof(dummynull)) == sizeof(dummynull))
        {
            //  TODO: not sure if this is correct way.
            const unsigned int propertiessize = savedata->m_MemorySummaryNode->SaveScriptData((SavePoint*)&savefilehelper);
            if (propertiessize >= 0)
            {
                savefilehelper._WriteBufferAndSetToStart();
                savefilehelper.Seek(filepos);

                if (savefilehelper.WriteBufferWithSize((const char*)&propertiessize, sizeof(propertiessize)) == sizeof(propertiessize))
                {
                    savefilehelper.WriteFromBuffer();

                    for (unsigned int i = 0; i < 6; ++i)
                    {
                        const int folderid = savedata->m_LoadedFolders[i] ? savedata->m_LoadedFolders[i]->m_Id.Id : -1;
                        if (savefilehelper.WriteBufferWithSize((const char*)&folderid, sizeof(folderid)) != sizeof(folderid))
                            break;
                    }

                    if (savefilehelper.WriteBufferWithSize((const char*)&savedata->m_SavedGameTimeMs, sizeof(savedata->m_SavedGameTimeMs)) &&
                        savefilehelper.WriteBufferWithSize((const char*)&savedata->m_SavedFrameNumber, sizeof(savedata->m_SavedFrameNumber)))
                    {
                        int crc = NULL;
                        char buf[256] = {};
                        unsigned int filesize = NULL;
                        Utils::crc32_init_default(&crc);
                        savefilehelper._WriteBufferAndSetToStart();

                        for (unsigned int len = savefilehelper.Read(buf, sizeof(buf)); len > 0; len = savefilehelper.Read(buf, sizeof(buf)))
                        {
                            Utils::generic_crc32(&crc, buf, len);
                            savepoint->WriteBufferWithSize(buf, len);

                            filesize += len;
                        }

                        Utils::crc32_gen(&crc);

                        const int endfilepos = savepoint->GetPosition();
                        savepoint->RewindFileToBeginning();
                        savepoint->Seek(checksumfilepos);

                        if (savepoint->WriteBufferWithSize((const char*)&crc, sizeof(crc)) == sizeof(crc) &&
                            savepoint->WriteBufferWithSize((const char*)&filesize, sizeof(filesize)) == sizeof(filesize))
                        {
                            savepoint->Seek(endfilepos);
                            return true;
                        }
                    }
                }
            }
        }
    }
    else
    {
        const int dummynegone = -1;

        if (savefilehelper.WriteBufferWithSize((const char*)&dummynegone, sizeof(dummynegone)) == sizeof(dummynegone))
        {
            for (unsigned int i = 0; i < 6; ++i)
            {
                const int folderid = savedata->m_LoadedFolders[i] ? savedata->m_LoadedFolders[i]->m_Id.Id : -1;
                if (savefilehelper.WriteBufferWithSize((const char*)&folderid, sizeof(folderid)) != sizeof(folderid))
                    break;
            }

            if (savefilehelper.WriteBufferWithSize((const char*)&savedata->m_SavedGameTimeMs, sizeof(savedata->m_SavedGameTimeMs)) &&
                savefilehelper.WriteBufferWithSize((const char*)&savedata->m_SavedFrameNumber, sizeof(savedata->m_SavedFrameNumber)))
            {
                int crc = NULL;
                char buf[256] = {};
                unsigned int filesize = NULL;
                Utils::crc32_init_default(&crc);
                savefilehelper._WriteBufferAndSetToStart();

                for (unsigned int len = savefilehelper.Read(buf, sizeof(buf)); len > 0; len = savefilehelper.Read(buf, sizeof(buf)))
                {
                    Utils::generic_crc32(&crc, buf, len);
                    savepoint->WriteBufferWithSize(buf, len);

                    filesize += len;
                }

                Utils::crc32_gen(&crc);

                const int endfilepos = savepoint->GetPosition();
                savepoint->RewindFileToBeginning();
                savepoint->Seek(checksumfilepos);

                if (savepoint->WriteBufferWithSize((const char*)&crc, sizeof(crc)) == sizeof(crc) &&
                    savepoint->WriteBufferWithSize((const char*)&filesize, sizeof(filesize)) == sizeof(filesize))
                {
                    savepoint->Seek(endfilepos);
                    return true;
                }
            }
        }
    }

    return false;
}