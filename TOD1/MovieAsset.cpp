#include "MovieAsset.h"

AssetInstance* MovieAsset::Instance;

MovieAsset::~MovieAsset()
{
	MESSAGE_CLASS_DESTROYED(MovieAsset);
}

AssetInstance* MovieAsset::GetInstancePtr() const
{
	return Instance;
}

void MovieAsset::DestroyResource()
{
	m_ResourceTimestamp = NULL;
}

void MovieAsset::CreateInstance()
{
	Instance = new AssetInstance("movie", (CREATOR)Create);
	Instance->m_FileExtensions.push_back("wmv");
	Instance->m_FileExtensions.push_back("bik");
	Instance->SetAlignment(16, 1);
	Instance->SetAlignment(16, 2);
	Instance->SetAlignment(16, 0);
}

MovieAsset* MovieAsset::Create()
{
	return new MovieAsset;
}