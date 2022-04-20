#include "GFXEffect.h"
#include "NumberType.h"
#include "VectorType.h"
#include "StringType.h"
#include "IntegerType.h"

int GFXEffect::TotalCreated;
EntityType* tGFXEffect;

GFXEffect* GFXEffect::Create(AllocatorIndex)
{
    return new GFXEffect;
}

void GFXEffect::Register()
{
    tGFXEffect = new EntityType("GFXEffect");
    tGFXEffect->InheritFrom(tNode);
    tGFXEffect->SetCreator((CREATOR)Create);

    tGFXEffect->RegisterProperty(tNUMBER, "MotionBlur", (EntityGetterFunction)&GetMotionBlur, (EntitySetterFunction)&SetMotionBlur, "control=slider|min=0|max=1");
    tGFXEffect->RegisterProperty(tNUMBER, "Brightness", (EntityGetterFunction)&GetBrightness, (EntitySetterFunction)&SetBrightness, "control=slider|min=0|max=2");
    tGFXEffect->RegisterProperty(tNUMBER, "Saturation", (EntityGetterFunction)&GetSaturation, (EntitySetterFunction)&SetSaturation, "control=slider|min=0|max=2");
    tGFXEffect->RegisterProperty(tNUMBER, "LightBleeding", (EntityGetterFunction)&GetLightBleeding, (EntitySetterFunction)&SetLightBleeding, "control=slider|min=0|max=1");
    tGFXEffect->RegisterProperty(tNUMBER, "VignetteIntensity", (EntityGetterFunction)&GetVignetteIntensity, (EntitySetterFunction)&SetVignetteIntensity, "control=slider|min=0|max=1");
    tGFXEffect->RegisterProperty(tVECTOR, "VignetteColor", (EntityGetterFunction)&GetVignetteColor, (EntitySetterFunction)&SetVignetteColor, "control=colorrgb");
    tGFXEffect->RegisterProperty(tNUMBER, "VignetteShape", (EntityGetterFunction)&GetVignetteShape, (EntitySetterFunction)&SetVignetteShape, "control=slider|min=.1|max=10");
    tGFXEffect->RegisterProperty(tNUMBER, "VignetteSize", (EntityGetterFunction)&GetVignetteSize, (EntitySetterFunction)&SetVignetteSize, "control=slider|min=0|max=1");
    tGFXEffect->RegisterProperty(tSTRING, "VignetteTexture", (EntityGetterFunction)&GetVignetteTexture, (EntitySetterFunction)&SetVignetteTexture, "control=resource|type=*.bmp");
    tGFXEffect->RegisterProperty(tNUMBER, "NoiseIntensity", (EntityGetterFunction)&GetNoiseIntensity, (EntitySetterFunction)&SetNoiseIntensity, "control=slider|min=0|max=1");
    tGFXEffect->RegisterProperty(tINTEGER, "NoiseBlendMode", (EntityGetterFunction)&GetNoiseBlendMode, (EntitySetterFunction)&SetNoiseBlendMode, "control=dropdown|Normal=0|Add=1");
    tGFXEffect->RegisterProperty(tSTRING, "NoiseTexture", (EntityGetterFunction)&GetNoiseTexture, (EntitySetterFunction)&SetNoiseTexture, "control=resource|type=*.bmp");

    tGFXEffect->PropagateProperties();
}

void GFXEffect::SetBrightness(const float brightness)
{
    m_EffectProperties.m_Brightness = brightness;
    m_Id._3 = 1;
}

const float GFXEffect::GetSaturation() const
{
    return m_EffectProperties.m_Saturation;
}

void GFXEffect::SetSaturation(const float saturation)
{
    m_EffectProperties.m_Saturation = saturation;
    m_Id._3 = 1;
}

const float GFXEffect::GetLightBleeding() const
{
    return m_EffectProperties.m_LightBleeding;
}

void GFXEffect::SetLightBleeding(const float lbleeding)
{
    m_EffectProperties.m_LightBleeding = lbleeding;
    m_Id._3 = 1;
}

const float GFXEffect::GetVignetteIntensity() const
{
    return m_EffectProperties.m_VignetteIntensity;
}

void GFXEffect::SetVignetteIntensity(const float intensity)
{
    m_EffectProperties.m_VignetteIntensity = intensity;
    m_Id._3 = 1;
}

void GFXEffect::GetVignetteColor(ColorRGB& outColor) const
{
    outColor = m_EffectProperties.m_VignetteColor;
}

void GFXEffect::SetVignetteColor(const ColorRGB& color)
{
    m_Id._3 = 1;

    m_EffectProperties.m_VignetteColor = color;
}

const float GFXEffect::GetVignetteShape() const
{
    return m_EffectProperties.m_VignetteShape;
}

void GFXEffect::SetVignetteShape(const float shape)
{
    m_Id._3 = 1;
    m_EffectProperties.m_VignetteShape = shape;
}

const float GFXEffect::GetVignetteSize() const
{
    return m_EffectProperties.m_VignetteSize;
}

void GFXEffect::SetVignetteSize(const float size)
{
    m_EffectProperties.m_VignetteSize = size;
    m_Id._3 = 1;
}

const char* const GFXEffect::GetVignetteTexture() const
{
    return m_EffectProperties.m_VignetteTexture ? m_EffectProperties.m_VignetteTexture.m_AssetPtr->GetName() : nullptr;
}

void GFXEffect::SetVignetteTexture(const char* const texturepath)
{
    if (!texturepath)
        return;

    m_Id._3 = 1;
    m_EffectProperties.m_VignetteTexture = AssetLoader(texturepath);
}

const float GFXEffect::GetNoiseIntensity() const
{
    return m_EffectProperties.m_NoiseIntensity;
}

void GFXEffect::SetNoiseIntensity(const float intensity)
{
    m_EffectProperties.m_NoiseIntensity = intensity;
    m_Id._3 = 1;
}

const GFXEffectProperties::eBlendMode GFXEffect::GetNoiseBlendMode() const
{
    return m_EffectProperties.m_NoiseBlendMode;
}

void GFXEffect::SetNoiseBlendMode(const GFXEffectProperties::eBlendMode blendmode)
{
    m_EffectProperties.m_NoiseBlendMode = blendmode;
    m_Id._3 = 1;
}

const char* const GFXEffect::GetNoiseTexture() const
{
    return m_EffectProperties.m_NoiseTexture ? m_EffectProperties.m_NoiseTexture.m_AssetPtr->GetName() : nullptr;
}

void GFXEffect::SetNoiseTexture(const char* const texturepath)
{
    if (!texturepath)
        return;

    m_EffectProperties.m_NoiseTexture = AssetLoader(texturepath);
    m_Id._3 = 1;
}

GFXEffect::GFXEffect() : Node(NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(GFXEffect);

    TotalCreated++;

    m_QuadTree->m_UserType |= 0x8000000;
    m_FrameBuffers[0] = nullptr;
    m_FrameBuffers[1] = nullptr;
    m_FrameBuffers[2] = nullptr;
}

void GFXEffect::SetMotionBlur(const float mblur)
{
    m_EffectProperties.m_MotionBlur = mblur;
}

const float GFXEffect::GetBrightness() const
{
    return m_EffectProperties.m_Brightness;
}

GFXEffect::~GFXEffect()
{
    MESSAGE_CLASS_DESTROYED(GFXEffect);
}

const float GFXEffect::GetMotionBlur() const
{
    return m_EffectProperties.m_MotionBlur;
}