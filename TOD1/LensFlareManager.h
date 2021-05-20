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
    CollisionProbe *m_CollisionProbe[4];
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
    LensFlareManager();	//	@8E3960
    virtual ~LensFlareManager();

    virtual void    Destroy() override; //  @8E3CA0
};

ASSERT_CLASS_SIZE(LensFlareManager, 176);