#include "MotionAnimSlot.h"
#include "TruthType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "StringType.h"

EntityType* tMotionAnimSlot;

MotionAnimSlot::~MotionAnimSlot()
{
    MESSAGE_CLASS_DESTROYED(MotionAnimSlot);

    if (m_CurrAnimListIndex >= NULL &&
        m_ActionAnimList[m_CurrAnimListIndex].m_WeaponClass &&
        strcmp(m_ActionAnimList[m_CurrAnimListIndex].m_WeaponClass, nullptr))
        delete m_ActionAnimList[m_CurrAnimListIndex].m_WeaponClass;
}

void MotionAnimSlot::ActionAnimListPushBackElement(int* args)
{
    const ActionAnim temp;
    m_ActionAnimList.push_back(temp);
}

void MotionAnimSlot::ActionAnimInfoListPushBackElement(int* args)
{
    m_ActionAnimInfoList.push_back(ActionAnimInfo());
}

const bool MotionAnimSlot::GetDummy0() const
{
    return false;
}

void MotionAnimSlot::SetDummy0(const bool)
{
}

const ePassiveActionAnimType MotionAnimSlot::GetMeleePassiveAnimType() const
{
    return m_Flags.MeleePassiveActionAnimType;
}

void MotionAnimSlot::SetMeleePassiveAnimType(const ePassiveActionAnimType animtype)
{
    m_Flags.MeleePassiveActionAnimType = animtype;
}

const eAimActionAnimType MotionAnimSlot::GetMeleeAimAnimType() const
{
    return m_Flags.MeleeAimActionAnimType;
}

void MotionAnimSlot::SetMeleeAimAnimType(const eAimActionAnimType animtype)
{
    m_Flags.MeleeAimActionAnimType = animtype;
}

const eShootActionAnimType MotionAnimSlot::GetMeleeShootAnimType() const
{
    return m_Flags.MeleeShootActionAnimType;
}

void MotionAnimSlot::SetMeleeShootAnimType(const eShootActionAnimType animtype)
{
    m_Flags.MeleeShootActionAnimType = animtype;
}

const eDefaultAnimDictType MotionAnimSlot::GetMeleeDefaultAnimDict() const
{
    return m_Flags.MeleeDefaultAnimDict;
}

void MotionAnimSlot::SetMeleeDefaultAnimDict(const eDefaultAnimDictType animtype)
{
    m_Flags.MeleeDefaultAnimDict = animtype;
}

const eDefaultAnimDictType MotionAnimSlot::GetMeleeShootAndAimDict() const
{
    return m_Flags.MeleeShootAndAimDict;
}

void MotionAnimSlot::SetMeleeShootAndAimDict(const eDefaultAnimDictType animtype)
{
    m_Flags.MeleeShootAndAimDict = animtype;
}

const bool MotionAnimSlot::MeleeAimAllowed() const
{
    return m_Flags.MeleeAimAllowed;
}

void MotionAnimSlot::SetMeleeAimAllowed(const bool allowed)
{
    m_Flags.MeleeAimAllowed = allowed;
}

const bool MotionAnimSlot::MeleeAimHighLow() const
{
    return m_Flags.MeleeAimHighLow;
}

void MotionAnimSlot::SetMeleeAimHighLow(const bool allowed)
{
    m_Flags.MeleeAimHighLow = allowed;
}

const bool MotionAnimSlot::GetDummy1() const
{
    return false;
}

void MotionAnimSlot::SetDummy1(const bool)
{
}

const ePassiveActionAnimType MotionAnimSlot::GetGunPassiveAnimType() const
{
    return m_Flags.GunPassiveActionAnimType;
}

void MotionAnimSlot::SetGunPassiveAnimType(const ePassiveActionAnimType animtype)
{
    m_Flags.GunPassiveActionAnimType = animtype;
}

const eAimActionAnimType MotionAnimSlot::GetGunAimAnimType() const
{
    return m_Flags.GunAimActionAnimType;
}

