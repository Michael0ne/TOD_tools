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

#pragma message(TODO_IMPLEMENTATION)
void ModelAsset::_856E60()
{
}