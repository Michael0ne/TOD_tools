#include "GFXEffect.h"

GFXEffect::GFXEffect() : Node(NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(GFXEffect);

	m_ResourceInfo = nullptr;
	m_NoiseTexture = nullptr;

	field_8C = field_9C = 1;
	field_50 = field_54 = field_58 = 0;

	m_VignetteColor = ColorRGB();

	m_Brightness = 1.0f;
	m_Saturation = 1.0f;
	m_NoiseBlendMode = 1;
	m_VignetteShape = 2.0f;
	m_MotionBlur = 0.0f;
	m_LightBleeding = 0.0f;
	m_VignetteIntensity = 0.0f;
	m_VignetteSize = 0.0f;
	m_NoiseIntensity = 0.0f;

	++TotalCreated;
}

int& GFXEffect::TotalCreated = *(int*)0xA3E050;