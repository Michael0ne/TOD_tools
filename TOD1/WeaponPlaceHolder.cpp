#include "WeaponPlaceHolder.h"

EntityType* tWeaponPlaceholder;

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
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "WeaponSubType", (EntityGetterFunction)GetWeaponSubType, (EntitySetterFunction)&SetWeaponSubType, "control=autodropdown|SUB_WEAPON_TYPES");
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "MeleeWeaponSubType", (EntityGetterFunction)&GetMeleeWeaponSubType, (EntitySetterFunction)&SetMeleeWeaponSubType, "control=autodropdown|MELEE_WEAPON_TYPES");
    tWeaponPlaceholder->RegisterProperty(tINTEGER, "BulletDrop", (EntityGetterFunction)&GetBulletDropType, (EntitySetterFunction)&SetBulletDropType, "control=autodropdown|BULLET_DROP_TYPES");
    tWeaponPlaceholder->RegisterProperty(tTRUTH, "Respawnable", (EntityGetterFunction)&IsRespawnable, (EntitySetterFunction)&SetIsRespawnable, "control=truth");
    tWeaponPlaceholder->RegisterProperty(tNUMBER, "RespawnTime", (EntityGetterFunction)&GetRespawnTime, (EntitySetterFunction)&SetRespawnTime, "control=string");

    tWeaponPlaceholder->PropagateProperties();
}

WeaponPlaceHolder* WeaponPlaceHolder::Create(AllocatorIndex)
{
    return new WeaponPlaceHolder;
}