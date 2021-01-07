#include "GfxInternal_Dx9.h"
#include "Window.h"
#include "Renderer.h"
#include "File.h"
#include "LogDump.h"

GfxInternal_Dx9* g_RendererDx = NULL;
const D3DMATRIX& GfxInternal_Dx9::g_IdentityMatrix = *(D3DMATRIX*)0xA0AD38;
Map<int, int>& GfxInternal_Dx9::g_UnkMap_1 = *(Map<int, int>*)0xA39F58;
Map<int, int>& GfxInternal_Dx9::g_UnkMap_2 = *(Map<int, int>*)0xA39F38;
Map<int, int>& GfxInternal_Dx9::g_RenderedTexturesMap = *(Map<int, int>*)0xA39F50;
void* GfxInternal_Dx9::g_RenderBuffer = (void*)0xA35E60;

void GfxInternal_Dx9::GetScreenResolution(Vector2<int>& outRes)
{
	outRes.x = g_RendererDx->m_DisplayMode.Width;
	outRes.y = g_RendererDx->m_DisplayMode.Height;
}

GfxInternal_Dx9::GfxInternal_Dx9(const Vector2<int>* resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int unk1)
{
	MESSAGE_CLASS_CREATED(GfxInternal_Dx9);

	m_DisplayModesList = List<DisplayModeInfo>();
	m_UnkList_1 = List<int>();

	//	NOTE: Another list allocation is here.
	(*(void (__cdecl*)())0x45F3C0)();

	m_IdentityMatrix = g_IdentityMatrix;

	if (!(m_Direct3DInterface = Direct3DCreate9(DIRECT3D_VERSION)))
		IncompatibleMachineParameterError(4, 0);

	m_SceneBegan = false;
	m_DeviceLost = false;
	field_1B1[0] = 1;
	field_1B1[1] = 1;
	field_1B1[2] = 0;
	m_ScreenSize = (Vector2<int>*)resolution;
	field_975C = -1;
	m_FramesyncQuery = nullptr;
	m_DeviceResetIssued = false;

	if (m_Direct3DInterface->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_DeviceCaps) != D3D_OK)
		IncompatibleMachineParameterError(3, 0);

	GetDeviceCaps();

	m_pTexturesArray = nullptr;
	field_2D4 = 0;
	field_2D8 = 0;
	field_2DC = 0;
	field_2E0 = 0;
	field_2E4 = 0;
	field_2E8 = 0;
	field_2EC = 0;
	field_2F0 = 0;
	field_2F4 = 0;
	field_2F8 = 0;
	field_2FC = 0;
	field_300 = 0;
	field_304 = 0;
	field_308 = 0;
	field_30C = 0;
	m_Direct3DDevice = 0;
	m_pTexturesArray_2[0] = 0;
	m_pTexturesArray_2[1] = 0;
	field_9700 = 0;
	field_9704 = 0;
	m_nUnkTexturesStage[0] = 0;
	m_nUnkTexturesStage[1] = 0;
	m_nUnkTexturesStage[2] = 0;
	m_nUnkTexturesStage[3] = 0;
	m_pIndexBuffer = nullptr;
	m_CurrentVertexBuffer = nullptr;
	m_RenderTarget = nullptr;
	field_9670 = -1;
	field_9674 = 0;
	m_Direct3DVertexDeclaration = nullptr;
	field_9750 = 0;
	field_9664 = 0;
	
	m_ViewportWidth = *(float*)resolution;
	m_ViewportHeight = *((float*)resolution + 4);
	m_DisplayMode.Width = *(unsigned int*)resolution;
	m_DisplayMode.Height = *((unsigned int*)resolution + 4);

	//	NOTE: Propagate default display modes.
	const int modesTotal = 5;
	DisplayModeInfo DefaultDisplayModes[modesTotal] = {
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

	//	NOTE: EnumDisplayModes inlined!
	//	NOTE: Modes are now hard coded, maybe add option to change it?
	int adapterModesTotal = m_Direct3DInterface->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
	int adapterMode = 0;
	D3DDISPLAYMODE adapterModes;

	for (int adapterIndex = 0; adapterIndex < adapterModesTotal; ++adapterIndex) {
		memset(&adapterModes, 0, sizeof(adapterModes));
		m_Direct3DInterface->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, adapterMode, &adapterModes);
		unsigned int ind = 0;

		if (m_DisplayModesList.m_CurrIndex > 0) {
			DisplayModeInfo** mode = m_DisplayModesList.m_Elements;

			while ((*mode)->width != adapterModes.Width ||
				(*mode)->height != adapterModes.Height) {
				++ind;
				++mode;
				if (ind >= m_DisplayModesList.m_CurrIndex)
					break;
			}

			if (ind >= m_DisplayModesList.m_CurrIndex) {
				adapterMode = adapterIndex + 1;

				continue;
			}

			DisplayModeInfo* mode_sel = m_DisplayModesList.m_Elements[ind];

			if (mode_sel->refreshrate <= 85 && adapterModes.RefreshRate > mode_sel->refreshrate) {
				mode_sel->available = true;
				mode_sel->refreshrate = adapterModes.RefreshRate;
				mode_sel->format = adapterModes.Format;
			}
		}

		adapterMode = adapterIndex + 1;
	}

	debug("Kapow will use these modes when in full-screen:\n");

	for (unsigned int i = 0; i < m_DisplayModesList.m_CurrIndex; ++i)
		debug("%ix%i @ %iHz - format=%i, available=%i\n", 
			m_DisplayModesList.m_Elements[i]->width,
			m_DisplayModesList.m_Elements[i]->height,
			m_DisplayModesList.m_Elements[i]->refreshrate,
			m_DisplayModesList.m_Elements[i]->format,
			m_DisplayModesList.m_Elements[i]->available);

	//	NOTE: EnumDisplayModes inlined --ends

	//	Fallback to default display mode if all of the found adapter modes are unavailable.
	if (m_DisplayMode.Width == 0 || m_DisplayMode.Height) {
		m_DisplayMode.Width = 800;
		m_DisplayMode.Height = 600;
		m_ViewportWidth = 800;
		m_ViewportHeight = 600;

		DisplayModeInfo mode;

		if (GetRegistryResolution(mode)) {
			if (m_DisplayModesList.m_CurrIndex > 0) {
				DisplayModeInfo** _modes = m_DisplayModesList.m_Elements;
				unsigned int index = 0;

				while (true) {
					if ((*_modes)->width == mode.width && (*_modes)->height == mode.height && (*_modes)->available)
						break;
					++index;
					++_modes;

					if (index >= m_DisplayModesList.m_CurrIndex)
						break;
				}

				m_DisplayMode.Width = mode.width;
				m_DisplayMode.Height = mode.height;
				m_ViewportWidth = (float)mode.width;
				m_ViewportHeight = (float)mode.height;
			}
		}

		RememberResolution();
	}

	//	Finalize.
	if ((int)resolution & 2)
		SetupWindowParams(m_DisplayMode.Width, m_DisplayMode.Height);
	else
		SetupWindowParams(m_DisplayMode);

	m_f9758 = 0.0;
	m_nViewportSurfaceIndex = 0;
	field_553C = 0;
	m_FlushDirectly = false;
}

