#include "WeaponPlaceHolder.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "NumberType.h"

EntityType* tWeaponPlaceholder;

const int WeaponPlaceHolder::GetResourceType() const
{
    return m_Flags_15.ResourceType;
}

void WeaponPlaceHolder::SetResourceType(const int restype)
{
    m_Flags_15.ResourceType = restype;
}

const int WeaponPlaceHolder::GetStatus() const
{
    return m_Flags_15.Status;
}

void WeaponPlaceHolder::SetStatus(const int status)
{
    const int currentStatus = m_Flags_15.Status;
    StoreProperty(16, &currentStatus, tINTEGER);

    m_Flags_15.Status = status;
}

const bool WeaponPlaceHolder::IsLocked() const
{
    return m_Flags_15.Locked;
}

void WeaponPlaceHolder::SetIsLocked(const bool locked)
{
    const bool currentLockedStatus = m_Flags_15.Locked;
    StoreProperty(17, &currentLockedStatus, tTRUTH);

    m_Flags_15.Locked = currentLockedStatus;
}

const int WeaponPlaceHolder::GetPopSensitiveType() const
{
    return m_Flags_15.PopSensitiveType;
}

void WeaponPlaceHolder::SetPopSensitiveType(const int ptype)
{
    m_Flags_15.PopSensitiveType = ptype;
}

const bool WeaponPlaceHolder::IsVIP() const
{
    return m_Flags_15.IsVIP;
}

void WeaponPlaceHolder::SetIsVIP(const bool vip)
{
    m_Flags_15.IsVIP = vip;
}

const float WeaponPlaceHolder::GetVIPTimer() const
{
    return m_VIPTimer;
}

void WeaponPlaceHolder::SetVIPTimer(const float viptimer)
{
    StoreProperty(18, &m_VIPTimer, tNUMBER);
    m_VIPTimer = viptimer;
}

const int WeaponPlaceHolder::GetPriority() const
{
    return m_Flags_16.Priority;
}

void WeaponPlaceHolder::SetPriority(const int priority)
{
    const int currentPriority = m_Flags_16.Priority;
    StoreProperty(19, &currentPriority, tINTEGER);

    m_Flags_16.Priority = priority;
}

const int WeaponPlaceHolder::GetWeaponSubType() const
{
    return m_Flags_16.WeaponSubType;
}

void WeaponPlaceHolder::SetWeaponSubType(const int subtype)
{
    m_Flags_16.WeaponSubType = subtype;
}

const int WeaponPlaceHolder::GetMeleeWeaponSubType() const
{
    return m_Flags_16.MeleeWeaponSubType;
}

void WeaponPlaceHolder::SetMeleeWeaponSubType(const int msubtype)
{
    m_Flags_16.MeleeWeaponSubType = msubtype;
}

const int WeaponPlaceHolder::GetBulletDrop() const
{
    return m_Flags_16.BulletDropType;
}

void WeaponPlaceHolder::SetBulletDrop(const int bdrop)
{
    m_Flags_16.BulletDropType = bdrop;
}

const bool WeaponPlaceHolder::IsRespawnable() const
{
    return m_Flags_16.Respawnable;
}

void WeaponPlaceHolder::SetIsRespawnable(const bool respawnable)
{
    m_Flags_16.Respawnable = respawnable;
}

const float WeaponPlaceHolder::GetRespawnTime() const
{
    return m_RespawnTime;
}

void WeaponPlaceHolder::SetRespawnTime(const float resptime)
{
    m_RespawnTime = resptime;
}

void WeaponPlaceHolder::Register()
{
    tWeaponPlaceholder = new EntityType("WeaponPlaceHolder");
    tWeaponPlaceholder->InheritFrom(tPlaceholder);
    tWeaponPlaceholder->SetCreator((CREATOR)Create);

    tWeaponPlaceholder->RegisterProperty(tINTEGER, "ResourceType", (EntityGetterFunction)&GetResourceType, (EntitySetterFunction)&SetResourceType, nullptr);
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "Status", (EntityGetterFunction)&GetStatus, (EntitySetterFunction)&SetStatus, nullptr, 16);
    tWeaponPlaceholder->RegisterProperty(tTRUTH, "Locked", (EntityGetterFunction)&IsLocked, (EntitySetterFunction)&SetIsLocked, nullptr, 17);
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "PopSensitiveType", (EntityGetterFunction)&GetPopSensitiveType, (EntitySetterFunction)&SetPopSensitiveType, "control=autodropdown|POPSENSITIVE_TYPES");
    tWeaponPlaceholder->RegisterProperty(tTRUTH, "IsVIP", (EntityGetterFunction)&IsVIP, (EntitySetterFunction)&SetIsVIP, "control=truth");
    tWeaponPlaceholder->RegisterProperty(tNUMBER, "VIPTimer", (EntityGetterFunction)&GetVIPTimer, (EntitySetterFunction)&SetVIPTimer, "control=slider|min=0.0|max=100.0", 18);
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "Priority", (EntityGetterFunction)&GetPriority, (EntitySetterFunction)&SetPriority, nullptr, 19);
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "WeaponSubType", (EntityGetterFunction)&GetWeaponSubType, (EntitySetterFunction)&SetWeaponSubType, "control=autodropdown|SUB_WEAPON_TYPES");
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "MeleeWeaponSubType", (EntityGetterFunction)&GetMeleeWeaponSubType, (EntitySetterFunction)&SetMeleeWeaponSubType, "control=autodropdown|MELEE_WEAPON_TYPES");
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "BulletDrop", (EntityGetterFunction)&GetBulletDrop, (EntitySetterFunction)&SetBulletDrop, "control=autodropdown|BULLET_DROP_TYPES");
    tWeaponPlaceholder->RegisterProperty(tTRUTH, "Respawnable", (EntityGetterFunction)&IsRespawnable, (EntitySetterFunction)&SetIsRespawnable, "control=truth");
    tWeaponPlaceholder->RegisterProperty(tNUMBER, "RespawnTime", (EntityGetterFunction)&GetRespawnTime, (EntitySetterFunction)&SetRespawnTime, "control=string");

    tWeaponPlaceholder->PropagateProperties();
}

WeaponPlaceHolder* WeaponPlaceHolder::Create(AllocatorIndex)
{
    return new WeaponPlaceHolder;
}