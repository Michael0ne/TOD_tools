#include "BaseAsset.h"
#include "AssetManager.h"
#include "ScriptDatabase.h"

std::vector<String> Asset::OpenResourcesList;
unsigned int Asset::TotalResourcesCreated;
unsigned int Asset::LastOpenResourceIndex;
unsigned int Asset::TextureAssetAllocatorId;
const char* const Asset::BlockTypeExtension[] = { ".", "map", "submap", "mission", "cutscene", "playerdata", "", "" };

unsigned int AssetInstance::AssetAlignment[3];
std::vector<AssetInstance*> AssetInstance::Assets;

Asset::~Asset()
{
    MESSAGE_CLASS_DESTROYED(Asset);

    if (m_GlobalResourceId > 0)
        g_AssetManager->m_ResourcesInstancesList[m_GlobalResourceId] = nullptr;

    --TotalResourcesCreated;
    delete m_ResourcePath;
}

bool Asset::stub3(unsigned char a1, int, int)
{
    m_Flags.m_FlagBits._13 = a1;
    return false;
}

bool Asset::stub4() const
{
    return m_Flags.m_FlagBits._13 & 1;
}

void Asset::stub5(int)
{
    return;
}

void Asset::GetResourcesDir(String& outDir, PlatformId platformId) const
{
    outDir = "";
}

void Asset::ApplyAssetData(int*)
{
    return;
}

char Asset::SetResourcePlaceholder()
{
    return 0;
}

int Asset::stub9() const
{
    return (m_Flags.m_FlagBits._19 != 0) + 1;
}

void Asset::GetResourceName(String& outName, int a2)
{

    if (a2)
        _851800(outName, m_ResourcePath, 0, 0);
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

    m_ResourceTimestamp = NULL;
    delete m_ResourcePath;

    if (dontmakeglobal)
        m_GlobalResourceId = NULL;
    else
        m_GlobalResourceId = g_AssetManager->InsertTypeListItem(this);

    m_Flags.m_Flags = 0;
    m_Flags.m_FlagBits.NotUsed = m_Flags.m_FlagBits._17 = m_Flags.m_FlagBits._18 = m_Flags.m_FlagBits._19 = 1;
}

const char* Asset::AddResToOpenListAndReturnName() const
{
    unsigned int resind = LastOpenResourceIndex;
    LastOpenResourceIndex = (LastOpenResourceIndex + 1) % 10;
    OpenResourcesList[resind] = g_AssetManager->GetResourcePathSceneRelative(m_ResourcePath);

    return OpenResourcesList[resind].m_szString;
}

#pragma message(TODO_IMPLEMENTATION)
void Asset::_851800(String& outstr, const char* inpath, bool a3, bool a4) const
{
}

void Asset::Destroy(Asset* res)
{
    res->~Asset();

    if ((res->m_Flags.m_FlagBits._0 & 1) != 0)
        MemoryManager::ReleaseMemory(res, false);
}

Asset* Asset::CreateInstance(size_t classsize)
{
    Asset* a = (Asset*)new char[classsize];
    a->m_Flags.m_FlagBits.NotUsed = true;

    return a;
}

void Asset::SetReferenceCount(unsigned char count)
{
    m_Flags.m_ReferenceCount = count;
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
                m_Flags.m_FlagBits.AssetRegion = i;
                return;
            }
    }
    else
        m_Flags.m_FlagBits.AssetRegion = 6;
}

const char* const Asset::GetResourceCountryCode() const
{
    unsigned int countrycode = m_Flags.m_FlagBits.AssetRegion & 15;

    //	NOTE: obvious fix - original code assumes that asset country code cannot be tampered with, since it's validated when loading.
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

    //	NOTE: this is temporary just to get VMT pointer, so destroy after use.
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