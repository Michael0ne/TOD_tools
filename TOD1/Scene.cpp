#include "Scene.h"
#include "SceneSaveLoad.h"
#include "LogDump.h"
#include "Timer.h"
#include "Progress.h"
#include "ScriptDatabase.h"
#include "SavePoint.h"
#include "FrameBuffer.h"
#include "CollisionProbe.h"
#include "GfxInternal.h"
#include "Camera.h"
#include "FrameBasedSubAllocator.h"
#include "ModelAsset.h"
#include "CollisionList.h"
#include "NumberType.h"
#include "TruthType.h"
#include "IntegerType.h"
#include "SceneSaveLoad.h"
#include "StreamedSoundBuffers.h"
#include "ScriptThread.h"
#include "Control.h"
#include "EditorCamera.h"
#include "LoadScreenInfo.h"
#include "MoviePlayer.h"
#include "DynamicSurroundGeometry.h"
#include "SoundSlot.h"
#include "Model.h"
#include "Character.h"
#include "SoundEmitter.h"

EntityType* tScene = nullptr;
Scene* Scene::SceneInstance = nullptr;
QuadTree* Scene::MainQuadTree;
QuadTree* Scene::AuxQuadTree;
std::vector<Scene::EntityReference>* Scene::DanglingEntityReferences;
std::map<int*, int>* Scene::DanglingEntityReferencesMap;
std::vector<Character*>     Scene::CharactersList;
std::vector<Model*>         Scene::ModelsList;
std::vector<SoundEmitter*>  Scene::SoundEmittersList;

uint32_t Scene::QuadTreeNodesAllocated;
Scene::QuadTreeNode* Scene::MainQuadTreeNodes;
Scene::QuadTreeNode* Scene::AuxQuadTreeNodes;
short Scene::_A120E8 = -1;
int Scene::_A3DD40;

int Scene::RealTimeMs;
int Scene::GameTimeMs;
int Scene::NextUpdateTime;
int Scene::TotalFrames;
int Scene::NewFrameNumber;
bool Scene::IsRewindBufferInUse = true;
bool Scene::LoadingAssetBlock;
float Scene::RealTimePassed;
float Scene::TimePassed;
UINT64 Scene::CreationTime;
int* Scene::_A3CEE4;
int* Scene::_A3CEE8;
int Scene::_A3DA80[100];
int Scene::_A3D8D8[100];
int Scene::_A3DC38[4];
int Scene::UpdateOrBlockingListSize;
bool Scene::_A3D858;
float   Scene::FrameRateHistory[100];
int Scene::FrameRateHistoryIndex;
float   Scene::FrameTimeTotal;
bool Scene::_A3D890;

int Scene::PreBlocksUnloadedCommand;
int Scene::BlocksUnloadedCommand;
int Scene::InvalidatePlaceholderModelCommand = -1;
int Scene::RewindOrRetryFinishedCommand = -1;

void Scene::AllocateMainQuadTreeNodes(const unsigned int num, const AllocatorIndex allocind)
{
    if (MainQuadTreeNodes)
        return;

    QuadTreeNodesAllocated = num;
    MainQuadTreeNodes = new QuadTreeNode[num];

    for (uint32_t i = 0; i < num - 1; ++i)
        MainQuadTreeNodes[i].field_0[0] = i;

    MainQuadTreeNodes[num - 1].field_0[0] = -1;
    _A120E8 = 0;
}

void Scene::AllocateAuxQuadTreeNodes(const uint32_t num, const AllocatorIndex allocIndex)
{
    if (AuxQuadTreeNodes)
        return;

    AuxQuadTreeNodes = new QuadTreeNode[num];
    if (num - 1 > 0)
    {
        for (uint32_t i = 0; i < num - 1; ++i)
            AuxQuadTreeNodes[i].field_8[2] = i;
    }

    AuxQuadTreeNodes[num - 1].field_8[2] = -1;
}

Scene::Scene() : Folder_()
{
    MESSAGE_CLASS_CREATED(Scene);

    m_PlayMode = MODE_STOP;
    m_NodesWithUpdateOrBlockingScripts = NULL;
    m_FrameBuffer_1 = new FrameBuffer(0, 36, 2);
    m_FrameBuffer_2 = new FrameBuffer(0, 36, 2);
    m_SharedProbe = (CollisionProbe*)tCollisionProbe->CreateNode();
    SceneInstance = this;
    m_GameCamera = nullptr;
    m_EditorCamera = nullptr;
    m_QuadTreesAllocated = false;
    StoreGameCamera();
    m_CameraPosition = { 10000.f, -10000.f, 50000.f, 0.f };
    m_SaveGameSize = NULL;
    NextUpdateTime = NULL;
    RealTimeMs = NULL;
    GameTimeMs = NULL;
    m_RewindResumeTimeMs = NULL;
    m_InitMode = NULL;
    CreationTime = Timer::GetMilliseconds();
    m_TimeMultiplier = 1.f;
    field_118 = 1.f;
    m_RewindTimeMultiplier = 1.f;
    m_FixedFramerate = false;
    m_FixedFramerateVal = 0.039999999f;

    for (unsigned int i = 0; i < 31; i++)
    {
        m_FrameBuffers[i] = new FrameBuffer(100, 20, 2);
        g_GfxInternal->SetBufferRenderBufferPointerByIndex(i, m_FrameBuffers[i]);
    }

    m_List_5.resize(1024);
    m_RewindBuffer1 = nullptr;
    m_RewindBuffer2 = nullptr;
    _A3CEE4 = NULL;
    _A3CEE8 = NULL;
    field_C8 = NULL;
    m_SaveLoadState = STATE_DONE;
    m_SaveData = nullptr;
    m_MemoryCards = nullptr;
    m_WindMode = NULL;
    field_1A8 = NULL;
    m_WindPause = false;
    field_1AA = NULL;
    m_FlushRewindRequested = false;
    m_SharedProbe->SetFlags(0x20);

    for (unsigned int i = 0; i < 8; i++)
    {
        m_LoadedBlocks[i] = nullptr;
        field_1D0[i] = NULL;
    }

    memset(_A3DA80, NULL, sizeof(_A3DA80));
    memset(_A3D8D8, NULL, sizeof(_A3D8D8));
    memset(_A3DC38, NULL, sizeof(_A3DC38));

    field_268 = NULL;
    m_ActiveCamera = nullptr;

    RewindOrRetryFinishedCommand = GetCommandByName("rewind_or_retry_finished(integer)");
    BuildSceneTree();
    AllocateRewindBuffer();

    m_AssetBlockInfo = new AssetInfo();
}

