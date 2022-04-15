#include "GoodiePlaceHolder.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "NumberType.h"

EntityType* tGoodiePlaceholder;

const int GoodiePlaceHolder::GetResourceType() const
{
    return m_Flags_13.ResourceType;
}

void GoodiePlaceHolder::SetResourceType(const int restype)
{
    m_Flags_13.ResourceType = restype;
}

const int GoodiePlaceHolder::GetStatus() const
{
    return m_Flags_13.Status;
}

void GoodiePlaceHolder::SetStatus(const int status)
{
    const int currentStatus = m_Flags_13.Status;
    StoreProperty(16, &currentStatus, tINTEGER);

    m_Flags_13.Status = status;
}

const bool GoodiePlaceHolder::IsLocked() const
{
    return m_Flags_13.Locked;
}

void GoodiePlaceHolder::SetIsLocked(const bool locked)
{
    const bool currentLocked = m_Flags_13.Locked;
    StoreProperty(17, &currentLocked, tTRUTH);

    m_Flags_13.Locked = locked;
}

const int GoodiePlaceHolder::GetPopSensitiveType() const
{
    return m_Flags_13.PopSensitiveType;
}

void GoodiePlaceHolder::SetPopSensitiveType(const int popsenstype)
{
    m_Flags_13.PopSensitiveType = popsenstype;
}

const bool GoodiePlaceHolder::IsVIP() const
{
    return m_Flags_13.VIP;
}

void GoodiePlaceHolder::SetIsVIP(const bool vip)
{
    m_Flags_13.VIP = vip;
}

const float GoodiePlaceHolder::GetVIPTimer() const
{
    return m_VIPTimer;
}

void GoodiePlaceHolder::SetVIPTimer(const float timer)
{
    StoreProperty(18, &m_VIPTimer, tNUMBER);
    m_VIPTimer = timer;
}

const int GoodiePlaceHolder::GetPriority() const
{
    return m_Flags_14.Priority;
}

void GoodiePlaceHolder::SetPriority(const int priority)
{
    const int currentPriority = m_Flags_14.Priority;
    StoreProperty(16, &currentPriority, tINTEGER);

    m_Flags_14.Priority = priority;
}

const int GoodiePlaceHolder::GetGoodieSubType() const
{
    return m_Flags_14.GoodieSubType;
}

void GoodiePlaceHolder::SetGoodieSubType(const int subtype)
{
    m_Flags_14.GoodieSubType = subtype;
}

const bool GoodiePlaceHolder::IsRespawnable() const
{
    return m_Flags_14.Respawnable;
}

void GoodiePlaceHolder::SetIsRespawnable(const bool resp)
{
    m_Flags_14.Respawnable = resp;
}

const float GoodiePlaceHolder::GetRespawnTime() const
{
    return m_RespawnTime;
}

void GoodiePlaceHolder::SetRespawnTime(const float resptime)
{
    m_RespawnTime = resptime;
}

void GoodiePlaceHolder::Register()
{
    tGoodiePlaceholder = new EntityType("GoodiePlaceHolder");
    tGoodiePlaceholder->InheritFrom(tPlaceholder);
    tGoodiePlaceholder->SetCreator((CREATOR)Create);

    tGoodiePlaceholder->RegisterProperty(tINTEGER, "ResourceType", (EntityGetterFunction)&GetResourceType, (EntitySetterFunction)&SetResourceType, nullptr);
    tGoodiePlaceholder->RegisterProperty(tINTEGER, "Status", (EntityGetterFunction)&GetStatus, (EntitySetterFunction)&SetStatus, nullptr, 16);
    tGoodiePlaceholder->RegisterProperty(tTRUTH, "Locked", (EntityGetterFunction)&IsLocked, (EntitySetterFunction)&SetIsLocked, nullptr, 17);
    tGoodiePlaceholder->RegisterProperty(tINTEGER, "PopSensitiveType", (EntityGetterFunction)&GetPopSensitiveType, (EntitySetterFunction)&SetPopSensitiveType, "control=autodropdown|POPSENSITIVE_TYPES");
    tGoodiePlaceholder->RegisterProperty(tTRUTH, "IsVIP", (EntityGetterFunction)&IsVIP, (EntitySetterFunction)&SetIsVIP, "control=truth");
    tGoodiePlaceholder->RegisterProperty(tNUMBER, "VIPTimer", (EntityGetterFunction)&GetVIPTimer, (EntitySetterFunction)&SetVIPTimer, "control=slider|min=0.0|max=100.0", 18);
    tGoodiePlaceholder->RegisterProperty(tINTEGER, "Priority", (EntityGetterFunction)&GetPriority, (EntitySetterFunction)&SetPriority, nullptr, 19);
    tGoodiePlaceholder->RegisterProperty(tINTEGER, "GoodieSubType", (EntityGetterFunction)&GetGoodieSubType, (EntitySetterFunction)&SetGoodieSubType, "control=autodropdown|GOODIE_TYPES");
    tGoodiePlaceholder->RegisterProperty(tTRUTH, "Respawnable", (EntityGetterFunction)&IsRespawnable, (EntitySetterFunction)&SetIsRespawnable, "control=truth");
    tGoodiePlaceholder->RegisterProperty(tNUMBER, "RespawnTime", (EntityGetterFunction)&GetRespawnTime, (EntitySetterFunction)&SetRespawnTime, "control=string");

    tGoodiePlaceholder->PropagateProperties();
}

GoodiePlaceHolder* GoodiePlaceHolder::Create(AllocatorIndex)
{
    return new GoodiePlaceHolder;
}
