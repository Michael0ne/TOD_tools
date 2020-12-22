#pragma once

#include "Node.h"

#define GFXEFFECT_CLASS_SIZE 160

enum GfxNoiseBlendMode
{
	BLEND_NORMAL = 0,
	BLEND_ADD = 1
};

class GFXEffect : public Node
{
protected:
	int field_50;
	int field_54;
	int field_58;
	float m_MotionBlur;
	float m_Brightness;
	float m_Saturation;
	float m_LightBleeding;
	float m_VignetteIntensity;	//	NOTE: range from 0.f to 1.f
	ColorRGB m_VignetteColor;
	float m_VignetteShape;	//	NOTE: range from 0.1f to 10.f
	float m_VignetteSize;
	ResType::Texture* m_VignetteTexture;
	int field_8C;
	float m_NoiseIntensity;
	GfxNoiseBlendMode m_NoiseBlendMode;
	ResType::Texture* m_NoiseTexture;
	int field_9C;

public:
	GFXEffect();	//	@8E2F70

	static int&		TotalCreated;	//	@A3E050
};

static_assert(sizeof(GFXEffect) == GFXEFFECT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GFXEffect));