#include "Folder.h"
#include "File.h"
#include "IntegerType.h"
#include "AssetManager.h"
#include "Scene.h"
#include "FrameBasedSubAllocator.h"
#include "Node.h"

EntityType* tFolder;

int Folder_::CurrentBlockId = -1;

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

void Folder_::ReadAssetBlockFile(AssetInfo& assinfo, const char* const assfname) const
{
    FileBuffer assfile(assfname, 161, true);
    assinfo.m_AssetInfo_Shared.m_ResourceDataBufferPtr = g_AssetManager->LoadResourceBlock(&assfile, (int*)assinfo.m_AssetInfo_Shared.m_ResourceAllocatedAlignedBufferPtr, &assinfo.m_AssetInfo_Shared.m_ResourceDataBufferSize, (m_BlockId * 8) >> 3);

    if (assinfo.m_AssetInfo_Shared.m_ResourceDataBufferPtr)
        LogDump::LogA("Read asset block file: %s\n", assfname);
    else
        LogDump::LogA("Could not read the asset block file; probably due to checksums: %s\n", assfname);
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

    FrameBasedSubAllocator* framealloc = (FrameBasedSubAllocator*)MemoryManager::AllocatorsList[Asset::AllocatorIndexByBlockType((8 * m_BlockId) >> 3)];
    if (Scene::SceneInstance->m_PlayMode != 1)
    {
        LogDump::LogA("*** checking for dangling nodes ***\n");

        while (true)
        {
            Node* loadednode = (Node*)g_AssetManager->FindFirstEntity();
            if (!loadednode)
                break;

            while ( loadednode->m_Id.BlockId - 1 != ((8 * m_BlockId) >> 3))
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

#pragma message(TODO_IMPLEMENTATION)
void Folder_::LoadAssetBlock()
{
    const Scene* sceneNode = Scene::SceneInstance;
    char fragmentPath[512] = {};
    String folderFragmentName;
    const char* const languageCode = Script::GetCurrentCountryCode();

    g_AssetManager->GetPlatformSpecificPath(fragmentPath, sceneNode->GetFragment(), nullptr, AssetManager::PlatformId::PC);
    strcat(fragmentPath, "/");

    FileBuffer::ExtractFileName(folderFragmentName, m_Fragment->m_Name);
    strcat(fragmentPath, folderFragmentName.m_Str);

    String folderFragmentPath;
    //  TODO: at offset 0x50 the 3 lowest bits are block type index, but different code parts reference it in a different way.
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