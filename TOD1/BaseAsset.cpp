#include "BaseAsset.h"
#include "AssetManager.h"
#include "ScriptDatabase.h"

std::vector<String> Asset::OpenResourcesList;
unsigned int Asset::TotalResourcesCreated;
unsigned int Asset::TextureAssetAllocatorId;
const char* const Asset::BlockTypeExtension[] = { ".", "map", "submap", "mission", "cutscene", "playerdata", "main", "" };

unsigned int AssetInstance::AssetAlignment[3];
std::vector<AssetInstance*> AssetInstance::Assets;

AssetLoader::~AssetLoader()
{
    MESSAGE_CLASS_DESTROYED(AssetLoader);

    if (m_AssetPtr)
        g_AssetManager->DecreaseResourceReferenceCount(m_AssetPtr);

    delete m_Empty;
}

AssetLoader& AssetLoader::operator=(const AssetLoader& rhs)
{
    MESSAGE_CLASS_CREATED(AssetLoader);

    m_Empty = nullptr;

    if (m_AssetPtr)
        g_AssetManager->DecreaseResourceReferenceCount(m_AssetPtr);

    m_AssetPtr = rhs.m_AssetPtr;

    if (rhs.m_AssetPtr)
        g_AssetManager->IncreaseResourceReferenceCount(rhs.m_AssetPtr);

    return *this;
}

AssetInstance* AssetInstance::GetAssetInstanceByName(const char* const asspath)
{
    if (Assets.size() <= 0)
        return nullptr;

    char* asspath_ = (char*)asspath;
    String::ToLowerCase(asspath_);
    asspath_ = strrchr(asspath_, '.') + 1;

    for (auto it = Assets.cbegin(); it != Assets.cend(); it++)
        if ((*it)->m_FileExtensions.size() <= 0)
            continue;
        else
            for (auto ext = (*it)->m_FileExtensions.cbegin(); ext != (*it)->m_FileExtensions.cend(); ext++)
                if (strcmp(ext->m_Str, asspath_) == 0)
                    return *it;

    return nullptr;
}

Asset::~Asset()
{
    MESSAGE_CLASS_DESTROYED(Asset);

    if (m_GlobalResourceId > 0)
        g_AssetManager->m_ResourcesInstancesList[m_GlobalResourceId] = nullptr;

    --TotalResourcesCreated;
    delete[] m_ResourcePath;
}

bool Asset::stub3(unsigned char a1, int, int)
{
    m_Flags._18 = a1;
    return false;
}

bool Asset::stub4() const
{
    return m_Flags._18 & 1;
}

void Asset::stub5(int)
{
    return;
}

void Asset::GetResourcesDir(String& outDir, PlatformId platformId) const
{
    outDir = "";
}

void Asset::ApplyAssetData(CompiledAssetInfo* assetInfoPtr)
{
    return;
}

char Asset::SetResourcePlaceholder()
{
    return 0;
}

int Asset::stub9() const
{
    return (m_Flags._19 != 0) + 1;
}

void Asset::GetResourceName(String& outName, int originalVersionPath)
{

    if (originalVersionPath)
        GetInfoFilePath(outName, m_ResourcePath, 0, 0);
    else
        outName = m_ResourcePath;
}

void Asset::LoadResource(const char* const resPath)
{
    return;
}

void Asset::DestroyResource()
{
    m_ResourceTimestamp = NULL;
}

Asset::Asset(bool dontmakeglobal)
{
    MESSAGE_CLASS_CREATED(Asset);

    if (!TotalResourcesCreated)
        OpenResourcesList.reserve(10);

    TotalResourcesCreated++;
    m_ResourcePath = nullptr;

    m_ResourceTimestamp = NULL;

    if (dontmakeglobal)
        m_GlobalResourceId = NULL;
    else
        m_GlobalResourceId = g_AssetManager->AddAssetReference(this);

    m_Flags.NotUsed = true;
    m_Flags.HasPlaceHolder = true;
    m_Flags._18 = true;
    m_Flags._19 = 1;
}

