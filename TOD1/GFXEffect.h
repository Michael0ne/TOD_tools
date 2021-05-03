#pragma once
#include "Node.h"
#include "TextureAsset.h"

class Scene_Buffer;

struct GFXEffectProperties
{
    enum BlendMode
    {
        NORMAL      = 0,
        ADD         = 1
    };

    friend class GFXEffect;
protected:
    float           m_MotionBlur = 0.f;
    float           m_Brightness = 1.f;
    float           m_Saturation = 1.f;
    float           m_LightBleeding = 0.f;
    float           m_VignetteIntensity = 0.f;	//	NOTE: range from 0.f to 1.f
    ColorRGB        m_VignetteColor;
    float           m_VignetteShape = 2.f;	//	NOTE: range from 0.1f to 10.f
    float           m_VignetteSize = 0.f;
    TextureAsset   *m_VignetteTexture = nullptr;
    bool            field_8C = true;
    float           m_NoiseIntensity = 0.f;
    BlendMode       m_NoiseBlendMode = ADD;

    TextureAsset   *m_NoiseTexture = nullptr;
    bool            field_9C = true;

    void            ClearEffectProperties();	//	@8E3620
};

class GFXEffect : public Node
{
protected:
    Scene_Buffer   *field_50;
    Scene_Buffer   *field_54;
    Scene_Buffer   *field_58;

     GFXEffectProperties m_EffectProperties;
public:
    virtual ~GFXEffect();	//	@8E2EF0
    GFXEffect();	//	@8E2F70

    static int      TotalCreated;	//	@A3E050
};

ASSERT_CLASS_SIZE(GFXEffect, 160);