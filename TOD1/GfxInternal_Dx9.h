#pragma once
#include "Types.h"
#include "MemoryManager.h"
#include "GfxInternal_Dx9_Texture.h"
#include "Light.h"
#include <d3dx9math.h>
#include <vector>

struct GfxInternal_Dx9_Vertex
{
	struct VertexDeclaration
	{
		unsigned int	m_Stride;
		unsigned int	m_FVF;

		VertexDeclaration(unsigned int s, unsigned int f)
			: m_Stride(s), m_FVF(f)
		{};
	};

	int							m_Verticies;
	int							field_4;
	int							m_FVF;
	int							m_Length;
	char*						m_BufferPtr;
	short						m_Stride;
	char						field_16;
	int							m_FVFIndex;
	int							m_Flags;
	int							field_20;
	LPDIRECT3DVERTEXBUFFER9		m_Direct3DVertexBuffer;

public:
	GfxInternal_Dx9_Vertex(int FVFindex, int size, int flags);	//	@464E70
	~GfxInternal_Dx9_Vertex();	//	@465070

	void* operator new(size_t size)
	{
		if (!MemoryManager::Released)
			return MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::AllocatorsList[DEFAULT]->Free(ptr);
		ptr = nullptr;
	}

	void						CreateVertexBuffer();	//	@464CC0

	static void					CreateVerticesMap();	//	@4651B0

	static const VertexDeclaration	VertexDeclarations[];	//	@A0ABD0
	static std::map<int, GfxInternal_Dx9_Vertex*>*	VertexBufferMap;	//	@A39F58
};

struct GfxInternal_Dx9_IndexBuffer
{
	int m_TotalIndicies;
	int m_IsTriangle;
	D3DPRIMITIVETYPE m_PrimitiveType;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	IDirect3DIndexBuffer9* m_Direct3DIndexBuffer;
};

//	NOTE: main wrapper for game graphics (D3D9).
//	TODO: as a future possibility - opengl version?
class GfxInternal_Dx9
{
	friend class GfxInternal;
	friend class VirtualHud;
	struct DisplayModeInfo
	{
		unsigned int	m_Width;
		unsigned int	m_Height;
		bool			m_Available;
		unsigned int	m_RefreshRate;
		int				m_Format;
	};

	struct VerticesBuffer
	{
		float	field_0;
		float	field_4;
		float	field_8;
		float	field_C;
		int		field_10;
		Vector2f field_14;
	};

	struct LightStatus
	{
		bool	m_Enabled;
		Light*	m_Light;
	};

public:
	IDirect3DDevice9*	m_Direct3DDevice;
protected:
	IDirect3DQuery9*	m_FramesyncQuery;
	char		m_DeviceResetIssued;
	char		field_9;
	char		field_A;
	int			field_C;
	int			m_TotalPrimitivesDrawn;
	int			m_TotalVerticiesDrawn;
	std::vector<DisplayModeInfo>	m_DisplayModesList;
	union {
		unsigned char m_VertexShader11 : 1;
		unsigned char m_VertexShader20 : 1;
		unsigned char m_PixelShader13 : 1;
		unsigned char m_PixelShader20 : 1;
	}			m_ShaderCapabilities;
	int			m_MaxVertexShaderConst;
	unsigned char	m_TextureRequiresPow2;
	unsigned char	m_MipmapLodBias;
	unsigned char	m_Anisotropy;
	unsigned char	m_MinAnisotropy;
	int			m_MagAnisotropy;
	int			m_MaxAnisotropy;
	unsigned char	m_BlendOP;
	unsigned char	m_FogAndSpecularAlpha;
	unsigned char	m_RangeFog;
	unsigned char	m_SeparateAlphaBlend;
	IDirect3D9*	m_Direct3DInterface;
	D3DCAPS9	m_DeviceCaps;
	int			field_174;
	D3DPRESENT_PARAMETERS	m_PresentParameters;
	char		m_DeviceLost;
	char		field_1B1;
	char		field_1B2;
	char		field_1B3;

