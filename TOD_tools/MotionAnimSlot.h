#pragma once

#include "AnimSlot.h"

#define MOTION_ANIM_SLOT_CLASS_SIZE 188

class MotionAnimSlot : public AnimSlot
{
protected:
	char m_CurrAnimInfoListIndex;
	char m_CurrAnimListIndex;
	List<int> m_ActionAnimInfoList;
	List<int> m_ActionAnimList;
	unsigned int m_Flags;
	int m_Flags_1;
	unsigned int m_Flags_2;

public:
	MotionAnimSlot() : AnimSlot()	//	NOTE: no constructor present
	{
		MESSAGE_CLASS_CREATED(MotionAnimSlot);

		m_ActionAnimInfoList = List<int>(0x1A300);
		m_ActionAnimList = List<int>(0x1A300);

		m_CurrAnimListIndex = (char)255;
		m_CurrAnimInfoListIndex = (char)255;
	}
};

static_assert(sizeof(MotionAnimSlot) == MOTION_ANIM_SLOT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(MotionAnimSlot));