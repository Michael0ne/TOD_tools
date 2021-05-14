#ifdef DIRECTX
#include "Globals.h"
#include "GfxInternal_Dx9.h"
#include "Window.h"
#include "GfxInternal.h"
#include "File.h"
#include "LogDump.h"
#include "InputKeyboard.h"
#include "VirtualHud.h"
#include "GfxInternal_Dx9_Surface.h"
#include "BuiltinType.h"
#include <directxtex\include\DDSTextureLoader9.h>
#include <DxErr.h>

GfxInternal_Dx9* g_GfxInternal_Dx9 = nullptr;
LPDIRECT3DDEVICE9 g_Direct3DDevice = nullptr;
bool GfxInternal_Dx9::ProcessingInput;
D3DCULL GfxInternal_Dx9::CullModes[3] = { D3DCULL_NONE, D3DCULL_CW, D3DCULL_CCW };
HMENU GfxInternal_Dx9::WindowMenu;
D3DFORMAT GfxInternal_Dx9::SupportedFormats[] = { D3DFMT_D24S8, D3DFMT_D24X8, D3DFMT_D24X4S4, D3DFMT_D16, D3DFMT_D32, D3DFMT_D15S1 };
float GfxInternal_Dx9::NoiseTime;
int GfxInternal_Dx9::NoiseState;
std::map<int, GfxInternal_Dx9_Texture*> GfxInternal_Dx9::RenderedTexturesMap;
const DirectX::XMMATRIX GfxInternal_Dx9::IdentityMatrix =
{
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
};

void GfxInternal_Dx9::GetScreenResolution(ScreenResolution& outRes)
{
    outRes = g_GfxInternal_Dx9->m_DisplayModeResolution;
}

GfxInternal_Dx9::GfxInternal_Dx9(const ScreenResolution& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int unk1)
{
    MESSAGE_CLASS_CREATED(GfxInternal_Dx9);

    m_ParticleOrient = { 0.f, 0.f, 0.f, 1.f };
    m_IdentityMatrix = IdentityMatrix;

    m_Direct3DInterface = Direct3DCreate9(DIRECT3D_VERSION);
    if (FAILED(m_Direct3DInterface))
        IncompatibleMachineParameterError(ERRMSG_DIRECTX9_NOT_FOUND, 0);

    m_RenderingScene = false;
    m_DeviceLost = false;
    m_ShouldCreateVerticies = 1;
    m_FSAA = FSAA;
    field_975C = -1;
    m_FramesyncQuery = nullptr;
    m_DeviceResetIssued = false;
    field_1B2 = 1;
    g_GfxInternal_Dx9 = this;

    if (m_Direct3DInterface->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_DeviceCaps) != D3D_OK)
        IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_VIDEOCARD, 0);

    GetDeviceCaps(m_Direct3DInterface, &m_DeviceCaps);

    ZeroMemory(m_TexturesArray, 16 * sizeof(LPDIRECT3DTEXTURE9));
    m_Direct3DDevice = 0;
    m_TexturesArray_2[0] = 0;
    m_TexturesArray_2[1] = 0;
    field_9700 = 0;
    field_9704 = 0;
    m_TextureStages[0] = 0;
    m_TextureStages[1] = 0;
    m_TextureStages[2] = 0;
    m_TextureStages[3] = 0;
    m_IndexBuffer = nullptr;
    m_CurrentVertexBuffer = nullptr;
    field_9670 = -1;
    field_9674 = 0;
    m_Direct3DVertexDeclaration = nullptr;
    field_9664 = 0;
    m_ViewportResolution = resolution;
    m_DisplayModeResolution = resolution;

    DisplayModeInfo DefaultDisplayModes[] = {
        {640, 480, false, 0, 0},
        {800, 600, false, 0, 0},
        {1024, 768, false, 0, 0},
        {1280, 1024, false, 0, 0},
#ifdef INCLUDE_FIXES
        {1920, 1080, false, 0, 0}
#else
        {1600, 1200, false, 0, 0}
#endif
    };

    for (unsigned int i = 0; i < (sizeof(DefaultDisplayModes) / sizeof(DefaultDisplayModes[0])); i++)
        m_DisplayModesList.push_back(DefaultDisplayModes[i]);

    EnumDisplayModes();

    if (!m_DisplayModeResolution.x || !m_DisplayModeResolution.y)
    {
        m_DisplayModeResolution = { 800, 600 };
        m_ViewportResolution = { 800, 600 };
        Vector2<unsigned int> res;

        if (GetRegistryResolution(res) && m_DisplayModesList.size())
            for (unsigned int i = 0; i < m_DisplayModesList.size(); i++)
                if (m_DisplayModesList[i].m_Available &&
                    m_DisplayModesList[i].m_Width == res.x &&
                    m_DisplayModesList[i].m_Height == res.y)
                    m_DisplayModeResolution = m_ViewportResolution = res;

        RememberResolution();
    }

    if (FSAA & 2)
        SetupWindowParamsAntialiased(m_DisplayModeResolution.x, m_DisplayModeResolution.y);
    else
        SetupWindowParamsNoAntiAliasing(m_DisplayModeResolution);

    field_9758 = 0.f;
    m_ActiveViewportSurfaceIndex = NULL;
    m_DrawingText = false;
    m_ActiveTextRenderBuffer = NULL;
    m_FlushDirectly = false;
}

GfxInternal_Dx9::~GfxInternal_Dx9()
{
    MESSAGE_CLASS_DESTROYED(GfxInternal_Dx9);

    LPDIRECT3DSURFACE9 depthsurf = nullptr;
    m_Direct3DDevice->GetDepthStencilSurface(&depthsurf);
    depthsurf->Release();

    if (m_DepthStencilSurface)
    {
        m_DepthStencilSurface->Release();
        m_DepthStencilSurface = nullptr;
    }

    for (unsigned int i = 0; i < (sizeof(m_ViewportTexturesArray) / sizeof(m_ViewportTexturesArray[0])); ++i)
        if (m_ViewportTexturesArray[i])
            delete m_ViewportTexturesArray[i];

    for (unsigned int i = 0; i < (sizeof(m_TexturesArray) / sizeof(m_TexturesArray[0])); ++i)
    {
        if (m_TexturesArray[i])
        {
            m_TexturesArray[i]->Release();
            m_TexturesArray[i] = nullptr;
        }
    }

    DestroyVertexBuffersObjects();
    
    m_Direct3DDevice->Release();
    m_Direct3DDevice = nullptr;

    m_Direct3DInterface->Release();
    m_Direct3DInterface = nullptr;

    delete m_ParticleMeshBuffer;
    g_GfxInternal_Dx9 = nullptr;
}