const char* Asset::GetName() const
{
    //  NOTE: a special list to keep last open resources is an editor leftover.
    static unsigned int LastOpenResourceIndex;
    unsigned int resind = LastOpenResourceIndex;
    LastOpenResourceIndex = (LastOpenResourceIndex + 1) % 10;
    OpenResourcesList[resind] = g_AssetManager->GetResourcePathSceneRelative(m_ResourcePath);

    return OpenResourcesList[resind].m_Str;
}

#pragma message(TODO_IMPLEMENTATION)
void Asset::GetInfoFilePath(String& outstr, const char* inpath, bool a3, bool a4) const
{
}

void Asset::Destroy(Asset* res)
{
    res->~Asset();

    if (res->m_Flags.NotUsed)
        MemoryManager::ReleaseMemory(res, false);
}

Asset* Asset::CreateInstance(size_t classsize)
{
    Asset* asset = nullptr;
    if (AssetInstance::AssetAlignment[0])
        asset = (Asset*)MemoryManager::AllocatorsList[TextureAssetAllocatorId]->AllocateAligned(classsize, AssetInstance::AssetAlignment[0], __FILE__, __LINE__);
    else
        asset = (Asset*)MemoryManager::AllocatorsList[TextureAssetAllocatorId]->Allocate(classsize, __FILE__, __LINE__);

    asset->m_Flags.NotUsed = 1;

    return asset;
}

void Asset::AllocateResourceForBlockLoad(const unsigned int size, int** bufaligned, int* buf, const unsigned int blockid)
{
    int* bufblock = (int*)MemoryManager::AllocateByType(AllocatorIndexByBlockType(blockid), AssetInstance::AssetAlignment[0] + size);

    *buf = (int32_t)bufblock;
    *bufaligned = (int*)( ~(AssetInstance::AssetAlignment[0] - 1) & ((int)bufblock + AssetInstance::AssetAlignment[0] - 1) );
}

void Asset::Instantiate(CompiledAssetInfo* assetBuffer, Asset* assetPtr)
{
    //  NOTE: first instantiate call will fill basic asset structure fields: vmt pointer, name pointer and global id.
    if (assetBuffer->m_AssetType == CompiledAssetInfo::AssetType::THREE)
        *(uint32_t*)assetPtr = (uint32_t)AssetInstance::Assets[*(uint32_t*)assetPtr]->m_ResourceTypeMethods;

    assetBuffer->ParseAssetData((const uint8_t**)((uint32_t*)assetPtr + 4), 0, 0, -1);

    if (assetBuffer->m_AssetType == CompiledAssetInfo::AssetType::THREE)
        assetPtr->m_GlobalResourceId = g_AssetManager->AddAssetReference(assetPtr);

    //  NOTE: depending on what asset this is, go further and parse rest of the data.
    assetPtr->ApplyAssetData(assetBuffer);
}

void Asset::SetReferenceCount(unsigned char count)
{
    m_Flags.ReferenceCount = count;
}

void Asset::EncodeCountryCode(const char* const countrycode)
{
    if (countrycode)
    {
        for (unsigned int i = 0; i < (sizeof(Script::CountryCodes) / sizeof(Script::CountryCodes[0])); ++i)
#ifdef INCLUDE_FIXES
            if (countrycode[0] == Script::CountryCodes[i][0] && countrycode[1] == Script::CountryCodes[i][1])
#else
            if (strcmp(countrycode, Script::CountryCodes[i]) == NULL)
#endif
            {
                m_Flags.AssetRegion = i;
                return;
            }
    }
    else
        m_Flags.AssetRegion = 6;
}

const char* const Asset::GetResourceCountryCode() const
{
    const unsigned int countrycode = m_Flags.AssetRegion;

    // NOTE: obvious fix - original code assumes that asset country code cannot be tampered with, since it's validated when loading.
#ifdef INCLUDE_FIXES
    if (countrycode < 6)
#else
    if (countrycode != 6)
#endif
        return Script::CountryCodes[countrycode];
    else
        return nullptr;
}

