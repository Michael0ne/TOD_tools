#include "BaseAsset.h"
#include "AssetManager.h"
#include "ScriptDatabase.h"

std::vector<String> Asset::OpenResourcesList;
unsigned int Asset::TotalResourcesCreated;
unsigned int Asset::LastOpenResourceIndex;

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

void Asset::GetResourcesDir(String& outDir, PlatformId platformId)
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

void Asset::Destroy(Asset* res)
{
	res->~Asset();

	if (res->m_Flags.m_FlagBits.NotUsed)
		delete res;
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