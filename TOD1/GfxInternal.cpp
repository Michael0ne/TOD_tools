#include "GfxInternal.h"
#include "Timer.h"
#include "LogDump.h"
#include "BuiltinType.h"

GfxInternal        *g_GfxInternal = nullptr;
bool                GfxInternal::WideScreen; // @A39F12
bool                GfxInternal::FSAA;
float               GfxInternal::RatioXY = 1.0f; // @A119F4
float               GfxInternal::_A3A064;
DirectX::XMMATRIX   GfxInternal::_A3A268;

GfxInternal::GfxInternal(const Vector2<unsigned int>& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1, const Vector3<float>* buffersDimens)
{
    MESSAGE_CLASS_CREATED(GfxInternal);

    g_GfxInternal = this;

    m_TimeDelta = 0.f;
    m_FramesRendered = 0;
    m_TimeStart = Timer::GetMilliseconds();
    field_34 = 0;

#ifdef OPENGL
    g_GfxInternal_OGL = new GfxInternal_OGL(resolution, unused1, unused2, FSAA, NULL);
#endif
#ifdef DIRECTX
    g_GfxInternal_Dx9 = new GfxInternal_Dx9(resolution, unused1, unused2, FSAA, NULL);
#endif

    field_20 = unk1;
    m_RenderBufferTotal = buffersCount;

    m_RenderLayers = (RenderLayer*)MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(sizeof(RenderLayer) * buffersCount, __FILE__, __LINE__);

    if (buffersCount > 0)
    {
        for (uint32_t i = 0; i < buffersCount; i++)
        {
            RenderLayer* layer = &m_RenderLayers[i];
            if (layer)
                layer = new RenderLayer(*buffersDimens);

            layer->ViewportResolution_1 = { 0, 0 };
            layer->ViewportResolution_2 = { g_GfxInternal_Dx9->m_ViewportResolution.x, g_GfxInternal_Dx9->m_ViewportResolution.y };

            buffersDimens++;
        }
    }

    m_RenderBufferEmpty = false;

    CreateCheckerboardTextures();
    _420390();

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

    delete m_RenderLayers;
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
                    FrameBuffer** fb = &m_RenderLayers->_f10;
                    int j = 0;
                    bool bdis = false;
                    while (!fb)
                    {
                        fb += 69;
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

            g_GfxInternal_Dx9->Present();
            m_FrameEndTime = __rdtsc();
        } while (g_GfxInternal_Dx9->ProcessGameInput());
    }

    _41F950();
    m_TimeDelta = (float)(Timer::GetMilliseconds() - m_TimeStart) * 0.001f;
    m_TimeStart = Timer::GetMilliseconds();

    g_GfxInternal_Dx9->ResetStream();
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal::CallSceneCallback()
{
    if (!m_SceneCallback)
        return;

    INT64 clockCycles = Timer::ClockGetCyclesMilliseconds();
    const double frameTime = (double)clockCycles * 16.6;
    const INT64 clockStart = __rdtsc();
    const double frameDiff = frameTime - (clockStart - m_FrameEndTime);
}

/// <summary>
/// Set the color to fill the screen with after clear operation.
/// </summary>
/// <param name="color">RGBA color</param>
/// <param name="index">What layer will be affected. Set to -1 for all layers.</param>
void GfxInternal::SetClearColorForBufferIndex(const ColorRGB& color, int index)
{
    if (index != -1)
        m_RenderLayers[index].ClearColor = color;
    else
        if (m_RenderBufferTotal > NULL)
            for (unsigned int i = NULL; i < m_RenderBufferTotal; i++)
                m_RenderLayers[i].ClearColor = color;
}

void GfxInternal::SetClearFlagsForBufferIndex(const unsigned int flags, const int index)
{
    if (index != -1)
        m_RenderLayers[index].ClearFlags = flags;
    else
        if (m_RenderBufferTotal > NULL)
            for (unsigned int i = NULL; i < m_RenderBufferTotal; i++)
                m_RenderLayers[i].ClearFlags = flags;
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
            g_GfxInternal_Dx9->Present();
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
    m_RenderLayers[index].FrameBufferPtr = buf;
}

