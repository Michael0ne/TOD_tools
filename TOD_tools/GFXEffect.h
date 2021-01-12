#pragma once

#include "Node.h"
#include "ResourcesTypes.h"

#define GFXEFFECT_CLASS_SIZE 160

struct GFXEffectProperties
{
	friend class GFXEffect;
protected:
	float m_MotionBlur = 0.f;
	float m_Brightness = 1.f;
	float m_Saturation = 1.f;
	float m_LightBleeding = 0.f;
	float m_VignetteIntensity = 0.f;	//	NOTE: range from 0.f to 1.f
	ColorRGB m_VignetteColor;
	float m_VignetteShape = 2.f;	//	NOTE: range from 0.1f to 10.f
	float m_VignetteSize = 0.f;

	ResType::Texture* m_VignetteTexture = nullptr;	//	TODO: these two may be 'ResourceHolder', in destructor they're referenced in same place.
	bool field_8C = true;

	float m_NoiseIntensity = 0.f;

	enum
	{
		BLEND_NORMAL = 0,
		BLEND_ADD
	} m_NoiseBlendMode = BLEND_ADD;

	ResType::Texture* m_NoiseTexture = nullptr;
	bool field_9C = true;

	void	ClearEffectProperties();	//	@8E3620
};

class GFXEffect : public Node
{
protected:
	class Scene_Buffer* field_50;
	class Scene_Buffer* field_54;
	class Scene_Buffer* field_58;

	 GFXEffectProperties m_EffectProperties;
public:
	virtual ~GFXEffect();	//	@8E2EF0
	GFXEffect();	//	@8E2F70

	static int		TotalCreated;	//	@A3E050
};

static_assert(sizeof(GFXEffect) == GFXEFFECT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GFXEffect));