void MotionAnimSlot::SetGunAimAnimType(const eAimActionAnimType animtype)
{
    m_Flags.GunAimActionAnimType = animtype;
}

const eShootActionAnimType MotionAnimSlot::GetGunShootAnimType() const
{
    return m_Flags.GunShootActionAnimType;
}

void MotionAnimSlot::SetGunShootAnimType(const eShootActionAnimType animtype)
{
    m_Flags.GunShootActionAnimType = animtype;
}

const eDefaultAnimDictType MotionAnimSlot::GetGunDefaultAnimDict() const
{
    return m_Flags.GunDefaultAnimDict;
}

void MotionAnimSlot::SetGunDefaultAnimDict(const eDefaultAnimDictType animtype)
{
    m_Flags.GunDefaultAnimDict = animtype;
}

const eDefaultAnimDictType MotionAnimSlot::GetGunShootAndAimDict() const
{
    return m_Flags.GunShootAndAimDict;
}

void MotionAnimSlot::SetGunShootAndAimDict(const eDefaultAnimDictType animtype)
{
    m_Flags.GunShootAndAimDict = animtype;
}

const bool MotionAnimSlot::GunAimAllowed() const
{
    return m_Flags.GunAimAllowed;
}

void MotionAnimSlot::SetGunAimAllowed(const bool allowed)
{
    m_Flags.GunAimAllowed = allowed;
}

const bool MotionAnimSlot::GunAimHighLow() const
{
    return m_Flags.GunAimHighLow;
}

void MotionAnimSlot::SetGunAimHighLow(const bool allowed)
{
    m_Flags.GunAimHighLow = allowed;
}

const int MotionAnimSlot::GetCurrAnimInfoListIndex() const
{
    return m_CurrAnimInfoListIndex;
}

void MotionAnimSlot::SetCurrAnimInfoListIndex(const char index)
{
    m_CurrAnimInfoListIndex = index;
}

const int MotionAnimSlot::GetCurrAnimListIndex() const
{
    return m_CurrAnimListIndex;
}

void MotionAnimSlot::SetCurrAnimListIndex(const char index)
{
    m_CurrAnimListIndex = index;
}

const ePassiveActionAnimType MotionAnimSlot::GetActionAnimInfoListPassiveAnimType() const
{
    return m_CurrAnimInfoListIndex >= 0 ? m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.PassiveActionAnimType : (ePassiveActionAnimType)-1;
}

void MotionAnimSlot::SetActionAnimInfoListPassiveAnimType(const ePassiveActionAnimType animtype)
{
    if (m_CurrAnimInfoListIndex >= 0)
        m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.PassiveActionAnimType = animtype;
}

const eAimActionAnimType MotionAnimSlot::GetActionAnimInfoListAimAnimType() const
{
    return m_CurrAnimInfoListIndex >= 0 ? m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.AimActionAnimType : (eAimActionAnimType)-1;
}

void MotionAnimSlot::SetActionAnimInfoListAimAnimType(const eAimActionAnimType animtype)
{
    if (m_CurrAnimInfoListIndex >= 0)
        m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.AimActionAnimType = animtype;
}

const eShootActionAnimType MotionAnimSlot::GetActionAnimInfoListShootAnimType() const
{
    return m_CurrAnimInfoListIndex >= 0 ? m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.ShootActionAnimType : (eShootActionAnimType)-1;
}

void MotionAnimSlot::SetActionAnimInfoListShootAnimType(const eShootActionAnimType animtype)
{
    if (m_CurrAnimInfoListIndex >= 0)
        m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.ShootActionAnimType = animtype;
}

const eDefaultAnimDictType MotionAnimSlot::GetActionAnimInfoListDefaultAnimDict() const
{
    return m_CurrAnimInfoListIndex >= 0 ? m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.DefaultAnimDict : (eDefaultAnimDictType)-1;
}

void MotionAnimSlot::SetActionAnimInfoListDefaultAnimDict(const eDefaultAnimDictType animtype)
{
    if (m_CurrAnimInfoListIndex >= 0)
        m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.DefaultAnimDict = animtype;
}