void GfxInternal_Dx9::SetFlushDirectly()
{
    m_FlushDirectly = !m_FlushDirectly;
    LogDump::LogA("FlushDirectly set to %i\n", m_FlushDirectly);
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::SetupViewportSurface()
{
    //if (m_ViewportTexturesArray[0])
        //SetRenderTarget(m_ViewportTexturesArray[m_ActiveViewportSurfaceIndex]);
}

void GfxInternal_Dx9::HandleDeviceLost()
{
    HRESULT presentresult = m_Direct3DDevice->Present(nullptr, nullptr, NULL, nullptr);

    if (FAILED(presentresult))
    {
        if (presentresult == D3DERR_DEVICELOST)
        {
            LogDump::LogA("Present: Lost the device..\n");
            m_DeviceLost = true;
        }
        LogDump::LogA("Couldn't blit from back to front buffer", presentresult);
    }

    if (field_1B2 && m_FramesyncQuery && !m_DeviceLost)
    {
        m_FramesyncQuery->Issue(D3DISSUE_END);
        m_DeviceResetIssued = true;
    }
}

void GfxInternal_Dx9::Clear(unsigned char flags, const ColorRGB& clearcolor)
{
    unsigned char flags_ = 0;
    if (flags & 1)
        flags_ = 1;
    if (flags & 2)
        flags_ |= 6;

    ResetTextures();
    m_Direct3DDevice->Clear(0, nullptr, flags_, COLOR_BGRA(clearcolor), 1.f, 0);
}

void GfxInternal_Dx9::SetZBias(unsigned int zbias)
{
    if (m_ZBias == zbias)
        return;

    m_ZBias = zbias;
    SetProjection(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
}

void GfxInternal_Dx9::SetFogProperties(unsigned int fogmode, const ColorRGB& color, float start, float end, float density)
{
    m_Direct3DDevice->SetRenderState(D3DRS_FOGCOLOR, COLOR_BGRA(color));

    if (fogmode)
    {
        m_Direct3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, fogmode == 1 ? D3DFOG_EXP : D3DFOG_EXP2);
        m_Direct3DDevice->SetRenderState(D3DRS_FOGDENSITY, (DWORD)density);
        m_FogDensity = density;
    }
    else
    {
        m_Direct3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
        m_Direct3DDevice->SetRenderState(D3DRS_FOGSTART, (DWORD)start);
        m_Direct3DDevice->SetRenderState(D3DRS_FOGEND, (DWORD)end);

        SetProjection(m_FOV, m_AspectRatio, m_NearPlane, end);
    }

    m_FogStart = start;
    m_FogEnd = end;
    m_FogColor = color;
    m_FogMode = fogmode;
}

void GfxInternal_Dx9::SetupRenderer()
{
    g_Direct3DDevice = m_Direct3DDevice;
    
    m_TexProperties[1].field_4 = 4;
    m_TexProperties[1].field_24 = 1;
    m_TexProperties[1].field_C = 2;
    m_TexProperties[1].field_10 = 2;
    m_TexProperties[1].field_14 = 0;
    m_TexProperties[1].field_8 = 4;
    m_TexProperties[1].field_18 = 2;
    m_TexProperties[1].field_1C = 2;
    m_TexProperties[1].field_20 = 3;

    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_COLORARG0, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_ALPHAARG0, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    m_TexProperties[1].field_4C = 1;
    m_TexProperties[1].field_2C = 1;
    m_TexProperties[1].field_34 = 2;
    m_TexProperties[1].field_38 = 2;
    m_TexProperties[1].field_3C = 0;
    m_TexProperties[1].field_30 = 1;
    m_TexProperties[1].field_40 = 2;
    m_TexProperties[1].field_44 = 2;
    m_TexProperties[1].field_48 = 3;

    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_COLOROP, D3DTOP_DISABLE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_COLORARG0, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_ALPHAARG0, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_Direct3DDevice->SetTextureStageState(m_TexProperties[1].field_28, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    m_TexProperties[1].field_54 = 0;
    m_TexProperties[1].field_58 = 0;
    m_TexProperties[1].field_64 = 0;
    m_TexProperties[1].field_68 = 0;
    m_TexProperties[1].field_5C[1] = 1;
    m_TexProperties[1].field_6C[1] = 1;

    g_Direct3DDevice->SetSamplerState(m_TexProperties[1].field_50, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    g_Direct3DDevice->SetSamplerState(m_TexProperties[1].field_50, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    g_Direct3DDevice->SetSamplerState(m_TexProperties[1].field_60, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    g_Direct3DDevice->SetSamplerState(m_TexProperties[1].field_60, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    m_TexProperties[1].m_LightingEnabled = false;
    m_TexProperties[1].field_8A = 1;
    m_TexProperties[1].m_CullMode = 1;
    m_TexProperties[1].m_ZWrite = true;
    m_TexProperties[1].m_ZTest = true;
    m_TexProperties[1].m_AlphaBlend = 0;

    g_Direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_Direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    g_Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    g_Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    memcpy(&m_TexProperties[0], &m_TexProperties[1], sizeof(RenderProperties));

    m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_Direct3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU | 1);
    m_Direct3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
    m_Direct3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, -1);
    m_Direct3DDevice->SetRenderState(D3DRS_LASTPIXEL, FALSE);
    
    SetProjection(70, 1, 1, 1000);
    
    m_Direct3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    m_Direct3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));

    material.Ambient = { 1, 1, 1, 1 };
    material.Diffuse = { 1, 1, 1, 1 };
    material.Specular = { 1, 1, 1, 1 };
    material.Power = 10;

    m_Direct3DDevice->SetMaterial(&material);

    m_Direct3DDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
    m_Direct3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
    m_Direct3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
    m_Direct3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
    m_Direct3DDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

    if (m_SeparateAlphaBlend)
        m_Direct3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
    else
        m_Direct3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

    m_LightingEnabled = false;
    m_TexProperties[0].m_LightingEnabled = false;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    m_AlphaTestThreshhold = 0.0099999998f;
    m_Direct3DDevice->SetRenderState(D3DRS_ALPHAREF, 2);
    m_Filter = 1;
    m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_BlendMode = 0;
    m_Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_AlphaChannelEnabled = false;
    m_TexProperties[0].m_AlphaBlend = 0;
    m_TexProperties[0].field_8 = 3;
    m_TexProperties[0].field_24 = 1;
    m_TextureAddressModes[0] = 0;
    m_TexProperties[0].field_54 = 0;
    m_TexProperties[0].field_5C[1] = 1;
    m_TexProperties[0].field_50 = 0;
    
    if (m_FlushDirectly)
    {
        g_Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    }

    m_EnvironmentMapColors = { 1, 1, 1, 0 };
    m_EnvironmentMapCoefficient = 1;
    m_EnvironmentMapOpacity = 0;
    SetEnvironmentMapOpacity(1);

    m_AlphaTestEnabled = true;
    m_TexProperties[0].m_AlphaTest = 1;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    SetWorldMatrix(&IdentityMatrix);

    if (m_ZBias)
    {
        m_ZBias = 0;
        SetProjection(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
    }

    m_MipMapBias = -2;
    m_FogEnabled = 0;
    m_Direct3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
    
    m_WireframeColor = D3DCOLOR_ARGB(0, 0, 1, 1);
    SetRenderStateWireframe(false);

    m_MipMappingEnabled = true;
    m_TexProperties[0].field_5C[0] = 1;
    m_TexProperties[0].field_5C[1] = 1;
    m_TexProperties[0].field_6C[0] = 1;
    m_TexProperties[0].field_6C[1] = 1;

    if (m_FlushDirectly)
    {
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_60, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    }

    SetFogProperties(0, BuiltinType::ColorWhite, 1, 100, 0.050000001f);

    if (m_TexturesArray_2[0])
    {
        m_TextureStages[0] = 1;
        m_TexturesArray_2[0] = 0;
    }

    if (m_TexturesArray_2[1])
    {
        m_TextureStages[1] = 1;
        m_TexturesArray_2[1] = 0;
    }

    m_EnvironmentMapCoefficient = 1;
    m_EnvironmentMapColors = { 1, 1, 1, m_EnvironmentMapOpacity };
    ToggleEnvironmentMap(false);
}

void GfxInternal_Dx9::DestroyVertexBuffersObjects()
{
    for (unsigned int i = 0; i < 4; ++i)
        if (m_VertexBuffer[i])
            delete m_VertexBuffer[i];
}

void GfxInternal_Dx9::ResetStream()
{
    if (m_DeviceLost)
        return;

    m_Direct3DDevice->SetIndices(nullptr);
    m_Direct3DDevice->SetStreamSource(0, nullptr, 0, 0);
    m_Direct3DDevice->SetStreamSource(1, nullptr, 0, 0);

    if (m_TexturesArray_2[0])
    {
        m_TextureStages[0] = 1;
        m_TexturesArray_2[0] = 0;
    }

    if (m_TexturesArray_2[1])
    {
        m_TextureStages[1] = 1;
        m_TexturesArray_2[1] = 0;
    }

    ResetTextures();
}

GfxInternal_Dx9::DisplayModeInfo* GfxInternal_Dx9::IsScreenResolutionAvailable(unsigned int width, unsigned int height, bool dontignoreunavailable)
{
    if (m_DisplayModesList.size() == 0)
        return nullptr;

    for (unsigned int i = NULL; i != m_DisplayModesList.size(); i++)
            if (m_DisplayModesList[i].m_Width == width &&
                m_DisplayModesList[i].m_Height == height &&
                m_DisplayModesList[i].m_Available ||
                !dontignoreunavailable)
                return &m_DisplayModesList[i];

    return nullptr;
}

void GfxInternal_Dx9::EnumDisplayModes()
{
    //	TODO: display modes are hardcoded, maybe add option to change it?
    int adapterModesTotal = m_Direct3DInterface->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
    int adapterMode = 0;
    D3DDISPLAYMODE adapterModes;

    for (int adapterIndex = 0; adapterIndex < adapterModesTotal; ++adapterIndex)
    {
        ZeroMemory(&adapterModes, sizeof(adapterModes));
        m_Direct3DInterface->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, adapterMode, &adapterModes);
        unsigned int ind = 0;

        if (m_DisplayModesList.size() > 0)
        {
            for (std::vector<DisplayModeInfo>::iterator it = m_DisplayModesList.begin(); it != m_DisplayModesList.end(); ++it, ++ind)
                if (it->m_Width == adapterModes.Width && it->m_Height == adapterModes.Height)
                    break;

            if (ind >= m_DisplayModesList.size())
            {
                adapterMode = adapterIndex + 1;
                continue;
            }

            DisplayModeInfo* mode_sel = &m_DisplayModesList[ind];

            if (mode_sel->m_RefreshRate <= 85 && adapterModes.RefreshRate > mode_sel->m_RefreshRate)
            {
                mode_sel->m_Available = true;
                mode_sel->m_RefreshRate = adapterModes.RefreshRate;
                mode_sel->m_Format = adapterModes.Format;
            }
        }

        adapterMode = adapterIndex + 1;
    }

    LogDump::LogA("Kapow will use these modes when in full-screen:\n");

    for (unsigned int i = 0; i < m_DisplayModesList.size(); ++i)
        LogDump::LogA("%ix%i @ %iHz - format=%i, available=%i\n",
            m_DisplayModesList[i].m_Width,
            m_DisplayModesList[i].m_Height,
            m_DisplayModesList[i].m_RefreshRate,
            m_DisplayModesList[i].m_Format,
            m_DisplayModesList[i].m_Available);
}

char GfxInternal_Dx9::GetLightStatus(const LightStatus* lightptr) const
{
    if (m_SceneLights.size() <= 0)
        return NULL;

    for (unsigned int i = 0; i < m_SceneLights.size(); ++i)
        if (&m_SceneLights[i] == lightptr)
            return m_SceneLights[i].m_Enabled;

    return NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::CreateSurfaces()
{
    if (!m_ShouldCreateVerticies)
        return;

    m_SurfaceDoubleSized = false;

    if (m_FSAA & 0x200)
    {
        m_SurfaceDoubleSized = true;
        ScreenResolution res = { m_DisplayModeResolution.x * 2, m_DisplayModeResolution.y * 2 };
        m_ViewportTexturesArray[0] = new GfxInternal_Dx9_Texture(res, 1, 3);
        m_ViewportTexturesArray[1] = new GfxInternal_Dx9_Texture(res, 1, 3);
        m_ViewportTexturesArray[2] = new GfxInternal_Dx9_Texture(m_DisplayModeResolution, 1, 3);
        m_ViewportTexturesArray[3] = new GfxInternal_Dx9_Texture(m_DisplayModeResolution, 1, 3);
    }
    else
    {
        m_SurfaceDoubleSized = false;
        ScreenResolution res = { m_DisplayModeResolution.x / 2, m_DisplayModeResolution.y / 2 };
        m_ViewportTexturesArray[0] = new GfxInternal_Dx9_Texture(m_DisplayModeResolution, 1, 3);
        m_ViewportTexturesArray[1] = new GfxInternal_Dx9_Texture(m_DisplayModeResolution, 1, 3);
        m_ViewportTexturesArray[2] = new GfxInternal_Dx9_Texture(res, 1, 3);
        m_ViewportTexturesArray[3] = new GfxInternal_Dx9_Texture(res, 1, 3);
    }

    m_DepthStencilSurface = nullptr;

    //	NOTE: go through all supported formats and try to create first available depth surface.
    for (const D3DFORMAT* format = SupportedFormats; format; format++)
    {
        LogDump::LogA("Creating depth surface of size (%i,%i)\n", m_ViewportTexturesArray[0]->m_SurfaceSize.x, m_ViewportTexturesArray[0]->m_SurfaceSize.y);

        if (m_DepthStencilSurface)
        {
            m_DepthStencilSurface->Release();
            m_DepthStencilSurface = nullptr;
        }

        if (SUCCEEDED(m_Direct3DDevice->CreateDepthStencilSurface(
            m_ViewportTexturesArray[0]->m_SurfaceSize.x,
            m_ViewportTexturesArray[0]->m_SurfaceSize.y,
            *format,
            D3DMULTISAMPLE_NONE,
            0,
            TRUE,
            &m_DepthStencilSurface,
            nullptr)))
            break;
    }

    if (FAILED(m_Direct3DDevice->CreateQuery(D3DQUERYTYPE_EVENT, &m_FramesyncQuery)))
    {
        LogDump::LogA("NOTE: Could not create a frameSync query\n");
        m_FramesyncQuery = nullptr;
    }

    //_464DC0();	//	NOTE: fills vertex map with empty verticies?
    //_4606D0();	//	NOTE: fills yet another vertex map with empty verticies.
    SetupRenderer();	//	NOTE: finalize Direct 3D initialization.

    m_ShouldCreateVerticies = false;
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::CreateParticleMeshBuffer()
{
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::RenderTexturedQuad2D_4(const GfxInternal_Dx9_Texture&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&, const ColorRGB&, const ColorRGB&, const ColorRGB&)
{
}

void GfxInternal_Dx9::EndParticleSystem(bool a1)
{
    if (a1)
    {
        m_ParticleRenderDisabled = true;
        m_ParticleSystemEnded = a1;
    }
    else
    {
        m_ParticleRenderDisabled = false;
        if (!m_ParticleMeshBuffer)
            CreateParticleMeshBuffer();
    }
}

void GfxInternal_Dx9::SetupWindowParamsAntialiased(unsigned int width, unsigned int height)
{
    const DisplayModeInfo* mode = IsScreenResolutionAvailable(width, height, true);

    if (mode || (mode = IsScreenResolutionAvailable(640, 480, true)) != 0)
    {
        m_Windowed = true;
        m_DisplayModeFormat = (D3DFORMAT)mode->m_Format;	//	D3DFORMAT is just a enum
        m_DisplayModeResolution.x = mode->m_Width;
        m_DisplayModeResolution.y = mode->m_Height;

        g_Window->SetWindowResolutionRaw(m_DisplayModeResolution);
        ZeroMemory(&m_PresentParameters, sizeof(D3DPRESENT_PARAMETERS));

        m_PresentParameters.Windowed = false;
        m_PresentParameters.BackBufferWidth = m_DisplayModeResolution.x;
        m_PresentParameters.BackBufferHeight = m_DisplayModeResolution.y;
        m_PresentParameters.BackBufferFormat = m_DisplayModeFormat;
        m_PresentParameters.FullScreen_RefreshRateInHz = mode->m_RefreshRate;

        CreateRenderDevice();
    }
}

void GfxInternal_Dx9::SetupWindowParamsNoAntiAliasing(const ScreenResolution mode)
{
    D3DDISPLAYMODE _mode;

    m_Windowed = false;
    g_Window->SetWindowResolutionDontMove(mode);

    char* errorstr = nullptr;
    HRESULT result = m_Direct3DInterface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &_mode);
    if (SUCCEEDED(result))
    {
        ZeroMemory(&m_PresentParameters, sizeof(D3DPRESENT_PARAMETERS));

        m_DisplayModeFormat = _mode.Format;
        m_PresentParameters.Windowed = true;
        m_PresentParameters.BackBufferFormat = _mode.Format;
        m_PresentParameters.FullScreen_RefreshRateInHz = 0;

        CreateRenderDevice();
    }
    else
    {
        Utils::GetDXErrorString(result, errorstr);
        MessageBoxA(g_Window->m_WindowHandle, errorstr, "Couldn't get current display mode", NULL);
    }
}

bool GfxInternal_Dx9::SetScreenResolution(unsigned int width, unsigned int height)
{
    if (m_DisplayModeResolution.x != width || m_DisplayModeResolution.y != height)
    {
        if (m_Windowed)
        {
            const DisplayModeInfo* dispmode = IsScreenResolutionAvailable(width, height, true);
            if (!dispmode)
                return false;

            m_DisplayModeResolution = { width, height };
            m_DisplayModeFormat = (D3DFORMAT)dispmode->m_Format;
            g_Window->SetWindowResolutionRaw(m_DisplayModeResolution);
            m_PresentParameters.FullScreen_RefreshRateInHz = dispmode->m_RefreshRate;
            m_PresentParameters.BackBufferFormat = (D3DFORMAT)dispmode->m_Format;
        }
        else
        {
            m_DisplayModeResolution = { width, height };
            m_PresentParameters.BackBufferWidth = width;
            m_PresentParameters.BackBufferHeight = height;
            Reset();
            RememberResolution();
        }
    }

    VirtualHud::VirtualHudInstance.SetVirtualHudScreenSize((float)width, (float)height, GfxInternal::WideScreen ? (((float)width * 0.0625f) / ((float)height * 0.1111f)) : m_AspectRatio, 1.f);
    return true;
}

bool GfxInternal_Dx9::SetupScreenRes()
{
    bool windowed = m_Windowed;
    if (!windowed &&
        IsScreenResolutionAvailable(m_DisplayModeResolution.x, m_DisplayModeResolution.y, true))
        return false;

    m_Windowed = !windowed;
    m_PresentParameters.Windowed = !windowed;

    if (windowed)
    {
        SetMenu(g_Window->m_WindowHandle, WindowMenu);
        g_Window->SetWindowResolutionDontMove(m_DisplayModeResolution);
    }
    else
    {
        WindowMenu = GetMenu(g_Window->m_WindowHandle);
        SetMenu(g_Window->m_WindowHandle, NULL);
        g_Window->SetWindowResolutionRaw(m_DisplayModeResolution);
    }

    if (m_PresentParameters.Windowed)
    {
        D3DDISPLAYMODE dispmode;
        HRESULT dispmoderes = m_Direct3DInterface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dispmode);
        char* errorstr = nullptr;
        if (FAILED(dispmoderes))
        {
            Utils::GetDXErrorString(dispmoderes, errorstr);
            MessageBox(g_Window->m_WindowHandle, errorstr, "Couldn't get current display mode", MB_OK);
            return false;
        }

        m_DisplayModeFormat = dispmode.Format;
        m_PresentParameters.FullScreen_RefreshRateInHz = 0;
    }
    else
    {
        DisplayModeInfo& dispmode = *IsScreenResolutionAvailable(m_DisplayModeResolution.x, m_DisplayModeResolution.y, true);
        m_DisplayModeFormat = (D3DFORMAT)dispmode.m_Format;
        m_PresentParameters.FullScreen_RefreshRateInHz = dispmode.m_RefreshRate;
    }

    m_PresentParameters.BackBufferWidth = m_DisplayModeResolution.x;
    m_PresentParameters.BackBufferHeight = m_DisplayModeResolution.y;
    m_PresentParameters.BackBufferFormat = m_DisplayModeFormat;

    ShowCursor(m_PresentParameters.Windowed);

    Reset();

    return true;
}

bool GfxInternal_Dx9::GetRegistryResolution(Vector2<unsigned int>& mode)
{
    HKEY phkResult;
    DWORD type;
    DWORD cbdata;
    unsigned int x, y;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, Window::RegistryKey, 0, 1, &phkResult))
        return false;

    if (RegQueryValueExA(phkResult, "XRes", 0, &type, (LPBYTE)&x, &cbdata) < 0 ||
        type != 4 ||
        RegQueryValueExA(phkResult, "YRes", 0, &type, (LPBYTE)&y, &cbdata) < 0 ||
        type != 4)
        return false;

    mode.x = x;
    mode.y = y;

    RegCloseKey(phkResult);

    return true;
}

bool GfxInternal_Dx9::ProcessGameInput()
{
    if (!ProcessingInput)
        return ProcessingInput;

    g_Window->ProcessMessages();

    if (g_InputKeyboard)
    {
        g_InputKeyboard->Process();
        g_InputKeyboard->Reset();

        if (g_InputKeyboard->m_Acquired && g_InputKeyboard->m_ButtonStates[VK_PAUSE] & 1)
            ProcessingInput = false;
    }

    return ProcessingInput;
}

void GfxInternal_Dx9::SetFVF(unsigned int fvf)
{
    if (fvf == m_FVF)
        return;

    m_Direct3DDevice->SetFVF(fvf);
    m_FVF = fvf;
    m_Direct3DVertexDeclaration = nullptr;
}

void GfxInternal_Dx9::SetVertexDeclaration(IDirect3DVertexDeclaration9* vertdec)
{
    if (m_Direct3DVertexDeclaration == vertdec)
        return;

    m_Direct3DDevice->SetVertexDeclaration(vertdec);
    m_Direct3DVertexDeclaration = vertdec;
    m_FVF = -1;
}

void GfxInternal_Dx9::IssueDeviceReset()
{
    if (m_FramesyncQuery && !m_DeviceLost)
    {
        m_FramesyncQuery->Issue(1);
        m_DeviceResetIssued = true;
    }
}

void GfxInternal_Dx9::ProcessFramesyncQuery()
{
    if (m_FramesyncQuery && !m_DeviceLost)
    {
        if (m_DeviceResetIssued)
            while (m_FramesyncQuery->GetData(nullptr, NULL, D3DGETDATA_FLUSH) == 1);
        m_DeviceResetIssued = false;
    }
}

void GfxInternal_Dx9::EndScene()
{
    if (m_RenderingScene)
        m_Direct3DDevice->EndScene();
    m_RenderingScene = false;
}

void GfxInternal_Dx9::SetTextureStage1TransformationMatrix() const
{
    DirectX::XMMATRIX mat;

    mat.r[0] =
    {
        (m_ViewMatrix.r[0].m128_f32[0] * -0.5f) + (m_ViewMatrix.r[0].m128_f32[3] * 0.5f),
        (m_ViewMatrix.r[0].m128_f32[1] * -0.5f) + (m_ViewMatrix.r[0].m128_f32[3] * 0.5f),
        0,
        0
    };

    mat.r[1] =
    {
        (m_ViewMatrix.r[1].m128_f32[0] * -0.5f) + (m_ViewMatrix.r[1].m128_f32[3] * 0.5f),
        (m_ViewMatrix.r[1].m128_f32[1] * -0.5f) + (m_ViewMatrix.r[1].m128_f32[3] * 0.5f),
        0,
        0
    };

    mat.r[2] =
    {
        (m_ViewMatrix.r[2].m128_f32[0] * -0.5f) + (m_ViewMatrix.r[2].m128_f32[3] * 0.5f),
        (m_ViewMatrix.r[2].m128_f32[1] * -0.5f) + (m_ViewMatrix.r[2].m128_f32[3] * 0.5f),
        0,
        0
    };

    mat.r[3] =
    {
        (m_ViewMatrix.r[3].m128_f32[0] * -0.5f) + (m_ViewMatrix.r[3].m128_f32[3] * 0.5f),
        (m_ViewMatrix.r[3].m128_f32[1] * -0.5f) + (m_ViewMatrix.r[3].m128_f32[3] * 0.5f),
        0,
        0
    };

    m_Direct3DDevice->SetTransform(D3DTS_TEXTURE1, (const D3DMATRIX*)&mat);
}

void GfxInternal_Dx9::ToggleEnvironmentMap(bool enable)
{
    if (enable)
    {
        m_Direct3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, COLOR_BGRA(m_EnvironmentMapColors));

        m_TexProperties[0].field_34 = 1;
        m_TexProperties[0].field_4C = 1;
        m_TexProperties[0].field_38 = 2;
        m_TexProperties[0].field_3C = 3;
        m_TexProperties[0].field_2C = 25;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_28, D3DTSS_COLORARG0, 1);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_28, D3DTSS_COLORARG1, 2);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_28, D3DTSS_COLORARG2, 3);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_28, D3DTSS_COLOROP, 25);
        }

        m_Direct3DDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        m_Direct3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
        SetTextureStage1TransformationMatrix();
        m_EnvironmentMapEnabled = enable;
    }
    else
    {
        m_Direct3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, -1);

        m_TexProperties[0].field_4C = 1;
        m_TexProperties[0].field_38 = 2;
        m_TexProperties[0].field_3C = 1;
        m_TexProperties[0].field_2C = 25;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_28, D3DTSS_COLORARG1, 2);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_28, D3DTSS_COLORARG2, 1);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_28, D3DTSS_COLOROP, 25);
        }

        m_Direct3DDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        m_Direct3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
        
        DirectX::XMMATRIX matident = DirectX::XMMatrixIdentity();
        m_Direct3DDevice->SetTransform(D3DTS_TEXTURE1, (const D3DMATRIX*)&matident);

        m_EnvironmentMapEnabled = false;
    }
}