void GfxInternal::_41F950()
{
    if (m_RenderBufferTotal <= 0)
        return;

    for (size_t i = 0; i < m_RenderBufferTotal; ++i)
    {
        FrameBuffer* fb = m_RenderLayers[i]._f10;
        while (fb)
        {
            fb->m_Flags.m_FlagBits._5 = false;

            FrameBuffer* fb_ = fb;
            fb = fb->field_58;

            fb_ = nullptr;
        }

        m_RenderLayers[i]._f10 = nullptr;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal::ExecuteRenderBuffer(int a1, int a2, int a3)
{
    if (a1 > a2)
        return;

    DirectX::XMMATRIX mat = _A3A268;
    for (int i = 0; i < a1; ++i)
    {
        RenderLayer* layer = &m_RenderLayers[i];
        if (layer->_f10 || layer->ClearFlags)
        {
            //  NOTE: update renderer resolution if it's changed.
            if (layer->ViewportResolution_1.x != g_GfxInternal_Dx9->m_ViewportResolution_1.x ||
                layer->ViewportResolution_1.y != g_GfxInternal_Dx9->m_ViewportResolution_1.y ||
                layer->ViewportResolution_2.x != g_GfxInternal_Dx9->m_ViewportResolution.x ||
                layer->ViewportResolution_2.y != g_GfxInternal_Dx9->m_ViewportResolution.y)
                g_GfxInternal_Dx9->SetViewport(layer->ViewportResolution_1, layer->ViewportResolution_2);

            if (layer->ClearFlags)
                g_GfxInternal_Dx9->Clear(layer->ClearFlags, layer->ClearColor);

            g_GfxInternal_Dx9->SetProjection(layer->ProjectionMatrixParams.Fov, layer->ProjectionMatrixParams.XYRatio, layer->ProjectionMatrixParams.NearClip, layer->ProjectionMatrixParams.FarClip);
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
FrameBuffer* GfxInternal::_41F8F0(FrameBuffer* fb, unsigned int index)
{
    FrameBuffer* ret = (FrameBuffer*)m_RenderLayers[index]._f10;
    m_RenderLayers[index]._f10 = fb;

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

    MutableTextureBuffer* bwtexsurfmut = new MutableTextureBuffer(bwtexsurf);
    m_CheckerboardTextures.push_back(new Texture(bwtexsurfmut));
    delete bwtexsurfmut;

    //  NOTE: pink & yellow checkerboard texture.
    Surface* pytexsurf = new Surface(64, 64);

    for (unsigned int x = 0; x < 64; ++x)
        for (unsigned int y = 0; y < 64; ++y)
            pytexsurf->SetPixelColor(x, y, (x % 16 < 8) || (y % 16 < 8) ? BuiltinType::ColorPink : BuiltinType::ColorYellow );

    MutableTextureBuffer* pytexsurfmut = new MutableTextureBuffer(pytexsurf);
    m_CheckerboardTextures.push_back(new Texture(pytexsurfmut));
    delete pytexsurfmut;
}

void GfxInternal::GetViewMatrixForBufferIndex(DirectX::XMMATRIX& mat, const unsigned int ind) const
{
    mat = m_RenderLayers[ind].ViewMatrix;
}

void GfxInternal::_420390()
{
    constexpr float PI_6 = 0.52359879f; //  NOTE: Pi / 6
    constexpr float ONE_TWELFTH = 1.f / 12.f;

    unsigned int faceIndex = 0;
    unsigned int faceVertInd = 0;

    m_Mesh = new Mesh(1, 0, 1);
    m_Mesh->AddVertex(faceIndex++, { 0, 50, 0 }, { 0, 1, 0 }, { 0.5, 0.5 });
    m_Mesh->AddVertex(faceIndex++, { 0, -50, 0 }, { 0, -1, 0 }, { 0.5, 0.5 });

    for (unsigned int i = 0; i < 12; ++i)
    {
        m_Mesh->AddVertex(faceIndex++,
            { sinf((float)i * PI_6) * 2.f, 50.f, cosf((float)i * PI_6) },
            { 0.f, 1.f, 0.f },
            { sinf((float)i * PI_6) + 0.5f, cosf((float)i * PI_6) + 0.5f });

        m_Mesh->AddVertex(faceIndex++,
            { sinf((float)(i + 1) * PI_6), 50.f, cosf((float)(i + 1) * PI_6) },
            { 0.f, 1.f, 0.f },
            { sinf((float)(i + 1) * PI_6), cosf((float)(i + 1) * PI_6) });

        m_Mesh->SetVertexIndex(faceVertInd++, 0);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 1);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 2);

        m_Mesh->AddVertex(faceIndex++,
            { sinf((float)i * PI_6) * 2.f, -50.f, cosf((float)i * PI_6) },
            { 0.f, -1.f, 0.f },
            { sinf((float)i * PI_6) + 0.5f, cosf((float)i * PI_6) + 0.5f });

        m_Mesh->AddVertex(faceIndex++,
            { sinf((float)(i + 1) * PI_6), -50.f, cosf((float)(i + 1) * PI_6) },
            { -1.f, 0.f, 0.f },
            { sinf((float)(i + 1) * PI_6) + 0.5f, cosf((float)(i + 1) * PI_6) + 0.5f });

        m_Mesh->SetVertexIndex(faceVertInd++, 1);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 1);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 2);

        m_Mesh->AddVertex(faceIndex++,
            { sinf((float)i * PI_6) * 2.f, 50.f, cosf((float)i * PI_6) * 2.f },
            { sinf((float)i * PI_6), 0.f, cosf((float)i * PI_6) },
            { (float)i * ONE_TWELFTH, 0.f });
        m_Mesh->AddVertex(faceIndex++,
            { sinf((float)(i + 1) * PI_6) * 2.f, 50.f, cosf((float)(i + 1) * PI_6) },
            { sinf((float)(i + 1) * PI_6), 0.f, cosf((float)(i + 1) * PI_6) },
            { (float)(i + 1) * ONE_TWELFTH, 0.f });
        m_Mesh->AddVertex(faceIndex++,
            { sinf((float)i * PI_6) * 2.f, -50.f, cosf((float)i * PI_6) * 2.f },
            { sinf((float)i * PI_6), 0.f, cosf((float)i * PI_6) },
            { (float)(i + 1) * ONE_TWELFTH, 1.f });

        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 3);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 1);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 3);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex - 2);
        m_Mesh->SetVertexIndex(faceVertInd++, faceIndex++);
    }

    m_MeshBuffer = new MeshBuffer(m_Mesh, NULL);
}