	int			field_1B4;
	int			field_1B8;
	int			m_TextureBlendingOp;
	int			field_1C0;
	int			field_1C4;
	int			field_1C8;
	int			field_1CC;
	int			field_1D0;
	int			field_1D4;
	char		field_1D8;
	int			field_1DC;
	int			field_1E0;
	int			field_1E4;
	int			field_1E8;
	int			field_1EC;
	int			field_1F0;
	int			field_1F4;
	int			field_1F8;
	int			field_1FC;
	char		field_200;
	int			field_204;
	int			field_208;
	int			field_20C;
	char		field_210;	//	NOTE: these 2 below are set when changing 'MipMapEnabled'.
	char		field_211;
	int			field_214;
	int			field_218;
	int			field_21C;
	char		field_220;	//	NOTE: these 2 below are also set when changing 'MipMapEnabled'.
	char		field_221;
	bool		field_222;
	bool		field_223;
	bool		field_224;
	bool		m_ZTestEnabled;
	char		m_ZWriteEnabled;
	int			field_228;
	int			field_22C;
	int			field_230;
	int			field_234;
	int			m_CullMode;
	char		field_23C;
	char		m_TextureBlendingEnabled;
	char		field_23E;

	int			m_TextureStageStateType;
	int			field_244;
	int			field_248;
	int			field_24C;
	int			field_250;
	int			field_254;
	int			field_258;
	int			field_25C;
	int			field_260;
	char		field_264;
	int			field_268;
	int			field_26C;
	int			field_270;
	int			field_274;
	int			field_278;
	int			field_27C;
	int			field_280;
	int			field_284;
	int			field_288;
	char		field_28C;
	int			field_290;
	int			field_294;
	int			field_298;
	char		field_29C;
	char		field_29D;
	int			field_2A0;
	int			field_2A4;
	int			field_2A8;
	char		field_2AC;
	char		field_2AD;
	char		field_2AE;
	char		field_2AF;
	char		field_2B0;
	char		field_2B1;
	char		field_2B2;
	char		field_2B3;
	int			field_2B4;
	int			field_2B8;
	int			field_2BC;
	int			field_2C0;
	int			field_2C4;
	char		field_2C8;
	char		field_2C9;
	char		field_2CA;
	char		field_2CB;
	bool		m_FlushDirectly;	//	2CC

	IDirect3DTexture9*	m_TexturesArray[16];
public:
	bool		m_Windowed;
protected:
	bool		m_RenderingScene;
	D3DFORMAT	m_DisplayModeFormat;
	unsigned int	m_ViewportWidth_1;	//	NOTE: backbuffer dimensions?
	unsigned int	m_ViewportHeight_1;
	Vector2<unsigned int>	m_ViewportResolution;
	Vector2<unsigned int>	m_DisplayModeResolution;

	VerticesBuffer	m_VerticesBuffer[750];
	char		m_DrawingText;
	int			m_ActiveTextRenderBuffer;
	int			m_ActiveTextureColor;
	float		m_FOV;
	float		m_AspectRatio;
	float		m_NearPlane;
	float		m_FarPlane;
	GfxInternal_Dx9_Vertex*	m_VertexBuffer[4];
	D3DXMATRIX	m_ProjectionMatrix;
	D3DXMATRIX	m_ViewMatrix;
	D3DXMATRIX	m_IdentityMatrix;
	D3DXMATRIX	m_ModelMatrix;
	D3DXMATRIX	m_BoneMatrix;

	int			field_56A4[4080];