void GfxInternal_Dx9::SetEnvironmentMapCoef(float coef)
{
    m_EnvironmentMapCoefficient = coef;
    m_EnvironmentMapColors = { coef, coef, coef, m_EnvironmentMapOpacity };
}

void GfxInternal_Dx9::RememberResolution()
{
    HKEY phkResult;
    LPDWORD disposition = 0;

    if (!RegCreateKeyExA(HKEY_CURRENT_USER, Window::RegistryKey, 0, 0, 0, 0xF003F, 0, &phkResult, disposition)) {
        RegSetValueExA(phkResult, "XRes", 0, 4, (const BYTE*)&m_DisplayModeResolution.x, 4);
        RegSetValueExA(phkResult, "YRes", 0, 4, (const BYTE*)&m_DisplayModeResolution.y, 4);
        RegCloseKey(phkResult);
    }
}

void GfxInternal_Dx9::SetCullMode(unsigned int cullmode)
{
#ifdef INCLUDE_FIXES
    //	NOTE: sanity check.
    m_TexProperties[0].m_CullMode = cullmode > 2 ? 0 : cullmode;
#else
    m_TexProperties[0].m_CullMode = cullmode;
#endif
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_CULLMODE, CullModes[cullmode]);
}

HRESULT GfxInternal_Dx9::GetDeviceCaps(IDirect3D9* d3d, D3DCAPS9* d3dcaps)
{
    m_ShaderCapabilities.m_VertexShader11 = d3dcaps->VertexShaderVersion >= 0xFFFE0101;
    m_ShaderCapabilities.m_VertexShader20 = d3dcaps->VertexShaderVersion >= 0xFFFE0200;
    m_ShaderCapabilities.m_PixelShader13 = d3dcaps->PixelShaderVersion >= 0xFFFF0103;
    m_ShaderCapabilities.m_PixelShader20 = d3dcaps->PixelShaderVersion >= 0xFFFF0200;

    m_MaxVertexShaderConst = d3dcaps->MaxVertexShaderConst;

    m_MipmapLodBias = (d3dcaps->RasterCaps >> 13) & 1;
    m_TextureRequiresPow2 = (d3dcaps->TextureCaps >> 1) & 1;
    m_Anisotropy = (d3dcaps->RasterCaps >> 17) & 1;
    m_MinAnisotropy = (d3dcaps->TextureFilterCaps >> 10) & 1;
    m_MagAnisotropy = (d3dcaps->TextureFilterCaps >> 26) & 1;
    m_MaxAnisotropy = d3dcaps->MaxAnisotropy;
    m_BlendOP = (d3dcaps->PrimitiveMiscCaps >> 1) & 1;
    m_FogAndSpecularAlpha = (char)(d3dcaps->PrimitiveMiscCaps) & 1;
    m_SeparateAlphaBlend = (char)(d3dcaps->RasterCaps) & 1;
    m_RangeFog = (d3dcaps->PrimitiveMiscCaps >> 17) & 1;

    LogDump::LogA("CAPS: VS11=%i, VS20=%i, PS13=%i, PS20=%i\n", m_ShaderCapabilities.m_VertexShader11, m_ShaderCapabilities.m_VertexShader20, m_ShaderCapabilities.m_PixelShader13, m_ShaderCapabilities.m_PixelShader20);
    LogDump::LogA("CAPS: Max vertex shader constants = %i\n", m_MaxVertexShaderConst);
    LogDump::LogA("CAPS: Mipmap LOD bias = %i\n", m_MipmapLodBias);
    LogDump::LogA("CAPS: Texture requires pow2 = %i\n", m_TextureRequiresPow2);
    LogDump::LogA("CAPS: Anisotropic filtering=%i (min=%i, mag=%i), Max anisotropy=%i\n", m_Anisotropy, m_MinAnisotropy, m_MagAnisotropy, m_MaxAnisotropy);
    LogDump::LogA("CAPS: BlendOP = %i\n", m_BlendOP);
    LogDump::LogA("CAPS: FogAndSpecularAlpha = %i\n", m_FogAndSpecularAlpha);
    LogDump::LogA("CAPS: RangeFog = %i\n", m_SeparateAlphaBlend);;
    LogDump::LogA("CAPS: SeparateAlphaBlend = %i\n", m_RangeFog);

    return d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, NULL, D3DRTYPE_TEXTURE, D3DFMT_DXT1);
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::DestroySurfaces()
{
    if (m_ShouldCreateVerticies)
        return;

    if (m_DepthStencilSurface)
    {
        m_DepthStencilSurface->Release();
        m_DepthStencilSurface = nullptr;
    }

    for (unsigned int i = 0; i < (sizeof(m_ViewportTexturesArray) / sizeof(m_ViewportTexturesArray[0])); ++i)
        delete m_ViewportTexturesArray[i];

    if (m_FramesyncQuery)
    {
        m_FramesyncQuery->Release();
        m_FramesyncQuery = nullptr;
    }

    //_464D60();
    //_460670();

    m_FVF = -1;
    m_Direct3DVertexDeclaration = nullptr;
    m_ShouldCreateVerticies = true;
}

