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
		return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
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
	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, true);
	}

	Scene_Buffer108()
	{
		MESSAGE_CLASS_CREATED(Scene_Buffer108);
	}

	~Scene_Buffer108()
	{
		MESSAGE_CLASS_DESTROYED(Scene_Buffer108);
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
	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, true);
	}

	Scene_Buffer68()
	{
		MESSAGE_CLASS_CREATED(Scene_Buffer68);
	}

	~Scene_Buffer68()
	{
		MESSAGE_CLASS_DESTROYED(Scene_Buffer68);
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
	char field_0[4];
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
		return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	void	CreateRenderer(void* resolution, int unk1, int unk2, int fsaa, int buffersCount, int unk4, Vector3<float>* buffers);	//	@421320
	void	_41FDF0(Vector4<float>* size, int bufferIndex);	//	@41FDF0	NOTE: maybe 'SetBackBufferSize'?
	void	_SetBufferStateByIndex(int state, int index);	//	@41FD90

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

extern Renderer* g_Renderer;

static_assert(sizeof(Renderer) == RENDERER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Renderer));