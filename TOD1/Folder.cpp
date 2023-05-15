#include "Folder.h"
#include "File.h"
#include "IntegerType.h"
#include "AssetManager.h"
#include "Scene.h"
#include "FrameBasedSubAllocator.h"
#include "Node.h"
#include "File.h"
#include "Timer.h"

EntityType* tFolder;

int Folder_::CurrentBlockId = -1;
int Folder_::_A11B84[9] =
{
    0, 1, 0, 0, 0, -1, -1, -1
};

void Folder_::DestroyAllChildren()
{
    if (m_FirstChild)
    {
        Node* firstchild = m_FirstChild;
        Node* nextsibling = firstchild->m_NextSibling;
        do
        {
            firstchild->Destroy();
            firstchild = nextsibling;
        } while (nextsibling);
    }

    m_FirstChild = nullptr;
}

void Folder_::ReadAssetBlockFile(AssetInfo::ActualAssetInfo* assinfo, const char* const assfname) const
{
    FileBuffer assfile(assfname, 161, true);
    assinfo->m_ResourceDataBufferPtr = g_AssetManager->LoadResourceBlock(&assfile, (int*)assinfo->m_ResourceAllocatedAlignedBufferPtr, &assinfo->m_ResourceDataBufferSize, m_BlockId);

    if (assinfo->m_ResourceDataBufferPtr)
        LogDump::LogA("Read asset block file: %s\n", assfname);
    else
        LogDump::LogA("Could not read the asset block file; probably due to checksums: %s\n", assfname);
}

void Folder_::FixDanglingAssets(AssetInfo::ActualAssetInfo* assinfo)
{
    g_AssetManager->DestroyDanglingAssets((char*)assinfo->m_ResourceDataBufferPtr, (char*)assinfo->m_ResourceDataBufferPtr + assinfo->m_ResourceDataBufferSize);
    uint32_t assetIndex = g_AssetManager->FindFirstLoadedAsset(0);
    Asset* assetInstance = nullptr;

    std::vector<Asset*> danglingAssets;

    if (assetIndex)
        assetInstance = g_AssetManager->m_ResourcesInstancesList[assetIndex];

    for (Asset* i = assetInstance; i; i = g_AssetManager->GetNextLoadedAsset(i))
    {
        if ((char*)i >= (char*)assinfo->m_ResourceDataBufferPtr && (char*)i < ((char*)assinfo->m_ResourceDataBufferPtr + assinfo->m_ResourceDataBufferSize))
            danglingAssets.push_back(i);
    }

    LogDump::LogA("\n*****************  Number of dangling assets : %d\n\n", danglingAssets.size());
    DWORD64 timeStart = __rdtsc();

    for (Node* entity = (Node*)g_AssetManager->FindFirstEntity(); entity; entity = (Node*)g_AssetManager->FindNextEntity(entity))
    {
        if (!entity->m_ScriptEntity)
            continue;

        EntityType* entityScript = entity->m_ScriptEntity;
        while (tNode != entityScript)
        {
            entityScript = entityScript->Parent;
            if (!entityScript)
                break;
        }

        if (entityScript)
            entity->nullsub_6((const std::vector<void*>&)danglingAssets);
    }

    DWORD64 timeEnd = __rdtsc();
    LogDump::LogA("time: %d\n", (timeEnd - timeStart) / Timer::ClockGetCyclesMilliseconds());

    g_AssetManager->DestroyDanglingAssets((char*)assinfo->m_ResourceDataBufferPtr, (char*)assinfo->m_ResourceDataBufferPtr + assinfo->m_ResourceDataBufferSize);

    assetIndex = g_AssetManager->FindFirstLoadedAsset(0);
    assetInstance = nullptr;

    if (assetIndex)
        assetInstance = g_AssetManager->m_ResourcesInstancesList[assetIndex];

    for (Asset* i = assetInstance; i; i = g_AssetManager->GetNextLoadedAsset(i))
    {
        if ((char*)i >= (char*)assinfo->m_ResourceDataBufferPtr && (char*)i < ((char*)assinfo->m_ResourceDataBufferPtr + assinfo->m_ResourceDataBufferSize))
            i->GetName();   //  NOTE: this was probably intended for some debugging purposes.
    }

    assinfo->m_ResourceDataBufferPtr = nullptr;
    MemoryManager::ReleaseMemory(assinfo->m_ResourceAllocatedAlignedBufferPtr, true);
    assinfo->m_ResourceAllocatedAlignedBufferPtr = nullptr;
    assinfo->m_ResourceDataBufferSize = 0;
}

