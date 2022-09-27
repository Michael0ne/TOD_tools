#include "AssetManager.h"
#include "Globals.h"
#include "File.h"
#include "LogDump.h"
#include "Timer.h"
#include "Progress.h"
#include "ScriptDatabase.h"
#include "GfxInternal.h"
#include "Scene.h"
#include "Model.h"

AssetManager* g_AssetManager;
bool AssetManager::ChecksumChecked;
bool AssetManager::_A3D7C0;

void AssetManager::CorrectTextureResourcePath(String& outPath, const char* respath, RegionCode region, PlatformId platform)
{
    const char* const pcplatformdir = strstr(respath, "pc_lores");
    if (!platform)
    {
        if (!pcplatformdir)
        {
            outPath = respath;
            return;
        }

        char pathfinal[128] = {};
;
        strncpy(pathfinal, respath, pcplatformdir - respath);
        strcat(pathfinal, "pc_lores");
        strcat(pathfinal, &respath[pcplatformdir - respath + 8]);

        outPath = pathfinal;
        return;
    }

    if (platform != PlatformId::PS2)
    {
        if (platform != PlatformId::XBOX)
        {
            outPath = respath;
            return;
        }

        if (!pcplatformdir)
        {
            outPath = respath;
            return;
        }

        char pathfinal[128] = {};

        strncpy(pathfinal, respath, pcplatformdir - respath);
        
        if (!region)
        {
            strcat(pathfinal, "xbox_ntsc");
            strcat(pathfinal, &respath[pcplatformdir - respath + 8]);

            outPath = pathfinal;
            return;
        }

        if (region != REGION_USA)
        {
            outPath = pathfinal;
            return;
        }

        strcat(pathfinal, "xbox_pal");
        strcat(pathfinal, &respath[pcplatformdir - respath + 8]);

        outPath = pathfinal;
        return;
    }

    if (!pcplatformdir)
    {
        outPath = respath;
        return;
    }

    const char* const pathsuffix = &respath[pcplatformdir - respath + 8];
    char pathprefix[128] = {};

    if (region)
    {
        if (region != REGION_USA)
        {
            // TODO: check if this is correct.
            outPath = respath;
            outPath.Append(pathsuffix);
            return;
        }

        strncpy(pathprefix, respath, pcplatformdir - respath);
        outPath = pathprefix;
        outPath.Append("ps2_pal");
        outPath.Append(pathsuffix);
        return;
    }

    strncpy(pathprefix, respath, pcplatformdir - respath);
    outPath = pathprefix;
    outPath.Append("ps2_ntsc");
    outPath.Append(pathsuffix);
}

AssetManager::RegionCode AssetManager::RegionIdByName(const String& region)
{
    if (region.Equal("europe"))
        return REGION_EUROPE;
    else
        if (region.Equal("usa"))
            return REGION_USA;
        else
            if (region.Equal("asia"))
                return REGION_ASIA;
            else
                return REGION_NOT_SET;
}

#pragma message(TODO_IMPLEMENTATION)
void AssetManager::GetResourcePath(String& outStr, const char* path) const
{
    if (!path || !*path)
        return;

    String path_;
    if (m_LoadedAssetsNames.size())
        path_ = m_LoadedAssetsNames[m_LoadedAssetsNames.size() - 1];

    // TODO: what this does exactly?
    outStr = path;
}

void AssetManager::IncreaseResourceReferenceCount(Asset* _res)
{
    ++_res->m_Flags.ReferenceCount;
}

void AssetManager::DecreaseResourceReferenceCount(Asset* _res)
{
    --_res->m_Flags.ReferenceCount;
}

const char* AssetManager::GetCurrentSceneName() const
{
    return m_LoadedAssetsNames.size() ? m_LoadedAssetsNames.end()->m_Str : nullptr;
}

void AssetManager::BuildFastFindNodeVector()
{
    FastFindInfo ffi;
    FillFastFindNodeVector(Scene::SceneInstance, &ffi);
    m_FastFindNodeVector.push_back(ffi);
}

void AssetManager::DeleteFastFindNodeVector()
{
    m_FastFindNodeVector.clear();
}

void AssetManager::FillFastFindNodeVector(Node* baseNode, FastFindInfo* ffi)
{
    Node* child = baseNode->m_FirstChild;
    while (child)
    {
        String childName(child->GetName());
        if (childName.Empty())
        {
            EntityType* scriptEntity = child->m_ScriptEntity;

            while (tModel != scriptEntity)
            {
                scriptEntity = scriptEntity->m_Parent;
                if (!scriptEntity)
                {
                    break;
                }
            }

            if (!scriptEntity || !((Model*)child)->m_ModelRes)
            {
                if (child->m_FirstChild)
                    FillFastFindNodeVector(baseNode, ffi);

                child = child->m_NextSibling;
                continue;
            }

            ModelAsset* modelAsset = ((Model*)child)->m_ModelRes.GetAsset<ModelAsset>();
            FileBuffer::ExtractFileName(childName, modelAsset->GetName());
        }
        else
        {
            childName.ToLowerCase();
        }

        const int32_t nameChecksum = Utils::CalcCRC32(childName.m_Str, childName.m_Length);

        FastFindInfo ffiTemp;
        ffiTemp.m_Index = ffi->m_Index;
        ffiTemp.m_Node = child;
        ffiTemp.m_NodeNameCRC = nameChecksum;

        m_FastFindNodeVector.push_back(ffiTemp);

        ffi->m_Index++;

        if (child->m_FirstChild)
            FillFastFindNodeVector(child, ffi);

        child = child->m_NextSibling;
    }
}

