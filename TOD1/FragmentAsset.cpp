#include "FragmentAsset.h"
#include "Scene.h"

AssetInstance* FragmentAsset::Instance;

FragmentAsset::FragmentInfo::FragmentInfo(FragmentInfo** rhs, AllocatorIndex(*FindSuitableAllocatorProc)(size_t desiredsize, FragmentAsset*), FragmentAsset* owner)
{
    MESSAGE_CLASS_CREATED(FragmentInfo);

    field_10 = FindSuitableAllocatorProc;
    field_0 = NULL;
    field_4 = nullptr;
    field_8 = NULL;
    field_14 = owner;
    field_C = (*rhs)->field_C;

    /*if (field_4)
        field_4->_406350(field_8 & 0x7FFFFFFF);*/

    field_4 = nullptr;
    field_8 = 0x80000000;
}

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

    MemoryManager::ReleaseMemory(field_20, false);
}

AllocatorIndex FragmentAsset::GetAllocatorForAsset(size_t size, FragmentAsset* asset)
{
    const int32_t biggerSize = size + 1024;

    if (biggerSize <= MemoryManager::AllocatorsList[CUTSCENE_OR_REWIND]->GetBiggestUsedMemoryBlock())
        return CUTSCENE_OR_REWIND;

    if (Scene::LoadingAssetBlock && biggerSize <= MemoryManager::AllocatorsList[DEFRAGMENTING]->GetBiggestUsedMemoryBlock())
        return DEFRAGMENTING;

    if (Scene::LoadingAssetBlock && biggerSize <= MemoryManager::AllocatorsList[MISSION_ASSETS]->GetBiggestUsedMemoryBlock())
        return MISSION_ASSETS;

    if (biggerSize <= MemoryManager::AllocatorsList[RENDERLIST]->GetBiggestUsedMemoryBlock())
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

    assetInfoPtr->_85E160((uint8_t**)&field_20, (uint8_t**)&assetInfoPtr, 2, -1);

    if (assetInfoPtr->m_AssetType != CompiledAssetInfo::AssetType::THREE)
        return;

    AllocatorIndexForSize GetAllocatorIndex = nullptr;
    AllocatorIndex suitableAllocatorIndex;

    if ((field_24 & 1) != 0)
        GetAllocatorIndex = GetAllocatorForAsset;

    if (GetAllocatorIndex)
        suitableAllocatorIndex = GetAllocatorIndex(sizeof(FragmentInfo), this);

    field_20 = (FragmentInfo*)MemoryManager::AllocatorsList[suitableAllocatorIndex]->Allocate_A(sizeof(FragmentInfo), nullptr, NULL);
    *field_20 = FragmentInfo(&field_20, GetAllocatorIndex, this);
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
uint32_t* FragmentAsset::FragmentInfo::_406450()
{
    if (field_8 >= 0)
    {
        /*if (field_4)
            field_4->_406350(field_8 & 0x7FFFFFFF);*/

        field_4 = nullptr;
        field_8 = 0x80000000;
    }

    field_4 = (uint32_t*)ALIGN_4BYTES(field_0);
    if (field_4)
        return (uint32_t*)ALIGN_4BYTES(field_4[2]);
    else
        return nullptr;
}

void FragmentAsset::FragmentInfo::_406490()
{
    if (ALIGN_4BYTES(field_0) != NULL)
        _4069F0();

    field_0 = NULL;
    field_4 = NULL;
    field_8 = NULL;
}

void FragmentAsset::FragmentInfo::_4069F0()
{
    if (field_0 && (field_0 & 1) == 0)
    {
        if (ALIGN_4BYTES(field_0) != NULL && (field_0 & 2) != NULL)
            _4069C0((FragmentInfo*)(ALIGN_4BYTES(field_0)));
    }
    else
    {
        FragmentInfo* fragmentInfo = (FragmentInfo*)(ALIGN_4BYTES(field_0));
        if (!fragmentInfo)
            return;

        uint32_t unk = *(uint32_t*)(field_0 + 8);
        if (!unk || (unk & 1) != 0)
            MemoryManager::ReleaseMemory((void*)(ALIGN_4BYTES(unk)), false);

        fragmentInfo->_4069F0();
        MemoryManager::ReleaseMemory(fragmentInfo, false);
    }
}

uint32_t* FragmentAsset::FragmentInfo::_406320(uint32_t* data)
{
    if (data != (uint32_t*)(field_4[1] + ALIGN_4BYTES(field_4[2])))
        return data;

    field_4 = (uint32_t*)(ALIGN_4BYTES(field_4[0]));
    if (field_4)
        return (uint32_t*)(ALIGN_4BYTES(field_4[2]));
    else
        return nullptr;
}

void FragmentAsset::FragmentInfo::_4069C0(FragmentInfo* fragmentInfo)
{
    uint32_t unk = fragmentInfo->field_8;
    if (!unk || (unk & 1) != 0)
        MemoryManager::ReleaseMemory((void*)(ALIGN_4BYTES(unk)), false);

    fragmentInfo->_4069F0();
}