#include "MeshColorAsset.h"

AssetInstance* MeshColorAsset::Instance;

#pragma message(TODO_IMPLEMENTATION)
MeshColorAsset::MeshColorAsset() : Asset(false)
{
    MESSAGE_CLASS_CREATED(MeshColorAsset);
}

#pragma message(TODO_IMPLEMENTATION)
MeshColorAsset::~MeshColorAsset()
{
    MESSAGE_CLASS_DESTROYED(MeshColorAsset);
}

AssetInstance* MeshColorAsset::GetInstancePtr() const
{
    return Instance;
}

void MeshColorAsset::CreateInstance()
{
    Instance = new AssetInstance("meshcoor", (CREATOR)Create);

    Instance->m_FileExtensions.push_back("meshcolor");
    Instance->m_FileExtensions.push_back("lighting");
}

MeshColorAsset* MeshColorAsset::Create()
{
    return new MeshColorAsset;
}