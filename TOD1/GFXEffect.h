#pragma once
#include "Node.h"
#include "FrameBuffer.h"

struct GFXEffectProperties
{
    enum eBlendMode
    {
        NORMAL = 0,
        ADD = 1
    };

    float           m_MotionBlur = 0.f;
    float           m_Brightness = 1.f;
    float           m_Saturation = 1.f;
    float           m_LightBleeding = 0.f;
    float           m_VignetteIntensity = 0.f; // NOTE: range from 0.f to 1.f
    ColorRGB        m_VignetteColor;
    float           m_VignetteShape = 2.f; // NOTE: range from 0.1f to 10.f
    float           m_VignetteSize = 0.f;
    AssetLoader     m_VignetteTexture;
    float           m_NoiseIntensity = 0.f;
    eBlendMode      m_NoiseBlendMode = ADD;
    AssetLoader     m_NoiseTexture;

    inline GFXEffectProperties()
    {
        MESSAGE_CLASS_CREATED(GFXEffectProperties);
    }
    ~GFXEffectProperties() // @8E3620
    {
        MESSAGE_CLASS_DESTROYED(GFXEffectProperties);
    }
};

class GFXEffect : public Node
{
protected:
    FrameBuffer    *m_FrameBuffers[3];
    GFXEffectProperties m_EffectProperties;
public:
    GFXEffect(); // @8E2F70
    virtual ~GFXEffect(); // @8E2EF0

    const float     GetMotionBlur() const;  //  @905800
    void            SetMotionBlur(const float mblur);   //  @8E2BA0
    const float     GetBrightness() const;  //  @8A6D40
    void            SetBrightness(const float brightness);  //  @686DB0
    const float     GetSaturation() const;  //  @905830
    void            SetSaturation(const float saturation);  //  @659E20
    const float     GetLightBleeding() const;   //  @905840
    void            SetLightBleeding(const float lbleeding);    //  @659E40
    const float     GetVignetteIntensity() const;   //  @905850
    void            SetVignetteIntensity(const float intensity);    //  @659E60
    void            GetVignetteColor(ColorRGB& outColor) const; //  @496C60
    void            SetVignetteColor(const ColorRGB& color);    //  @659E80
    const float     GetVignetteShape() const;   //  @89AD10
    void            SetVignetteShape(const float shape);    //  @659EB0
    const float     GetVignetteSize() const;    //  @916E30
    void            SetVignetteSize(const float size);  //  @659ED0
    const char* const   GetVignetteTexture() const; //  @8E2C30
    void            SetVignetteTexture(const char* const texturepath);  //  @8E3070
    const float     GetNoiseIntensity() const;  //  @5A1D40
    void            SetNoiseIntensity(const float intensity);   //  @659EF0
    const GFXEffectProperties::eBlendMode    GetNoiseBlendMode() const;  //  @928790
    void            SetNoiseBlendMode(const GFXEffectProperties::eBlendMode blendmode);  //  755290
    const char* const   GetNoiseTexture() const;    //  @8E2C50
    void            SetNoiseTexture(const char* const texturepath); //  @8E3140

    static void     Register(); //  @8E3210
    static GFXEffect* Create(AllocatorIndex);   //  @8E36C0

    static int      TotalCreated; // @A3E050
};

extern EntityType* tGFXEffect;  //  @A3E054

ASSERT_CLASS_SIZE(GFXEffect, 160);