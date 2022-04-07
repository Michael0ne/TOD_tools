#include "HavocPlaceHolder.h"

EntityType* tHavocPlaceHolder;

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