void GfxInternal_Dx9::_44EE40()
{
    ++field_9674;
}

void GfxInternal_Dx9::SetTextureScroll(float* a1, float a2)
{
    if (a2 != 0.f)
    {
        DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();

        mat.r[2].m128_f32[1] = (*(a1 + 6) * a2) / *(a1 + 6);
        m_Direct3DDevice->SetTransform(D3DTS_TEXTURE0, (const D3DMATRIX*)&mat);
        m_Direct3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    }
    else
        m_Direct3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
}

void GfxInternal_Dx9::SetParticleSize(unsigned int size)
{
    m_ParticleSize = size;
}

void GfxInternal_Dx9::SetParticleOrient(const Quaternion<float>& orient)
{
    m_ParticleOrient = *(Vector4f*)&orient;
}

void GfxInternal_Dx9::SetParticleAlign(unsigned int alignment)
{
    m_ParticleAlign = alignment;
}

void GfxInternal_Dx9::SetParticleColor(unsigned int clr)
{
    m_ParticleColor = clr;
}

IDirect3DVertexBuffer9* GfxInternal_Dx9::CreateVertexBuffer(unsigned int length, unsigned int usage, unsigned int fvf, IDirect3DVertexBuffer9* vb)
{
    m_Direct3DDevice->CreateVertexBuffer(length, usage, fvf, D3DPOOL_MANAGED, &vb, nullptr);
    return vb;
}

