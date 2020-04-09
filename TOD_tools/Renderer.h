#pragma once

#include "stdafx.h"

#include "Types.h"
#include "List.h"
#include "MemoryAllocators.h"

#include "GfxInternal_Dx9_Texture.h"

#define RENDERER_CLASS_SIZE 72

struct Scene_Buffer276
{
	Vector3<float> m_vUnkDimensionsVec_1;
	int field_C;
	int field_10;
	char field_14[4];
	Vector4f m_vRightVec;
	Vector4f m_vUpVec;
	Vector4f m_vInVec;
	Vector4f m_vUnkVec_1;
	Vector4f m_vRightVec_1;
	Vector4f m_vUpVec_1;
	Vector4f m_vInVec_1;
	Vector4f m_vUnkVec_2;
	int _pad;
	Vector4f m_vRightVec_2;
	Vector4f m_vUpVec_2;
	Vector4f m_vInVec_2;
	Vector4f m_vUnkVec_3;
	char field_DC[4];
	float m_fE0;
	float m_fE4;
	float m_fE8;
	float m_fEC;
	Vector2<float> m_vUnkVec_4;
	Vector2<float> m_vResolution;
	int field_100;
	ColorRGB m_unkColor;
};

struct ScreenProperties
{
private:
	float m_fVirtualHudScreensizeWidth;
	float m_fVirtualHudScreensizeHeight;
	float field_8;	//	NOTE: scaleX?
	float field_C;	//	NOTE: scaleY?
	float m_nScreenWidth;
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
};


class Renderer
{
private:
	char field_0[4];
	List<GfxInternal_Dx9_Texture*> m_TexturesList;
	int field_14;
	int* field_18;
	int m_nUnkBuffersCount;
	int field_20;
	Scene_Buffer276* m_pUnkBuffersArray;
	float m_fTimeDelta;
	int m_nTimeMilliseconds;
	int field_30;
	int field_34;
	__int64 m_nUnkTime_1;
	__int64 m_nUnkTime_2;

public:

	Renderer()
	{
		debug("Renderer created at %X\n", this);
	}

	~Renderer()
	{
		debug("Renderer destroyed!\n");
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	void	CreateRenderer(void* resolution, int unk1, int unk2, int fsaa, int buffersCount, int unk4, void* buffers);	//	@421320
	void	_41FDF0(Vector4<float>* size, int bufferIndex);	//	@41FDF0	NOTE: maybe 'SetBackBufferSize'?

	static bool& WideScreen;
	static bool FSAA;
	static float& RatioXY;
	static ScreenProperties& g_ScreenProperties;
	static int& _A08704;
	static int& _A0870C;
	static int& _A0872C;
};

extern Renderer* g_Renderer;

static_assert(sizeof(Renderer) == RENDERER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Renderer"));