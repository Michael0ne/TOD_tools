#include "Renderer.h"
#include "Performance.h"

Renderer*	g_Renderer = nullptr;
bool		Renderer::WideScreen;	//	@A39F12
bool		Renderer::FSAA;
float		Renderer::RatioXY = 1.0f;	//	@A119F4
Renderer::Renderer_Buffer2	Renderer::_A08704[28] =
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

#pragma message(TODO_IMPLEMENTATION)
Renderer::Renderer(const Vector2<int>* resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1, const Vector3<float>* buffersDimens)
{
	MESSAGE_CLASS_CREATED(Renderer);

	g_Renderer = this;

	m_TexturesList = List<GfxInternal_Dx9_Texture>();
	m_TimeDelta = 0.f;
	field_30 = 0;
	m_TimeMilliseconds = Performance::GetMilliseconds();
	field_34 = 0;

	g_RendererDx = new GfxInternal_Dx9(resolution, unused1, unused2, FSAA, NULL);

	field_20 = unk1;
	m_RenderBufferTotal = buffersCount;

	m_RenderBufferArray = (Buffer276*)new Buffer276[buffersCount];

	if (buffersCount > 0)
	{
		for (; buffersCount; buffersCount--)
		{
			//	NOTE: since when calling 'new' can't really call constructor with parameters as needed, do this.
			m_RenderBufferArray[buffersCount] = Buffer276(*buffersDimens);

			m_RenderBufferArray[buffersCount].m_ViewportDimensions_1 = { 0.f, 0.f };
			m_RenderBufferArray[buffersCount].m_ViewportDimensions_2 = { g_RendererDx->m_ViewportWidth, g_RendererDx->m_ViewportHeight };

			buffersDimens++;
		}
	}

	m_RenderBufferEmpty = false;

	//	Allocate textures list.
	//(*(void(__thiscall*)(Renderer*))0x4210E0)(this);
	//	Allocate something
	//(*(void(__thiscall*)(Renderer*))0x420390)(this);

	field_35 = 1;
	m_Time_1 = 0.f;
	m_CallbackUnknown = nullptr;
	m_RenderEndTime = __rdtsc();

#ifdef INCLUDE_FIXES
	ShowCursor(TRUE);
#else
	ShowCursor(FALSE);
#endif
}

Renderer::~Renderer()
{
	MESSAGE_CLASS_DESTROYED(Renderer);

	delete m_Buffer68;
	delete m_Buffer108;

	delete g_RendererDx;

	delete m_RenderBufferArray;
}

void Renderer::SetClearColorForBufferIndex(const ColorRGB& color, int index)
{
	if (index != -1)
		m_RenderBufferArray[index].m_ClearColor = color;
	else
		if (m_RenderBufferTotal > NULL)
			for (unsigned int i = NULL; i < m_RenderBufferTotal; i++)
				m_RenderBufferArray[i].m_ClearColor = color;
}

void Renderer::SetClearFlagsForBufferIndex(const unsigned int flags, const int index)
{
	if (index != -1)
		m_RenderBufferArray[index].m_ClearFlags = flags;
	else
		if (m_RenderBufferTotal > NULL)
			for (unsigned int i = NULL; i < m_RenderBufferTotal; i++)
				m_RenderBufferArray[i].m_ClearFlags = flags;
}

void Renderer::SetRenderBufferIsEmpty(bool _empty)
{
	m_RenderBufferEmpty = _empty;
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