Scene::~Scene()
{
    MESSAGE_CLASS_DESTROYED(Scene);

    FreeRewindBuffer();
    ReleaseQuadTreeAndRenderlist();
    m_SharedProbe->Destroy();

    for (unsigned int i = 0; i < 31; ++i)
    {
        if (m_FrameBuffers[i])
            delete m_FrameBuffers[i];
        g_GfxInternal->SetBufferRenderBufferPointerByIndex(i, nullptr);
    }

    delete m_FrameBuffer_1;
    delete m_FrameBuffer_2;

    if (IsRewindBufferInUse && m_RewindBuffer1)
    {
        LogDump::LogA("Free rewind buffer\n");
        ResetRewindBuffer(true);

        if (m_RewindBuffer1)
            delete m_RewindBuffer1;

        if (m_RewindBuffer2)
            delete m_RewindBuffer2;
    }

    _A3DD40 = NULL;
}

void Scene::Destroy()
{
    ResetRewindBuffer(true);
    g_SceneSaveLoad->ResetSavedPlayMode();
    SoundSlot::DeallocateStreams(nullptr);

    if (GetFragment())
    {
        if (g_AssetManager->m_LoadBlocks)
        {
            m_PlayMode = MODE_STOP;
            UnloadLoadedFolders();
        }

        SetFragment(nullptr);

        FrameBasedSubAllocator* allocator = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType(0)];
        if (strcmp(allocator->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
            allocator->RemoveLast();
    }

    ((Folder_*)this)->Destroy();

    g_AssetManager->m_BlocksUnloaded = false;

    g_AssetManager->_8794B0(nullptr);

    int32_t assetIndex = g_AssetManager->FindFirstLoadedAsset(0);
    if (assetIndex)
    {
        for (Asset* asset = g_AssetManager->m_ResourcesInstancesList[assetIndex]; asset; asset = g_AssetManager->GetNextLoadedAsset(asset))
        {
            LogDump::LogA("UNFREED ASSET: %s, refcount=%d\n", asset->GetName(), asset->m_Flags.ReferenceCount);
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::Update()
{
}

void Scene::Render()
{
    const DWORD64 startTime = __rdtsc();

    for (uint32_t i = 0; i < 31; ++i)
    {
        FrameBuffer* fb = m_FrameBuffers[i];

        fb->Reset();
        fb->SubmitEnableLightingCommand(0);

        if (i == 0 || i == 2 || i == 19)
        {
            fb->SubmitEnableAlphaChannelCommand(false);
            fb->SubmitEnableAlphaTestCommand(false);
        }
        else
        {
            fb->SubmitEnableAlphaChannelCommand(true);
            fb->SubmitEnableAlphaTestCommand(true);
        }

        if (i == 16 || i == 15)
            fb->SubmitSetBlendModeCommand(true);
        else
            fb->SubmitSetBlendModeCommand(false);
    }

    m_FrameBuffers[4]->SubmitSetZBiasCommand(1);
    m_FrameBuffers[5]->SubmitSetZBiasCommand(2);
    m_FrameBuffers[19]->SubmitEnableZTestCommand(true);

    g_GfxInternal->SetClearFlagsForBufferIndex(2, 27);

    m_ActiveCamera->UpdateCameraMatrix();
    Camera::StoreActiveCameraPosition();

    DirectX::XMMATRIX cameraMatrix;
    m_ActiveCamera->GetMatrix(cameraMatrix);

    if (!g_LoadScreenInfo->m_Enabled)
    {
        if (!MoviePlayer::Instance)
        {
            if (DynamicSurroundGeometry::_A3E0DC)
                DynamicSurroundGeometry::_8FA270(DynamicSurroundGeometry::_A3E0DC); //  TODO: convert to object method once complete.
            g_GfxInternal->SetGameCameraMatrix(&Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix);
        }

        MoviePlayer::Instance->Render();
    }

    for (uint32_t i = 30; i; i--)
        m_FrameBuffers[i]->_436BF0();

    if (m_CollisionListList.size())
    {
        void* scratchpadMemory = MemoryManager::AllocatorsList[SCRATCHPAD]->AllocateAligned(ALIGN_4BYTES(4 * m_CollisionListList.size() + 127), 64, __FILE__, __LINE__);
        for (uint32_t i = m_CollisionListList.size(); i; --i)
        {
            if (m_CollisionListList[i]->m_Owner && ((Node*)m_CollisionListList[i]->m_Owner)->m_QuadTree)
                m_CollisionListList[i]->CommitCollision();
        }

        MemoryManager::ReleaseMemory(scratchpadMemory, true);
    }

    NewFrameNumber++;

    const DWORD64 endTime = __rdtsc();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::ReleaseQuadTreeAndRenderlist()
{
    if (!MainQuadTree)
    {
        LogDump::LogA("quadtrees and renderlists not allocated\n");
        return;
    }

    ClearNodesLists();
    g_GfxInternal->SetRenderBufferIsEmpty(true);
    g_AssetManager->DestroySceneNodesFrameBuffers(0);
    const unsigned int resind = g_AssetManager->FindFirstLoadedAsset(0);

    if (resind)
        for (Asset* a = g_AssetManager->m_ResourcesInstancesList[resind]; a; a = g_AssetManager->GetNextLoadedAsset(a))
            if (a->GetInstancePtr() == ModelAsset::Instance)
                ((ModelAsset*)a)->_856E60();

    _895F50();
}

void Scene::LoadResourceBlockIntoSceneBuffer(const char* assetname, AssetInfo::ActualAssetInfo* assetinfo)
{
    FileBuffer assetfile(assetname, 161, true);

    assetinfo->m_ResourceDataBufferPtr = g_AssetManager->LoadResourceBlock(&assetfile, (int*)&assetinfo->m_ResourceAllocatedAlignedBufferPtr, &assetinfo->m_ResourceDataBufferSize, Asset::BlockTypeNumber::NONE);
    const DWORD64 startTime = __rdtsc();

    g_AssetManager->_878030();
    g_AssetManager->InstantiateAssetsAndClearAssetsList();
    g_AssetManager->MakeSpaceForAssetsList();

    const DWORD64 endTime = __rdtsc();

    LogDump::LogA("Timings: FixupAssetRefsInLoadedAssetBlocks: %f\n", (endTime - startTime) / Timer::ClockGetCycles());

    if (assetinfo->m_ResourceDataBufferPtr)
        LogDump::LogA("read asset block file: %s\n", assetname);
    else
        LogDump::LogA("Asset file could not be loaded: %s\n", assetname);
}

void Scene::CreateSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode, unsigned int savesize)
{
    LogDump::LogA("save point creation pending!\n");

    if (!savesize)
    {
        LogDump::LogA("Trying to create empty savepoint??? Bailing out.\n");
        return;
    }

    m_SaveSlotIndex = slotind;
    m_SaveLoadState = STATE_SAVE;
    m_MemoryCardIndex = memcardind;
    m_SavePointOperationError = STATUS_OK;
    m_SaveDir = savedirectory;
    m_SaveGameSize = savesize;
    m_SaveData = summarynode;
}

void Scene::RestoreSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode, MemoryCards* memcards)
{
    m_SaveLoadState = STATE_LOAD;
    m_MemoryCardIndex = memcardind;
    m_SaveSlotIndex = slotind;
    m_SavePointOperationError = STATUS_OK;
    m_SaveDir = savedirectory;
    m_SaveData = summarynode;
    m_MemoryCards = memcards;
}

void Scene::LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode)
{
    m_SaveLoadState = STATE_LOAD_SUMMARY;
    m_MemoryCardIndex = memcardind;
    m_SaveSlotIndex = slotind;
    m_SavePointOperationError = STATUS_OK;
    m_SaveDir = savedirectory;
    m_SaveData = summarynode;

    char slotstr[16] = {};
    sprintf(slotstr, "Slot%02d", m_SaveSlotIndex);
    SavePoint savepoint(MemoryCardInfo[m_MemoryCardIndex], m_SaveDir.m_Str, slotstr, SAVEPOINT_SAVE_SIZE);

    if (savepoint.Open(STATUS_SUCCESS))
    {
        if (!g_SceneSaveLoad->LoadSaveSummary(&savepoint, m_SaveData))
            m_SavePointOperationError = STATUS_CANT_READ_SAVE_DATA;
    }
    else
        m_SavePointOperationError = STATUS_CANT_OPEN_FILE;

    if (m_SavePointOperationError == STATUS_CANT_READ_SAVE_DATA)
    {
        if (!MemoryCardInfo[m_MemoryCardIndex]->m_SaveFolderPath.m_Length)
        {
            LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");

            m_SavePointOperationError = STATUS_SAVEDIR_NOT_READY;
            m_SaveLoadState = STATE_DONE;
        }

        if (!MemoryCardInfo[m_MemoryCardIndex]->m_Formatted ||
            !FileBuffer::IsDirectoryValid(MemoryCardInfo[m_MemoryCardIndex]->m_SaveFolderPath.m_Str))
        {
            m_SavePointOperationError = STATUS_SAVEDIR_NOT_READY;
            m_SaveLoadState = STATE_DONE;
        }
    }

    m_SaveLoadState = STATE_DONE;
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::ProfileMemory(const int)
{
    LogDump::LogA("(memory profiler is not available in this build)\n");
    // TODO: calls profiler method of MemoryManager class.
}

void Scene::ClearNodesLists()
{
    UpdateOrBlockingListSize = NodesWithUpdateOrBlockingScripts.size();

    for (unsigned int i = 0; i < NodesWithUpdateOrBlockingScripts.size(); ++i)
        NodesWithUpdateOrBlockingScripts[i].m_Node->ClearFromBlockingList();

    NodesWithUpdateOrBlockingScripts.clear();

    m_NodesWithUpdateOrBlockingScripts = NULL;

    for (unsigned int i = 0; i < NodesList.size(); ++i)
        NodesList[i].QDTree->m_Owner->ClearFromSceneList();

    NodesList.clear();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::_895F50()
{
}

Folder_* Scene::GetLoadedBlockByIndex(const unsigned int index) const
{
    return (Folder_*)m_LoadedBlocks[index];
}

void Scene::LoadSceneSession(void) const
{
    if (!GetFragment() ||
        m_PlayMode != MODE_STOP ||
        g_AssetManager->m_LoadBlocks)
        return;

    String fragmentpath, fragmentname, sessionpath;
    FileBuffer::ExtractFileDir(fragmentpath, GetFragment());
    FileBuffer::ExtractFileName(fragmentname, GetFragment());
    fragmentpath.Replace('/', '_');
    fragmentpath.Append(fragmentname.m_Str);
    fragmentpath.Append(".editorsession");
    sessionpath = "/data/sessions/";
    sessionpath.Append(fragmentpath.m_Str);

    if (!FileBuffer::FindFileEverywhere(sessionpath.m_Str, 0))
        return;

    /*
    *   Editor Session file format:
    *   (this is my best guess)
    *
    *   1:  version
    *   2:  <should this be loaded?> <folder's unique id> <fragment's unique id> <unused> <unused>
    *   3:  repeat line above as many times as needed
    */

    FileBuffer f(sessionpath.m_Str, 1, true);
    unsigned int fileversion = 0;
    f._scanf(&f, "%d\n", &fileversion);

    if (fileversion != EDITOR_SESSION_FILE_VERSION)
        return;

    bool shouldload = false;
    unsigned int folduniqid, fragmentuniqid;
    unsigned char dummy = 0;
    Folder_* folder = nullptr;

    while (f._scanf(&f, "%d %d %d %d %d\n", &shouldload, &folduniqid, &fragmentuniqid, &dummy, &dummy) == 5)
    {
        if (!folduniqid)
            continue;

        folder = nullptr;
        Folder_* foundfolderentity = (Folder_*)g_AssetManager->FindFirstEntity();

        if (!foundfolderentity)
            continue;

        do
        {
            EntityType* folderscript = foundfolderentity->m_ScriptEntity;
            if (folderscript)
            {
                bool noparent = false;
                while (tFolder != folderscript)
                {
                    folderscript = folderscript->m_Parent;
                    if (!folderscript)
                    {
                        noparent = true;
                        break;
                    }
                }

                if (noparent)
                {
                    foundfolderentity = (Folder_*)g_AssetManager->FindNextEntity(foundfolderentity);
                    continue;
                }

                Fragment* frgm = foundfolderentity->m_Fragment;
                const unsigned int frgmid = frgm ? frgm->m_UniqueId.m_Id.m_Id1 : 0;

                if (frgmid == folduniqid)
                {
                    const unsigned int foldid = frgm ? frgm->m_UniqueId.m_Id.m_Id2 : 0;
                    if (foldid == fragmentuniqid)
                        folder = foundfolderentity;
                }
            }

            foundfolderentity = (Folder_*)g_AssetManager->FindNextEntity(foundfolderentity);
        } while (foundfolderentity);

        if (folder)
        {
            folder->m_BlockId |= 0x20000000;
            if (shouldload)
                SceneInstance->LoadMap(folder->GetBlockId() - 1, folder);
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::InstantiateAssetsToLists()
{
    m_QuadTreesList.clear();
    m_ParticleSystemsList.clear();
    CharactersList.clear();
    ModelsList.clear();
    TimePassed = 0.f;

    for (Node* node = (Node*)g_AssetManager->FindFirstEntity(); node; node = (Node*)g_AssetManager->FindNextEntity(node))
    {
        EntityType* scriptEntity = node->m_ScriptEntity;
        if (!scriptEntity)
            continue;

        while (tNode != scriptEntity)
        {
            scriptEntity = scriptEntity->m_Parent;
            if (!scriptEntity)
                break;
        }

        if (!scriptEntity)
            continue;

        if (m_PlayMode != MODE_STOP)
        {
            scriptEntity = node->m_ScriptEntity;
            if (scriptEntity)
            {
                while (tCharacter != scriptEntity)
                {
                    scriptEntity = scriptEntity->m_Parent;
                    if (!scriptEntity)
                        break;
                }

                if (scriptEntity)
                    CharactersList.push_back((Character*)node);
            }

            scriptEntity = node->m_ScriptEntity;
            if (scriptEntity)
            {
                while (tModel != scriptEntity)
                {
                    scriptEntity = scriptEntity->m_Parent;
                    if (!scriptEntity)
                        break;
                }

                if (scriptEntity)
                {
                    ((Model*)node)->m_AlphaFlags.m_FlagBits.GlobalId = ModelsList.size();
                    ModelsList.push_back((Model*)node);
                }
            }
        }

        if (node->m_QuadTree)
        {
            node->m_QuadTree->field_4D &= ~0x20;
            if (node->m_QuadTree->m_UserType < 0)
                node->m_QuadTree->Refresh();
        }

        scriptEntity = node->m_ScriptEntity;
        if (scriptEntity)
        {
            while (tSoundEmitter != scriptEntity)
            {
                scriptEntity = scriptEntity->m_Parent;
                if (!scriptEntity)
                    break;
            }

            if (scriptEntity)
                SoundEmittersList.push_back((SoundEmitter*)node);
        }
    }

    for (uint32_t i = 0; i < CollisionProbe::ProbesList.size(); ++i)
    {
        CollisionProbe* probe = CollisionProbe::ProbesList[i];
        probe->m_Updated = false;
        probe->m_HintNode = nullptr;
        probe->m_RealNode = nullptr;
        probe->m_ClosestNode = nullptr;
        probe->m_ClosestCollisionVolume = NULL;

        probe->ClearCache_Impl();
    }

    for (uint32_t i = 0; i < m_ParticleSystemsList.size(); ++i)
        m_ParticleSystemsList[i] |= 1;
}

void Scene::AddCollisionList(CollisionInfo* list)
{
    m_CollisionListList.push_back(list);
    list->SetListGlobalIndex(m_CollisionListList.size());
}

void Scene::RemoveCollisionList(CollisionInfo* list)
{
    if (!m_CollisionListList.size())
        return;

    unsigned int i = 0;
    for (; i < m_CollisionListList.size(); ++i)
        if (m_CollisionListList[i] == list)
            break;
        else
            if (i == m_CollisionListList.size())
                return;

    list->SetListGlobalIndex(-1);
    m_CollisionListList.erase(m_CollisionListList.begin() + i, m_CollisionListList.begin() + i + 1);

    for (; i < m_CollisionListList.size(); ++i)
        m_CollisionListList[i]->SetListGlobalIndex(i);
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::LoadMap(const unsigned int blockind, Node* foldernode)
{
}

float Scene::GetTimeMultiplier() const
{
    return m_TimeMultiplier;
}

void Scene::SetTimeMultiplier(const float multiplier)
{
    StoreProperty(10, &m_TimeMultiplier, tNUMBER);
    m_TimeMultiplier = multiplier;
}

float Scene::GetRewindTimeMultiplier() const
{
    return m_RewindTimeMultiplier;
}

void Scene::SetRewindTimeMultiplier(const float mult)
{
    m_RewindTimeMultiplier = mult;
}

char Scene::GetInitMode() const
{
    return m_InitMode;
}

void Scene::SetInitMode(const char initmode)
{
    m_InitMode = initmode;
}

float Scene::GetWindSize() const
{
    if (!m_RewindBuffer1 || !m_RewindBuffer2)
        return 0.f;

    int windsize1 = m_RewindBuffer1->m_List_1.size() ? m_RewindBuffer1->m_List_1.end()->m_GameTimeMs : NULL;
    int windsize2 = m_RewindBuffer2->m_List_1.size() ? m_RewindBuffer2->m_List_1.end()->m_GameTimeMs : NULL;

    if (!windsize1 || !windsize2)
        return 0.f;

    if (windsize1 > windsize2)
        windsize2 = windsize1;

    return (GameTimeMs - windsize2) * 0.001f;
}

void Scene::_894810(const int index, const bool enabled)
{
    if (index == -1)
        return;

    //  NOTE: wtf?
    if (enabled)
        *(int*)&(m_CollisionListList[index]->m_Owner) |= 1;
    else
        *(int*)&(m_CollisionListList[index]->m_Owner) &= ~1;
}

const int Scene::GetPlaymode() const
{
    return m_PlayMode;
}

const float Scene::GetRewindResumeTime() const
{
    return (float)(m_RewindResumeTimeMs * 0.001f);
}

const int Scene::GetRewindResumeTimeMs() const
{
    return m_RewindResumeTimeMs;
}

void Scene::SetWindMode(const int* args)
{
    field_1A8 = *args != false;
    m_WindMode = *args == false;
}

void Scene::SetWindPause(const int* args)
{
    field_1AA = *args != false;
    m_WindPause = *args == false;
}

void Scene::SetPauseMode(const int* args)
{
    if (*args)
    {
        m_PlayMode = MODE_PAUSED;
        StoreGameCamera();
    }
    else
    {
        if (m_PlayMode == MODE_PAUSED)
            m_PlayMode = MODE_INGAME;
        StoreGameCamera();
    }
}

void Scene::FlushRewind(const int* args)
{
    m_FlushRewindRequested = true;
}

void Scene::ResetGame(const int* args)
{
    Node* blocks[6] = {};
    g_SceneSaveLoad->ResetGame(blocks);
}

void Scene::GetLoadedBlock(int* args)
{
    *args = (int)m_LoadedBlocks[args[1]];
}

void Scene::SetLoadedBlock(int* args)
{
    LoadMap(args[0], (Node*)args[1]);
}

void Scene::UpdateLoadedBlocks(int* args)
{
    UpdateLoadedBlocks_Impl(1, (Node*)args[0]);
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::UpdateLoadedBlocks_Impl(const int dummy, Node* callbackEntity)
{
    g_StreamedSoundBuffers->WaitForSoftPause();
}

void Scene::BuildFastFindNodeVector(int* args)
{
    g_AssetManager->BuildFastFindNodeVector();
}

void Scene::DeleteFastFindNodeVector(int* args)
{
    g_AssetManager->DeleteFastFindNodeVector();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::UnloadLoadedFolders()
{
}

void Scene::Reset()
{
    if (m_PlayMode == MODE_STOP)
        return;

    if (IsRewindBufferInUse)
        ResetRewindBuffer(true);

    g_SceneSaveLoad->ResetSavedPlayMode();
    const int playmodeStopCommand = GetCommandByName("playmode_stop");
    if (playmodeStopCommand >= 0)
        TriggerScriptForAllChildren(playmodeStopCommand, this, nullptr);

    if (g_AssetManager->m_LoadBlocks)
        UnloadLoadedFolders();

    g_AssetManager->m_BlocksUnloaded = true;
    DestroyAllChildren();
    g_AssetManager->m_BlocksUnloaded = false;

    m_LoadedBlocks[0] = nullptr;
    m_LoadedBlocks[1] = nullptr;
    m_LoadedBlocks[2] = nullptr;
    m_LoadedBlocks[3] = nullptr;
    m_LoadedBlocks[4] = nullptr;
    m_LoadedBlocks[5] = nullptr;
    m_LoadedBlocks[6] = nullptr;
    m_LoadedBlocks[7] = nullptr;

    ClearNodesLists();
    InstantiateAssetsToLists();
    ScriptThread::ResetCache();
    m_SharedProbe->Reset_Impl();
    m_Fragment->m_FragmentRes.GetAsset<FragmentAsset>()->ApplyFragmentResource(m_Id.Id, true);
    g_AssetManager->_8794B0(0);
    Scriptbaked::AssignCommonNodes();
    NextUpdateTime = 0;
    GameTimeMs = 0;
    InstantiateAllChildren();
    UpdateLoadedBlocks_Impl(1, nullptr);
    EnumSceneCamerasAndUpdate();
    TotalFrames = 0;
    NewFrameNumber = 0;
}

void Scene::SyncEditorCamera(const bool kdtreealloc, const int gamepadindex)
{
    m_QuadTreesAllocated = kdtreealloc;
    if (!kdtreealloc)
    {
        Control::ControllersUsedByEditor[0] = false;
        Control::ControllersUsedByEditor[1] = false;

        StoreGameCamera();
        return;
    }

    if (m_GameCamera && m_EditorCamera && m_PlayMode != MODE_STOP)
    {
        Vector4f cameraPos;
        Orientation cameraOrient;
        m_GameCamera->GetWorldPos(cameraPos);
        m_EditorCamera->SetPos(cameraPos);

        m_GameCamera->GetOrient(cameraOrient);
        m_EditorCamera->SetOrient(cameraOrient);
        m_EditorCamera->StoreProperty(11, &m_EditorCamera->m_Fov, tNUMBER);
        m_EditorCamera->m_Fov = m_GameCamera->m_Fov;

        if (EntityType::IsParentOf(tEditorCamera, m_EditorCamera))
            m_EditorCamera->m_GamepadIndex = gamepadindex;
    }

    if (gamepadindex >= 0)
    {
        Control::ControllersUsedByEditor[gamepadindex] = true;
        StoreGameCamera();
    }
    else
    {
        Control::ControllersUsedByEditor[0] = false;
        Control::ControllersUsedByEditor[1] = false;

        StoreGameCamera();
    }
}

void Scene::AnnotateSphere_Impl(const Vector4f& pos, const int a2, const int a3, const int a4) const
{
}

void Scene::AnnotateLine_Impl(const Vector4f& lineStart, const Vector4f& lineEnd, const int a3, const int a4) const
{
}

void Scene::AnnotatePoint_Impl(const Vector4f& point, const int a2, const int a3) const
{
}

void Scene::SetParticleSystemUsed(const int particleSystemIndex, const bool used)
{
    if (particleSystemIndex == -1)
        return;

    m_ParticleSystemsList[particleSystemIndex] = used;
}

void Scene::CreateQuadTrees()
{
    AllocateMainQuadTreeNodes(10000,    RENDERLIST);
    AllocateAuxQuadTreeNodes(12200,     RENDERLIST);

    MainQuadTree    = new QuadTree(0x4000, 4);
    AuxQuadTree     = new QuadTree(0x4000, 8);

    if (m_QuadTree)
        m_QuadTree->Refresh();

    for (Node* node = m_FirstChild; node; node = node->m_NextSibling)
        RefreshChildrenQuadTrees(node);

    m_QuadTreesAllocated = true;
}

void Scene::Start()
{
    if (m_PlayMode != MODE_STOP)
        return;

    m_PlayMode = MODE_INGAME;
    UpdateLoadedBlocks_Impl(1, nullptr);
    EnumSceneCamerasAndUpdate();
    RealTimeMs = NULL;
    GameTimeMs = NULL;
    NextUpdateTime = NULL;
    TotalFrames = NULL;
    NewFrameNumber = NULL;

    const int startCommandId = GetCommandByName("start");
    if (startCommandId >= 0)
        TriggerScriptForAllChildren(startCommandId, this, nullptr);

    InstantiateAssetsToLists();
    g_SceneSaveLoad->_874940();

    if (IsRewindBufferInUse)
    {
        AllocateRewindBuffer();
        ResetRewindBuffer(true);
    }

    m_RewindResumeTimeMs = NULL;
    TotalFrames = NULL;
    NewFrameNumber = NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::Load(const char* sceneName)
{
    m_StartTimeMs = Timer::GetMilliseconds();
    int alloctotalbefore;

    FreeRewindBuffer();
    ReleaseQuadTreeAndRenderlist();

    LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Initialization", Timer::GetMilliseconds() - m_StartTimeMs, Timer::GetMilliseconds() - m_StartTimeMs);

    g_Progress->Reset();
    g_Progress->AddLoadbarPhase("load scene", 25 * Timer::ClockGetCycles(), true);
    g_Progress->_40E7F0(1, __rdtsc());

    if (g_AssetManager->m_LoadBlocks)
    {
        char scene_path[1024] = {};
        g_AssetManager->GetPlatformSpecificPath(scene_path, sceneName, "", AssetManager::PlatformId::PC);
        strcat(scene_path, "/");

        char pathdummy[1024] = {};
        char scene_fname[128] = {};
        FileBuffer::ExtractFilePath(sceneName, pathdummy, scene_fname, pathdummy);
        strcat(scene_path, scene_fname);

        String block_path_shared, block_path_localised;
        Folder_::GetResourcePathRelative(block_path_shared, scene_path, Asset::BlockTypeNumber::NONE, 0);
        Folder_::GetResourcePathRelative(block_path_localised, scene_path, Asset::BlockTypeNumber::NONE, Script::GetCurrentCountryCode());
#ifdef INCLUDE_FIXES
        if (!FileBuffer::FindFileEverywhere(block_path_shared.m_Str, 0))
        {
            LogDump::LogA("Asset shared block not found: \"%s\"!\n", block_path_shared.m_Str);
            return;
        }
#else
        FileBuffer::FindFileEverywhere(block_path_shared.m_Str);
#endif
#ifdef INCLUDE_FIXES
        if (!FileBuffer::FindFileEverywhere(block_path_localised.m_Str, 0))
        {
            LogDump::LogA("Asset localization block not found: \"%s\"!\n", block_path_shared.m_Str);
            return;
        }
#else
        FileBuffer::FindFileEverywhere(block_path_localised.m_Str);
#endif

        int mainAssetAllocMem = MemoryManager::AllocatorsList[MAIN_ASSETS]->GetFreeMemory();
        LogDump::LogA("asset block before: %0.1f Kb\n", mainAssetAllocMem * 0.0009765625f);

        FrameBasedSubAllocator* allocator = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType(0)];
        if (strcmp(allocator->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
            allocator->MakeNew();

        LoadingAssetBlock = true;
        ((Defragmentator*)MemoryManager::AllocatorsList[DEFRAGMENTING])->field_20 = false;
        LoadResourceBlockIntoSceneBuffer(block_path_shared.m_Str, &m_AssetBlockInfo->m_AssetInfo_Shared);
        LoadResourceBlockIntoSceneBuffer(block_path_localised.m_Str, &m_AssetBlockInfo->m_AssetInfo_Localised);

        m_BlockId = m_BlockId | 0x80000000;
        LoadingAssetBlock = false;
        alloctotalbefore = MemoryManager::AllocatorsList[MAIN_ASSETS]->GetFreeMemory();
        LogDump::LogA("asset block took %0.1f Kb\n", (mainAssetAllocMem - MemoryManager::AllocatorsList[MAIN_ASSETS]->GetFreeMemory()) * 0.0009765625f);
    }

    LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Load main asset block", Timer::GetMilliseconds() - m_StartTimeMs, Timer::GetMilliseconds() - m_StartTimeMs);

    m_StartTimeMs = Timer::GetMilliseconds();

    g_AssetManager->m_ActiveBlockId = (8 * m_BlockId) >> 3;
    _A3D858 = 1;

    FrameBasedSubAllocator* allocator = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType(0)];
    if (strcmp(allocator->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
        allocator->MakeNew();

    UINT64 fragmentloadstarttime = __rdtsc();
    g_AssetManager->_878030();
    SetFragment(sceneName);
    g_AssetManager->MakeSpaceForAssetsList();
    LogDump::LogA("Timings: SetFragment: %f\n", (__rdtsc() - fragmentloadstarttime) / Timer::ClockGetCycles());
    const int alloctotal = MemoryManager::AllocatorsList[MAIN_ASSETS]->GetFreeMemory();
    LogDump::LogA("Scene graph took %0.1f KB\n", (alloctotal - alloctotalbefore) * 0.0009765625f);
    LogDump::LogA("Asset block after: %0.1f KB\n", alloctotal * 0.0009765625f);

    const int freeassetind = g_AssetManager->FindFirstLoadedAsset(0);
    if (freeassetind)
    {
        for (FragmentAsset* frgm = (FragmentAsset*)g_AssetManager->m_ResourcesInstancesList[freeassetind]; frgm; frgm = (FragmentAsset*)g_AssetManager->GetNextLoadedAsset(frgm))
        {
            if (frgm->GetInstancePtr() == FragmentAsset::Instance)
            {
                if ((frgm->field_24 & 1) == 0)
                {
                    LogDump::LogA("NOT UNLOADING FRAGMENT: %s\n", frgm->m_ResourcePath);
                    continue;
                }

                if (!frgm->field_20)
                {
                    frgm->field_20 = nullptr;
                    continue;
                }

                delete frgm->field_20;
            }
        }
    }

    ((Defragmentator*)MemoryManager::AllocatorsList[DEFRAGMENTING])->field_20 = true;
    _A3D858 = false;
    g_AssetManager->m_ActiveBlockId = -1;
    _896BA0();
    AllocateRewindBuffer();
    DWORD currtimems = Timer::GetMilliseconds();
    LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Scene load and apply", currtimems - m_StartTimeMs, currtimems - m_StartTimeMs);
    m_StartTimeMs = currtimems;
    LoadSceneSession();
    currtimems = Timer::GetMilliseconds();
    LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Load current asset blocks", currtimems - m_StartTimeMs, currtimems - m_StartTimeMs);
    m_StartTimeMs = currtimems;
    currtimems = Timer::GetMilliseconds();
    LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Compile 'test' folder scripts", currtimems - m_StartTimeMs, currtimems - m_StartTimeMs);
    m_StartTimeMs = currtimems;
    Scriptbaked::InstantiateGlobalScripts();
    EnumSceneCamerasAndUpdate();
    TotalFrames = 0;
    NewFrameNumber = 0;
    InstantiateAssetsToLists();
    g_Progress->Complete();
}

void Scene::FinishCreation(const char* logTitle)
{
    LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", logTitle, Timer::GetMilliseconds() - m_StartTimeMs, Timer::GetMilliseconds() - m_StartTimeMs);
    m_StartTimeMs = Timer::GetMilliseconds();
}

void Scene::StoreGameCamera()
{
    if (m_GameCamera && m_EditorCamera != nullptr && m_PlayMode != 1)
        m_ActiveCamera = m_QuadTreesAllocated ? (Camera*)m_EditorCamera : m_GameCamera;
    else
        if (m_EditorCamera)
            m_ActiveCamera = (Camera*)m_EditorCamera;
        else
            if (m_GameCamera)
                m_ActiveCamera = m_GameCamera;
            else
                m_ActiveCamera = nullptr;

    if (m_ActiveCamera)
    {
        Camera::StoreActiveCameraPosition();

        if ((((m_CameraPosition.x - Camera::ActiveCameraPosition.x) * (m_CameraPosition.x - Camera::ActiveCameraPosition.x)) +
            ((m_CameraPosition.z - Camera::ActiveCameraPosition.z) * (m_CameraPosition.z - Camera::ActiveCameraPosition.z)) +
            ((m_CameraPosition.y - Camera::ActiveCameraPosition.y) * (m_CameraPosition.y - Camera::ActiveCameraPosition.y))) > 100.f)
        {
            LogDump::LogA("Camera has moved more than 10m. Forcing Lod-calculation\n");
            CalculateLod(0);
        }

        m_CameraPosition = Camera::ActiveCameraPosition;
    }
}

void Scene::EnumSceneCamerasAndUpdate()
{
    m_EditorCamera = nullptr;
    m_GameCamera = nullptr;

    for (Node* n = m_FirstChild; n; n = n->m_NextSibling)
    {
        EntityType* scent = n->m_ScriptEntity;
        if (!scent)
            continue;

        while (tCamera != scent)
        {
            scent = scent->m_Parent;
            if (!scent)
                break;
        }

        if (!m_EditorCamera && strcmp(n->m_Name, "editorcamera") == NULL)
            m_EditorCamera = (EditorCamera*)n;

        if (!m_GameCamera && strcmp(n->m_Name, "gamecamera") == NULL)
            m_GameCamera = (Camera*)n;
    }

    StoreGameCamera();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::AllocateRewindBuffer()
{
    if (IsRewindBufferInUse && !m_RewindBuffer1)
    {
        if (MemoryManager::AllocatorsList[CUTSCENE_OR_REWIND]->GetAllocationsMadeTotal() > 0)
        {
            LogDump::LogA("cannot allocate rewind buffer - memory block is in use!\n");
            return;
        }

        LogDump::LogA("Allocate rewind buffer...\n");
        m_RewindBuffer1 = new TransactionBuffer(RewindBufferSize_1);
        m_RewindBuffer2 = new TransactionBuffer(RewindBufferSize_2);
        field_268 = NULL;
    }

    m_RewindBuffer1->m_List_1.resize(1);
    m_RewindBuffer1->m_ListHead = (void*)&(m_RewindBuffer1->m_List_1.begin());
    m_RewindBuffer1->m_Size = NULL;
    m_RewindBuffer1->field_20 = NULL;

    m_RewindBuffer2->m_List_1.resize(1);
    m_RewindBuffer2->m_ListHead = (void*)&(m_RewindBuffer2->m_List_1.begin());
    m_RewindBuffer2->m_Size = NULL;
    m_RewindBuffer2->field_20 = NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::BuildSceneTree()
{
    if (MainQuadTree)
        return;

    /*g_Blocks->ResetSceneChildrenNodes(true);
    AllocateQuadTrees();
    AdjustNodesListsSize();*/
}

void Scene::FreeRewindBuffer()
{
    if (IsRewindBufferInUse && m_RewindBuffer1)
    {
        LogDump::LogA("Free rewind buffer\n");
        ResetRewindBuffer(true);

        delete m_RewindBuffer1;
        delete m_RewindBuffer2;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::ResetRewindBuffer(bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::_896BA0()
{
    if (MainQuadTree)
        return;

    g_AssetManager->DestroySceneNodesFrameBuffers(1);
    CreateQuadTrees();

}

void Scene::TriggerScriptForAllChildren(int scriptId, Node* node, int* args)
{
    node->TriggerGlobalScript(scriptId, args);

    for (Node* n = node->m_FirstChild; n; n = n->m_NextSibling)
        TriggerScriptForAllChildren(scriptId, n, args);
}

void Scene::RefreshChildrenQuadTrees(Node* node)
{
    if (node->m_QuadTree)
        node->m_QuadTree->Refresh();

    for (Node* node_ = node->m_FirstChild; node_; node_ = node_->m_NextSibling)
        RefreshChildrenQuadTrees(node_);
}

void Scene::Register()
{
    tScene = new EntityType("Scene");
    tScene->InheritFrom(tNode);
    tScene->SetCreator((CREATOR)Create);

    tScene->RegisterProperty(tNUMBER, "timemultiplier", (EntityGetterFunction)&GetTimeMultiplier, (EntitySetterFunction)&SetTimeMultiplier, nullptr, 10);
    tScene->RegisterProperty(tNUMBER, "rewindtimemultiplier", (EntityGetterFunction)&GetRewindTimeMultiplier, (EntitySetterFunction)&SetRewindTimeMultiplier, nullptr);
    tScene->RegisterProperty(tTRUTH, "initmode", (EntityGetterFunction)&GetInitMode, (EntitySetterFunction)&SetInitMode, nullptr);
    tScene->RegisterProperty(tNUMBER, "windsize", (EntityGetterFunction)&GetWindSize, nullptr, nullptr);
    tScene->RegisterProperty(tINTEGER, "playmode", (EntityGetterFunction)&GetPlaymode, nullptr, nullptr);
    tScene->RegisterProperty(tNUMBER, "rewindresumetime", (EntityGetterFunction)&GetRewindResumeTime, nullptr, nullptr);
    tScene->RegisterProperty(tINTEGER, "rewindresumetime_ms", (EntityGetterFunction)&GetRewindResumeTimeMs, nullptr, nullptr);

    tScene->RegisterScript("setwindmode(truth)", (EntityFunctionMember)&SetWindMode, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("setwindpause(truth)", (EntityFunctionMember)&SetWindPause, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("setpausemode(truth)", (EntityFunctionMember)&SetPauseMode, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("flushrewind", (EntityFunctionMember)&FlushRewind, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("ResetGame", (EntityFunctionMember)&ResetGame, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("getloadedblock(integer):entity", (EntityFunctionMember)&GetLoadedBlock, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("setloadedblock(integer,entity)", (EntityFunctionMember)&SetLoadedBlock, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("updateloadedblocks(entity)", (EntityFunctionMember)&UpdateLoadedBlocks, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("buildfastfindnodevector", (EntityFunctionMember)&BuildFastFindNodeVector, NULL, NULL, NULL, nullptr, nullptr);
    tScene->RegisterScript("deletefastfindnodevector", (EntityFunctionMember)&DeleteFastFindNodeVector, NULL, NULL, NULL, nullptr, nullptr);

    PreBlocksUnloadedCommand = GetMessage("pre_blocks_unloaded", true);
    BlocksUnloadedCommand = GetMessage("blocks_unloaded", true);
    InvalidatePlaceholderModelCommand = GetMessage("invalidate_placeholder_model", true);

    tScene->PropagateProperties();
}

Scene* Scene::Create(AllocatorIndex)
{
    return new Scene();
}