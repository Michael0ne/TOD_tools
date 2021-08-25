#pragma once
#include "Node.h"

class LensFlare : public Node
{
    enum BlendMode
    {
        NORMAL = 0,
        ADD = 1
    };

protected:
    TextureAsset   *m_TextureRes;
    int             field_54;
    float           m_Offset;
    float           m_Size;
    BlendMode       m_BlendMode;
    float           m_FadeCameraAngle1;
    float           field_68;
    float           m_FadeCameraAngle2;
    float           field_70;
    float           m_FadeCamBlindDiming;
    float           m_Opacity;
    ColorRGB        m_Color;
    float           m_FadeFlareAngle;
    float           field_90;
    char            m_InverseFlareAngle;

public:
    LensFlare();	//	@8E3F50
    virtual ~LensFlare();   //  @8E5670

    void            SetColor(const float* args);    //  @8E3840

    static LensFlare*   Create(AllocatorIndex); //  @8E56B0
};

ASSERT_CLASS_SIZE(LensFlare, 152);