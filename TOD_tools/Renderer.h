#pragma once

#include "stdafx.h"

#include "GfxInternal_Dx9.h"

#define RENDERER_CLASS_SIZE 72

class Scene_Buffer276
{
public:
	Vector3<float> m_vRes_1;
	int field_C;
	int field_10;
	int field_14;
	Vector4f m_vRightVec_1;
	Vector4f m_vUpVec_1;
	Vector4f m_vInVec_1;
	Orientation m_vOrient_1;
	Vector4f m_vRightVec_2;
	Vector4f m_vUpVec_2;
	Vector4f m_vInVec_2;
	Orientation m_vOrient_2;
	int field_98;
	Vector4f m_vRightVec_3;
	Vector4f m_vUpVec_3;
	Vector4f m_vInVec_3;
	Orientation m_vOrient_3;
	int field_DC;
	Vector4f m_vDimens_1;
	Vector2<float> m_vRes_2;
	Vector2<float> m_vRes_3;
	int field_100;
	Vector4f m_vPos_1;

public:
	Scene_Buffer276()
	{
		MESSAGE_CLASS_CREATED(Scene_Buffer276);
	}
	~Scene_Buffer276()
	{
		MESSAGE_CLASS_DESTROYED(Scene_Buffer276);
	}

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void	Init(const Vector3<float>& vDimensions);	//	@41FE80
	inline void	SetResolution(const Vector2<float>& vResolution)
	{
		m_vRes_3 = vResolution;
	}
};

struct Scene_Buffer2
{
public:
	int		field_0;	//	Could be 0, 1 or 2. Maybe enum?
	int		m_nIndex;	//	Always incremental from 0 to 27.
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

struct ScreenProperties
{
private:
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

class Renderer
{
private:
	bool m_RenderBufferEmpty;	//	NOTE: this is set when failed to allocate space for buffer from stack.
	List<GfxInternal_Dx9_Texture> m_TexturesList;
	Scene_Buffer68* m_pBuffer68;
	Scene_Buffer108* m_pBuffer108;
	int m_BuffersCount;
	int field_20;
	Scene_Buffer276* m_pBuffersArray;
	float m_fTimeDelta;
	int m_nTimeMilliseconds;
	int field_30;
	int field_34;
	__int64 m_nUnkTime_1;
	__int64 m_nUnkTime_2;

public:
	Renderer()
	{
		MESSAGE_CLASS_CREATED(Renderer);
	}
	~Renderer()
	{
		MESSAGE_CLASS_DESTROYED(Renderer);

		if (m_BuffersCount > 0)
			while (m_BuffersCount) {
				if (&m_pBuffersArray[m_BuffersCount])
					delete& m_pBuffersArray[m_BuffersCount];
				m_BuffersCount--;
			}
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void	CreateRenderer(void* resolution, int unk1, int unk2, int fsaa, int buffersCount, int unk4, Vector3<float>* buffers);	//	@421320
	void	_41FDF0(Vector4<float>* size, int bufferIndex);	//	@41FDF0	NOTE: maybe 'SetBackBufferSize'?
	void	_SetBufferStateByIndex(int state, int index);	//	@41FD90
	void	SetRenderBufferIsEmpty(bool);	//	@420170

	__int64	GetTime()
	{
		return m_nUnkTime_1;
	}

	static bool& WideScreen;
	static bool FSAA;
	static float& RatioXY;
	static ScreenProperties& g_ScreenProperties;
	static int& _A08704;
	static int& _A0870C;
	static int& _A0872C;
	static Scene_Buffer2& Buffer_A08704;
};

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

extern Renderer* g_Renderer;

static_assert(sizeof(Renderer) == RENDERER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Renderer));