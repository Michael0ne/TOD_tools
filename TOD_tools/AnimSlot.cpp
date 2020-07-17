#include "AnimSlot.h"

AnimSlot::AnimSlot() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(AnimSlot);

	m_Target_1 = nullptr;
	field_54 = m_Flags = 1;
	m_Speed = m_CrossBlendSpeed = 1.0f;
	m_LoopMode_1 = m_AnimFlags = 0;

	m_OverrideAverage = Vector4f();
	m_OverrideEscape = Vector4f();
}