AssetInstance::AssetInstance(const char* const assetname, Asset* (*creatorptr)())
{
    MESSAGE_CLASS_CREATED(AssetInstance);

    m_ResourceTypeName = assetname;
    m_ResourceIndex = Assets.size();
    m_Creator = creatorptr;
    field_2C = 0;
    m_VerifyChecksum = false;
    m_Alignment[0] = m_Alignment[1] = m_Alignment[2] = 16;

    if (AssetAlignment[0] < 16)
        AssetAlignment[0] = 16;

    if (AssetAlignment[1] < 16)
        AssetAlignment[1] = 16;

    if (AssetAlignment[2] < 16)
        AssetAlignment[2] = 16;

    Assets.push_back(this);

    // NOTE: this is temporary just to get VMT pointer, so destroy after use.
    //      The custom 'new' operator for Asset will set a special flag, so the object is not free'd, but only constructor is called.
    Asset* asset = m_Creator();
    m_ResourceTypeMethods = (void*)(*(int*)asset);
    Asset::Destroy(asset);
}

void AssetInstance::SetAlignment(unsigned int size, unsigned int slot)
{
    m_Alignment[slot] = size;

    if (AssetAlignment[slot] < size)
        AssetAlignment[slot] = size;
}

void AssetInstance::SetResourcePathAndGetResourcesDir(String& outResourcesDir, const char* const resourcepath, const unsigned char platform) const
{
    Asset* asset = m_Creator();
    asset->SetResourcePath(resourcepath);
    asset->GetResourcesDir(outResourcesDir, (Asset::PlatformId)platform);
    Asset::Destroy(asset);
}

AllocatorIndex Asset::AllocatorIndexByBlockType(unsigned int blocktype)
{
    if (!blocktype ||
        !strcmp(BlockTypeExtension[blocktype], "map") ||
        !strcmp(BlockTypeExtension[blocktype], "submap"))
        return MAIN_ASSETS;

    if (!strcmp(BlockTypeExtension[blocktype], "mission"))
        return MISSION_ASSETS;

    if (!strcmp(BlockTypeExtension[blocktype], "cutscene"))
        return CUTSCENE_OR_REWIND;

    if (!strcmp(BlockTypeExtension[blocktype], "playerdata"))
        return PLAYER_DATA;

    return DEFAULT;
}

void Asset::SetResourcePath(const char* const respath)
{
    if (m_ResourcePath)
        delete[] m_ResourcePath;

    size_t respathlen = strlen(respath);
    m_ResourcePath = new char[respathlen + 1];
    strncpy((char*)m_ResourcePath, respath, respathlen);
}

void Asset::_851430(CompiledAssetInfo* assetInfoPtr, CompiledAssetInfo** assetInfoDataPtr)
{
    assetInfoPtr->ParseAssetData((const uint8_t**)&m_ResourcePath, &((*assetInfoDataPtr)->m_AssetSize), 0, -1);

    if (assetInfoPtr->m_AssetType == CompiledAssetInfo::AssetType::TWO)
    {
        m_Flags.ReferenceCount = NULL;
        m_Flags.NotUsed = true;
        m_GlobalResourceId = NULL;
        m_ResourceTimestamp = NULL;
        *(uint32_t*)this = GetInstancePtr()->m_ResourceIndex;
    }
}

void AssetLoader::LoadAssetByName(const char* const name)
{
    if (!name || !name[0])
        return;

    String respath;
    g_AssetManager->GetResourcePath(respath, name);
    m_AssetPtr = g_AssetManager->FindLoadedAsset(respath.m_Str);
    if (!m_AssetPtr)
        m_AssetPtr = g_AssetManager->LoadResourceFile(respath.m_Str);

    g_AssetManager->IncreaseResourceReferenceCount(m_AssetPtr);
}