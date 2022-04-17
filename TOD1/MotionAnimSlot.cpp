#include "MotionAnimSlot.h"

EntityType* tMotionAnimSlot;

MotionAnimSlot::~MotionAnimSlot()
{
    MESSAGE_CLASS_DESTROYED(MotionAnimSlot);

    if (m_CurrAnimListIndex >= NULL &&
        m_ActionAnimList[m_CurrAnimListIndex].m_WeaponClass &&
        strcmp(m_ActionAnimList[m_CurrAnimListIndex].m_WeaponClass, nullptr))
        delete m_ActionAnimList[m_CurrAnimListIndex].m_WeaponClass;
}

void MotionAnimSlot::Register()
{
    tMotionAnimSlot = new EntityType("MotionAnimSlot");
    tMotionAnimSlot->InheritFrom(tAnimSlot);
    tMotionAnimSlot->SetCreator((CREATOR)Create);

    //  
}

MotionAnimSlot* MotionAnimSlot::Create(AllocatorIndex)
{
    return new MotionAnimSlot;
}