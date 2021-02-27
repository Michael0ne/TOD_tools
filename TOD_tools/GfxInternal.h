#pragma once

#include "GfxInternal_Dx9.h"

#define RENDERER_CLASS_SIZE 72

class Buffer276
{
public:
	Vector3f	m_BufferSize;
	class Scene_Buffer*	m_RenderBuffer;
	int*		field_10;
	char		field_14;
	D3DXMATRIX	m_ViewMatrix;
	D3DXMATRIX	m_MatrixUnknown_1;
	int			field_98;
	D3DXMATRIX	m_MatrixUnknown_2;
	char		field_DC;
	Vector4f	m_ProjectionMatrixParams;	//	NOTE: fov, xy_ratio, near_clip, far_clip.
	Vector2f	m_ViewportDimensions_1;
	Vector2f	m_ViewportDimensions_2;
	int			m_ClearFlags;
	ColorRGB	m_ClearColor;

public:
	Buffer276() {};
	Buffer276(const Vector3f& bufferSize);	//	@41FE80

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}
};

struct Scene_Buffer108
{
private:
	int	field_0;
	int	field_4;
	int	field_8;
	int	field_C;
	int	field_10;
	int	field_14;
	int	field_18;
	int	field_1C;
	int	field_20;
	int	field_24;
	int	field_28;
	int	field_2C;
	int	field_30;
	int	field_34;
	int	field_38;
	int	field_3C;
	int	field_40;
	int	field_44;
	int	field_48;
	int	field_4C;
	int	field_50;
	int	field_54;
	int	field_58;
	int	field_5C;
	int	field_60;
	int	field_64;
	int	field_68;

public:
	Scene_Buffer108()
	{
		MESSAGE_CLASS_CREATED(Scene_Buffer108);
	}
	~Scene_Buffer108()
	{
		MESSAGE_CLASS_DESTROYED(Scene_Buffer108);
	}

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, true);
	}

	void	Init(unsigned int unk1, unsigned char unk2, unsigned int unk3);	//	@422330
};

struct Scene_Buffer68
{
private:
	int	field_0;
	int	field_4;
	int	field_8;
	int	field_C;
	int	field_10;
	int	field_14;
	int	field_18;
	int	field_1C;
	int	field_20;
	int	field_24;
	int	field_28;
	int	field_2C;
	int	field_30;
	int	field_34;
	int	field_38;
	int	field_3C;
	int	field_40;

public:
	Scene_Buffer68()
	{
		MESSAGE_CLASS_CREATED(Scene_Buffer68);
	}
	~Scene_Buffer68()
	{
		MESSAGE_CLASS_DESTROYED(Scene_Buffer68);
	}

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, true);
	}

	void	Init(const Scene_Buffer108& buf, unsigned int unk);	//	@4617D0
};

class ScreenProperties
{
	friend class ScriptType_Builtin;
protected:
	float m_fVirtualHudScreensizeWidth;
	float m_fVirtualHudScreensizeHeight;
	float field_8;	//	NOTE: scaleX?
	float field_C;	//	NOTE: scaleY?
	float m_fScreenWidth;
	float m_fScreenHeight;
	float m_fScreenRatio;
	float m_fScreenSafeArea;
	char m_bSafeArea;	//	NOTE: is this float?
	char field_21[3];
	float m_fVirtualHudScreensizeHalfWidth;
	float m_fVirtualHudScreensizeHalfHeight;
	float field_2C;
	float field_30;
	float m_fScreenWidthHalf;
	float m_fScreenHeightHalf;
	float field_3C;
	float field_40;

public:
	void	SetHudScreenSize(float width, float height, float unk1, float unk2);	//	@420D60
	void	AdjustWindowScalings();	//	@420190
	void	SetSafeArea(float area);	//	@420DD0
	void	SetWindowProperties(float width, float height, float ratio, float safearea);	//	@420D90
};

