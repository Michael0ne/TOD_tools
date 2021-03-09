#include "GfxInternal.h"
#include "Performance.h"
#include "LogDump.h"

GfxInternal*	g_GfxInternal = nullptr;
bool		GfxInternal::WideScreen;	//	@A39F12
bool		GfxInternal::FSAA;
float		GfxInternal::RatioXY = 1.0f;	//	@A119F4
GfxInternal::Renderer_Buffer2	GfxInternal::_A08704[28] =
{
	{0, 1},
	{0, 2},
	{1, 3},
	{1, 4},
	{1, 5},
	{0, 6},
	{2, 7},
	{0, 8},
	{1, 9},
	{1, 10},
	{1, 11},
	{1, 12},
	{1, 13},
	{2, 14},
	{1, 15},
	{1, 16},
	{1, 17},
	{1, 18},
	{1, 19},
	{2, 20},
	{2, 21},
	{1, 22},
	{1, 23},
	{2, 24},
	{2, 25},
	{1, 26},
	{1, 27},
	{1, 0}
};	//	@A08704
ScreenProperties	g_ScreenProperties;	//	@A08810
std::map<int, int>* Scene_Buffer68::MeshBuffersMap;

#pragma message(TODO_IMPLEMENTATION)
GfxInternal::GfxInternal(const Vector2<int>& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1, const Vector3<float>* buffersDimens)
{
	MESSAGE_CLASS_CREATED(GfxInternal);

	g_GfxInternal = this;

	m_TexturesList = List<GfxInternal_Dx9_Texture>();
	m_TimeDelta = 0.f;
	m_FramesRendered = 0;
	m_TimeMilliseconds = Performance::GetMilliseconds();
	field_34 = 0;

	g_GfxInternal_Dx9 = new GfxInternal_Dx9(resolution, unused1, unused2, FSAA, NULL);

	field_20 = unk1;
	m_RenderBufferTotal = buffersCount;

	m_RenderBufferArray = new Buffer276[buffersCount];

	if (buffersCount > 0)
	{
		for (unsigned int i = 0; buffersCount--; i++)
		{
			m_RenderBufferArray[i] = Buffer276(*buffersDimens++);

			m_RenderBufferArray[i].m_ViewportDimensions_1 = { 0.f, 0.f };
			m_RenderBufferArray[i].m_ViewportDimensions_2 = { (float)g_GfxInternal_Dx9->m_ViewportResolution.x, (float)g_GfxInternal_Dx9->m_ViewportResolution.y };
		}
	}

	m_RenderBufferEmpty = false;

	//	Allocate textures list.
	//(*(void(__thiscall*)(Renderer*))0x4210E0)(this);
	//	Allocate something
	//(*(void(__thiscall*)(Renderer*))0x420390)(this);

	field_35 = 1;
	m_Time_1 = 0.f;
	m_SceneCallback = nullptr;
	m_RenderEndTime = __rdtsc();

#ifdef INCLUDE_FIXES
	ShowCursor(TRUE);
#else
	ShowCursor(FALSE);
#endif
}

GfxInternal::~GfxInternal()
{
	MESSAGE_CLASS_DESTROYED(GfxInternal);

	delete m_Buffer68;
	delete m_Buffer108;

	delete g_GfxInternal_Dx9;

	delete m_RenderBufferArray;
}

void GfxInternal::SetClearColorForBufferIndex(const ColorRGB& color, int index)
{
	if (index != -1)
		m_RenderBufferArray[index].m_ClearColor = color;
	else
		if (m_RenderBufferTotal > NULL)
			for (unsigned int i = NULL; i < m_RenderBufferTotal; i++)
				m_RenderBufferArray[i].m_ClearColor = color;
}

void GfxInternal::SetClearFlagsForBufferIndex(const unsigned int flags, const int index)
{
	if (index != -1)
		m_RenderBufferArray[index].m_ClearFlags = flags;
	else
		if (m_RenderBufferTotal > NULL)
			for (unsigned int i = NULL; i < m_RenderBufferTotal; i++)
				m_RenderBufferArray[i].m_ClearFlags = flags;
}

void GfxInternal::SetRenderBufferIsEmpty(bool _empty)
{
	m_RenderBufferEmpty = _empty;
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal::PrepareForNewLevel()
{
	/*
	if (g_RendererDx->m_SceneBegan &&
		g_Renderer->m_FramesRendered > g_RendererDx->field_975C)
	{
		LogDump::LogA("GfxInternal::PrepareForNewLevel\n");
		g_RendererDx->m_Direct3DDevice->EvictManagedResources();
		if (g_RendererDx->BeginScene())
		{
			g_RendererDx->SetRenderTarget(nullptr);
			GfxInternal_Dx9_Texture::DrawAllTextures();
			
			if (g_RendererDx->m_SceneBegan)
				g_RendererDx->m_Direct3DDevice->EndScene();
			g_RendererDx->m_SceneBegan = false;
			g_RendererDx->HandleDeviceLost();
		}

		g_RendererDx->field_975C = g_Renderer->m_FramesRendered;
	}
	*/
}

void GfxInternal::DumpScreenShot(class GfxInternal_Dx9_Surface* surf) const
{
	g_GfxInternal_Dx9->DumpScreenShot(surf);
}

Vector2<int>& GfxInternal::GetScreenResolution(Vector2<int>& res) const
{
	return (res = g_GfxInternal_Dx9->m_DisplayModeResolution, res);
}

bool GfxInternal::IsScreenResolutionAvailable(int width, int height) const
{
	return g_GfxInternal_Dx9->IsScreenResolutionAvailable(width, height, true) != false;
}

bool GfxInternal::IsWideScreen()
{
	return WideScreen;
}

void ScreenProperties::SetHudScreenSize(float width, float height, float unk1, float unk2)
{
	field_8 = unk1;
	field_C = unk2;
	m_fVirtualHudScreensizeWidth = width;
	m_fVirtualHudScreensizeHeight = height;

	AdjustWindowScalings();
}

#pragma message(TODO_IMPLEMENTATION)
void ScreenProperties::AdjustWindowScalings()
{
	(*(void(__thiscall*)(ScreenProperties*))0x420190)(this);
}

void ScreenProperties::SetSafeArea(float area)
{
	m_bSafeArea = true;
	m_fScreenSafeArea = area;

	AdjustWindowScalings();
}

void ScreenProperties::SetWindowProperties(float width, float height, float ratio, float safearea)
{
	m_fScreenWidth = width;
	m_fScreenHeight = height;
	m_fScreenRatio = ratio;
	
	if (!m_bSafeArea)
		m_fScreenSafeArea = safearea;

	AdjustWindowScalings();
}

Buffer276::Buffer276(const Vector3f& bufferSize)
{
	m_BufferSize = bufferSize;
	field_14 = 1;
	field_10 = NULL;
	m_RenderBuffer = NULL;
	field_DC = NULL;
	m_ClearFlags = NULL;
	
	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixIdentity(&m_MatrixUnknown_1);
	D3DXMatrixIdentity(&m_MatrixUnknown_2);

	m_ProjectionMatrixParams = { 70.f, 1.f, 1.f, 1000.f };
	m_ClearColor = { 0.f, 0.f, 0.f, 1.f };
}

void Scene_Buffer68::CreateMeshBufferMap()
{
	MeshBuffersMap = new std::map<int, int>();
}