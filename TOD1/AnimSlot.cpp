#include "AnimSlot.h"

AnimSlot::AnimSlot() : Node(NODE_MASK_EMPTY)
{
 MESSAGE_CLASS_CREATED(AnimSlot);

 m_TargetAnimation = nullptr;
 field_54 = m_Flags = 1;
 m_Speed = m_CrossBlendSpeed = 1.0f;
 m_LoopMode_1 = m_AnimFlags = 0;

 m_OverrideAverage = {};
 m_OverrideEscape = {};
}

AnimSlot::~AnimSlot()
{
 MESSAGE_CLASS_DESTROYED(AnimSlot);
}