#include "GfxInternal.h"
#include "Performance.h"
#include "LogDump.h"
#include "BuiltinType.h"

GfxInternal        *g_GfxInternal = nullptr;
bool                GfxInternal::WideScreen;	//	@A39F12
bool                GfxInternal::FSAA;
float               GfxInternal::RatioXY = 1.0f;	//	@A119F4
float               GfxInternal::_A3A064;
DirectX::XMMATRIX   GfxInternal::_A3A268;

GfxInternal::Renderer_Buffer2   GfxInternal::_A08704[28] =
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
};

#pragma message(TODO_IMPLEMENTATION)
GfxInternal::GfxInternal(const Vector2<unsigned int>& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1, const Vector3<float>* buffersDimens)
{
    MESSAGE_CLASS_CREATED(GfxInternal);

    g_GfxInternal = this;

    m_TimeDelta = 0.f;
    m_FramesRendered = 0;
    m_TimeMilliseconds = Performance::GetMilliseconds();
    field_34 = 0;

#ifdef OPENGL
    g_GfxInternal_OGL = new GfxInternal_OGL(resolution, unused1, unused2, FSAA, NULL);
#endif
#ifdef DIRECTX
    g_GfxInternal_Dx9 = new GfxInternal_Dx9(resolution, unused1, unused2, FSAA, NULL);
#endif

    field_20 = unk1;
    m_RenderBufferTotal = buffersCount;

    m_RenderBufferArray = new Buffer276[buffersCount];

    if (buffersCount > 0)
    {
        for (unsigned int i = 0; buffersCount--; i++)
        {
            m_RenderBufferArray[i] = Buffer276(*buffersDimens++);

            m_RenderBufferArray[i].m_ViewportDimensions_1 = { 0, 0 };
            m_RenderBufferArray[i].m_ViewportDimensions_2 = { g_GfxInternal_Dx9->m_ViewportResolution.x, g_GfxInternal_Dx9->m_ViewportResolution.y };
        }
    }

    m_RenderBufferEmpty = false;

    CreateCheckerboardTextures();
    //	Allocate something
    //(*(void(__thiscall*)(Renderer*))0x420390)(this);

    field_35 = 1;
    m_Time_1 = 0.f;
    m_SceneCallback = nullptr;
    m_FrameEndTime = __rdtsc();

#ifdef INCLUDE_FIXES
    ShowCursor(TRUE);
#else
    ShowCursor(FALSE);
#endif
}

GfxInternal::~GfxInternal()
{
    MESSAGE_CLASS_DESTROYED(GfxInternal);

    delete m_MeshBuffer;
    delete m_Mesh;

    delete g_GfxInternal_Dx9;

    delete m_RenderBufferArray;
}