const DisplayModeInfo* GfxInternal_Dx9::IsScreenResolutionAvailable(int width, int height, bool dontIgnoreUnavailable)
{
	if (m_DisplayModesList.m_CurrIndex <= 0)
		return nullptr;

	DisplayModeInfo** mode = m_DisplayModesList.m_Elements;
	unsigned int index = 0;

	while (true) {
		if ((*mode)->width == width &&
			(*mode)->height == height &&
			(*mode)->available || !dontIgnoreUnavailable)
			break;
		++index;
		++mode;

		if (index >= m_DisplayModesList.m_CurrIndex)
			return nullptr;
	}

	return m_DisplayModesList.m_Elements[index];
}

void GfxInternal_Dx9::SetupWindowParams(int width, int height)
{
	const DisplayModeInfo* mode = IsScreenResolutionAvailable(width, height, true);

	if (mode || (mode = IsScreenResolutionAvailable(640, 480, true)) != 0) {
		m_Windowed = true;
		m_nDisplayModeFormat = (D3DFORMAT)mode->format;	//	D3DFORMAT is just a enum
		m_DisplayMode.Width = mode->width;
		m_DisplayMode.Height = mode->height;

		g_Window->SetWindowResolutionRaw(m_DisplayMode);
		memset(&m_PresentParameters, 0, sizeof(m_PresentParameters));

		m_PresentParameters.Windowed = false;
		m_PresentParameters.BackBufferWidth = m_DisplayMode.Width;
		m_PresentParameters.BackBufferHeight = m_DisplayMode.Height;
		m_PresentParameters.BackBufferFormat = m_nDisplayModeFormat;
		m_PresentParameters.FullScreen_RefreshRateInHz = mode->refreshrate;

		CreateRenderDevice();
	}
}

