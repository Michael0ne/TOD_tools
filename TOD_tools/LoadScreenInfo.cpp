#include "LoadScreenInfo.h"
#include "Performance.h"
#include "LogDump.h"
#include "Blocks.h"
#include "Renderer.h"

LoadScreenInfo* g_LoadScreenInfo = nullptr;

#pragma message(TODO_IMPLEMENTATION)
void LoadScreenInfo::LoadTexture()
{
	if (m_TexturePath.Empty())
		m_TexturePath.Set(LOADSCREEN_DEFAULT_TEXTURE);

	AllocatorIdForTextureResourceLoading = RENDERLIST;
	IsLoadingLoadScreenTexture = true;

	String respath;
	_GetTextureResourcePath(resPath, m_TexturePath.m_szString, IsRegionEurope(), NULL);

	LogDump::LogA("LoadScreen:%s\n", respath.m_szString);
	m_TextureResource = g_Blocks->LoadResourceFile(respath.m_szString);

	if (*g_Renderer->m_TexturesList.m_Elements == m_TextureResource->m_Texture)	//	NOTE: 'm_TexturesList' is probably 'm_PlaceholderTextures'.
		LogDump::LogA("TEXTURE NOT FOUND!!!!!\n");

	IsLoadingLoadScreenTexture = false;
	AllocatorIdForTextureResourceLoading = DEFAULT;
}

LoadScreenInfo::LoadScreenInfo(const char* texturePath)
{
	MESSAGE_CLASS_CREATED(LoadScreenInfo);

	m_TexturePath = String(texturePath);
	m_Enabled = false;
}

void LoadScreenInfo::SetTexturePath(const char* _path)
{
	m_TexturePath.Set(_path);
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
	_ResetSceneChildrenNodes(1);
	LoadTexture();
}

void LoadScreenInfo::Deactivate()
{
	if (!m_Enabled)
		return;

	LogDump::LogA("Disabling load screen (%0.1f sec)\n", (Performance::GetMilliseconds() - m_StartTime) * 0.001f);
	m_Enabled = false;
}