AllocatorIndex AssetManager::GetAllocatorType() const
{
    if (m_LoadBlocks && m_ActiveBlockId >= NULL)
        return Asset::AllocatorIndexByBlockType(g_AssetManager->m_ActiveBlockId);
    else
        return DEFAULT;
}

int AssetManager::AddAssetReference(Asset* a)
{
    m_HasDanglingEntities = 1;

    if (m_ResourcesInstancesList.size() <= 1)
    {
        unsigned int _ind = m_ResourcesInstancesList.size();
        AddTypesListItemAtPos(a, _ind);

        return _ind;
    }

    unsigned int ind = 0;
    for (unsigned int i = 1; i < m_ResourcesInstancesList.size(); i++)
        if (!m_ResourcesInstancesList[i])
            ind = i;

    if (!ind)
        ind = m_ResourcesInstancesList.size();

    AddTypesListItemAtPos(a, ind);

    return ind;
}

#ifdef INCLUDE_FIXES
void AssetManager::GetPlatformSpecificPath(char* outStr, const char* respath, const char* resext, PlatformId platform)
#else
void AssetManager::GetPlatformSpecificPath(String& outStr, const char* respath, const char* resext, PlatformId platform)
#endif
{
    char buff[1024] = {};

    switch (platform)
    {
    case PlatformId::PC:
        strcpy(buff, "/data_pc");
        break;
    case PlatformId::PS2:
        strcpy(buff, "/data_ps2");
        break;
    case PlatformId::XBOX:
        strcpy(buff, "/data_xbox");
        break;
    }

    String respath_str;
    GetResourcePath(respath_str, respath);
    char res_dir[1024] = {};
    char res_name[128] = {};
    char res_ext[16] = {};
    FileBuffer::ExtractFilePath(respath_str.m_Str, res_dir, res_name, res_ext);

    strcat(buff, res_dir + 5);
    strcat(buff, res_name);

    if (resext && strcmp(resext, ""))
    {
        strcat(buff, ".");
        strcat(buff, resext);
    }

#ifdef INCLUDE_FIXES
    strcpy(outStr, buff);
#else
    outStr = buff;
#endif
}

const char* AssetManager::GetResourcePathSceneRelative(const char* const path)
{
    const String& scenename = m_LoadedAssetsNames.back();
    if (strncmp(path, scenename.m_Str, scenename.m_Length))
        if (strstr(scenename.m_Str, "/data/") &&
            strstr(path, "/data/"))
            return path + 5;
        else
            return path;
    else
        return &path[scenename.m_Length];
}

AssetHeaderStruct_t::Header_t::Header_t()
{
    field_20 = 0x13579BDF;
    field_24 = 0x2468ACE0;
    field_28 = 0xFDB97531;
    field_2C = 0x80000062;
    field_30 = 0x40000020;
    field_34 = 0x10000002;
    field_38 = 0x7FFFFFFF;
    field_3C = 0x3FFFFFFF;
    field_40 = 0xFFFFFFF;
    field_44 = 0x80000000;
    field_48 = 0xC0000000;
    field_4C = 0xF0000000;

    memset(m_OriginalKey, NULL, sizeof(m_OriginalKey));
}

void AssetHeaderStruct_t::Header_t::DecodeFingerprintKey(char* key, char* keydata)
{
    _4010C0(key);

    for (unsigned int i = 0; i < strlen(keydata); i++)
    {
        *key = keydata[i];
        _4011A0(key);
        keydata[i] = *key;
    }
}

void AssetHeaderStruct_t::Header_t::_4010C0(const char* key)
{
    strcpy(m_OriginalKey, key);

    field_20 = key[3] | ((key[2] | ((key[1] | (0x0000 | (key[0] << 8))) << 8)) << 8);
    if (!field_20)
        field_20 = 0x13579BDF;

    field_24 = key[7] | ((key[6] | ((key[5] | (key[4] << 8)) << 8)) << 8);
    if (!field_24)
        field_24 = 0x2468ACE0;

    field_28 = key[11] | ((key[10] | ((key[9] | (key[8] << 8)) << 8)) << 8);
    if (!field_28)
        field_28 = 0xFDB97531;
}

