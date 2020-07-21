#pragma once

#include "Node.h"

#define GFXEFFECT_CLASS_SIZE 160

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
	float m_VignetteIntensity;
	ColorRGB m_VignetteColor;
	float m_VignetteShape;
	float m_VignetteSize;
	int* m_ResourceInfo;
	int field_8C;
	float m_NoiseIntensity;
	int m_NoiseBlendMode;
	int* m_NoiseTexture;
	int field_9C;

public:
	GFXEffect();	//	@8E2F70

	static int&		TotalCreated;	//	@A3E050
};

static_assert(sizeof(GFXEffect) == GFXEFFECT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GFXEffect));