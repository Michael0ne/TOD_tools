#include "ModelAsset.h"

AssetInstance* ModelAsset::Instance;

int ModelAsset::PivotIndexByName(const char* const pivotname) const
{
	if (m_MeshList.size() <= 0)
		return -1;

	for (unsigned int i = 0; i < m_MeshList.size(); ++i)
		if (_stricmp(m_MeshList[i].m_Name, pivotname) == NULL)
			return i;

	return -1;
}

void ModelAsset::CreateInstance()
{
	Instance = new AssetInstance("modelres", (CREATOR)Create);

	Instance->m_FileExtensions.push_back("model");
	Instance->SetAlignment(16, 1);
	Instance->SetAlignment(16, 2);
	Instance->SetAlignment(16, 0);
}

ModelAsset* ModelAsset::Create()
{
	return new ModelAsset;
}

ModelAsset::ModelAsset() : Asset(false)
{
	MESSAGE_CLASS_CREATED(ModelAsset);

	field_40 = 0;
	field_54 = 0;
	field_58 = (int*)1;
	field_5C &= 0xFFFFFFF0;
	m_BoundingRadius = {};
	SetReferenceCount(1);
	field_5C &= 0xFFFFFF2F | 0x20;
}

AssetInstance* ModelAsset::GetInstancePtr() const
{
	return Instance;
}

#pragma message(TODO_IMPLEMENTATION)
void ModelAsset::_856E60()
{
}

#pragma message(TODO_IMPLEMENTATION)
ModelAsset::Mesh::Mesh()
{
	MESSAGE_CLASS_CREATED(Mesh);
}

#pragma message(TODO_IMPLEMENTATION)
ModelAsset::Mesh::~Mesh()
{
	MESSAGE_CLASS_DESTROYED(Mesh);
}