const eShootActionAnimType MotionAnimSlot::GetActionAnimInfoListShootAndAimDict() const
{
    return m_CurrAnimInfoListIndex >= 0 ? m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.ShootActionAnimType : (eShootActionAnimType)-1;
}

void MotionAnimSlot::SetActionAnimInfoListShootAndAimDict(const eShootActionAnimType animtype)
{
    if (m_CurrAnimInfoListIndex)
        m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.ShootActionAnimType = animtype;
}

const bool MotionAnimSlot::ActionAnimInfoListAimAllowed() const
{
    return m_CurrAnimInfoListIndex >= 0 ? m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.AimAllowed : false;
}

void MotionAnimSlot::SetActionAnimInfoListAimAllowed(const bool allowed)
{
    if (m_CurrAnimInfoListIndex)
        m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.AimAllowed = allowed;
}

const bool MotionAnimSlot::ActionAnimInfoListAimHighLow() const
{
    return m_CurrAnimInfoListIndex >= 0 ? m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.AimHighLow : false;
}

void MotionAnimSlot::SetActionAnimInfoListAimHighLow(const bool allowed)
{
    if (m_CurrAnimInfoListIndex)
        m_ActionAnimInfoList[m_CurrAnimInfoListIndex].m_Flags.AimHighLow = allowed;
}

