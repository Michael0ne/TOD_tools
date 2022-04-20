#pragma once
#include "Node.h"
#include "FrameBuffer.h"

class CollisionProbe;

class LensFlareManager : public Node
{
protected:
    int             field_50;
    int             field_54;
    int             field_58;
    int             field_5C;
    int             field_60;
    int             field_64;
    Vector4f        field_68;
    FrameBuffer*    m_FrameBuffer;
    CollisionProbe *m_CollisionProbes[4];
    int             field_8C;
    int             field_90;
    int             field_94;
    float           m_SizeReduction;
    float           m_FadeDistance;
    float           m_OcclusionFactor;
    int             field_A4;
    float           m_EmitterSize;
    float           m_LineCheckLength;

public:
    LensFlareManager(); // @8E3960
    virtual ~LensFlareManager();

    virtual void    Destroy() override; //  @8E3CA0

    const float     GetSizeReduction() const;   //  @8D5110
    void            SetSizeReduction(const float sizereduction);    //  @88AAC0
    const float     GetFadeDistance() const;    //  @916DF0
    void            SetFadeDistance(const float fadedistance);  //  @88AAE0
    const float     GetEmitterSize() const; //  @8D5150
    void            SetEmitterSize(const float emittersize);   //  @88AB60
    const float     GetOcclusionFactor() const; //  @4B29A0
    const float     GetLineCheckLength() const; //  @8E38A0
    void            SetLineCheckLength(const float length); //  @8E3880

    static void     Register(); //  @8E3AB0
    static LensFlareManager*    Create(AllocatorIndex); //  @8E5620
};

extern EntityType* tLensFlareManager;   //  @A3E060

ASSERT_CLASS_SIZE(LensFlareManager, 176);