void Folder_::GetResourcePathRelative(String& outPath, String resourceName, Asset::BlockTypeNumber blockType, const char* languageCode)
{
    char fileExt[16] = {};
    char fileDir[1024] = {};
    char fileName[128] = {};

    FileBuffer::ExtractFilePath(resourceName.m_Str, fileDir, fileName, fileExt);

    if (languageCode)
    {
        strcat(fileDir, languageCode);
        strcat(fileDir, "/");
    }
    else
        strcat(fileDir, "shared/");

    strcat(fileDir, fileName);

    if (languageCode)
    {
        strcat(fileDir, "_");
        strcat(fileDir, languageCode);
    }

    strcat(fileDir, ".");
    strcat(fileDir, blockType ? Asset::BlockTypeExtension[blockType] : Asset::BlockTypeExtension[Asset::BlockTypeNumber::MAIN]);

    outPath = fileDir;
}

Folder_::Folder_() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(Folder_);

    m_BlockId = 0;
    m_AssetBlockInfo = nullptr;
}

void Folder_::UnloadBlocks()
{
    m_Flags.HasFragment = false;

    if (m_Fragment)
    {
        if (!Scene::SceneInstance->m_PlayMode)
            CurrentBlockId = m_BlockId;

        if (m_BlockId < 0)
            g_AssetManager->m_BlocksUnloaded = 1;

        delete m_Fragment->m_Name;
        if (m_Fragment->m_FragmentRes.GetAsset<FragmentAsset>()->m_ResourcePath)
        {
            m_Fragment->m_Name = new char[52];
            strcpy(m_Fragment->m_Name, m_Fragment->m_FragmentRes.GetAsset<FragmentAsset>()->m_ResourcePath);
        }
        else
            m_Fragment->m_Name = nullptr;

        m_Fragment->LoadResourceFile(nullptr);
        DestroyAllChildren();

        if (_31)
            g_AssetManager->m_BlocksUnloaded = false;

        CurrentBlockId = -1;
    }

    FrameBasedSubAllocator* framealloc = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType(m_BlockId)];
    if (Scene::SceneInstance->m_PlayMode != 1)
    {
        LogDump::LogA("*** checking for dangling nodes ***\n");

        while (true)
        {
            Node* loadednode = (Node*)g_AssetManager->FindFirstEntity();
            if (!loadednode)
                break;

            while ( loadednode->m_Id.BlockId - 1 != (m_BlockId) )
            {
                loadednode = (Node*)g_AssetManager->FindNextEntity(loadednode);
                if (!loadednode)
                {
                    if (!strcmp(framealloc->GetAllocatorName(), "FrameBasedSubAllocator"))
                        framealloc->RemoveLast();

                    return;
                }
            }

            LogDump::LogA("deleting dangling node %s (id=%d) from block %d\n", loadednode->m_Name ? loadednode->m_Name : "", loadednode->m_Id.Id, loadednode->m_Id.BlockId - 1);
        }
    }

    if (!strcmp(framealloc->GetAllocatorName(), "FrameBasedSubAllocator"))
        framealloc->RemoveLast();
}

const int Folder_::GetBlockId() const
{
    return m_BlockId;
}

void Folder_::SetBlockId(unsigned int blockid)
{
    if (m_BlockId == blockid)
        return;

    m_BlockId = blockid;
    if (blockid)
        m_Flags.HasFragment = false;

    if (_29 || m_TaggedForUnload || _31)
        m_AssetBlockInfo = new AssetInfo;
    else
        delete m_AssetBlockInfo;
}

