#include "FragmentAsset.h"

AssetInstance* FragmentAsset::Instance;

#pragma message(TODO_IMPLEMENTATION)
FragmentAsset::FragmentAsset(char a1) : Asset(a1)
{
	MESSAGE_CLASS_CREATED(FragmentAsset);

	field_20 = new int[6];	//	TODO: replace with class constructor once known.
	field_24 &= 0xFFFFFFFD;
}

FragmentAsset::~FragmentAsset()
{
	MESSAGE_CLASS_DESTROYED(FragmentAsset);
	
	delete[] field_20;
}

AssetInstance* FragmentAsset::GetInstancePtr() const
{
	return Instance;
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
void FragmentAsset::ApplyFragmentResource(unsigned int entityId, bool)
{
}