void MotionAnimSlot::Register()
{
    tMotionAnimSlot = new EntityType("MotionAnimSlot");
    tMotionAnimSlot->InheritFrom(tAnimSlot);
    tMotionAnimSlot->SetCreator((CREATOR)Create);

    tMotionAnimSlot->RegisterScript("ActionAnimList_PushBackElement", (EntityFunctionMember)&ActionAnimListPushBackElement);
    tMotionAnimSlot->RegisterScript("ActionAnimInfoList_PushBackElement", (EntityFunctionMember)&ActionAnimInfoListPushBackElement);

    tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy0", (EntityGetterFunction)&GetDummy0, (EntitySetterFunction)&SetDummy0, "control=drawline|name=Melee Action Anim Info");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "M_PassiveAnimType", (EntityGetterFunction)&GetMeleePassiveAnimType, (EntitySetterFunction)&SetMeleePassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "M_AimAnimType", (EntityGetterFunction)&GetMeleeAimAnimType, (EntitySetterFunction)&SetMeleeAimAnimType, "control=autodropdown|AIM_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "M_ShootAnimType", (EntityGetterFunction)&GetMeleeShootAnimType, (EntitySetterFunction)&SetMeleeShootAnimType, "control=autodropdown|SHOOT_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "M_DefaultAnimDict", (EntityGetterFunction)&GetMeleeDefaultAnimDict, (EntitySetterFunction)&SetMeleeDefaultAnimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "M_ShootAndAimDict", (EntityGetterFunction)&GetMeleeShootAndAimDict, (EntitySetterFunction)&SetMeleeShootAndAimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "M_AimAllowed", (EntityGetterFunction)&MeleeAimAllowed, (EntitySetterFunction)&SetMeleeAimAllowed, "control=truth");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "M_AimHighLow", (EntityGetterFunction)&MeleeAimHighLow, (EntitySetterFunction)&SetMeleeAimHighLow, "control=truth");

    tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy1", (EntityGetterFunction)&GetDummy1, (EntitySetterFunction)&SetDummy1, "control=drawline|name=Gun Action Anim Info");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "G_PassiveAnimType", (EntityGetterFunction)&GetGunPassiveAnimType, (EntitySetterFunction)&SetGunPassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "G_AimAnimType", (EntityGetterFunction)&GetGunAimAnimType, (EntitySetterFunction)&SetGunAimAnimType, "control=autodropdown|AIM_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "G_ShootAnimType", (EntityGetterFunction)&GetGunShootAnimType, (EntitySetterFunction)&SetGunShootAnimType, "control=autodropdown|SHOOT_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "G_DefaultAnimDict", (EntityGetterFunction)&GetGunDefaultAnimDict, (EntitySetterFunction)&SetGunDefaultAnimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "G_ShootAndAimDict", (EntityGetterFunction)&GetGunShootAndAimDict, (EntitySetterFunction)&SetGunShootAndAimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "G_AimAllowed", (EntityGetterFunction)&GunAimAllowed, (EntitySetterFunction)&SetGunAimAllowed, "control=truth");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "G_AimHighLow", (EntityGetterFunction)&GunAimHighLow, (EntitySetterFunction)&SetGunAimHighLow, "control=truth");

    /*tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy2", (EntityGetterFunction)&GetDummy2, (EntitySetterFunction)&SetDummy2, "control=drawline|name=AutoLong Action Anim Info");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "AL_PassiveAnimType", (EntityGetterFunction)&GetAutoLongPassiveAnimType, (EntitySetterFunction)&SetAutoLongPassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "AL_AimAnimType", (EntityGetterFunction)&GetAutoLongAimAnimType, (EntitySetterFunction)&SetAutoLongAimAnimType, "control=autodropdown|AIM_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "AL_ShootAnimType", (EntityGetterFunction)&GetAutoLongShootAnimType, (EntitySetterFunction)&SetAutoLongShootAnimType, "control=autodropdown|SHOOT_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "AL_DefaultAnimDict", (EntityGetterFunction)&GetAutoLongDefaultAnimDict, (EntitySetterFunction)&SetAutoLongDefaultAnimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "AL_ShootAndAimDict", (EntityGetterFunction)&GetAutoLongShootAndAimDict, (EntitySetterFunction)&SetAutoLongShootAndAimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "AL_AimAllowed", (EntityGetterFunction)&AutoLongAimAllowed, (EntitySetterFunction)&SetAutoLongAimAllowed, "control=truth");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "AL_AimHighLow", (EntityGetterFunction)&AutoLongAimHighLow, (EntitySetterFunction)&SetAutoLongAimHighLow, "control=truth");

    tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy3", (EntityGetterFunction)&GetDummy3, (EntitySetterFunction)&SetDummy3, "control=drawline|name=Dual Action Anim Info");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "D_PassiveAnimType", (EntityGetterFunction)&GetDualPassiveAnimType, (EntitySetterFunction)&SetDualPassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "D_AimAnimType", (EntityGetterFunction)&GetDualAimAnimType, (EntitySetterFunction)&SetDualAimAnimType, "control=autodropdown|AIM_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "D_ShootAnimType", (EntityGetterFunction)&GetDualShootAnimType, (EntitySetterFunction)&SetDualShootAnimType, "control=autodropdown|SHOOT_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "D_DefaultAnimDict", (EntityGetterFunction)&GetDualDefaultAnimDict, (EntitySetterFunction)&SetDualDefaultAnimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "D_ShootAndAimDict", (EntityGetterFunction)&GetDualShootAndAimDict, (EntitySetterFunction)&SetDualShootAndAimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "D_AimAllowed", (EntityGetterFunction)&DualAimAllowed, (EntitySetterFunction)&SetDualAimAllowed, "control=truth");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "D_AimHighLow", (EntityGetterFunction)&DualAimHighLow, (EntitySetterFunction)&SetDualAimHighLow, "control=truth");

    tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy4", (EntityGetterFunction)&GetDummy4, (EntitySetterFunction)&SetDummy4, "control=drawline|name=Thrown weapons Action Anim Info");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "HG_PassiveAnimType", (EntityGetterFunction)&GetThrownPassiveAnimType, (EntitySetterFunction)&SetThrownPassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "HG_AimAnimType", (EntityGetterFunction)&GetThrownAimAnimType, (EntitySetterFunction)&SetThrownAimAnimType, "control=autodropdown|AIM_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "HG_ShootAnimType", (EntityGetterFunction)&GetThrownShootAnimType, (EntitySetterFunction)&SetThrownShootAnimType, "control=autodropdown|SHOOT_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "HG_DefaultAnimDict", (EntityGetterFunction)&GetThrownDefaultAnimDict, (EntitySetterFunction)&SetThrownDefaultAnimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "HG_ShootAndAimDict", (EntityGetterFunction)&GetThrownShootAndAimDict, (EntitySetterFunction)&SetThrownShootAndAimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "HG_AimAllowed", (EntityGetterFunction)&ThrownAimAllowed, (EntitySetterFunction)&SetThrownAimAllowed, "control=truth");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "HG_AimHighLow", (EntityGetterFunction)&ThrownAimHighLow, (EntitySetterFunction)&SetThrownAimHighLow, "control=truth");

    tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy5", (EntityGetterFunction)&GetDummy5, (EntitySetterFunction)&SetDummy5, "control=drawline|name=Shield Action Anim Info");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "SH_PassiveAnimType", (EntityGetterFunction)&GetShieldPassiveAnimType, (EntitySetterFunction)&SetShieldPassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "SH_AimAnimType", (EntityGetterFunction)&GetShieldAimAnimType, (EntitySetterFunction)&SetShieldAimAnimType, "control=autodropdown|AIM_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "SH_ShootAnimType", (EntityGetterFunction)&GetShieldShootAnimType, (EntitySetterFunction)&SetShieldShootAnimType, "control=autodropdown|SHOOT_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "SH_DefaultAnimDict", (EntityGetterFunction)&GetShieldDefaultAnimDict, (EntitySetterFunction)&SetShieldDefaultAnimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "SH_ShootAndAimDict", (EntityGetterFunction)&GetShieldShootAndAimDict, (EntitySetterFunction)&SetShieldShootAndAimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "SH_AimAllowed", (EntityGetterFunction)&ShieldAimAllowed, (EntitySetterFunction)&SetShieldAimAllowed, "control=truth");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "SH_AimHighLow", (EntityGetterFunction)&ShieldAimHighLow, (EntitySetterFunction)&SetShieldAimHighLow, "control=truth");

    tMotionAnimSlot->RegisterProperty(tTRUTH, "dummy6", (EntityGetterFunction)&GetDummy6, (EntitySetterFunction)&SetDummy6, "control=drawline|name=Guitar Action Anim Info");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "MG_PassiveAnimType", (EntityGetterFunction)&GetGuitarPassiveAnimType, (EntitySetterFunction)&SetGuitarPassiveAnimType, "control=autodropdown|PASSIVE_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "MG_AimAnimType", (EntityGetterFunction)&GetGuitarAimAnimType, (EntitySetterFunction)&SetGuitarAimAnimType, "control=autodropdown|AIM_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "MG_ShootAnimType", (EntityGetterFunction)&GetGuitarShootAnimType, (EntitySetterFunction)&SetGuitarShootAnimType, "control=autodropdown|SHOOT_ACTION_ANIM_TYPES");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "MG_DefaultAnimDict", (EntityGetterFunction)&GetGuitarDefaultAnimDict, (EntitySetterFunction)&SetGuitarDefaultAnimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tINTEGER, "MG_ShootAndAimDict", (EntityGetterFunction)&GetGuitarShootAndAimDict, (EntitySetterFunction)&SetGuitarShootAndAimDict, "control=autodropdown|DEFAULT_ANIM_DICTS");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "MG_AimAllowed", (EntityGetterFunction)&GuitarAimAllowed, (EntitySetterFunction)&SetGuitarAimAllowed, "control=truth");
    tMotionAnimSlot->RegisterProperty(tTRUTH, "MG_AimHighLow", (EntityGetterFunction)&GuitarAimHighLow, (EntitySetterFunction)&SetGuitarAimHighLow, "control=truth");*/

    tMotionAnimSlot->RegisterProperty(tINTEGER, "CurrAnimInfoListIndex", (EntityGetterFunction)&GetCurrAnimInfoListIndex, (EntitySetterFunction)&SetCurrAnimInfoListIndex, nullptr);
    tMotionAnimSlot->RegisterProperty(tINTEGER, "CurrAnimListIndex", (EntityGetterFunction)&GetCurrAnimListIndex, (EntitySetterFunction)&SetCurrAnimListIndex, nullptr);
    tMotionAnimSlot->RegisterProperty(tINTEGER, "ActionAnimInfoList_PassiveAnimType", (EntityGetterFunction)&GetActionAnimInfoListPassiveAnimType, (EntitySetterFunction)&SetActionAnimInfoListPassiveAnimType, nullptr);
    tMotionAnimSlot->RegisterProperty(tINTEGER, "ActionAnimInfoList_AimAnimType", (EntityGetterFunction)&GetActionAnimInfoListAimAnimType, (EntitySetterFunction)&SetActionAnimInfoListAimAnimType, nullptr);
    tMotionAnimSlot->RegisterProperty(tINTEGER, "ActionAnimInfoList_ShootAnimType", (EntityGetterFunction)&GetActionAnimInfoListShootAnimType, (EntitySetterFunction)&SetActionAnimInfoListShootAnimType, nullptr);
    tMotionAnimSlot->RegisterProperty(tINTEGER, "ActionAnimInfoList_DefaultAnimDict", (EntityGetterFunction)&GetActionAnimInfoListDefaultAnimDict, (EntitySetterFunction)&SetActionAnimInfoListDefaultAnimDict, nullptr);
    tMotionAnimSlot->RegisterProperty(tINTEGER, "ActionAnimInfoList_ShootAndAimDict", (EntityGetterFunction)&GetActionAnimInfoListShootAndAimDict, (EntitySetterFunction)&SetActionAnimInfoListShootAndAimDict, nullptr);
    tMotionAnimSlot->RegisterProperty(tTRUTH, "ActionAnimInfoList_AimAllowed", (EntityGetterFunction)&ActionAnimInfoListAimAllowed, (EntitySetterFunction)&SetActionAnimInfoListAimAllowed, nullptr);
    tMotionAnimSlot->RegisterProperty(tTRUTH, "ActionAnimInfoList_AimHighLow", (EntityGetterFunction)&ActionAnimInfoListAimHighLow, (EntitySetterFunction)&SetActionAnimInfoListAimHighLow, nullptr);

    /*tMotionAnimSlot->RegisterProperty(tSTRING, "ActionAnimList_WeaponClass", (EntityGetterFunction)&GetActionAnimListWeaponClass, (EntitySetterFunction)&SetActionAnimListWeaponClass, nullptr);
    tMotionAnimSlot->RegisterProperty(tEntity, "ActionAnimList_Passive", (EntityGetterFunction)&GetActionAnimListPassive, (EntitySetterFunction)&SetActionAnimListPassive, nullptr);
    tMotionAnimSlot->RegisterProperty(tEntity, "ActionAnimList_AimLow", (EntityGetterFunction)&GetActionAnimListAimLow, (EntitySetterFunction)&SetActionAnimListAimLow, nullptr);
    tMotionAnimSlot->RegisterProperty(tEntity, "ActionAnimList_AimHigh", (EntityGetterFunction)&GetActionAnimListAimHigh, (EntitySetterFunction)&SetActionAnimListAimHigh, nullptr);
    tMotionAnimSlot->RegisterProperty(tEntity, "ActionAnimList_ShootLow", (EntityGetterFunction)&GetActionAnimListShootLow, (EntitySetterFunction)&SetActionAnimListShootLow, nullptr);
    tMotionAnimSlot->RegisterProperty(tEntity, "ActionAnimList_ShootHigh", (EntityGetterFunction)&GetActionAnimListShootHigh, (EntitySetterFunction)&SetActionAnimListShootHigh, nullptr);*/

    tMotionAnimSlot->PropagateProperties();
}

MotionAnimSlot* MotionAnimSlot::Create(AllocatorIndex)
{
    return new MotionAnimSlot;
}