void Folder_::UnloadAssets()
{
    if (m_BlockId >= 0)
    {
        _31 = 0;
        return;
    }

    FixDanglingAssets(&m_AssetBlockInfo->m_AssetInfo_Shared);
    FixDanglingAssets(&m_AssetBlockInfo->m_AssetInfo_Localised);

    FrameBasedSubAllocator* allocator = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType(m_BlockId)];
    if (strcmp(allocator->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
        allocator->RemoveLast();

    _31 = 0;
}

void Folder_::LoadAssetBlock()
{
    char fragmentPlatformPath[512] = {};
    String folderFragmentName;
    String assetSharedPath, assetLocalisedPath;
    const char* const sceneFragmentName = Scene::SceneInstance->GetFragment();
    const Asset::BlockTypeNumber blockType = (Asset::BlockTypeNumber)(m_BlockId);

    g_AssetManager->GetPlatformSpecificPath(fragmentPlatformPath, sceneFragmentName, nullptr, AssetManager::PlatformId::PC);
    strcat(fragmentPlatformPath, "/");

    FileBuffer::ExtractFileName(folderFragmentName, m_Fragment->m_Name);
    strcat(fragmentPlatformPath, folderFragmentName.m_Str);

    GetResourcePathRelative(assetSharedPath, fragmentPlatformPath, blockType, nullptr);
    GetResourcePathRelative(assetLocalisedPath, fragmentPlatformPath, blockType, Script::GetCurrentCountryCode());

    if (!g_AssetManager->m_LoadBlocks || Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_STOP)
        return;

    FileBuffer::FindFileEverywhere(assetSharedPath.m_Str, 0);

    FrameBasedSubAllocator* allocator = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType(blockType)];
    if (strcmp(allocator->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
        allocator->MakeNew();

    ReadAssetBlockFile(&m_AssetBlockInfo->m_AssetInfo_Shared, assetSharedPath.m_Str);

    FileBuffer::FindFileEverywhere(assetLocalisedPath.m_Str, 0);

    ReadAssetBlockFile(&m_AssetBlockInfo->m_AssetInfo_Localised, assetLocalisedPath.m_Str);

    _31 = 1;
    AssetManager::_A3D7C0 = false;
}

void Folder_::LoadFragment()
{
    FrameBasedSubAllocator* allocator = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType(m_BlockId)];
    if (strcmp(allocator->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
        allocator->MakeNew();

    m_Flags.HasFragment = true;

    Node* parent = this;
    do
    {
        parent = parent->m_Parent;
    } while (!parent->GetFragment());

    if (m_Fragment)
    {
        g_AssetManager->AddLoadedAssetName((const char*)ALIGN_4BYTES((unsigned int)(parent->m_Fragment->m_FragmentRes.m_AssetPtr->m_ResourcePath)));
        m_Fragment->LoadResourceFile(m_Fragment->m_Name);

        g_AssetManager->m_LoadedAssetsNames.pop_back();
        delete[] m_Fragment->m_Name;

        m_Fragment->ApplyFragment();

        InstantiateAllChildren();
    }
}

Node* Folder_::_87E640() const
{
    if (this == Scene::SceneInstance)
        return nullptr;

    const int32_t goodBlockId = _A11B84[m_BlockId];
    if (!goodBlockId)
        return nullptr;

    Node* child = m_Parent->m_FirstChild;
    if (!child)
        return nullptr;

    while (true)
    {
        EntityType* entityScript = child->m_ScriptEntity;
        if (entityScript)
        {
            while (tFolder != entityScript)
            {
                entityScript = entityScript->Parent;
                if (!entityScript)
                    break;
            }

            if (entityScript)
                if (((Folder_*)child)->m_BlockId == goodBlockId)
                    return child;
        }

        child = child->m_NextSibling;
        if (!child)
            return nullptr;
    }
}

void Folder_::Register()
{
    tFolder = new EntityType("Folder");
    tFolder->InheritFrom(tNode);
    tFolder->SetCreator((CREATOR)Create);

    tFolder->RegisterProperty(tINTEGER, "block_id", (EntityGetterFunction)&GetBlockId, (EntitySetterFunction)&SetBlockId, "control=dropdown|All=0|Map=1|Submap=2|Mission=3|Cutscene=4|Playerdata=5");

    tFolder->PropagateProperties();
}

Folder_* Folder_::Create()
{
    return new Folder_();
}