//	NOTE: wtf? Originally passed by value, lol.
void GfxInternal_Dx9::SetupWindowParams(D3DDISPLAYMODE mode)
{
	D3DDISPLAYMODE _mode;

	m_Windowed = false;

	g_Window->SetWindowResolutionDontMove(mode);

	HRESULT result = m_Direct3DInterface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &_mode);
	if (result >= 0) {
		memset(&m_PresentParameters, 0, sizeof(m_PresentParameters));

		m_nDisplayModeFormat = _mode.Format;
		m_PresentParameters.Windowed = true;
		m_PresentParameters.BackBufferFormat = _mode.Format;
		m_PresentParameters.FullScreen_RefreshRateInHz = 0;

		CreateRenderDevice();
	}
	else
		MessageBoxA(g_Window->m_hWindow, Utils::GetErrorCodeDescription(result), "Couldn't get current display mode", NULL);
}

bool GfxInternal_Dx9::GetRegistryResolution(DisplayModeInfo& mode)
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

	mode.width = x;
	mode.height = y;

	RegCloseKey(phkResult);

	return true;
}

void GfxInternal_Dx9::RememberResolution()
{
	HKEY phkResult;
	LPDWORD disposition = 0;

	if (!RegCreateKeyExA(HKEY_CURRENT_USER, Window::RegistryKey, 0, 0, 0, 0xF003F, 0, &phkResult, disposition)) {
		RegSetValueExA(phkResult, "XRes", 0, 4, (const BYTE*)&m_DisplayMode.Width, 4);
		RegSetValueExA(phkResult, "YRes", 0, 4, (const BYTE*)&m_DisplayMode.Height, 4);
		RegCloseKey(phkResult);
	}
}

int GfxInternal_Dx9::GetDeviceCaps()
{
	m_ShaderCapabilities.m_VertexShader11 = m_DeviceCaps.VertexShaderVersion >= 0xFFFE0101;
	m_ShaderCapabilities.m_VertexShader20 = m_DeviceCaps.VertexShaderVersion >= 0xFFFE0200;
	m_ShaderCapabilities.m_PixelShader13 = m_DeviceCaps.PixelShaderVersion >= 0xFFFF0103;
	m_ShaderCapabilities.m_PixelShader20 = m_DeviceCaps.PixelShaderVersion >= 0xFFFF0200;

	m_MaxVertexShaderConst = m_DeviceCaps.MaxVertexShaderConst;

	m_MipmapLodBias = (m_DeviceCaps.RasterCaps >> 13) & 1;
	m_TextureRequiresPow2 = (m_DeviceCaps.TextureCaps >> 1) & 1;
	m_Anisotropy = (m_DeviceCaps.RasterCaps >> 17) & 1;
	m_MinAnisotropy = (m_DeviceCaps.TextureFilterCaps >> 10) & 1;
	m_MagAnisotropy = (m_DeviceCaps.TextureFilterCaps >> 26) & 1;
	m_MaxAnisotropy = m_DeviceCaps.MaxAnisotropy;
	m_BlendOP = (m_DeviceCaps.PrimitiveMiscCaps >> 1) & 1;
	m_FogAndSpecularAlpha = (char)(m_DeviceCaps.PrimitiveMiscCaps) & 1;
	m_SeparateAlphaBlend = (char)(m_DeviceCaps.RasterCaps) & 1;
	m_RangeFog = (m_DeviceCaps.PrimitiveMiscCaps >> 17) & 1;

	LogDump::LogA("CAPS: VS11=%i, VS20=%i, PS13=%i, PS20=%i\n", m_ShaderCapabilities.m_VertexShader11, m_ShaderCapabilities.m_VertexShader20, m_ShaderCapabilities.m_PixelShader13, m_ShaderCapabilities.m_PixelShader20);
	LogDump::LogA("CAPS: Max vertex shader constants = %i\n", m_MaxVertexShaderConst);
	LogDump::LogA("CAPS: Mipmap LOD bias = %i\n", m_MipmapLodBias);
	LogDump::LogA("CAPS: Texture requires pow2 = %i\n", m_TextureRequiresPow2);
	LogDump::LogA("CAPS: Anisotropic filtering=%i (min=%i, mag=%i), Max anisotropy=%i\n", m_Anisotropy, m_MinAnisotropy, m_MagAnisotropy, m_MaxAnisotropy);
	LogDump::LogA("CAPS: BlendOP = %i\n", m_BlendOP);
	LogDump::LogA("CAPS: FogAndSpecularAlpha = %i\n", m_FogAndSpecularAlpha);
	LogDump::LogA("CAPS: RangeFog = %i\n", m_SeparateAlphaBlend);;
	LogDump::LogA("CAPS: SeparateAlphaBlend = %i\n", m_RangeFog);

	return m_Direct3DInterface->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, NULL, D3DRTYPE_TEXTURE, D3DFMT_DXT1);
}