class GfxInternal
{
private:
	bool							m_RenderBufferEmpty;	//	NOTE: this is set when failed to allocate space for buffer from stack.
	List<GfxInternal_Dx9_Texture>	m_TexturesList;
	Scene_Buffer68*					m_Buffer68;
	Scene_Buffer108*				m_Buffer108;
	unsigned int					m_RenderBufferTotal;
	int								field_20;
	Buffer276*						m_RenderBufferArray;
	float							m_TimeDelta;
	int								m_TimeMilliseconds;
	int								m_FramesRendered;
	char							field_34;
	char							field_35;
	float							m_Time_1;
	char							(* m_SceneCallback)(int);
	__int64							m_RenderEndTime;

public:
	GfxInternal(unsigned int resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1, const Vector3<float>* buffersDimens);	//	@421320
	~GfxInternal();	//	@421470

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}

	void							SetClearColorForBufferIndex(const ColorRGB& color, int index);	//	@41FDF0
	void							SetClearFlagsForBufferIndex(const unsigned int flags, const int index);	//	@41FD90
	void							SetRenderBufferIsEmpty(bool);	//	@420170
	void							PrepareForNewLevel();	//	@420180
	void							DumpScreenShot(class GfxInternal_Dx9_Surface* surf) const;	//	@420100
	Vector2<int>&					GetScreenResolution(Vector2<int>& res) const;	//	@41FD70
	bool							IsScreenResolutionAvailable(int width, int height) const;	//	@485460

	static bool						IsWideScreen();	//	@420120

	static bool						WideScreen;	//	@A39F12
	static bool						FSAA;
	static float					RatioXY;	//	@A119F4
	
	struct Renderer_Buffer2
	{
		unsigned int	field_0;
		unsigned int	field_4;
	};

	static Renderer_Buffer2			_A08704[28];	//	@A08704
};

extern ScreenProperties g_ScreenProperties;
extern GfxInternal* g_GfxInternal;

