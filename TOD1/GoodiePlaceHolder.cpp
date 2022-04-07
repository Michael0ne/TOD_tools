#include "GoodiePlaceHolder.h"

EntityType* tGoodiePlaceholder;

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
