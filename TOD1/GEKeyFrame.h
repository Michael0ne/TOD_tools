#pragma once
#include "Node.h"

class GEKeyFrame : public Node
{
protected:
    float m_KeyTranslationX;
    float m_KeyTranslationY;
    float m_KeyTranslationZ;
    float m_f5C;
    float m_KeyScaleX;
    float m_KeyScaleY;
    float m_KeyScaleZ;
    float m_f6C;
    float m_KeyRotationX;
    float m_KeyRotationY;
    float m_KeyRotationZ;
    float m_f7C;
    float m_KeyTime;
    float m_KeyOpacity;
    float m_KeyTextureScroll;

public:
    inline GEKeyFrame() : Node(NODE_MASK_EMPTY)
    {
        MESSAGE_CLASS_CREATED(GEKeyFrame);

        m_KeyTime = 0.0f;
        m_KeyTranslationX = m_KeyTranslationY = m_KeyTranslationZ = 0.0f;
        m_f5C = 0.0f;
        m_KeyScaleX = m_KeyScaleY = m_KeyScaleZ = 0.0f;
        m_f6C = 0.0f;
        m_KeyRotationX = m_KeyRotationY = m_KeyRotationZ = 0.0f;
        m_f7C = 0.0f;
        m_KeyOpacity = 1.0f;
        m_KeyTextureScroll = 0.0f;
    }

    const float         GetKeyTime() const; //  @89AD10
    void                SetKeyTime(const float keytime);    //  @8D5040

    const float         GetKeyTranslationX() const; //  @89A880
    void                SetKeyTranslationX(const float translationx);   //  @8A6C90

    const float         GetKeyTranslationY() const; //  @89A890
    void                SetKeyTranslationY(const float translationy);   //  @8A6CA0

    const float         GetKeyTranslationZ() const; //  @89A8A0
    void                SetKeyTranslationZ(const float translationz);   //  @8E3720

    const float         GetKeyScaleX() const;   //  @8A6D40
    void                SetKeyScaleX(const float scalex);   //  @8DDA50

    const float         GetKeyScaleY() const;   //  @905830
    void                SetKeyScaleY(const float scaley);   //  @8A6CD0

    const float         GetKeyScaleZ() const;   //  @905840
    void                SetKeyScaleZ(const float scalez);   //  @8DDA60

    const float         GetKeyRotationX() const;    //  @89AAB0
    void                SetKeyRotationX(const float rotationx); //  @5A1B70

    const float         GetKeyRotationY() const;//  @905860
    void                SetKeyRotationY(const float rotationy); //  @8E37A0

    const float         GetKeyRotationZ() const;    //  @905870
    void                SetKeyRotationZ(const float rotationz); //  @916E20

    const float         GetKeyOpacity() const;  //  @916E30
    void                SetKeyOpacity(const float opacity); //  @8D5060

    const float         GetKeyTextureScroll() const;    //  @8CB4C0
    void                SetKeyTextureScroll(const float scroll);    //  @8D5080

    static void Register(); //  @8DE060
    static GEKeyFrame* Create(AllocatorIndex);  //  @8E26B0
};

extern EntityType* tGEKeyFrame; //  @A3E004

ASSERT_CLASS_SIZE(GEKeyFrame, 140);