IDirect3DIndexBuffer9* GfxInternal_Dx9::CreateIndexBuffer(unsigned int length, unsigned int usage, IDirect3DIndexBuffer9* ib)
{
    m_Direct3DDevice->CreateIndexBuffer(length, usage, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib, nullptr);
    return ib;
}

IDirect3DTexture9* GfxInternal_Dx9::CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned int usage, D3DFORMAT format, IDirect3DTexture9* outTexture)
{
    m_Direct3DDevice->CreateTexture(width, height, levels, usage, format, D3DPOOL_MANAGED, &outTexture, nullptr);
    return outTexture;
}

void GfxInternal_Dx9::ResetTextures()
{
    for (unsigned int i = 0; i < 2; ++i)
    {
        if (m_TextureStages[i] == 1)
        {
            if (m_TexturesArray_2[i])
                m_TexturesArray_2[i]->SetTextureForStage(i);
            else
                m_Direct3DDevice->SetTexture(i, nullptr);
            
            m_TextureStages[i] = 0;
        }
    }

    if (!m_FlushDirectly)
        m_TexProperties[0] = m_TexProperties[1];
}

void GfxInternal_Dx9::Reset()
{
    m_DeviceLost = true;
    DestroySurfaces();

    char* errorstr = nullptr;
    HRESULT resetresult = m_Direct3DDevice->Reset(&m_PresentParameters);

    if (SUCCEEDED(resetresult))
    {
        LogDump::LogA("Device reset correctly\n");
        CreateSurfaces();
        m_DeviceLost = false;
    }
    else
    {
        Utils::GetDXErrorString(resetresult, errorstr);
        LogDump::LogA("%s: %s\n", "pd3dDevice->Reset", errorstr);
    }
}

