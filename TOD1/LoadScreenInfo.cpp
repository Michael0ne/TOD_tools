#include "LoadScreenInfo.h"
#include "Performance.h"
#include "LogDump.h"
#include "AssetManager.h"
#include "GfxInternal.h"
#include "ScriptDatabase.h"

LoadScreenInfo* g_LoadScreenInfo = nullptr;
unsigned int LoadScreenInfo::AllocatorIdForTextureResourceLoading;
bool LoadScreenInfo::IsLoadingLoadScreenTexture;

#pragma message(TODO_IMPLEMENTATION)
void LoadScreenInfo::LoadTexture()
{
	if (m_TexturePath.Empty())
		m_TexturePath = LOADSCREEN_DEFAULT_TEXTURE;

	AllocatorIdForTextureResourceLoading = RENDERLIST;
	IsLoadingLoadScreenTexture = true;

	//String respath;
	//Blocks::GetPlatformSpecificResourcePath(respath, m_TexturePath.m_szString, Script::IsRegionEurope() ? GameConfig::CountryCodes::COUNTRY_USA : GameConfig::CountryCodes::COUNTRY_EUROPE, ResType::PlatformId::PLATFORM_PC);

	//LogDump::LogA("LoadScreen:%s\n", respath.m_szString);
	//m_TextureResource = g_Blocks->LoadResourceFile(respath.m_szString);

	//if (*g_Renderer->m_TexturesList.m_Elements == m_TextureResource->m_Texture)	//	NOTE: 'm_TexturesList' is probably 'm_PlaceholderTextures'.
		//LogDump::LogA("TEXTURE NOT FOUND!!!!!\n");

	IsLoadingLoadScreenTexture = false;
	AllocatorIdForTextureResourceLoading = DEFAULT;
}

LoadScreenInfo::LoadScreenInfo(const char* texturePath)
{
	MESSAGE_CLASS_CREATED(LoadScreenInfo);

	m_TexturePath = texturePath;
	m_Enabled = false;
}

void LoadScreenInfo::SetTexturePath(const char* _path)
{
	m_TexturePath = _path;
}

void LoadScreenInfo::Enable(void* topNode)
{
	if (m_Enabled)
		return;

	m_StartTime = Performance::GetMilliseconds();
	LogDump::LogA("Enabling load screen\n");

	Show(topNode);

	m_Enabled = true;
}

#pragma message(TODO_IMPLEMENTATION)
void LoadScreenInfo::Show(void* topNode)
{
	DWORD startTime = Performance::GetMilliseconds();
	//_ResetSceneChildrenNodes(1);
	LoadTexture();
}

void LoadScreenInfo::Deactivate()
{
	if (!m_Enabled)
		return;

	LogDump::LogA("Disabling load screen (%0.1f sec)\n", (Performance::GetMilliseconds() - m_StartTime) * 0.001f);
	m_Enabled = false;
}