//	NOTE: this is used in 'ExecuteRenderCommand' @4342C0
enum RendererCommandsList
{
	CMD_SETMODELMATRIX = 0,
	CMD_SETCURRENTTEXTURE,
	CMD_SETTEXTURESCROLL,
	CMD_ENABLEENVIRONMENTMAP,
	CMD_SETENVIRONMENTMAPCOEF,
	CMD_SETBRIGHTNESS,
	CMD_RENDERTRIANGLE,
	CMD_RENDERTRIANGLE_2,
	CMD_RENDERTEXTUREDTRIANGLE,
	CMD_RENDERQUAD,
	CMD_RENDERLINE,
	CMD_RENDERLINE2D,
	CMD_RENDERTRIANGLE2D,
	CMD_RENDERQUAD2D,
	CMD_RENDERQUAD2D_2,
	CMD_RENDERFULLSCREENTEXTURE,
	CMD_RENDERVERTEXBUFFER,
	CMD_RENDERMESHBUFFER,
	CMD_RENDERMESHCOLORBUFFER,
	CMD_STARTCHARACTERRENDER,
	CMD_ENDCHARACTERRENDER,
	CMD_RENDERSKINNEDMESHBUFFER,
	CMD_SETBONEMATRIX,
	CMD_BEGINTEXT,
	CMD_RENDERTEXT,
	CMD_RENDERTEXT2,
	CMD_ENDTEXT,
	CMD_BEGINSHADOW,
	CMD_RENDERSHADOW,
	CMD_ENDSHADOW,
	CMD_CALL,
	CMD_ENABLELIGHTING,
	CMD_ENABLELIGHT,
	CMD_ENABLEZWRITE,
	CMD_ENABLEZTEST,
	CMD_ENABLEFOG,
	CMD_SETFOGPROPERTIES,
	CMD_SETFILTERMODE,
	CMD_SETCULLMODE,
	CMD_SETZBIAS,
	CMD_SETBLENDMODE,
	CMD_SETOPACITY,
	CMD_SETVIEWPORT,
	CMD_SETPROJECTION,
	CMD_SETMIPMAPBIAS,
	CMD_ENABLEMIPMAPPING,
	CMD_SETTEXTUREADDRESSMODE,
	CMD_SETRENDERTARGET,
	CMD_RENDERTEXTUREDQUAD2D_1,
	CMD_RENDERTEXTUREDQUAD2D_2,
	CMD_RENDERTEXTUREDQUAD2D_3,
	CMD_RENDERTEXTUREDQUAD2D_4,
	CMD_RENDERTEXTUREDQUAD_1,
	CMD_RENDERTEXTUREDQUAD_2,
	CMD_RENDERTEXTUREDQUAD_3,
	CMD_ENABLEALPHACHANNEL,
	CMD_SETALPHATESTTHRESHHOLD,
	CMD_ENABLEALPHATEST,
	CMD_PS2_CALLDMA,
	CMD_PS2_UPLOADVUCODE,
	CMD_PS2_RELOADCAMERAMATRIX,
	CMD_PS2_SETGUARDBAND,
	CMD_SETAXISALIGN,
	CMD_SETPARTICLE,
	CMD_BEGINPARTICLESYSTEM,
	CMD_ENDPARTICLESYSTEM,
	CMD_SETPARTICLESIZE,
	CMD_SETPARTICLEORIENT,
	CMD_RENDERPARTICLE,
	CMD_SETPARTICLEALIGN,
	CMD_SETPARTICLECOLOR,
	CMD_SETPARTICLEOPACITY,
	CMD_SETMIPMAPLEVELS,
	CMD_DRAWBRIGHTNESS,
	CMD_DRAWSATURATION,
	CMD_DRAWLIGHTBLEEDING,
	CMD_DRAWVIGNETTE,
	CMD_DRAWNOISE,
	CMD_PUSH_MODELMATRIX,
	CMD_PUSH_TEXTURE,
	CMD_PUSH_ENVIRONMENTMAP,
	CMD_PUSH_ENVIRONMENTMAPCOEF,
	CMD_PUSH_BRIGHTNESS,
	CMD_PUSH_ENABLELIGHTING,
	CMD_PUSH_ENABLELIGHT,
	CMD_PUSH_ZWRITE,
	CMD_PUSH_ZTEST,
	CMD_PUSH_FOG,
	CMD_PUSH_FOGPROPERTIES,
	CMD_PUSH_FILTER,
	CMD_PUSH_CULLMODE,
	CMD_PUSH_ZBIAS,
	CMD_PUSH_BLENDMODE,
	CMD_PUSH_OPACITY,
	CMD_PUSH_VIEWPORT,
	CMD_PUSH_PROJECTION,
	CMD_PUSH_MIPMAPBIAS,
	CMD_PUSH_MIPMAPPING,
	CMD_PUSH_TEXTUREADDRESSMODE,
	CMD_PUSH_RENDERTARGET,
	CMD_PUSH_ALPHACHANNEL,
	CMD_PUSH_SETALPHATESTTHRESHHOLD,
	CMD_PUSH_ENABLEALPHATEST,
	CMD_PUSH_PS2_SETGUARDBAND,
	CMD_POP_MODELMATRIX,
	CMD_POP_TEXTURE,
	CMD_POP_ENVIRONMENTMAP,
	CMD_POP_ENVIRONMENTMAPCOEF,
	CMD_POP_BRIGHTNESS,
	CMD_POP_ENABLELIGHTING,
	CMD_POP_ENABLELIGHT,
	CMD_POP_ZWRITE,
	CMD_POP_ZTEST,
	CMD_POP_FOG,
	CMD_POP_FOGPROPERTIES,
	CMD_POP_FILTER,
	CMD_POP_CULLMODE,
	CMD_POP_ZBIAS,
	CMD_POP_BLENDMODE,
	CMD_POP_OPACITY,
	CMD_POP_VIEWPORT,
	CMD_POP_PROJECTION,
	CMD_POP_MIPMAPBIAS,
	CMD_POP_MIPMAPPING,
	CMD_POP_TEXTUREADDRESSMODE,
	CMD_POP_RENDERTARGET,
	CMD_POP_ALPHACHANNEL,
	CMD_POP_SETALPHATESTTHRESHHOLD,
	CMD_POP_ENABLEALPHATEST,
	CMD_POP_PS2_SETGUARDBAND
};	//	@A089D8

static_assert(sizeof(GfxInternal) == RENDERER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GfxInternal));