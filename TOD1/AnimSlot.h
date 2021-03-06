#pragma once

#include "Node.h"

#define ANIMSLOT_CLASS_SIZE 140

enum E_ANIMSLOT_LOOPMODE
{
	MODE_LOOP = 0,
	MODE_ONESHOT = 1,
	MODE_RANDOM = 2
};

class AnimSlot : public Node
{
protected:
	int*		m_Target_1;
	int			field_54;
	int			m_LoopMode_1;
	float		m_Speed;
	float		m_CrossBlendSpeed;
	Vector4f	m_OverrideAverage;
	Vector4f	m_OverrideEscape;
	int			m_AnimFlags;
	unsigned int	m_Flags;
public:
	AnimSlot();	//	@905D90
};

static_assert(sizeof(AnimSlot) == ANIMSLOT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(AnimSlot));