void AssetHeaderStruct_t::Header_t::_4011A0(char* key)
{
    // NOTE: this routine reverses bytes
    unsigned int rounds = 2;
    unsigned char v3 = field_24 & 1,
        v7 = 0,
        v9 = 0,
        v12 = 0,
        v17 = 0,
        v19 = 0,
        v20 = 0,
        v21 = 0,
        v22 = 0,
        v23 = field_24 & 1,
        v24 = field_28 & 1,
        fld20 = field_20;

    do
    {
        if (fld20 & 1)
        {
            v7 = field_44 | ((fld20 ^ field_2C) >> 1);
            if (field_24 & 1)
            {
                v3 = v23 = 1;
                field_24 = field_48 | ((field_24 ^ field_30) >> 1);
            }
            else
            {
                v3 = v23 = 0;
                field_24 = field_3C & (field_24 >> 1);
            }
        }
        else
        {
            v7 = field_38 & (fld20 >> 1);
            if (field_28 & 1)
            {
                v24 = 1;
                field_28 = field_4C | ((field_28 ^ field_34) >> 1);
            }
            else
            {
                v24 = 0;
                field_28 = field_40 & (field_28 >> 1);
            }
        }

        v20 = (2 * v19) | v24 ^ v3;
        if (v7 & 1)
        {
            v9 = field_44 | ((v7 ^ field_2C) >> 1);
            if (field_24 & 1)
            {
                v23 = 1;
                field_24 = field_48 | ((field_24 ^ field_30) >> 1);
            }
            else
            {
                v23 = 0;
                field_24 = field_3C & (field_24 >> 1);
            }
        }
        else
        {
            v9 = field_38 & (v7 >> 1);
            if (field_28 & 1)
            {
                v24 = 1;
                field_28 = field_4C | ((field_28 ^ field_34) >> 1);
            }
            else
            {
                v24 = 0;
                field_28 = field_40 & (field_28 >> 1);
            }
        }

        v21 = (2 * v20) | v24 ^ v23;
        if (v9 & 1)
        {
            v12 = field_44 | ((v9 ^ field_2C) >> 1);
            if (field_24 & 1)
            {
                v23 = 1;
                field_24 = field_48 | ((field_24 ^ field_30) >> 1);
            }
        }
        else
        {
            v12 = field_38 & (v9 >> 1);
            if (field_28 & 1)
            {
                v24 = 1;
                field_28 = field_4C | ((field_28 ^ field_34) >> 1);
            }
            else
            {
                v24 = 0;
                field_28 = field_40 & (field_28 >> 1);
            }
        }

        v22 = (2 * v21) | v24 & v23;
        if (v12 & 1)
        {
            fld20 = field_44 | ((v12 ^ field_2C) >> 1);
            if (field_24 & 1)
            {
                v23 = 1;
                field_24 = field_48 | ((field_24 ^ field_30) >> 1);
            }
            else
            {
                v23 = 0;
                field_24 = field_3C & (field_24 >> 1);
            }
        }
        else
        {
            fld20 = field_38 & (v12 >> 1);
            if (field_28 & 1)
            {
                v24 = 1;
                field_28 = field_4C | ((field_28 ^ field_34) >> 1);
            }
            else
            {
                v24 = 0;
                field_28 = field_40 & (field_28 >> 1);
            }
        }

        v19 = v17 = (2 * v22) | v24 ^ v23;
        v3 = v23;

    } while (--rounds);

    field_20 = fld20;
    *key ^= v17;
    if (v17 == *key)
        *key = v17;
}