bool GfxInternal_Dx9::BeginScene()
{
    if (m_RenderingScene)
        return m_RenderingScene;

    HRESULT cooplevel = m_Direct3DDevice->TestCooperativeLevel();

    if (cooplevel == D3DERR_DEVICELOST)
    {
        LogDump::LogA("GfxInternal_Dx9::BeginScene - Device is currently lost.. wait til we get it back..\n");
        m_DeviceLost = true;

        return false;
    }

    if (cooplevel == D3DERR_DEVICENOTRESET)
    {
        LogDump::LogA("GfxInternal_Dx9::BeginScene - Device is lost, but we can now reset it..\n");
        Reset();
    }

    if (!m_DeviceLost)
    {
        if (field_1B2)
            ProcessFramesyncQuery();
        if (SUCCEEDED(m_Direct3DDevice->BeginScene()))
            m_RenderingScene = true;
    }

    return m_RenderingScene;
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::CreateRenderDevice()
{
    int behaviourFlags;

    m_PresentParameters.EnableAutoDepthStencil = false;
    m_PresentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;
    m_PresentParameters.BackBufferCount = 1;
    m_PresentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    if (m_Windowed)
        m_PresentParameters.PresentationInterval = field_1B2 != 0 ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    else
    {
        m_PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;;
        field_1B2 = 0;
    }

    if (m_Direct3DDevice)
    {
        m_Direct3DDevice->Release();
        m_Direct3DDevice = nullptr;
    }

    if (m_DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        LogDump::LogA("Creating a HW device.\n");
        behaviourFlags = 64;
    }
    else
    {
        LogDump::LogA("Creating a SW device.\n");
        behaviourFlags = 32;
    }

    m_Direct3DInterface->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        g_Window->m_WindowHandle,
        behaviourFlags,
        &m_PresentParameters,
        &m_Direct3DDevice);

    m_ShouldCreateVerticies = 1;

    CreateSurfaces();
    //CreateVertexBufferObjects(1024);
    CreateParticleMeshBuffer();

    const Vector2f res = { (float)m_ViewportResolution.x, (float)m_ViewportResolution.y };

    if (g_GfxInternal->WideScreen)
        VirtualHud::VirtualHudInstance.SetHudProperties(res, (float)((m_ViewportResolution.x * 0.0625) / (m_ViewportResolution.y * 0.1111)), 1.0f);
    else
        VirtualHud::VirtualHudInstance.SetHudProperties(res, m_AspectRatio, 1.0f);
}

void GfxInternal_Dx9::RenderTexturedQuad2D_2(const GfxInternal_Dx9_Texture& tex, const Vector2<float>& topleft, const Vector2<float>& topright, const Vector2<float>& bottomleft, const Vector2<float>& bottomright, const ColorRGB& clr)
{
    RenderTexturedQuad2D_4(
        tex,
        topleft,
        topright,
        bottomleft,
        bottomright,
        {0, 0},
        {0, ((float)tex.m_Resolution.y / (float)tex.m_SurfaceSize.y)},
        {((float)tex.m_Resolution.x / (float)tex.m_SurfaceSize.y), ((float)tex.m_Resolution.x / (float)tex.m_SurfaceSize.y)},
        {((float)tex.m_Resolution.x / (float)tex.m_SurfaceSize.y), 0},
        clr,
        clr,
        clr,
        clr
        );
}

void GfxInternal_Dx9::_45C790(float a1)
{
    if (a1 < 0.2f)
        return;

    m_TextureAddressModes[0] = 1;
    m_TexProperties[0].field_54 = 1;
    m_TexProperties[0].field_5C[1] = 1;
    m_TexProperties[0].field_58 = 1;

    if (m_FlushDirectly)
    {
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    }

    if (a1 >= 0.8f)
        a1 = 0.8f;

    m_Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    RenderTexturedQuad2D_2(
        m_ViewportTexturesArray[m_ActiveViewportSurfaceIndex],
        { 0, 0 },
        { 0, (float)m_ViewportResolution.y },
        { (float)m_ViewportResolution.x, (float)m_ViewportResolution.y },
        { (float)m_ViewportResolution.x, 0 },
        { 1, 1, 1, powf(a1,g_GfxInternal->m_TimeDelta * 33.333336f) }
    );

    if (m_BlendMode == 1)
    {
        m_Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    m_TextureAddressModes[0] = 0;
    m_TexProperties[0].field_54 = 0;
    m_TexProperties[0].field_58 = 0;
    m_TexProperties[0].field_5C[1] = 1;

    if (m_FlushDirectly)
    {
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    }
}

void GfxInternal_Dx9::RenderTexturedQuad2D_1(const GfxInternal_Dx9_Texture& tex, const Vector2<float>& top, const Vector2<float>& bottom, const ColorRGB& clr)
{
    const bool ztest = m_TexProperties[0].m_ZTest;
    Vector2f topRight, bottomLeft, bottomRight;

    topRight = { (float)bottom.x, (float)bottom.y };
    if (topRight.x == 0 && topRight.y == 0)
    {
        bottomRight = { (float)tex.m_Resolution.x, (float)tex.m_Resolution.y };
        bottomLeft = { (float)tex.m_Resolution.x, (float)tex.m_Resolution.y };
        topRight = { (float)tex.m_Resolution.x, (float)tex.m_Resolution.y };
    }

    m_TexProperties[0].m_ZTest = false;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

    bottomLeft = { (float)(top.x + topRight.x), (float)top.y };
    bottomRight = { bottomLeft.x, (float)(top.y + topRight.y) };
    topRight = { (float)top.x, (float)(top.y + topRight.y) };

    RenderTexturedQuad2D_2(tex, top, topRight, bottomRight, bottomLeft, clr);

    m_TexProperties[0].m_ZTest = ztest;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_ZENABLE, ztest);
}

void GfxInternal_Dx9::RenderViewport()
{
    if (field_9758 > 0.0099999998f)
    {
        m_AlphaChannelEnabled = true;
        m_TexProperties[0].m_AlphaBlend = 1;
        m_TexProperties[0].field_8A = 1;
        m_TexProperties[0].field_8 = 4;
        m_TexProperties[0].field_24 = 1;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        }

        _45C790(field_9758);

        field_9758 = 0;
        m_AlphaChannelEnabled = false;
        m_TexProperties[0].m_AlphaBlend = 0;
        m_TexProperties[0].field_8A = 1;
        m_TexProperties[0].field_8 = 3;
        m_TexProperties[0].field_24 = 1;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
        }
    }

    if (m_ViewportTexturesArray[0])
    {
        int filterMode = m_Filter;
        SetRenderTarget(nullptr);

        m_TextureAddressModes[0] = 1;
        m_TexProperties[0].field_54 = 1;
        m_TexProperties[0].field_58 = 1;
        m_TexProperties[0].field_5C[1] = 1;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
            g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        }

        Vector2f top, bottom;
        if (m_SurfaceDoubleSized)
        {
            if (m_Filter != 2)
            {
                m_Filter = 2;
                m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
                m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
                m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
                m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            }

            bottom = { (float)m_ViewportResolution.x, (float)m_ViewportResolution.y };
            top = { 0, 0 };
        }
        else
        {
            SetFilterMode(0);

            top = { 0, 0 };
            bottom = { 0, 0 };
        }

        RenderTexturedQuad2D_1(m_ViewportTexturesArray[m_ActiveViewportSurfaceIndex], top, bottom, { 1, 1, 1, 1 });

        m_TextureAddressModes[0] = 0;
        m_TexProperties[0].field_54 = 0;
        m_TexProperties[0].field_5C[1] = 1;
        m_TexProperties[0].field_58 = 0;
        
        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        }

        SetFilterMode(filterMode);
        m_ActiveViewportSurfaceIndex = 1 - m_ActiveViewportSurfaceIndex;
    }

    m_FVF = -1;
}

