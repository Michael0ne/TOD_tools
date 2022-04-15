#include "HavocPlaceHolder.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "NumberType.h"

EntityType* tHavocPlaceHolder;

const int HavocPlaceHolder::GetResourceType() const
{
    return m_HavocFlags_1.ResourceType;
}

void HavocPlaceHolder::SetResourceType(const int restype)
{
    m_HavocFlags_1.ResourceType = restype;
}

const int HavocPlaceHolder::GetStatus() const
{
    return m_HavocFlags_1.Status;
}

void HavocPlaceHolder::SetStatus(const int status)
{
    const int currentStatus = m_HavocFlags_1.Status;
    StoreProperty(16, &currentStatus, tINTEGER);

    m_HavocFlags_1.Status = status;
}

const bool HavocPlaceHolder::IsLocked() const
{
    return m_HavocFlags_1.Locked;
}

void HavocPlaceHolder::SetIsLocked(const bool locked)
{
    const int currentLockedStatus = m_HavocFlags_1.Locked;
    StoreProperty(17, &currentLockedStatus, tTRUTH);

    m_HavocFlags_1.Locked = locked;
}

const int HavocPlaceHolder::GetPopSensitiveType() const
{
    return m_HavocFlags_1.PopSensitiveType;
}

void HavocPlaceHolder::SetPopSensitiveType(const int popsenstype)
{
    m_HavocFlags_1.PopSensitiveType = popsenstype;
}

const bool HavocPlaceHolder::IsVIP() const
{
    return m_HavocFlags_1.IsVIP;
}

void HavocPlaceHolder::SetIsVIP(const bool vip)
{
    m_HavocFlags_1.IsVIP = vip;
}

const float HavocPlaceHolder::GetVIPTimer() const
{
    return m_VIPTimer;
}

void HavocPlaceHolder::SetVIPTimer(const float timer)
{
    StoreProperty(18, &m_VIPTimer, tNUMBER);
    m_VIPTimer = timer;
}

const int HavocPlaceHolder::GetPriority() const
{
    return m_HavocFlags_2.Priority;
}

void HavocPlaceHolder::SetPriority(const int priority)
{
    const int currentPriority = m_HavocFlags_2.Priority;
    StoreProperty(19, &currentPriority, tINTEGER);

    m_HavocFlags_2.Priority = priority;
}

const eObjectSubType HavocPlaceHolder::GetObjectSubType() const
{
    return (eObjectSubType)m_HavocFlags_2.ObjectSubType;
}

void HavocPlaceHolder::SetObjectSubType(const eObjectSubType subtype)
{
    m_HavocFlags_2.ObjectSubType = subtype;
}

const int HavocPlaceHolder::GetRealActiveTextureSet() const
{
    return m_HavocFlags_2.RealActiveTextureSet;
}

void HavocPlaceHolder::SetRealActiveTextureSet(const int texset)
{
    m_HavocFlags_2.RealActiveTextureSet = texset;
}

const eParticleEffect HavocPlaceHolder::GetDefaultParticleEffect() const
{
    return (eParticleEffect)m_HavocFlags_2.DefaultParticleEffect;
}

void HavocPlaceHolder::SetDefaultParticleEffect(const eParticleEffect effect)
{
    m_HavocFlags_2.DefaultParticleEffect = effect;
}

const int HavocPlaceHolder::GetReplacePivotID() const
{
    return m_HavocFlags_3.ReplacePivotID;
}

void HavocPlaceHolder::SetReplacePivotID(const int pivotid)
{
    const int currentReplacePivotId = m_HavocFlags_3.ReplacePivotID;
    StoreProperty(20, &currentReplacePivotId, tINTEGER);

    m_HavocFlags_3.ReplacePivotID = pivotid;
}

const int HavocPlaceHolder::GetComID() const
{
    return m_HavocFlags_4.ComID;
}

void HavocPlaceHolder::SetComID(const int comid)
{
    const int currentComId = m_HavocFlags_4.ComID;
    StoreProperty(21, &currentComId, tINTEGER);

    m_HavocFlags_4.ComID = comid;
}

const bool HavocPlaceHolder::IsBroken() const
{
    return m_HavocFlags_4.Broken;
}

void HavocPlaceHolder::SetIsBroken(const bool broken)
{
    const int currentBrokenStatus = m_HavocFlags_4.Broken;
    StoreProperty(22, &currentBrokenStatus, tTRUTH);

    m_HavocFlags_4.Broken = broken;
}

void HavocPlaceHolder::Register()
{
    tHavocPlaceHolder = new EntityType("HavocPlaceHolder");
    tHavocPlaceHolder->InheritFrom(tPlaceholder);
    tHavocPlaceHolder->SetCreator((CREATOR)Create);

    tHavocPlaceHolder->RegisterProperty(tINTEGER, "ResourceType", (EntityGetterFunction)&GetResourceType, (EntitySetterFunction)&SetResourceType, nullptr);
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "Status", (EntityGetterFunction)&GetStatus, (EntitySetterFunction)&SetStatus, nullptr, 16);
    tHavocPlaceHolder->RegisterProperty(tTRUTH, "Locked", (EntityGetterFunction)&IsLocked, (EntitySetterFunction)&SetIsLocked, nullptr, 17);
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "PopSensitiveType", (EntityGetterFunction)&GetPopSensitiveType, (EntitySetterFunction)&SetPopSensitiveType, nullptr);
    tHavocPlaceHolder->RegisterProperty(tTRUTH, "IsVIP", (EntityGetterFunction)&IsVIP, (EntitySetterFunction)&SetIsVIP, "control=truth");
    tHavocPlaceHolder->RegisterProperty(tNUMBER, "VIPTimer", (EntityGetterFunction)&GetVIPTimer, (EntitySetterFunction)&SetVIPTimer, "control=slider|min=0.0|max=100.0", 18);
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "Priority", (EntityGetterFunction)&GetPriority, (EntitySetterFunction)&SetPriority, nullptr, 19);
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "ObjectSubType", (EntityGetterFunction)&GetObjectSubType, (EntitySetterFunction)&SetObjectSubType, "control=autodropdown|SUB_OBJECTS");
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "Real_Active_Texture_Set", (EntityGetterFunction)&GetRealActiveTextureSet, (EntitySetterFunction)&SetRealActiveTextureSet, "control=slider|min=0|max=10");
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "Default_particle_effect", (EntityGetterFunction)&GetDefaultParticleEffect, (EntitySetterFunction)&SetDefaultParticleEffect, "control=dropdown|No_effect=0|Fire_small=1|Flies_swarm=2|AirfieldLightFlash=3");
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "Replace_pivot_ID", (EntityGetterFunction)&GetReplacePivotID, (EntitySetterFunction)&SetReplacePivotID, nullptr, 20);
    tHavocPlaceHolder->RegisterProperty(tINTEGER, "com_ID", (EntityGetterFunction)&GetComID, (EntitySetterFunction)&SetComID, nullptr, 21);
    tHavocPlaceHolder->RegisterProperty(tTRUTH, "Broken", (EntityGetterFunction)&IsBroken, (EntitySetterFunction)&SetIsBroken, nullptr, 22);

    tHavocPlaceHolder->PropagateProperties();
}

HavocPlaceHolder* HavocPlaceHolder::Create(AllocatorIndex)
{
    return new HavocPlaceHolder;
}