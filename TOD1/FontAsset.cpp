#include "FontAsset.h"

AssetInstance* FontAsset::Instance;

FontAsset::~FontAsset()
{
	MESSAGE_CLASS_DESTROYED(FontAsset);

	delete m_Font;
	delete field_20;
}

AssetInstance* FontAsset::GetInstancePtr() const
{
	return Instance;
}

#pragma message(TODO_IMPLEMENTATION)
void FontAsset::ApplyAssetData(int*)
{
}

void FontAsset::CreateInstance()
{
	Instance = new AssetInstance("font", (CREATOR)Create);

	Instance->m_FileExtensions.push_back("ttf");
	Instance->m_FileExtensions.push_back("font");
	Instance->SetAlignment(16, 1);
	Instance->SetAlignment(128, 2);
	Instance->SetAlignment(16, 0);
}

FontAsset* FontAsset::Create()
{
	return new FontAsset;
}