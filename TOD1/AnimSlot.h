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

    enum AnimFlags
    {
        FREEZE_ROOT_NODE = 0,
        OVERRIDE_AVERAGE_Z = 1,
        OVERRIDE_ESCAPE_X = 2,
        OVERRIDE_ESCAPE_Y = 3,
        OVERRIDE_ESCAPE_Z = 4,
        OVERRIDE_AVERAGE_X = 5,
        OVERRIDE_ALL = 6
    };

protected:
    AnimationAsset *m_TargetAnimation;
    int             field_54;
    int             m_LoopMode_1;
    float           m_Speed;
    float           m_CrossBlendSpeed;
    Vector4f        m_OverrideAverage;
    Vector4f        m_OverrideEscape;
    AnimFlags       m_AnimFlags;
    union
    {
        struct
        {
            unsigned    StallFirstFrame : 1;
        };
    }               m_Flags;
public:
    AnimSlot(); // @905D90
    virtual ~AnimSlot();
};

extern EntityType* tAnimSlot;

ASSERT_CLASS_SIZE(AnimSlot, 140);