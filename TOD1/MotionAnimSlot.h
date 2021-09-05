#pragma once
#include "AnimSlot.h"

class MotionAnimSlot : public AnimSlot
{
protected:
 char m_CurrAnimInfoListIndex;
 char m_CurrAnimListIndex;
 int m_ActionAnimInfoList[4];
 int m_ActionAnimList[4];
 unsigned int m_Flags;
 int m_Flags_1;
 unsigned int m_Flags_2;

public:
 MotionAnimSlot();
};

ASSERT_CLASS_SIZE(MotionAnimSlot, 188);