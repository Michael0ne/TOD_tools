#pragma once
#include "Node.h"
#include "AnimationAsset.h"

class AnimSlot : public Node
{
    friend class AnimLayer;

    enum Loopmode
    {
        LOOP = 0,
        ONESHOT,
        RANDOM
    };

protected:
    AnimationAsset *m_TargetAnimation;
    int             field_54;
    int             m_LoopMode_1;
    float           m_Speed;
    float           m_CrossBlendSpeed;
    Vector4f        m_OverrideAverage;
    Vector4f        m_OverrideEscape;
    int             m_AnimFlags;
    unsigned int    m_Flags;
public:
    AnimSlot(); // @905D90
    virtual ~AnimSlot();
};

ASSERT_CLASS_SIZE(AnimSlot, 140);