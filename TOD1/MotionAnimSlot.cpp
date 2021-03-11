#include "MotionAnimSlot.h"

MotionAnimSlot::MotionAnimSlot() : AnimSlot()
{
	MESSAGE_CLASS_CREATED(MotionAnimSlot);

	//m_ActionAnimInfoList = List<int>(0x1A300);
	//m_ActionAnimList = List<int>(0x1A300);

	m_CurrAnimListIndex = (char)255;
	m_CurrAnimInfoListIndex = (char)255;
}