void GfxInternal_Dx9::CreateRenderDevice()
{
	int behaviourFlags;

	m_PresentParameters.EnableAutoDepthStencil = false;
	m_PresentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;
	m_PresentParameters.BackBufferCount = 1;
	m_PresentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	if (m_Windowed)
		m_PresentParameters.PresentationInterval = field_1B1[1] != 0 ? 1 : 0x80000000;
	else{
		m_PresentParameters.PresentationInterval = 0x80000000;
		field_1B1[1] = 0;
	}

	if (m_Direct3DDevice) {
		m_Direct3DDevice->Release();
		m_Direct3DDevice = nullptr;
	}

	if (m_DeviceCaps.DevCaps & 0x10000) {
		debug("Creating a HW device.\n");
		behaviourFlags = 64;
	}else{
		debug("Creating a SW device.\n");
		behaviourFlags = 32;
	}

	m_Direct3DInterface->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_Window->m_hWindow,
		behaviourFlags,
		&m_PresentParameters,
		&m_Direct3DDevice);

	field_1B1[0] = 1;
	
	//	CreateSurfaces();
	//	::450610(1024)
	//	::450DB0()

	if (g_Renderer->WideScreen)
		g_ScreenProperties.SetWindowProperties(m_ViewportWidth, m_ViewportHeight, (float)((m_ViewportWidth * 0.0625) / (m_ViewportHeight * 0.1111)), 1.0f);
	else
		g_ScreenProperties.SetWindowProperties(m_ViewportWidth, m_ViewportHeight, m_fAspectRatio, 1.0f);
}

void GfxInternal_Dx9::SetupProjectionMatrix(float unk1, float aspectratio, float unk3, float farPlane)
{
	m_fFarPlane = farPlane;
	m_fAspectRatio = aspectratio;
	field_554C = unk3;
	field_5544 = unk1;

	double fov = atan2(tan(0.017453292 * unk1 * 0.5 * 0.75), 1.0) * 2;
	double nearPlane = ((m_ZBias * 0.001) + 1.0) * unk3;

	if (Renderer::WideScreen)
		TransformProjection(m_ProjectionMatrix, fov, 1.7777778, nearPlane, farPlane);
	else
		TransformProjection(m_ProjectionMatrix, fov, (m_ViewportWidth / m_ViewportHeight) / aspectratio, nearPlane, farPlane);

	m_Direct3DDevice->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &m_ProjectionMatrix);
}

void GfxInternal_Dx9::TransformProjection(D3DMATRIX& projMatrix, double fov, double aspectratio, double nearplane, double farplane)
{
	double fovHalf = fov * 0.5f;
	fov = cos(fovHalf);
	double Q = farplane / (farplane - nearplane);
	double Qneg = -(Q * nearplane);

	projMatrix._11 = (float)((fov / sin(fovHalf)) / aspectratio);
	projMatrix._12 = 0.0f;
	projMatrix._13 = 0.0f;
	projMatrix._14 = 0.0f;

	projMatrix._21 = 0.0f;
	projMatrix._22 = (float)(fov / sin(fovHalf));
	projMatrix._23 = 0.0f;
	projMatrix._24 = 0.0f;

	projMatrix._31 = 0.0f;
	projMatrix._32 = 0.0f;
	projMatrix._33 = (float)Q;
	projMatrix._34 = 1.0f;

	projMatrix._41 = 0.0f;
	projMatrix._42 = 0.0f;
	projMatrix._43 = (float)Qneg;
	projMatrix._44 = 0.0f;
}

void GfxInternal_Dx9::LoadDDSTexture(unsigned int index, const char* texturePath)
{
	if (m_pTexturesArray[index]) {
		(*m_pTexturesArray)->Release();
		*m_pTexturesArray = nullptr;
	}

	if (texturePath) {
		char textureExtension[32];
		char dummybuff[256];
		File::ExtractFilePath(texturePath, dummybuff, dummybuff, textureExtension);

		if (String::EqualIgnoreCase(textureExtension, "dds", strlen("dds")) &&
			GfxInternal_Dx9::_CreateD3DTextureFromFile(m_Direct3DDevice, (LPCWSTR)texturePath, m_pTexturesArray) < 0)
			*m_pTexturesArray = nullptr;
	}
}

#pragma message(TODO_IMPLEMENTATION)
HRESULT CALLBACK GfxInternal_Dx9::_CreateD3DTextureFromFile(IDirect3DDevice9* d3ddev, LPCWSTR texturePath, IDirect3DTexture9** outTexture)
{
	return (*(HRESULT(CALLBACK*)(IDirect3DDevice9*, LPCWSTR, IDirect3DTexture9**))0x964E47)(d3ddev, texturePath, outTexture);
}

inline void PATCH_RENDERER()
{
	void* dwFunc;

	_asm	mov eax, offset GfxInternal_Dx9::IsScreenResolutionAvailable
	_asm	mov dwFunc, eax
	//	Override IsScreenResolutionAvailable function call.
	hook(0x420152, dwFunc, PATCH_NOTHING);
}