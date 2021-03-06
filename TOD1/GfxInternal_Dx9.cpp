#include "GfxInternal_Dx9.h"
#include "Window.h"
#include "GfxInternal.h"
#include "File.h"
#include "LogDump.h"
#include "InputKeyboard.h"

GfxInternal_Dx9* g_GfxInternal_Dx9 = nullptr;
bool GfxInternal_Dx9::ProcessingInput;
std::map<int, int> GfxInternal_Dx9::RenderedTexturesMap;
const D3DMATRIX GfxInternal_Dx9::IdentityMatrix =
{
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

const GfxInternal_Dx9_Vertex::VertexDeclaration GfxInternal_Dx9_Vertex::VertexDeclarations[] =
{
	{ 20, D3DFVF_DIFFUSE | D3DFVF_XYZRHW },
	{ 24, D3DFVF_TEX1 | D3DFVF_XYZRHW},
	{ 28, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW },
	{ 24, D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 16, D3DFVF_DIFFUSE | D3DFVF_XYZ },
	{ 28, D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 32, D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 40, D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 24, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ },
	{ 36, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 48, D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ },
	{ 40, D3DFVF_NORMAL | D3DFVF_XYZ }
};
std::map<int, GfxInternal_Dx9_Vertex*>* GfxInternal_Dx9_Vertex::VertexBufferMap;

void GfxInternal_Dx9::GetScreenResolution(Vector2<int>& outRes)
{
	outRes = g_GfxInternal_Dx9->m_DisplayModeResolution;
}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9::GfxInternal_Dx9(const Vector2<int>& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int unk1)
{
	MESSAGE_CLASS_CREATED(GfxInternal_Dx9);

	m_DisplayModesList;
	//	TODO: call constructor for unknown struct 'field_1B4' at 45FF40.
	//	TODO: call constructor for unknown struct 'field_240' at 45FF40.
	m_ParticleOrient = { 0.f, 0.f, 0.f, 1.f };
	m_SceneLights;
	field_9770;	//	TODO: I guess custom allocator for this map is at 45F3C0.
	m_IdentityMatrix = IdentityMatrix;

	m_Direct3DInterface = Direct3DCreate9(DIRECT3D_VERSION);

	if (FAILED(m_Direct3DInterface))
		IncompatibleMachineParameterError(ERRMSG_DIRECTX9_NOT_FOUND, 0);

	m_RenderingScene = false;
	m_DeviceLost = false;
	field_1B1 = 1;
	field_174 = resolution.x;	//	TODO: not sure about that...
	field_975C = -1;
	m_FramesyncQuery = nullptr;
	m_DeviceResetIssued = false;

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

	//	FIXME: this const is ugly, replace with something.
	constexpr unsigned int modesTotal = 5;
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

	for (int i = 0; i < modesTotal; i++)
		m_DisplayModesList.AddElement(&DefaultDisplayModes[i]);

	EnumDisplayModes();

	if (!m_DisplayModeResolution.x || !m_DisplayModeResolution.y)
	{
		m_DisplayModeResolution = { 800, 600 };
		m_ViewportResolution = { 800, 600 };
		Vector2<int> res;

		if (GetRegistryResolution(res) && m_DisplayModesList.m_CurrIndex)
			for (unsigned int i = 0; i < m_DisplayModesList.m_CurrIndex; i++)
				if (m_DisplayModesList.m_Elements[i]->m_Available &&
					m_DisplayModesList.m_Elements[i]->m_Width == res.x &&
					m_DisplayModesList.m_Elements[i]->m_Height == res.y)
					m_DisplayModeResolution = m_ViewportResolution = res;

		RememberResolution();
	}

	if (FSAA & 2)
		SetupWindowParams(m_DisplayModeResolution.x, m_DisplayModeResolution.y);
	else
		SetupWindowParams_2(m_DisplayModeResolution);

	m_ViewportWidth_1 = m_ViewportHeight_1 = NULL;
	field_9758 = 0.f;
	m_ActiveViewportSurfaceIndex = NULL;
	m_DrawingText = false;
	m_ActiveTextRenderBuffer = NULL;
	m_FlushDirectly = false;
}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9::~GfxInternal_Dx9()
{
	MESSAGE_CLASS_DESTROYED(GfxInternal_Dx9);

	LPDIRECT3DSURFACE9 depthSurface = nullptr;
	m_Direct3DDevice->GetDepthStencilSurface(&depthSurface);
	depthSurface->Release();

	delete m_DepthStencilSurface;
	delete[] & m_ViewportTexturesArray;
	delete[] & m_TexturesArray;
	delete[] & m_VertexBuffer;	//	NOTE:	'DestroyVertexBufferObjects' @450710 inlined.

	m_Direct3DDevice->Release();
	m_Direct3DInterface->Release();

	m_Direct3DDevice = nullptr;
	m_Direct3DInterface = nullptr;

	g_GfxInternal_Dx9 = nullptr;
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

GfxInternal_Dx9::DisplayModeInfo* GfxInternal_Dx9::IsScreenResolutionAvailable(unsigned int width, unsigned int height, bool dontignoreunavailable)
{
	if (m_DisplayModesList.m_CurrIndex <= 0)
		return nullptr;

	for (unsigned int i = NULL; i != m_DisplayModesList.m_CurrIndex; i++)
		if (i + 1 == m_DisplayModesList.m_CurrIndex)
			break;
		else
			if (m_DisplayModesList.m_Elements[i]->m_Width == width &&
				m_DisplayModesList.m_Elements[i]->m_Height == height &&
				m_DisplayModesList.m_Elements[i]->m_Available ||
				!dontignoreunavailable)
				return m_DisplayModesList.m_Elements[i];

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

		if (m_DisplayModesList.m_CurrIndex > 0) {
			DisplayModeInfo** mode = m_DisplayModesList.m_Elements;

			while ((*mode)->m_Width != adapterModes.Width || (*mode)->m_Height != adapterModes.Height)
			{
				++ind;
				++mode;
				if (ind >= m_DisplayModesList.m_CurrIndex)
					break;
			}

			if (ind >= m_DisplayModesList.m_CurrIndex)
			{
				adapterMode = adapterIndex + 1;

				continue;
			}

			DisplayModeInfo* mode_sel = m_DisplayModesList.m_Elements[ind];

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

	for (unsigned int i = 0; i < m_DisplayModesList.m_CurrIndex; ++i)
		LogDump::LogA("%ix%i @ %iHz - format=%i, available=%i\n",
			m_DisplayModesList.m_Elements[i]->m_Width,
			m_DisplayModesList.m_Elements[i]->m_Height,
			m_DisplayModesList.m_Elements[i]->m_RefreshRate,
			m_DisplayModesList.m_Elements[i]->m_Format,
			m_DisplayModesList.m_Elements[i]->m_Available);
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::CreateSurfaces()
{
}

void GfxInternal_Dx9::SetupWindowParams(unsigned int width, unsigned int height)
{
	const DisplayModeInfo* mode = IsScreenResolutionAvailable(width, height, true);

	if (mode || (mode = IsScreenResolutionAvailable(640, 480, true)) != 0)
	{
		m_Windowed = true;
		m_DisplayModeFormat = (D3DFORMAT)mode->m_Format;	//	D3DFORMAT is just a enum
		m_DisplayModeResolution.x = mode->m_Width;
		m_DisplayModeResolution.y = mode->m_Height;

		g_Window->SetWindowResolutionRaw(m_DisplayModeResolution);
		memset(&m_PresentParameters, 0, sizeof(m_PresentParameters));

		m_PresentParameters.Windowed = false;
		m_PresentParameters.BackBufferWidth = m_DisplayModeResolution.x;
		m_PresentParameters.BackBufferHeight = m_DisplayModeResolution.y;
		m_PresentParameters.BackBufferFormat = m_DisplayModeFormat;
		m_PresentParameters.FullScreen_RefreshRateInHz = mode->m_RefreshRate;

		CreateRenderDevice();
	}
}

void GfxInternal_Dx9::SetupWindowParams_2(const Vector2<int> mode)
{
	D3DDISPLAYMODE _mode;

	m_Windowed = false;
	g_Window->SetWindowResolutionDontMove(mode);

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
		MessageBoxA(g_Window->m_WindowHandle, Utils::GetErrorCodeDescription(result), "Couldn't get current display mode", NULL);
}

bool GfxInternal_Dx9::GetRegistryResolution(Vector2<int>& mode)
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
}

void GfxInternal_Dx9::Reset()
{
	m_DeviceLost = true;
	DestroySurfaces();

	HRESULT resetresult = m_Direct3DDevice->Reset(&m_PresentParameters);

	if (SUCCEEDED(resetresult))
	{
		LogDump::LogA("Device reset correctly\n");
		CreateSurfaces();
		m_DeviceLost = false;
	}
	else
		LogDump::LogA("%s: %s\n", "pd3dDevice->Reset", Utils::GetErrorCodeDescription(resetresult));
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

	field_1B1 = 1;

	//	CreateSurfaces();
	//	::450610(1024)
	//	::450DB0()

	if (g_GfxInternal->WideScreen)
		g_ScreenProperties.SetWindowProperties((float)m_ViewportResolution.x, (float)m_ViewportResolution.y, (float)((m_ViewportResolution.x * 0.0625) / (m_ViewportResolution.y * 0.1111)), 1.0f);
	else
		g_ScreenProperties.SetWindowProperties((float)m_ViewportResolution.x, (float)m_ViewportResolution.y, m_AspectRatio, 1.0f);
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::EnableLight(void*, unsigned int lightindex)
{
}

void GfxInternal_Dx9::_45E5D0(void* light)
{
	for (unsigned int i = 0; i < m_SceneLights.m_CurrIndex; i++)
		if (m_SceneLights.m_Elements[i] == light)
		{
			EnableLight(light, 0);
			m_SceneLights.m_Elements[i]->m_Enabled = false;
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
		D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, fov_, (FLOAT)1.7777778, nearPlane, farplane);
	else
		D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, fov_, (m_ViewportResolution.x / m_ViewportResolution.y) / aspectratio, nearPlane, farplane);

	m_Direct3DDevice->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &m_ProjectionMatrix);
}

void GfxInternal_Dx9::LoadDDSTexture(unsigned int index, const char* texturePath)
{
	if (m_TexturesArray[index]) {
		(*m_TexturesArray)->Release();
		*m_TexturesArray = nullptr;
	}

	if (!texturePath || !(*texturePath))
		return;

	char textureExtension[16];
	char dummybuff[256];
	File::ExtractFilePath(texturePath, dummybuff, dummybuff, textureExtension);

	if (String::EqualIgnoreCase(textureExtension, "dds", strlen("dds")) &&
		FAILED(D3DXCreateTextureFromFileA(m_Direct3DDevice, texturePath, m_TexturesArray)))
		*m_TexturesArray = nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9::DumpScreenShot(GfxInternal_Dx9_Surface* surf)
{
	/*
	LogDump::LogA("Dumping screenshot!\n");

	LPDIRECT3DSURFACE9 backBufferSurface = nullptr;
	D3DSURFACE_DESC backBufferSurfaceDesc;
	D3DLOCKED_RECT backBufferRect;

	m_Direct3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBufferSurface);
	backBufferSurface->GetDesc(&backBufferSurfaceDesc);
	backBufferSurface->LockRect(&backBufferRect, nullptr, 16);	//	FIXME: replace 16 with actual flag.

	LogDump::LogA("copying from surface (%i,%i) --> (%i,%i)\n", backBufferSurfaceDesc.Width, backBufferSurfaceDesc.Height, surf->m_Width, surf->m_Height);

	//	TODO: convert colors
	for (unsigned int x = 0; x < backBufferSurfaceDesc.Width; x++)
		for (unsigned int y = 0; y < backBufferSurfaceDesc.Height; y++)
			switch (backBufferSurfaceDesc.Format)
			{
				case D3DFMT_X8R8G8B8:
				case D3DFMT_A8R8G8B8:
					surf->SetPixelColor(x, y, {});
					break;
				case D3DFMT_R5G6B5:
					surf->SetPixelColor(x, y, {});
					break;
				case D3DFMT_X1R5G5B5:
				case D3DFMT_A1R5G5B5:
					surf->SetPixelColor(x, y, {});
					break;
			}

	backBufferSurface->UnlockRect();
	if (backBufferSurface)
	backBufferSurface->Release();
	*/
}

GfxInternal_Dx9_Vertex::GfxInternal_Dx9_Vertex(int FVFindex, int size, int flags)
{
	m_BufferPtr = nullptr;
	m_Direct3DVertexBuffer = nullptr;
	field_16 = NULL;
	m_Verticies = size;
	m_Stride = VertexDeclarations[FVFindex].m_Stride;
	m_FVF = VertexDeclarations[FVFindex].m_FVF;
	m_Length = size * m_Stride;
	field_4 = NULL;
	m_Flags = flags;
	m_FVFIndex = FVFindex;
	field_20 = NULL;
	m_BufferPtr = new char[m_Length];

	CreateVertexBuffer();
	VertexBufferMap->insert({ 0, this });	//	TODO: is this correct?
}

void GfxInternal_Dx9_Vertex::CreateVertexBuffer()
{
	LPDIRECT3DVERTEXBUFFER9 vertbuff = nullptr;
	g_GfxInternal_Dx9->m_Direct3DDevice->CreateVertexBuffer(m_Length, m_Flags & 1 ? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY : D3DUSAGE_WRITEONLY, m_FVF, D3DPOOL_DEFAULT, &vertbuff, nullptr);

	if (vertbuff != m_Direct3DVertexBuffer)
		m_Direct3DVertexBuffer->Release();

	m_Direct3DVertexBuffer = vertbuff;
}

void GfxInternal_Dx9_Vertex::CreateVerticesMap()
{
	VertexBufferMap = new std::map<int, GfxInternal_Dx9_Vertex*>();
}