	char		field_9664;
	int			field_9668;
	int			m_WireframeColor;
	int			field_9670;
	int			field_9674;
	float		m_Opacity;
	float		m_EnvironmentMapCoefficient;
	ColorRGB	m_EnvironmentMapColors;
	char		m_AlphaChannelEnabled;
	char		m_AlphaTestEnabled;
	char		m_LightingEnabled;
	int			m_BlendMode;
	float		m_MipMapBias;
	bool		m_MipMappingEnabled;
	int			m_Filter;
	int			m_TextureAddressModes[4];
	float		m_ZBias;
	bool		m_FogEnabled;
	float		m_AlphaTestThreshhold;
	char		m_EnvironmentMap;
	int			m_FVF;
	IDirect3DVertexDeclaration9*	m_Direct3DVertexDeclaration;
	void*		m_GameCameraMatrix;
	int			m_RenderStateRop2Type;
	float		m_FogStart;
	float		m_FogEnd;
	float		m_FogDensity;
	ColorRGB	m_FogColor;
	IDirect3DIndexBuffer9*	m_IndexBuffer;
	GfxInternal_Dx9_Vertex*	m_CurrentVertexBuffer;
	GfxInternal_Dx9_Texture*	m_TexturesArray_2[2];
	int			field_9700;
	int			field_9704;
	char		m_TextureStages[2];
	GfxInternal_Dx9_Texture*	m_RenderTargetTexture;
	char		field_9710;
	int			m_ActiveViewportSurfaceIndex;
	GfxInternal_Dx9_Texture*	m_ViewportTexturesArray[4];
	IDirect3DSurface9*	m_DepthStencilSurface;
	int			m_AxisAlign;
	char		m_ParticleRenderDisabled;
	int			m_ParticleSize;
	Vector4f	m_ParticleOrient;
	int			m_ParticleAlign;
	int			m_ParticleColor;
	int*		m_ParticleMeshBuffer;	//	TODO: is this correct name?
	int			m_ParticleSystemEnded;
	float		field_9758;
	int			field_975C;
	std::vector<LightStatus>	m_SceneLights;
	std::map<int, int>	field_9770;
	int			field_977C;
	int			field_9780;
	int			field_9784;
	int			field_9788;
	int			field_978C;
	int			field_9790;
	int			field_9794;
	int			field_9798;
	int			field_979C;

public:
	GfxInternal_Dx9(const Vector2<unsigned int>& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int unk1);	//	@45E620
	~GfxInternal_Dx9();	//	@45DA20

	void* operator new(size_t size)
	{
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}

