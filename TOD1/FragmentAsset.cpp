#include "FragmentAsset.h"
#include "Scene.h"

AssetInstance* FragmentAsset::Instance;

FragmentAsset::FragmentInfo::~FragmentInfo()
{
    MESSAGE_CLASS_DESTROYED(FragmentInfo);
}

FragmentAsset::FragmentAsset(char a1) : Asset(a1)
{
    MESSAGE_CLASS_CREATED(FragmentAsset);

    field_20 = new FragmentInfo;
    field_24 &= ~2u;
}

FragmentAsset::~FragmentAsset()
{
    MESSAGE_CLASS_DESTROYED(FragmentAsset);

    delete[] field_20;
}

int32_t FragmentAsset::GetAllocatorForAsset(size_t size, FragmentAsset* asset)
{
    const size_t biggerSize = size + 1024;

    if (biggerSize <= MemoryManager::AllocatorsList[MISSION]->stub21())
        return MISSION;

    if (Scene::LoadingAssetBlock && biggerSize <= MemoryManager::AllocatorsList[DEFRAGMENTING]->stub21())
        return DEFRAGMENTING;

    if (Scene::LoadingAssetBlock && biggerSize <= MemoryManager::AllocatorsList[MISSION_ASSETS]->stub21())
        return MISSION_ASSETS;

    if (biggerSize <= MemoryManager::AllocatorsList[RENDERLIST]->stub21())
        return RENDERLIST;

    return DEFAULT;
}

AssetInstance* FragmentAsset::GetInstancePtr() const
{
    return Instance;
}

#pragma message(TODO_IMPLEMENTATION)
void FragmentAsset::ApplyAssetData(CompiledAssetInfo* assetInfoPtr)
{
    const uint8_t* assetPtr = (uint8_t*)this;
    assetInfoPtr->ParseInfo(&assetPtr, &assetInfoPtr, sizeof(FragmentAsset), 1, -1);

    if (assetInfoPtr->m_AssetType == CompiledAssetInfo::AssetType::ZERO ||
        assetInfoPtr->m_AssetType == CompiledAssetInfo::AssetType::ONE ||
        assetInfoPtr->m_AssetType == CompiledAssetInfo::AssetType::TWO)
        _851430(assetInfoPtr, &assetInfoPtr);
}

void FragmentAsset::CreateInstance()
{
    Instance = new AssetInstance("fragment", (CREATOR)Create);
    Instance->m_FileExtensions.push_back("fragment");
    Instance->m_FileExtensions.push_back("scene");
    Instance->m_VerifyChecksum = true;

    Instance->SetAlignment(16, 1);
    Instance->SetAlignment(16, 2);
    Instance->SetAlignment(16, 0);
}

FragmentAsset* FragmentAsset::Create()
{
    return new FragmentAsset(0);
}

#pragma message(TODO_IMPLEMENTATION)
void FragmentAsset::ApplyFragmentResource(const int32_t entityId, bool)
{
    if ((field_24 & 2) == 0)
        return;

    if (ALIGN_4BYTES(field_20) != NULL && field_20->_406450())
    {
        static uint32_t uniqueId0 = GetPropertyIdByName("unique_id0:integer");
        static uint32_t uniqueId1 = GetPropertyIdByName("unique_id1:integer");
        static uint32_t uniqueId2 = GetPropertyIdByName("unique_id2:integer");
        static uint32_t uniqueId3 = GetPropertyIdByName("unique_id3:integer");

        std::map<int, int> entitiesIdsMap;
        entitiesIdsMap[0] = entityId;

        if (entityId <= 0)
            g_AssetManager->m_EntityIdsMap = nullptr;
        else
            g_AssetManager->m_EntityIdsMap = new std::map<int, int>(entitiesIdsMap);
    }
    else
        LogDump::LogA("WARNING! Tried to apply free'd fragment '%s' - stuff may be missing; FIXME\n", GetName());
}

#pragma message(TODO_IMPLEMENTATION)
int FragmentAsset::FragmentInfo::_406450()
{
    return NULL;
}

void FragmentAsset::FragmentInfo::_406490()
{
    if (ALIGN_4BYTES(field_0) != NULL);

    field_0 = NULL;
    field_4 = NULL;
    field_8 = NULL;
}