void GfxInternal::Render(Surface* screenshotDumpSurface, const bool shouldRender, int a3, int a4)
{
    ++m_FramesRendered;

    a4 = a3 >= 0 ? a4 < 0 ? a3 : a4 : m_RenderBufferTotal - 1;
    a3 = a3 < 0 ? 0 : a3;

    bool frmclbckcalled = false;

    if (shouldRender)
    {
        do
        {
            if (g_GfxInternal_Dx9->BeginScene())
            {
                if (field_20 < 0)
                {
                    g_GfxInternal_Dx9->SetRenderTarget(nullptr);
                }
                else
                {
                    Vector3f* buffsize = &m_RenderBufferArray->field_10->m_BufferSize;
                    int j = 0;
                    bool bdis = false;
                    while (!buffsize)
                    {
                        buffsize++;
                        if (++j > field_20)
                        {
                            g_GfxInternal_Dx9->SetRenderTarget(nullptr);
                            bdis = true;
                            break;
                        }
                    }

                    if (!bdis)
                    {
                        g_GfxInternal_Dx9->SetupViewportSurface();
                        ExecuteRenderBuffer(a3, field_20, 0);
                        g_GfxInternal_Dx9->RenderViewport();
                    }
                }
            }

            ExecuteRenderBuffer(field_20 + 1, a4, 1);
            g_GfxInternal_Dx9->EndScene();

            if (screenshotDumpSurface)
                g_GfxInternal_Dx9->DumpScreenShot(screenshotDumpSurface);

            if (!frmclbckcalled)
            {
                CallSceneCallback();
                frmclbckcalled = true;
            }

            g_GfxInternal_Dx9->HandleDeviceLost();
            m_FrameEndTime = __rdtsc();
        } while (g_GfxInternal_Dx9->ProcessGameInput());
    }

    _41F950();
    m_TimeDelta = (float)(Performance::GetMilliseconds() - m_TimeMilliseconds) * 0.001f;
    m_TimeMilliseconds = Performance::GetMilliseconds();

    g_GfxInternal_Dx9->ResetStream();
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal::CallSceneCallback()
{
    if (!m_SceneCallback)
        return;

    //	TODO: ...
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

void GfxInternal::PrepareForNewLevel()
{
    if (g_GfxInternal_Dx9->m_RenderingScene &&
        g_GfxInternal->m_FramesRendered > g_GfxInternal_Dx9->field_975C)
    {
        LogDump::LogA("GfxInternal::PrepareForNewLevel\n");
        g_GfxInternal_Dx9->m_Direct3DDevice->EvictManagedResources();
        if (g_GfxInternal_Dx9->BeginScene())
        {
            g_GfxInternal_Dx9->SetRenderTarget(nullptr);
            Texture::DrawAllTextures();
            
            if (g_GfxInternal_Dx9->m_RenderingScene)
                g_GfxInternal_Dx9->m_Direct3DDevice->EndScene();
            g_GfxInternal_Dx9->m_RenderingScene = false;
            g_GfxInternal_Dx9->HandleDeviceLost();
        }

        g_GfxInternal_Dx9->field_975C = g_GfxInternal->m_FramesRendered;
    }
}

void GfxInternal::DumpScreenShot(class Surface* surf) const
{
    g_GfxInternal_Dx9->DumpScreenShot(surf);
}

ScreenResolution& GfxInternal::GetScreenResolution(ScreenResolution& res) const
{
    return (res = g_GfxInternal_Dx9->m_DisplayModeResolution, res);
}

bool GfxInternal::IsScreenResolutionAvailable(unsigned int width, unsigned int height) const
{
    return g_GfxInternal_Dx9->IsScreenResolutionAvailable(width, height, true) != false;
}

void GfxInternal::SetBufferRenderBufferPointerByIndex(unsigned int index, FrameBuffer* buf)
{
    m_RenderBufferArray[index].m_RenderBuffer = buf;
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal::_41F950()
{
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal::ExecuteRenderBuffer(int a1, int a2, int a3)
{
    if (a1 > a2)
        return;

    DirectX::XMMATRIX mat = _A3A268;
    for (int i = 0; i < a1; ++i)
    {
        Buffer276* buff = &m_RenderBufferArray[i];
        if (buff->field_10 || buff->m_ClearFlags)
        {
            if (buff->m_ViewportDimensions_1.x != g_GfxInternal_Dx9->m_ViewportResolution_1.x ||
                buff->m_ViewportDimensions_1.y != g_GfxInternal_Dx9->m_ViewportResolution_1.y ||
                buff->m_ViewportDimensions_2.x != g_GfxInternal_Dx9->m_ViewportResolution.x ||
                buff->m_ViewportDimensions_2.y != g_GfxInternal_Dx9->m_ViewportResolution.y)
                g_GfxInternal_Dx9->SetViewport(buff->m_ViewportDimensions_1, buff->m_ViewportDimensions_2);

            if (buff->m_ClearFlags)
                g_GfxInternal_Dx9->Clear(buff->m_ClearFlags, buff->m_ClearColor);

            g_GfxInternal_Dx9->SetProjection(buff->m_ProjectionMatrixParams.x, buff->m_ProjectionMatrixParams.y, buff->m_ProjectionMatrixParams.z, buff->m_ProjectionMatrixParams.a);

            //  TODO: something to with matricies, copy view matrix?

            /*
            if (_A3A064 < )
            {
                g_GfxInternal_Dx9->TransformStateView(&buff->m_ViewMatrix);
                g_GfxInternal_Dx9->field_A = buff->field_14;

                //  TODO: updates cached view matrix?
            }

            if (buff->m_RenderBuffer)
            {
                buff->m_RenderBuffer->ExecuteRenderCommand(buff->m_RenderBuffer->m_RenderBuffer[1]);
                buff->m_RenderBuffer->ExecuteRenderCommand(buff->m_RenderBuffer->m_RenderBuffer[0]);
            }

            //  TODO: make temp list with something...

            if (buff->m_RenderBuffer)
                buff->m_RenderBuffer->ExecuteRenderCommand(buff->m_RenderBuffer->m_RenderBuffer[2]);
            */
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
FrameBuffer* GfxInternal::_41F8F0(FrameBuffer* fb, unsigned int index)
{
    FrameBuffer* ret = (FrameBuffer*)m_RenderBufferArray[index].field_10;
    m_RenderBufferArray[index].field_10 = (Buffer276*)fb;

    return ret;
}

void GfxInternal::CreateCheckerboardTextures()
{
    if (m_CheckerboardTextures.size() < 2)
        m_CheckerboardTextures.reserve(2);
    
    m_CheckerboardTextures.clear();

    //  NOTE: black & white checkerboard texture.
    Surface* bwtexsurf = new Surface(64, 64);

    for (unsigned int x = 0; x < 64; ++x)
        for (unsigned int y = 0; y < 64; ++y)
            bwtexsurf->SetPixelColor(x, y, !(x % 16) || !(y % 16) ? BuiltinType::ColorWhite : BuiltinType::ColorBlack );

    SurfaceMutable* bwtexsurfmut = new SurfaceMutable(bwtexsurf);
    m_CheckerboardTextures.push_back(new Texture(bwtexsurfmut));
    delete bwtexsurfmut;

    //  NOTE: pink & yellow checkerboard texture.
    Surface* pytexsurf = new Surface(64, 64);

    for (unsigned int x = 0; x < 64; ++x)
        for (unsigned int y = 0; y < 64; ++y)
            pytexsurf->SetPixelColor(x, y, (x % 16 < 8) || (y % 16 < 8) ? BuiltinType::ColorPink : BuiltinType::ColorYellow );

    SurfaceMutable* pytexsurfmut = new SurfaceMutable(pytexsurf);
    m_CheckerboardTextures.push_back(new Texture(pytexsurfmut));
    delete pytexsurfmut;
}

void GfxInternal::GetViewMatrixForBufferIndex(DirectX::XMMATRIX& mat, const unsigned int ind) const
{
    mat = m_RenderBufferArray[ind].m_ViewMatrix;
}

AssetManager::RegionCode GfxInternal::GetRegion()
{
    return AssetManager::REGION_EUROPE;
}

bool GfxInternal::IsWideScreen()
{
    return WideScreen;
}

Buffer276::Buffer276(const Vector3f& bufferSize)
{
    m_BufferSize = bufferSize;
    field_14[0] = 1;
    field_10 = NULL;
    m_RenderBuffer = NULL;
    field_DC[0] = NULL;
    m_ClearFlags = NULL;
    
    m_ViewMatrix = DirectX::XMMatrixIdentity();
    m_MatrixUnknown_1 = DirectX::XMMatrixIdentity();
    m_MatrixUnknown_2 =DirectX::XMMatrixIdentity();

    m_ProjectionMatrixParams = { 70.f, 1.f, 1.f, 1000.f };
    m_ClearColor = { 0.f, 0.f, 0.f, 1.f };
}