void GfxInternal_Dx9::RenderFullscreenTexture(const GfxInternal_Dx9_Texture& tex)
{
    Vector2f top =
    {
        (float)m_DisplayModeResolution.x - (((float)m_DisplayModeResolution.x * 0.00125f) * 800.f) * 0.5f,
        ((float)m_DisplayModeResolution.y - ((g_GfxInternal->WideScreen ? (float)m_DisplayModeResolution.y * 0.0022222223f : (float)m_DisplayModeResolution.x * 0.00125f) * 600.f)) * 0.5f
    };
    Vector2f bottom =
    {
        ((float)m_DisplayModeResolution.x* 0.00125f) * 800.f,
        (g_GfxInternal->WideScreen ? (float)m_DisplayModeResolution.y * 0.0022222223f : (float)m_DisplayModeResolution.x * 0.00125f) * 600.f
    };
    ColorRGB clr = { 1.f, 1.f, 1.f, 1.f };

    RenderTexturedQuad2D_1(tex, top, bottom, clr);
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::EnableLight(void*, unsigned int lightindex)
{
}

void GfxInternal_Dx9::_45E5D0(LightStatus& light)
{
    for (unsigned int i = 0; i < m_SceneLights.size(); i++)
        if (&m_SceneLights[i] == &light)
        {
            EnableLight((void*)&light, 0);
            m_SceneLights[i].m_Enabled = false;
        }
}

void GfxInternal_Dx9::SetProjection(float fov, float aspectratio, float nearplane, float farplane)
{
    m_FarPlane = farplane;
    m_AspectRatio = aspectratio;
    m_NearPlane = nearplane;
    m_FOV = fov;

    FLOAT fov_ = (float)atan2(tan(0.017453292 * fov * 0.5 * 0.75), 1.0) * 2;
    FLOAT nearPlane = (float)(((m_ZBias * 0.001) + 1.0) * nearplane);

    if (GfxInternal::WideScreen)
        m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov_, 1.7f, nearPlane, farplane);
    else
        m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov_, (m_ViewportResolution.x / m_ViewportResolution.y) / aspectratio, nearPlane, farplane);

    m_Direct3DDevice->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, (const D3DMATRIX*)&m_ProjectionMatrix);
}

void GfxInternal_Dx9::LoadDDSTexture(unsigned int index, const char* texturePath)
{
    if (m_TexturesArray[index])
    {
        (*m_TexturesArray)->Release();
        *m_TexturesArray = nullptr;
    }

    if (!texturePath || !(*texturePath))
        return;

    char textureExtension[16];
    char dummybuff[256];
    File::ExtractFilePath(texturePath, dummybuff, dummybuff, textureExtension);

    if (String::EqualIgnoreCase(textureExtension, "dds", strlen("dds")) && FAILED(DirectX::CreateDDSTextureFromFile(m_Direct3DDevice, (const wchar_t*)texturePath, m_TexturesArray)) )
        *m_TexturesArray = nullptr;
}

HRESULT GfxInternal_Dx9::SetCurrentTextureIndex(unsigned int ind) const
{
    return m_Direct3DDevice->SetTexture(1, m_TexturesArray[ind]);
}

void GfxInternal_Dx9::EnableZTest(bool enabled)
{
    m_TexProperties[0].m_ZTest = enabled;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_ZENABLE, enabled);
}

void GfxInternal_Dx9::EnableZWrite(bool enabled)
{
    m_TexProperties[0].m_ZWrite = enabled;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, enabled);
}

void GfxInternal_Dx9::EnableFog(bool enabled)
{
    m_FogEnabled = enabled;
    m_Direct3DDevice->SetRenderState(D3DRS_FOGENABLE, enabled);
}

void GfxInternal_Dx9::GetFogParams(unsigned int* state, ColorRGB* color, float* start, float* end, float* density) const
{
    if (state)
        *state = m_FogMode;

    if (color)
        *color = m_FogColor;

    if (start)
        *start = m_FogStart;

    if (end)
        *end = m_FogEnd;

    if (density)
        *density = m_FogDensity;
}

void GfxInternal_Dx9::EnableLighting(bool enabled)
{
    m_LightingEnabled = enabled;
    m_TexProperties[0].m_LightingEnabled = enabled;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_LIGHTING, enabled);
}

void GfxInternal_Dx9::SetMipMapBias(float bias)
{
    if (m_MipMapBias != bias)
        m_MipMapBias = bias;
}

void GfxInternal_Dx9::SetMipMapping(bool enabled)
{
    m_MipMappingEnabled = enabled;
    m_TexProperties[0].field_5C[0] = enabled;
    m_TexProperties[0].field_5C[1] = 1;
    m_TexProperties[0].field_6C[0] = enabled;
    m_TexProperties[0].field_6C[1] = 1;

    if (m_FlushDirectly)
    {
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_50, D3DSAMP_MIPFILTER, enabled != 0 ? 2 : 0);
        g_Direct3DDevice->SetSamplerState(m_TexProperties[0].field_60, D3DSAMP_MIPFILTER, enabled != 0 ? 2 : 0);
    }
}

void GfxInternal_Dx9::SetFilterMode(unsigned int mode)
{
    if (m_Filter == mode)
        return;

    m_Filter = mode;

    if (mode > 0 && mode <= 2)
    {
        m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    else
    {
        m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        m_Direct3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }
}

void GfxInternal_Dx9::GetProjectionParams(float* fov, float* aspectratio, float* nearplane, float* farplane) const
{
    if (fov)
        *fov = m_FOV;

    if (aspectratio)
        *aspectratio = m_AspectRatio;

    if (nearplane)
        *nearplane = m_NearPlane;

    if (farplane)
        *farplane = m_FarPlane;
}

void GfxInternal_Dx9::SetRenderStateWireframe(bool enabled)
{
    m_Direct3DDevice->SetRenderState(D3DRS_FILLMODE, enabled ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
    m_TexProperties[0].field_24 = 1;

    if (enabled)
    {
        m_TexProperties[0].field_4 = 3;
        m_TexProperties[0].field_8 = 3;
        m_TexProperties[0].field_14 = 3;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_COLOROP, 3);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_ALPHAOP, 3);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_COLORARG2, 3);
        }

        m_Direct3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, m_WireframeColor);
    }
    else
    {
        m_TexProperties[0].field_4 = 4;
        m_TexProperties[0].field_8 = 4;
        m_TexProperties[0].field_14 = 0;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_COLOROP, 4);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_ALPHAOP, 4);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_COLORARG2, 0);
        }

        m_Direct3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, -1);
    }
}

void GfxInternal_Dx9::SetWireFrameColor(const ColorRGB& clr)
{
    m_WireframeColor = COLOR_BGRA(clr);
}

void GfxInternal_Dx9::SetWorldMatrix(const DirectX::XMMATRIX* worldmat)
{
    m_Direct3DDevice->SetTransform(D3DTS_WORLD, (const D3DMATRIX*)worldmat);
    m_WorldMatrix = *worldmat;
}

void GfxInternal_Dx9::SetEnvironmentMapOpacity(float opacity)
{
    if (opacity == m_EnvironmentMapOpacity)
        return;

    m_EnvironmentMapColors = { m_EnvironmentMapCoefficient, m_EnvironmentMapCoefficient, m_EnvironmentMapCoefficient, opacity };
    m_EnvironmentMapOpacity = opacity;

    m_Direct3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, COLOR_BGRA(m_EnvironmentMapColors));
}

