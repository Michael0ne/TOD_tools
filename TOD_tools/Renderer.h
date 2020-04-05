#pragma once

#include "stdafx.h"

#include "Types.h"
#include "List.h"

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
};

extern Renderer* g_Renderer;

static_assert(sizeof(Renderer) == RENDERER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Renderer"));