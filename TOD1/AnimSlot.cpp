#include "AnimSlot.h"

EntityType* tAnimSlot;

AnimSlot::AnimSlot() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(AnimSlot);

    m_TargetAnimation = nullptr;
    field_54 = 1;
    m_Flags.StallFirstFrame = true;
    m_Speed = m_CrossBlendSpeed = 1.0f;
    m_LoopMode_1 = 0;
    m_AnimFlags = FREEZE_ROOT_NODE;

    m_OverrideAverage = {};
    m_OverrideEscape = {};
}

AnimSlot::~AnimSlot()
{
    MESSAGE_CLASS_DESTROYED(AnimSlot);
}