#pragma message(TODO_IMPLEMENTATION)
Asset* AssetManager::LoadResourceFile(const char* const respath)
{
    const size_t respathlen = strlen(respath);
#ifdef INCLUDE_FIXES
    if (!respath || !respath[0] || !respathlen)
#else
    if (!respathlen)
#endif
        return nullptr;

    AssetInstance* assinst = AssetInstance::GetAssetInstanceByName(respath);
    if (_A3D7C0 && assinst == TextureAsset::Instance)
    {
        TextureAsset* texass = (TextureAsset*)TextureAsset::Instance->m_Creator();
        if (texass->SetResourcePlaceholder())
        {
            texass->m_ResourceTimestamp = NULL;
            texass->m_Flags.HasPlaceHolder = true;
        }

        texass->SetResourcePath(respath);
        return texass;
    }

    String resdir;
    assinst->SetResourcePathAndGetResourcesDir(resdir, respath, Asset::PlatformId::PC);

    if (resdir.Empty())
    {
        Asset* asset = assinst->m_Creator();
        asset->SetResourcePath(respath);
        LoadOriginalVersion(asset);

        return asset;
    }

    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void* AssetManager::LoadResourceBlock(FileBuffer* file, int* resbufferptr, unsigned int* resdatasize, unsigned int resblockid)
{
    LogDump::LogA("Loading resource block with ID=%i...\n", resblockid);
    unsigned int timeStart = Timer::GetMilliseconds();

    if (resblockid == 1)
    {
        AssetHeaderStruct_t assetHeaderStruct;

        //  NOTE: ID 1 used for 'map/submap'.
        //      For some reason, these have 'fingerprint' stuff before header. Engine doesn't use them anyway, maybe some editor leftover stuff?
        char keydatabuf[256] = {};
        char filekey[56] = {};

        //  NOTE: this part is ignored.
        for (unsigned int i = 0; i < 36; i++)
            filekey[i] = file->ReadBlock();

        //  NOTE: this reads 255 bytes, but next function will only read this buffer until the point null terminator is found.
        for (unsigned int i = 0; i < 255; i++)
            keydatabuf[i] = file->ReadBlock();

        assetHeaderStruct.field_38.DecodeFingerprintKey("1E564E3B-D243-4ec5-AFB7", keydatabuf);
        strcpy(m_FingerprintKey, keydatabuf);

        int dummy = NULL;
        file->Read(&dummy, sizeof(dummy));
        file->Read(&field_108, sizeof(field_108));
    }

    unsigned int assettimestamp = NULL;
    file->Read(&assettimestamp, sizeof(assettimestamp));

    if (m_CheckTimestamp && assettimestamp != m_EngineVersionTimestamp)
    {
        LogDump::LogA("Time stamp of engine changed. Ignoring asset block.\n");
        return nullptr;
    }

    if (ChecksumChecked)
    {
        unsigned int dummy = NULL;
        file->Read(&dummy, sizeof(dummy));
        file->Read(&dummy, sizeof(dummy));
    }
    else
    {
        unsigned int checksum = NULL;
        file->Read(&checksum, sizeof(checksum));

        if (GetGlobalPropertyListChecksum() != checksum)
#ifndef INCLUDE_FIXES
            return nullptr;
#else
            LogDump::LogA("Properties checksum mismatch! Asset has %x, engine has %x\n", checksum, GlobalPropertyListChecksum);
#endif

        file->Read(&checksum, sizeof(checksum));
        if (GetGlobalCommandListChecksum() != checksum)
#ifndef INCLUDE_FIXES 
            return nullptr;
#else
            LogDump::LogA("Commands checksum mismatch! Asset has %x, engine has %x\n", checksum, GlobalCommandListChecksum);
#endif

        ChecksumChecked = true;
    }

    unsigned int totalResources = NULL;
    unsigned int resourcesInfoSize = NULL;
    unsigned int maximumAssetSize = NULL;
    char*   resourcesInfoBuffer = nullptr;
    char*   resourceDataBuffer = nullptr;

    file->Read(&totalResources, sizeof(totalResources));
    file->Read(&resourcesInfoSize, sizeof(resourcesInfoSize));
    file->Read(&maximumAssetSize, sizeof(maximumAssetSize));

    *resdatasize = resourcesInfoSize;
    Asset::AllocateResourceForBlockLoad(resourcesInfoSize, (int**)&resourcesInfoBuffer, resbufferptr, resblockid);
    //resourceDataBuffer = (char*)MemoryManager::AllocateByType(RENDERLIST, maximumAssetSize);    //  TODO: uncomment, once BestFit allocator is done.
    resourceDataBuffer = (char*)MemoryManager::AllocateByType(DEFAULT, maximumAssetSize);
    g_Progress->UpdateProgressTime(NULL, __rdtsc());

    file->SetPosAligned(0);
    file->Read(resourcesInfoBuffer, resourcesInfoSize);

    std::vector<Asset*> ResList;

    if (totalResources > 0)
        ResList.resize(totalResources);

    time_t fileTimestamp = FileBuffer::GetFileTimestamp(file->GetFileName());
    int* resDataSizeTable = new int[totalResources];

    file->SetPosAligned(0);
    file->Read(resDataSizeTable, totalResources * sizeof(int32_t));

    if (totalResources > 0)
    {
        auto it = ResList.begin();
        for (unsigned int i = totalResources, j = 0; i; i--, j++)
        {
            Timer::GetMilliseconds();
            g_Progress->UpdateProgressTime(NULL, __rdtsc());

            const uint32_t assetSize = resDataSizeTable[j];
            if (assetSize > 0)
            {
                file->SetPosAligned(0);
                file->Read(resourceDataBuffer, assetSize);
            }

            CompiledAssetInfo compasset(CompiledAssetInfo::AssetType::THREE, resourcesInfoBuffer, resourceDataBuffer, 0, 2, -1);
            Asset::Instantiate(&compasset, (Asset*)resourcesInfoBuffer);    //  TODO: this needs more reversing so as below calls.

            Asset* currasset = (Asset*)resourcesInfoBuffer;
            *it = currasset;

            CompiledAssetInfo readyasset(CompiledAssetInfo::AssetType::ZERO, nullptr, nullptr, 0, 2, -1);
            currasset->ApplyAssetData(&readyasset);

            resourcesInfoBuffer += readyasset.GetAssetSize() + AssetInstance::AssetAlignment[0] - 1 & (~(AssetInstance::AssetAlignment[0] - 1));

            currasset->m_ResourceTimestamp = fileTimestamp;
            currasset->m_Flags.ReferenceCount = -1;

            it++;
        }
    }

    delete[] resDataSizeTable;

    if (totalResources > 0)
    {
        m_AssetsList.resize(totalResources);

        for (unsigned int i = 0; i < totalResources; ++i)
            m_AssetsList[i] = ResList[i];
    }

    LogDump::LogA("Done. Loading %d resource took %.2f secs.\n", totalResources, (Timer::GetMilliseconds() - timeStart) * 0.001f);
    MemoryManager::ReleaseMemory(resourceDataBuffer, false);

    return resourcesInfoBuffer;
}

Entity* AssetManager::FindFirstEntity()
{
    unsigned int nodeid = FindNodeById(0x100000);
    if (nodeid)
        return (m_NodesList[(nodeid >> 20) & 7][nodeid & 0xFF8FFFFF]);
    else
        return nullptr;
}

Entity* AssetManager::FindNextEntity(Entity* node)
{
    unsigned int nodeid = FindNodeById(node->m_Id.Id >> 8);
    if (nodeid)
        return (m_NodesList[(nodeid >> 20) & 7][nodeid & 0xFF8FFFFF]);
    else
        return nullptr;
}

void AssetManager::DestroySceneNodesFrameBuffers(const int)
{
    g_GfxInternal->Render(nullptr, false, -1, -1);
    MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->stub19();
    MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->GetTotalAllocations();

    if (Scene::SceneInstance)
    {
        for (Node* child = Scene::SceneInstance->m_FirstChild; child; child = child->m_NextSibling)
        {
            child->DestroyFrameBuffers();
            child->DestroyChildren();
        }
    }

    int alloc = MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->stub19();
    MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->GetTotalAllocations();

    if (alloc > 4 && Scene::SceneInstance)
        Scene::SceneInstance->ProfileMemory(0);

    g_GfxInternal->SetRenderBufferIsEmpty(false);
}

AssetManager::RegionCode AssetManager::GetRegion() const
{
    return m_RegionId;
}

String& AssetManager::GetDataPath(String& outstr) const
{
    outstr = "/data/";
    return outstr;
}

Asset* AssetManager::FindFirstLoadedAsset() const
{
    if (m_ResourcesInstancesList.size() <= 1)
        return nullptr;

    for (unsigned int i = 1; i < m_ResourcesInstancesList.size(); ++i)
        if (m_ResourcesInstancesList[i])
            return m_ResourcesInstancesList[i];

    return nullptr;
}

void AssetManager::DestroyTextureAsset(TextureAsset& ass)
{
    ass.DestroyResource();
    m_HasDanglingEntities = 1;
}

Asset* AssetManager::GetNextLoadedAsset(const Asset* a) const
{
    unsigned int i = a->m_GlobalResourceId + 1;
    if (i >= m_ResourcesInstancesList.size())
        return m_ResourcesInstancesList[0];

    for (Asset* ass = m_ResourcesInstancesList[i]; !ass; i++);

    if (i + 1 >= m_ResourcesInstancesList.size())
        return m_ResourcesInstancesList[0];
    else
        return m_ResourcesInstancesList[i];
}

#pragma message(TODO_IMPLEMENTATION)
void AssetManager::_878030()
{
    if (!m_LoadBlocks)
        return;

    Asset* loadedAsset = FindFirstLoadedAsset();
    if (loadedAsset)
    {
        do
        {
            const char* assetPath = (const char*)ALIGN_4BYTES(loadedAsset->m_ResourcePath);
            String loadedAssetName(!assetPath ? nullptr : assetPath);
            loadedAssetName.ToLowerCase();

            const int32_t loadedAssetNameChecksum = Utils::CalcCRC32(loadedAssetName.m_Str, loadedAssetName.m_Length);
            AssetInfo assetInfoTemp;
            assetInfoTemp.m_AssetNameCRC = loadedAssetNameChecksum;
            assetInfoTemp.m_Asset = loadedAsset;

            m_DefragmentatorList.push_back(assetInfoTemp);

            uint32_t nextAssetIndex = loadedAsset->m_GlobalResourceId + 1;
            if (nextAssetIndex >= m_ResourcesInstancesList.size())
            {
                nextAssetIndex = 0;
            }
            else
            {
                Asset** freeAssetPtr = &m_ResourcesInstancesList[nextAssetIndex];
                while (!*freeAssetPtr)
                {
                    nextAssetIndex++;
                    freeAssetPtr++;

                    if (nextAssetIndex >= m_ResourcesInstancesList.size())
                    {
                        nextAssetIndex = 0;
                        break;
                    }
                }
            }

            loadedAsset = m_ResourcesInstancesList[nextAssetIndex];
        } while (loadedAsset);
    }

    //  TODO: another call here, with unknown purpose for now.
}

void AssetManager::MakeSpaceForAssetsList()
{
    if (!m_LoadBlocks)
        return;

    m_AssetsList.reserve(1);
}

#pragma message(TODO_IMPLEMENTATION)
bool AssetManager::LoadOriginalVersion(Asset * asset)
{    
    char assetdir[1024] = {};
    char assetfilename[256] = {};
    char assetext[16] = {};

    FileBuffer::ExtractFilePath(asset->m_ResourcePath, assetdir, assetfilename, assetext);

    if (assetext && strcmp(assetext, "stream") == NULL)
        strcpy(strstr(asset->m_ResourcePath, "stream"), "wav");

    if (!asset->m_ResourcePath || !*asset->m_ResourcePath)
        return false;

    if (asset->m_ResourceTimestamp > NULL)
    {
        String resdir;
        asset->GetResourcesDir(resdir, Asset::PlatformId::PC);

        if (!resdir.Empty())
            return false;

        if (FileBuffer::FindFileEverywhere(asset->m_ResourcePath, 0))
            if (asset->GetResourceCountryCode() != Script::GetCurrentCountryCode())
                return false;
    }

    return false;
}

Asset* AssetManager::FindLoadedAsset(const char* const assetname)
{
    char* assnamelowered = (char*)assetname;
    String::ToLowerCase(assnamelowered);

    if (m_DefragmentatorList.size())
    {
        const unsigned int assnamechecksum = Utils::CalcCRC32(assnamelowered, strlen(assnamelowered));

        std::vector<AssetInfo>::iterator assresult = std::find(m_DefragmentatorList.begin(), m_DefragmentatorList.end(), AssetInfo{ assnamechecksum, nullptr });
        if (assresult != m_DefragmentatorList.end() && assnamechecksum == assresult->m_AssetNameCRC && strcmp(assnamelowered, assresult->m_Asset->m_ResourcePath) == NULL)
            return assresult->m_Asset;
        else
            return nullptr;
    }

    Asset* freeass = FindFirstLoadedAsset();
    if (!freeass)
        return nullptr;

    if (strcmp(assnamelowered, freeass->m_ResourcePath) == NULL)
        return freeass;

    unsigned int resind = freeass->m_GlobalResourceId + 1;
    if (resind >= m_ResourcesInstancesList.size())
        resind = 0;
    else
        for (; resind < m_ResourcesInstancesList.size(); ++resind)
            if (!m_ResourcesInstancesList[resind])
                break;

    return m_ResourcesInstancesList[resind];
}

void AssetManager::InstantiateAssetsAndClearAssetsList()
{
    for (unsigned int i = 0; i < m_AssetsList.size(); ++i)
    {
        CompiledAssetInfo cmpassinf(CompiledAssetInfo::AssetType::FOUR, (char*)m_AssetsList[i], nullptr, NULL, NULL, -1);
        Asset::Instantiate(&cmpassinf, m_AssetsList[i]);
    }

    m_AssetsList.clear();
}

Node* AssetManager::FindEntityById(const int id)
{
    const Entity::EntityId idValue = *(const Entity::EntityId*)&id;

    int blockId = idValue.BlockId;
    if (blockId == 0)
        if (m_ActiveBlockId == -1)
            blockId = 0;
        else
            blockId = m_ActiveBlockId;
    else
        blockId -= 1;

    int foundBlockId = -1;
    unsigned int foundEntIndex = 0;
    if (m_EntityIdsMap)
    {
        auto& foundEntityRef = m_EntityIdsMap->find(id);
        if (foundEntityRef != m_EntityIdsMap->end())
        {
            //  TODO: rewrite.
            unsigned int foundEntIndex = foundEntityRef->first & 0xFF8FFFFF;
            int foundBlockId = ((foundEntityRef->first >> 20) & 7) - 1;
            auto& entList = m_NodesList[foundBlockId];

            if (foundEntIndex <= 0 || foundEntIndex >= entList.size())
                return 0;
            else
                return (Node*)entList[foundEntIndex];
        }
    }

    foundEntIndex = id & 0xFF8FFFFF;
    foundBlockId = ((id >> 20) & 7) - 1;
    auto& entList = m_NodesList[foundBlockId];

    if (foundEntIndex <= 0 || foundEntIndex >= entList.size())
        return 0;
    else
        return (Node*)entList[foundEntIndex];
}

#pragma message(TODO_IMPLEMENTATION)
void AssetManager::_8794B0(const char* const respath)
{
    DestroyDanglingAssets(nullptr, nullptr);

    String resourcePath(respath);
    resourcePath.ToLowerCase();

    for (unsigned int i = 0; i < 3; ++i)
    {
        if (m_ResourcesInstancesList.size() <= 1)
            return;

        unsigned int occupiedAssetIndex = 1;
        while (!m_ResourcesInstancesList[occupiedAssetIndex++])
            if (occupiedAssetIndex >= m_ResourcesInstancesList.size())
                return;

        if (!occupiedAssetIndex || !m_ResourcesInstancesList[occupiedAssetIndex])
            return;

        bool assetLoaded = false;
        do
        {
            if (!resourcePath.Empty())
            {
                String currentResourcePath(m_ResourcesInstancesList[occupiedAssetIndex]->m_ResourcePath);
                currentResourcePath.ToLowerCase();

                if (!strstr(currentResourcePath.m_Str, resourcePath.m_Str))
                {
                    if ((size_t)(m_ResourcesInstancesList[occupiedAssetIndex]->m_GlobalResourceId + 1) < m_ResourcesInstancesList.size())
                    {
                        while (!m_ResourcesInstancesList[occupiedAssetIndex++])
                            if (occupiedAssetIndex >= m_ResourcesInstancesList.size())
                                occupiedAssetIndex = 0;
                    }
                    else
                        occupiedAssetIndex = 0;

                    continue;
                }
            }

            if (LoadOriginalVersion(m_ResourcesInstancesList[occupiedAssetIndex]))
                assetLoaded = true;

            if ((size_t)(m_ResourcesInstancesList[occupiedAssetIndex]->m_GlobalResourceId + 1) < m_ResourcesInstancesList.size())
            {
                while (!m_ResourcesInstancesList[occupiedAssetIndex++])
                    if (occupiedAssetIndex >= m_ResourcesInstancesList.size())
                        occupiedAssetIndex = 0;
            }
            else
                occupiedAssetIndex = 0;
        } while (m_ResourcesInstancesList[occupiedAssetIndex]);

        if (!assetLoaded)
            return;
    }
}

void AssetManager::DestroyDanglingAssets(const char* const resourceBufferDataBegin, const char* const resourceBufferDataEnd)
{
    if (m_ResourcesInstancesList.size() <= 1)
        return;

    bool danglingAssetFound;
    do
    {
        danglingAssetFound = false;
        unsigned int occupiedAssetIndex = 1;
        while (!m_ResourcesInstancesList[occupiedAssetIndex])
        {
            occupiedAssetIndex++;
            if (occupiedAssetIndex >= m_ResourcesInstancesList.size())
                return;
        }

        do
        {
            Asset* assetRef = m_ResourcesInstancesList[occupiedAssetIndex];
            if (!assetRef->m_Flags.ReferenceCount &&
                (!resourceBufferDataBegin || (char*)assetRef >= resourceBufferDataBegin) &&
                (!resourceBufferDataEnd || (char*)assetRef < resourceBufferDataEnd))
            {
                assetRef->DestroyResource();
                m_HasDanglingEntities = true;
                Asset::Destroy(assetRef);
                danglingAssetFound = true;
            }

            if (++occupiedAssetIndex >= m_ResourcesInstancesList.size())
                break;

            while (!m_ResourcesInstancesList[occupiedAssetIndex])
            {
                occupiedAssetIndex++;
                if (occupiedAssetIndex >= m_ResourcesInstancesList.size())
                    break;
            }
        } while (occupiedAssetIndex);
    } while (danglingAssetFound);
}

void AssetManager::AddTypesListItemAtPos(Asset* element, unsigned int index)
{
    m_ResourcesInstancesList.insert(m_ResourcesInstancesList.begin(), index, {});
    m_ResourcesInstancesList.push_back(element);
    m_HasDanglingEntities = 1;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned int AssetManager::FindNodeById(unsigned int id)
{
    unsigned int block_id;

    while (true)
    {
        block_id = ((id >> 20) & 7) - 1;

        if (((id & 0xFF8FFFFF) + 1) < m_NodesList[block_id].size())
            break;

        if (block_id >= 6)
            return NULL;

        id = (block_id + 1) << 20;
    };

    unsigned int i = (id & 0xFF8FFFFF) + 1;
    for (; i < m_NodesList[block_id].size(); ++i);

    return i | ((block_id + 1) << 20);
}

AssetManager::AssetManager(bool loadBlocks)
{
    MESSAGE_CLASS_CREATED(AssetManager);

    m_Defragmentator = MemoryManager::GetDefragmentator(DEFRAGMENTING);
    m_LoadBlocks = loadBlocks;

    g_AssetManager = this;

    m_NodesList[0].resize(11200);
    m_NodesList[1].resize(4100);
    m_NodesList[2].resize(6000);
    m_NodesList[3].resize(2800);
    m_NodesList[5].resize(100);

    m_NodesInNodeList[0] = 1;
    m_NodesInNodeList[1] = 1;
    m_NodesInNodeList[2] = 1;
    m_NodesInNodeList[3] = 1;
    m_NodesInNodeList[4] = 1;
    m_NodesInNodeList[5] = 1;
    m_HasDanglingEntities = NULL;
    m_RegionId = REGION_NOT_SET;
    m_ResourcesInstancesList.reserve(1);
    m_FragmentMainNodeId = NULL;
    m_BlocksUnloaded = NULL;
    m_EntityIdsMap = nullptr;
    m_CheckTimestamp = false;
    m_EngineVersionTimestamp = NULL;
    m_ActiveBlockId = -1;
    field_108 = 2;
}

AssetManager::~AssetManager()
{
    MESSAGE_CLASS_DESTROYED(AssetManager);

    Asset* assetRef = FindFirstLoadedAsset();
    if (assetRef)
    {
        unsigned int assetIndex = 0;
        do
        {
            if ((size_t)(assetRef->m_GlobalResourceId + 1) < m_ResourcesInstancesList.size())
            {
                while (!m_ResourcesInstancesList[assetIndex++])
                    if (assetIndex >= m_ResourcesInstancesList.size())
                        assetIndex = 0;
            }

            if (assetRef->m_ResourceTimestamp > 0)
            {
                assetRef->DestroyResource();
                m_HasDanglingEntities = true;
            }

            Asset::Destroy(assetRef);
            assetRef = m_ResourcesInstancesList[assetIndex];
        } while (assetRef);
    }

    g_AssetManager = nullptr;

    m_AssetsList.clear();
    m_LoadedAssetsNames.clear();
    m_ResourcesInstancesList.clear();

    for (unsigned int i = 0; i < 6; i++)
        m_NodesList[i].clear();

    m_FastFindNodeVector.clear();
}

void AssetManager::AddLoadedAssetName(const char* assetName)
{
    String sceneDir;
    FileBuffer::ExtractFileDir(sceneDir, assetName);

    if (sceneDir.m_Length > 0 && sceneDir.m_Str[sceneDir.m_Length - 1] != '/')
        sceneDir.Append("/");

    m_LoadedAssetsNames.push_back(sceneDir);
}

const char* const AssetManager::GetLastLoadedAssetName() const
{
    return m_LoadedAssetsNames.size() ? m_LoadedAssetsNames[m_LoadedAssetsNames.size() - 1].m_Str : nullptr;
}

unsigned int AssetManager::FindFirstLoadedAsset(unsigned int startIndex) const
{ 
    unsigned int nextIndex = startIndex + 1;
    if (nextIndex >= m_ResourcesInstancesList.size())
        return 0;

    for (const Asset* res = m_ResourcesInstancesList[nextIndex]; !res; res++)
        if (++nextIndex >= m_ResourcesInstancesList.size())
            return 0;

    return nextIndex;
}

unsigned int AssetManager::AddEntity(Entity* ent)
{
    unsigned int listind = m_ActiveBlockId == -1 ? 0 : m_ActiveBlockId;
    unsigned int listcap;

    if (m_FragmentMainNodeId)
    {
        listcap = m_FragmentMainNodeId;
        m_FragmentMainNodeId = NULL;
    }
    else
        listcap = m_NodesInNodeList[listind];
    listcap = listcap & 0xFF8FFFFF;

    if (listcap >= m_NodesList[listind].size())
        LogDump::LogA("Warning: vEntity[%d] is grown to size %d. Please adjust reserve() calls.\n", listind, listcap);

    m_NodesList[listind].push_back(ent);
    m_NodesInNodeList[listind]++;

    return listcap | ((listind + 1) << 20);
}

void AssetManager::SetRegion(RegionCode id)
{
    m_RegionId = id;
}

CompiledAssetInfo::CompiledAssetInfo(const AssetType assettype, const char* assetinstanceinfo, const char* assetdata, const int alignment, const int a5, const int a6)
{
    MESSAGE_CLASS_CREATED(CompiledAssetInfo);

    m_AssetType = assettype;
    m_AssetSize = 0;
    field_8 = 0;
    m_Alignment = alignment;

    field_24 = (char*)assetdata;
    field_2C = (char*)assetdata;
    field_20 = (char*)assetinstanceinfo;
    field_28 = (char*)assetinstanceinfo;

    field_30 = a6;
    field_34 = a5 & 1;
    field_35 = (a5 & 2) != 0;
}

void CompiledAssetInfo::ParseInfo(const uint8_t** assetPtr, CompiledAssetInfo** assetBufferPtr, const size_t assetClassSize, const int32_t a4, const int32_t a5)
{
    if (field_30 == -1 || a5 != -1 && field_30 == a5)
    {
        if (*assetPtr)
        {
            switch (m_AssetType)
            {
            case ZERO:
                _40CB90(assetClassSize, a4, -1);
                *assetBufferPtr = (CompiledAssetInfo*)*assetPtr;
                break;
            case ONE:
                memcpy(GetDataPtr(a4), *assetPtr, assetClassSize);
                *assetBufferPtr = (CompiledAssetInfo*)GetDataPtr(a4);
                _40CBD0(assetClassSize, a4, -1);
                break;
            case TWO:
                *assetBufferPtr = (CompiledAssetInfo*)*assetPtr;
                AddAssetToList(assetPtr, a4);
                break;
            case THREE:
                _40CB20(assetPtr, a4);
                *assetBufferPtr = (CompiledAssetInfo*)*assetPtr;
                break;
            case FOUR:
                *assetBufferPtr = (CompiledAssetInfo*)*assetPtr;
                break;
            default:
                return;
            }
        }
        else
        {
            *assetBufferPtr = nullptr;
        }
    }
    else
    {
        *assetBufferPtr = (CompiledAssetInfo*)*assetPtr;
    }
}

void CompiledAssetInfo::ParseAssetData(const uint8_t** assetdataptr, int* dataptr, int flags, int a4)
{
    if (field_30 != -1 || a4 != -1 && field_30 != a4)
        return;

    switch (m_AssetType)
    {
    case ZERO:
    {
        if (field_30 != -1)
            break;

        size_t slen = ALIGN_4BYTES(*assetdataptr) ? strlen((const char*)ALIGN_4BYTES(*assetdataptr)) + 1 : NULL;
        slen = ALIGN_4BYTES(slen + 3);
        if ((flags & 2) != 0)
            field_8 += slen;
        else
            m_AssetSize += slen;
        break;
    }
    case ONE:
    {
        if (!ALIGN_4BYTES(*assetdataptr))
            break;

        size_t slen = ALIGN_4BYTES((strlen((const char*)ALIGN_4BYTES(*assetdataptr)) + 4));
        *dataptr = ALIGN_4BYTES(*dataptr);

        memcpy(GetDataPtr(flags), (const void*)ALIGN_4BYTES(*assetdataptr), slen);
        *dataptr = ALIGN_4BYTES(*dataptr);
        *dataptr = (int)GetDataPtr(flags);
        *dataptr = ALIGN_4BYTES(*dataptr);

        if (field_30 == -1)
            if (flags & 2)
                field_24 += ALIGN_4BYTES(slen + 3);
            else
                field_20 += ALIGN_4BYTES(slen + 3);
        break;
    }
    case TWO:
        if (ALIGN_4BYTES(*assetdataptr))
            AddAssetToList(assetdataptr, flags);
        break;
    case THREE:
        _40CB20(assetdataptr, flags);
        *assetdataptr = (uint8_t*)ALIGN_4BYTES(*assetdataptr);
        break;
    default:
        break;
    }
}

int CompiledAssetInfo::GetAssetSize() const
{
    return m_AssetSize;
}

void CompiledAssetInfo::AlignDataOrSize(unsigned int alignment, unsigned char flags, int a3)
{
    if (field_30 == -1 || a3 != -1 && field_30 == a3)
    {
        switch (m_AssetType)
        {
        case ZERO:
            if ((a3 & 2) != 0)
                field_8 = ALIGN_4BYTES( ((~(alignment - 1) & (alignment + this->field_8 - 1)) + 3) );
            else
                m_AssetSize = ALIGN_4BYTES( ((~(alignment - 1) & (this->m_AssetSize + alignment - 1)) + 3) );
            break;
        case ONE:
            if ((a3 & 2) != 0)
                field_24 = (char*)ALIGN_4BYTES( ((~(alignment - 1) & (unsigned int)&field_24[alignment - 1]) + 3) );
            else
                field_20 = (char*)ALIGN_4BYTES( ((~(alignment - 1) & (unsigned int)&field_20[alignment - 1]) + 3) );
            break;
        default:
            break;
        }
    }
}

void CompiledAssetInfo::_40CB20(const uint8_t** dataptr, char flags) const
{
    if (flags & 1 || !*dataptr)
        return;

    if (*dataptr == (uint8_t*)0x80000000)
        *dataptr = nullptr;

    const uint8_t* readDataPtr = &(*dataptr)[(uint32_t)dataptr];
    *dataptr = readDataPtr;

    if ((flags & 2) != 0)
    {
        if ((flags & 4) == 0)
            *dataptr = &readDataPtr[field_2C - field_28];
    }
    else
        if ((flags & 4) != 0)
            *dataptr = &readDataPtr[field_28 - field_2C];
}

#pragma message(TODO_IMPLEMENTATION)
void CompiledAssetInfo::_85E160(uint8_t** dataptr, uint8_t** a2, uint32_t flags, uint32_t a4)
{
}

void CompiledAssetInfo::AddAssetToList(const uint8_t** dataptr, const int32_t flags)
{
    if (flags & 1)
        return;

    field_10.push_back({ const_cast<uint8_t**>(dataptr), flags });
}

char* CompiledAssetInfo::GetDataPtr(const int flags)
{
    return flags & 2 ? field_24 : field_20;
}

void CompiledAssetInfo::_40CB90(const uint32_t a1, const int8_t a2, const int32_t a3)
{
    if (field_30 == -1 || a3 != -1 && field_30 == a3)
    {
        if ((a2 & 2) != 0)
            field_8 += ALIGN_4BYTESUP(a1);
        else
            m_AssetSize += ALIGN_4BYTESUP(a1);
    }
}

void CompiledAssetInfo::_40CBD0(const uint32_t a1, const int8_t a2, const int32_t a3)
{
    if (field_30 == -1 || a3 != -1 && field_30 == a3)
    {
        if ((a2 & 2) != 0)
            field_24 += ALIGN_4BYTESUP(a1);
        else
            field_20 += ALIGN_4BYTESUP(a1);
    }
}