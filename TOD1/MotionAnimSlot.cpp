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

#pragma message(TODO_IMPLEMENTATION)
void MotionAnimSlot::Register()
{
    tMotionAnimSlot = new EntityType("MotionAnimSlot");
    tMotionAnimSlot->InheritFrom(tAnimSlot);
    tMotionAnimSlot->SetCreator((CREATOR)Create);

    //tMotionAnimSlot->RegisterScript("ActionAnimList_PushBackElement", (EntityFunctionMember)&ActionAnimListPushBackElement, NULL, NULL, NULL, nullptr, nullptr);
    //tMotionAnimSlot->RegisterScript("ActionAnimInfoList_PushBackElement", (EntityFunctionMember)&ActionAnimInfoListPushBackElement, NULL, NULL, NULL, nullptr, nullptr);
    //tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy0", (EntityGetterFunction)&GetDummy0, (EntitySetterFunction)&SetDummy0, "control=drawline|name=Melee Action Anim Info");
    //tMotionAnimSlot->RegisterProperty(tINTEGER, "M_PassiveAnimType", (EntityGetterFunction)&GetMPassiveAnimType, (EntitySetterFunction)&SetMPassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
}

MotionAnimSlot* MotionAnimSlot::Create(AllocatorIndex)
{
    return new MotionAnimSlot;
}