	void		RememberResolution();	//	@44CFF0
	bool		GetRegistryResolution(Vector2<unsigned int>&);	//	@44D080
	bool		ProcessGameInput();	//	@44D140
	void		stub1();	//	@44D1A0
	void		SetFVF(unsigned int);	//	@44D1B0
	void		SetVertexDeclaration(IDirect3DVertexDeclaration9*);	//	@44D1F0
	void		IssueDeviceReset();	//	@44D230
	void		ProcessFramesyncQuery();	//	@44D260
	void		EndScene();	//	@44D2A0
	void		stub2();	//	@44D2D0
	void		SetTExtureStage1TransformationMatrix();	//	@44D2E0
	void		EnableEnvironmentMap();	//	@44D5D0
	void		SetEnvironmentMapCoef(float);	//	@44D8C0
	void		LoadDDSTexture(unsigned int, const char*);	//	@44D920	//	NOTE: unused, but starting point for loading texture file.
	HRESULT		SetCurrentTextureIndex(unsigned int);	//	@44D9B0
	void		EnableZTest(bool);	//	@44D9D0
	void		EnableZWrite(bool);	//	@44DA10
	void		EnableFog(bool);	//	@44DA50
	void		GetFogParams(unsigned int* state, ColorRGB* color, float* start, float* end, float* density);	//	@44DA70
	void		EnableLighting(bool);	//	@44DAE0
	void		SetMipMapBias(float);	//	@44DB20
	void		SetMipMapping(bool);	//	@44DB50
	void		SetFilterMode(unsigned int);	//	@44DBE0
	void		SetViewport(Vector2<float>*, Vector2<float>*);	//	@44DCA0	//	NOTE: unused, but referenced.
	void		GetProjectionParams(float*, float* aspectratio, float*, float* farplane);
	void		SetRenderStateWireframe(bool);	//	@44DD00
	void		SetUnknownColor(ColorRGB*);	//	@44DE80
	void		SetWorldMatrix(D3DMATRIX*);	//	@44DEF0
	void		SetOpacity(float);	//	@44DF90
	void		EnableAlphaChannel(bool);	//	@44E090
	void		SetBlendMode(unsigned int);	//	@44E160
	void		EnableAlphaTest(bool);	//	@44E1A0
	void		SetAlphaTestThreshold(float);	//	@44E1E0
	void		SetRenderTarget(IDirect3DSurface9*);	//	@44E220
	void		TransformStateView(D3DMATRIX*);	//	@44E400
	void		SetProjection(float fov, float aspectratio, float nearplane, float farplane);	//	@44E580
	void		_44E680(const D3DMATRIX*, Vector2<float>*, unsigned int*);	//	@44E680
	int			GetAvailableTextureMemory() const;	//	@44E960	//	NOTE: unused completely.
	void		DumpScreenShot(class GfxInternal_Dx9_Surface* surf);	//	@44E970
	void		SetTextureAddressMode(int, int ind);	//	@44EB30
	void		SetCullMode(unsigned int);	//	@44EB80
	HRESULT		GetDeviceCaps(IDirect3D9*, D3DCAPS9*);	//	@44EBC0
	void		DestroySurfaces();	//	@44ED40
	void		stub3(int);	//	@44EE30
	void		_44EE40();	//	@44EE40	//	++field_9674;
	void		SetVertexBuffer(void*);	//	@44EE50	//	TODO: first param is some internal vertex class.
	void		SetIndexBuffer(void*);	//	@44EE60	//	TODO: first param is some internal index class.
	void		SetTextureScroll(float*, float);	//	@44EE70
	void		SetTextureIndex(class Texture*, unsigned int);	//	@44EF70
	void		BeginText(int, class Texture*, const ColorRGB&);	//	@44EFA0
	void		RenderText(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, bool);	//	@44F050
	void		RenderText2(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, bool);	//	@44F310
	void		BeginShadow(class Texture*);	//	@44F5B0
	void		EndShadow();	//	@44F600
	void		SetBoneMatrix(unsigned int, const D3DMATRIX&);	//	@44F640
	void		SetParticleSize(unsigned int);	//	@44F790
	void		SetParticleOrient(const Quaternion<float>&);	//	@44F7B0
	void		SetParticleAlign(unsigned int);	//	@44F7E0
	void		SetParticleColor(unsigned int);	//	@44F7F0
	IDirect3DVertexBuffer9* CreateVertexBuffer(unsigned int length, unsigned int usage, unsigned int fvf, IDirect3DVertexBuffer9* pool);	//	@44F800	//	NOTE: unused completely.
	IDirect3DIndexBuffer9* CreateIndexBuffer(unsigned int length, unsigned int usage, IDirect3DIndexBuffer9* pool);	//	@44F830	//	NOTE: unused completely.
	IDirect3DTexture9* CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned int usage, D3DFORMAT format, IDirect3DTexture9* outTexture);	//	@44F860
	void		UpdateTextureStage();	//	@44F8A0
	void		SetFlushDirectly();	//	@44F910	//	NOTE: unused completely.
	void		SetupViewportSurface();	//	@44F940
	void		HandleDeviceLost();	//	@44F960
	void		Clear(unsigned char flags, const ColorRGB& clearcolor);	//	@44F9D0
	void		SetZBias(unsigned int);	//	@44FAA0
	void		SetFogProperties(unsigned int, const ColorRGB& color, float start, float end, float density);	//	@44FAE0
	void		DrawIndexedPrimitive(unsigned int startindex, unsigned int, unsigned int minvertexindex, unsigned int);	//	@44FC40
	void		SetupRenderer();	//	@44FD00
	void		CreateVertexBuffersObjects(unsigned int);	//	@450610
	void		DestroyVertexBuffersObjects();	//	@450710
	void		RenderSkinnedMeshBuffer(void* meshbuffer);	//	@4507B0
	void		ResetStreamTextures();	//	@450810
	DisplayModeInfo* IsScreenResolutionAvailable(unsigned int width, unsigned int height, bool dontignoreunavailable);	//	@450890
	void		EnumDisplayModes();	//	@4508F0
	char		_4509F0(void*);	//	@4509F0
	void		CreateSurfaces();	//	@450A30
	void		_450DB0();	//	@450DB0
	void		Reset();	//	@451020
	bool		BeginScene();	//	@451080
	void		CreateRenderDevice();	//	@451110
	void		RenderLine2D(const Vector2<float>&, const Vector2<float>&, const ColorRGB&, float*);	//	@451270
	void		RenderLine(const Vector3<float>&, const Vector3<float>&, const ColorRGB&);	//	@4514B0
	void		_4516A0(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector3<float>&, unsigned int, unsigned int);	//	@4516A0
	void		RenderTriangle2D(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@451920
	void		RenderTriangle_2(const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const ColorRGB&, const float*, const float*);	//	@451BC0
	void		RenderTexturedTriangle(const Texture*, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&, const ColorRGB&, const ColorRGB&);	//	@451E90
	void		RenderQuad2D(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@452220
	void		RenderTexturedQuad2D_4(const Texture*, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&, const ColorRGB&, const ColorRGB&, const ColorRGB&);	//	@452260
	void		RenderTexturedQuad_3(const Texture*, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&, const ColorRGB&, const ColorRGB&, const ColorRGB&);	//	@452750
	void		EndText(unsigned int, unsigned int);	//	@453D60
	void		RenderShadow(const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, float r, float g, float b);	//	@453F20
	void		RenderMeshBuffer(const void* meshbuffer);	//	@4540E0
	void		RenderMeshColorBuffer(const void* meshcolorbuffer);	//	@454660
	void		RenderVertexBuffer(const class Vertex*, unsigned int startvertex, unsigned int primitivecount);	//	@454C80
	void		EndParticleSystem(bool);	//	@454D40
	void		RenderParticle(const Vector3<float>&);	//	@454D70
	void		DrawBrightness(float);	//	@45A9A0
	void		DrawSaturation(float);	//	@45AC00
	void		DrawVignette(const Texture*, const Vector3<float>&, float, float, float);	//	@45AE60
	void		SetupWindowParams(unsigned int width, unsigned int height);	//	@45BE30
	void		SetupWindowParams_2(const Vector2<unsigned int> resolution);	//	@45BEF0
	bool		SetScreenResolution(unsigned int width, unsigned int height);	//	@45BF90
	bool		SetupScreenRes();	//	@45C0D0
	void		RenderTriangle(const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const ColorRGB&);	//	@45C250
	void		RenderQuad(const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const ColorRGB&);	//	@45C270
	void		RenderTexturedQuad2D_3(const class Texture*, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@45C2B0
	void		RenderTexturedQuad_2(const class Texture*, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@45C2F0
	void		DrawNoise(const Texture*, float, int);	//	@45C330
	void		RenderTexturedQuad2D_2(const class Texture*, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@45C610
	void		RenderTexturedQuad_1(const class Texture*, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const ColorRGB&);	//	@45C6D0
	void		_45C790(float);	//	@45C790
	void		DrawLightBleeding(unsigned int);	//	@45C9A0
	void		RenderTexturedQuad2D_1(const class Texture*, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@45D4A0
	void		_45D5E0();	//	@45D5E0
	void		RenderFullscreenTexture(const class Texture*);	//	@45D940
	void		EnableLight(void*, unsigned int lightindex);	//	@45DBA0
	void		_45E5D0(LightStatus&);	//	@45E5D0	//	NOTE: toggle light from scene.

	static std::map<int, int>	RenderedTexturesMap;	//	@A39F50
	static const D3DMATRIX		IdentityMatrix;	//	@A0AD38
	static bool					ProcessingInput;	//	@A39F10

	static void GetScreenResolution(Vector2<unsigned int>& outRes);	//	@41FD70
};

extern GfxInternal_Dx9* g_GfxInternal_Dx9;	//	@A39F14

ASSERT_CLASS_SIZE(GfxInternal_Dx9, 38816);
ASSERT_CLASS_SIZE(GfxInternal_Dx9_Vertex, 40);