void GfxInternal_Dx9::EnableAlphaChannel(bool enabled)
{
    m_AlphaChannelEnabled = enabled;
    m_TexProperties[0].field_8A = 1;

    if (enabled)
    {
        m_TexProperties[0].m_AlphaBlend = true;
        m_TexProperties[0].field_8 = 4;
        m_TexProperties[0].field_24 = 1;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        }
    }
    else
    {
        m_TexProperties[0].m_AlphaBlend = false;
        m_TexProperties[0].field_8 = 3;
        m_TexProperties[0].field_24 = 1;

        if (m_FlushDirectly)
        {
            g_Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
            g_Direct3DDevice->SetTextureStageState(m_TexProperties[0].field_0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
        }
    }
}

void GfxInternal_Dx9::SetBlendMode(unsigned int mode)
{
    m_BlendMode = mode;

    if (mode && mode == 1)
        m_Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    else
        m_Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void GfxInternal_Dx9::EnableAlphaTest(bool enabled)
{
    m_AlphaTestEnabled = enabled;
    m_TexProperties[0].m_AlphaTest = enabled;
    m_TexProperties[0].field_8A = 1;

    if (m_FlushDirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, enabled);
}

void GfxInternal_Dx9::SetAlphaTestThreshold(float threshold)
{
    m_AlphaTestThreshhold = threshold;
    m_Direct3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(threshold * 255.f));
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::SetRenderTarget(IDirect3DSurface9* rt)
{
    m_RenderTarget = rt;

    if (!rt)
    {
        m_Direct3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &rt);

        if (m_DepthStencilSurface)
        {
            m_Direct3DDevice->SetRenderTarget(0, rt);
            m_Direct3DDevice->SetDepthStencilSurface(m_DepthStencilSurface);
        }
        else
        {
            IDirect3DSurface9* dss = nullptr;
            m_Direct3DDevice->GetDepthStencilSurface(&dss);
            m_Direct3DDevice->SetRenderTarget(0, rt);
            m_Direct3DDevice->SetDepthStencilSurface(dss);

            if (dss)
                dss->Release();
        }

        m_ViewportResolution = m_DisplayModeResolution;

        D3DVIEWPORT9 viewport;

        viewport.Width = m_DisplayModeResolution.x;
        viewport.Height = m_DisplayModeResolution.y;
        viewport.X = 0;
        viewport.Y = 0;
        viewport.MinZ = 0.f;
        viewport.MaxZ = 1.f;

        m_Direct3DDevice->SetViewport(&viewport);
        rt->Release();
    }
    else
    {
        //	TODO: once GfxSurface is complete - uncomment.
        //	GfxSurface surface;
        //	surface.CreateDirect3DSurface(nullptr);

        if (m_DepthStencilSurface)
        {
            //m_Direct3DDevice->SetRenderTarget(0, surface);
            m_Direct3DDevice->SetDepthStencilSurface(m_DepthStencilSurface);
        }
        else
        {
            IDirect3DSurface9* dss = nullptr;
            m_Direct3DDevice->GetDepthStencilSurface(&dss);
            //m_Direct3DDevice->SetRenderTarget(0, surface);
            m_Direct3DDevice->SetDepthStencilSurface(dss);

            if (dss)
                dss->Release();
        }

        D3DVIEWPORT9 viewport;

        //viewport.Width = surface.m_Width;
        //viewport.Height = surface.m_Height;
        viewport.X = 0;
        viewport.Y = 0;
        viewport.MinZ = 0.f;
        viewport.MaxZ = 1.f;

        m_Direct3DDevice->SetViewport(&viewport);
    }
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::TransformStateView(DirectX::XMMATRIX* mat)
{
    m_IdentityMatrix = *mat;
    m_ViewMatrix = *mat;

    m_ViewMatrix.r[0].m128_f32[3] = 0;
    m_ViewMatrix.r[1].m128_f32[3] = 0;
    m_ViewMatrix.r[2].m128_f32[3] = 0;
    
    m_ViewMatrix.r[3] = { 0, 0, 0, 1 };

    DirectX::XMMATRIX vmat;
    //_4687D0(mat, vmat);

    m_Direct3DDevice->SetTransform(D3DTS_VIEW, (const D3DMATRIX*)&vmat);

    if (m_EnvironmentMapEnabled)
        SetTextureStage1TransformationMatrix();
}

unsigned int GfxInternal_Dx9::GetAvailableTextureMemory() const
{
    return m_Direct3DDevice->GetAvailableTextureMem();
}

void GfxInternal_Dx9::DumpScreenShot(GfxInternal_Dx9_Surface* surf)
{
    LogDump::LogA("Dumping screenshot!\n");

    LPDIRECT3DSURFACE9 backBufferSurface = nullptr;
    D3DSURFACE_DESC backBufferSurfaceDesc;
    D3DLOCKED_RECT backBufferRect;

    m_Direct3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBufferSurface);
    backBufferSurface->GetDesc(&backBufferSurfaceDesc);
    backBufferSurface->LockRect(&backBufferRect, nullptr, D3DLOCK_READONLY);

    LogDump::LogA("copying from surface (%i,%i) --> (%i,%i)\n", backBufferSurfaceDesc.Width, backBufferSurfaceDesc.Height, surf->m_Width, surf->m_Height);

    ColorRGB clr;
    for (unsigned int x = 0; x < backBufferSurfaceDesc.Width; x++)
    {
        for (unsigned int y = 0; y < backBufferSurfaceDesc.Height; y++)
        {
            char dwclr[4] = {};
            clr = {};

            switch (backBufferSurfaceDesc.Format)
            {
            case D3DFMT_X8R8G8B8:
            case D3DFMT_A8R8G8B8:
                *(int*)&dwclr = (int)((char*)backBufferRect.pBits + 4 * x + y * backBufferRect.Pitch);
                break;
            case D3DFMT_R5G6B5:
                *(short*)&dwclr = (short)((char*)backBufferRect.pBits + 4 * x + y * backBufferRect.Pitch);
                *(int*)&dwclr = 8 * (*(short*)&dwclr & 0x1F | (4 * (*(short*)&dwclr & 0x7E0 | (8 * (*(short*)&dwclr & 0xF800)))));
                break;
            case D3DFMT_X1R5G5B5:
            case D3DFMT_A1R5G5B5:
                *(short*)&dwclr = (short)((char*)backBufferRect.pBits + 4 * x + y * backBufferRect.Pitch);
                *(int*)&dwclr = 8 * (*(short*)&dwclr & 0x1F | (8 * (*(short*)&dwclr & 0x3E0 | (8 * (*(short*)&dwclr & 0x7C00)))));
                break;
            }

            clr = { dwclr[3] * 0.0039215689f, dwclr[2] * 0.0039215689f, dwclr[1] * 0.0039215689f, 1.f };
            surf->SetPixelColor(x, y, clr);
        }
    }

    backBufferSurface->UnlockRect();
    if (backBufferSurface)
        backBufferSurface->Release();
}

GfxInternal_Dx9::RenderProperties::RenderProperties()
{
    MESSAGE_CLASS_CREATED(RenderProperties);

    field_0 = NULL;
    field_50 = NULL;
    field_28 = 1;
    field_60 = 1;
}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9::RenderProperties::RenderProperties(const RenderProperties& rhs)
{
    MESSAGE_CLASS_CREATED(RenderProperties);
}

void GfxInternal_Dx9::RenderProperties::SetTextureAmbientColor(const ColorRGB& clr, bool flushdirectly)
{
    m_AmbientColor = clr;
    field_8A = 1;

    if (flushdirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_AMBIENT, COLOR_BGRA(clr));
}

void GfxInternal_Dx9::RenderProperties::ToggleLighting(bool enabled, bool flushdirectly)
{
    m_LightingEnabled = enabled;
    field_8A = 1;

    if (flushdirectly)
        g_Direct3DDevice->SetRenderState(D3DRS_LIGHTING, enabled);
}
#endif