void GfxInternal::GetBackBufferResolution(ScreenResolution& outResolution, const unsigned int bufferindex) const
{
    outResolution = m_RenderLayers[bufferindex].ViewportResolution_1;
}

const ScreenResolution& GfxInternal::GetViewportResolution() const
{
    return g_GfxInternal_Dx9->m_ViewportResolution;
}

void GfxInternal::SetBufferProjectionMatrixParams(const float fov, const float ratio, const float nearClip, const float farClip, const int32_t bufferIndex)
{
    if (bufferIndex == -1)
    {
        for (uint32_t i = 0; i < m_RenderBufferTotal; ++i)
            m_RenderLayers[i].ProjectionMatrixParamsVector = Vector4f(fov, ratio, nearClip, farClip);
    }
    else
    {
        m_RenderLayers[bufferIndex].ProjectionMatrixParamsVector = Vector4f(fov, ratio, nearClip, farClip);
    }
}

void GfxInternal::SetBufferViewMatrixByIndex(const DirectX::XMMATRIX& mat, const int32_t bufferIndex)
{
    if (bufferIndex == -1)
    {
        for (uint32_t i = 0; i < m_RenderBufferTotal; ++i)
        {
            m_RenderLayers[i].ViewMatrix = mat;
        }
    }
    else
    {
        m_RenderLayers[bufferIndex].ViewMatrix = mat;
    }
}

void GfxInternal::SetGameCameraMatrix(CameraMatrix* mat)
{
    g_GfxInternal_Dx9->m_GameCameraMatrix = mat;
}

AssetManager::RegionCode GfxInternal::GetRegion()
{
    return AssetManager::REGION_EUROPE;
}

bool GfxInternal::IsWideScreen()
{
    return WideScreen;
}

RenderLayer::RenderLayer(const Vector3f& bufferSize)
{
    MESSAGE_CLASS_CREATED(RenderLayer);

    FrameResolution = bufferSize;
    _f14 = 1;
    _f10 = nullptr;
    FrameBufferPtr = nullptr;
    _fDC = 0;
    ClearFlags = 0;
    ClearColor = BuiltinType::ColorBlack;

    ViewMatrix = DirectX::XMMatrixIdentity();
    MatrixUnk = DirectX::XMMatrixIdentity();
    MatrixUnk_1 = DirectX::XMMatrixIdentity();

    ProjectionMatrixParams.Fov = 70.0f;
    ProjectionMatrixParams.XYRatio = 1.f;
    ProjectionMatrixParams.NearClip = 1.f;
    ProjectionMatrixParams.FarClip = 1000.f;
}