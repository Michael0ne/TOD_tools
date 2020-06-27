#pragma once

#include "stdafx.h"
#include "Types.h"

#define GFXEFFECT_CLASS_SIZE 160

class GFXEffect
{
private:
	void* lpVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	void* lpPositionVtbl;
	int field_28;
	int field_2C;
	int* field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	int field_58;
	float m_fMotionBlur;
	float m_fBrightness;
	float m_fSaturation;
	float m_fLightBleeding;
	float m_fVignetteIntensity;
	ColorRGB m_VignetteColor;
	float m_fVignetteShape;
	float m_fVignetteSize;
	int m_szVignetteTexture;
	int field_8C;
	float m_fNoiseIntensity;
	int m_nNoiseBlendMode;
	int m_szNoiseTexture;
	int field_9C;

public:
	static int&		ms_TotalCreated;	//	@A3E050
};

static_assert(sizeof(GFXEffect) == GFXEFFECT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GFXEffect));