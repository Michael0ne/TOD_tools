#pragma once
#include "Node.h"

class LensFlare : public Node
{
    enum eBlendMode
    {
        NORMAL = 0,
        ADD = 1
    };

protected:
    AssetLoader         m_Texture;
    float               m_Offset;
    float               m_Size;
    eBlendMode          m_BlendMode;
    float               m_FadeCameraAngleDeg1;
    float               m_FadeCameraAngleCos1;
    float               m_FadeCameraAngleDeg2;
    float               m_FadeCameraAngleCos2;
    float               m_FadeCamBlindDiming;
    float               m_Opacity;
    ColorRGB            m_Color;
    float               m_FadeFlareAngle;
    float               m_FadeFlareAngleCos;
    char                m_InverseFlareAngle;

public:
    LensFlare(); // @8E3F50
    virtual ~LensFlare();   //  @8E5670

    const char* const   GetTexture() const; //  @8E5660
    void                SetTexture(const char* const texturepath);  //  @8E56F0
    const eBlendMode    GetBlendMode() const;   //  @8E3700
    void                SetBlendMode(const eBlendMode blendmode);  //  @8E3710
    const float         GetOpacity() const; //  @905870
    void                SetOpacity(const float opacity);    //  @916E20
    const float         GetOffset() const;  //  @89A8A0
    void                SetOffset(const float offset);  //  @8E3720
    void                GetColorRGB(ColorRGB& outColor) const;  //  @8E3810
    void                SetColorRGB(const ColorRGB& color); //  @8E3840
    const float         GetSize() const;    //  @905800
    void                SetSize(const float size);  //  @8E3730
    const float         GetFadeCameraAngle1() const;    //  @905830
    void                SetFadeCameraAngle1(const float angle); //  @8E3740
    const float         GetFadeCameraAngle2() const;    //  @905850
    void                SetFadeCameraAngle2(const float angle); //  @8E3770
    const float         GetFadeCamBlindDiming() const;  //  @905860
    void                SetFadeCamBlindDiming(const float diming);  //  @8E37A0
    const float         GetFadeFlareAngle() const;  //  @501090
    void                SetFadeFlareAngle(const float angle);   //  @8E37B0
    const bool          InverseFlareAngle() const;  //  @8E3800
    void                SetInverseFlareAngle(const bool inverse);   //  @8E37F0

    static void         Register(); //  @8E5180
    static LensFlare*   Create(AllocatorIndex); //  @8E56B0
};

extern EntityType* tLensFlare;  //  @A3E05C

ASSERT_CLASS_SIZE(LensFlare, 152);