#include "CharacterPlaceholder.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "NumberType.h"
#include "StringType.h"

EntityType* tCharacterPlaceHolder;
int CharacterPlaceHolder::GetPlaceholderModelCommand = -1;

CharacterPlaceHolder::~CharacterPlaceHolder()
{
    MESSAGE_CLASS_DESTROYED(CharacterPlaceHolder);

    delete[] m_DynamicNodeName;
}

Vector4f* CharacterPlaceHolder::GetBounds(Vector4f& outbounds)
{
    outbounds = {0, 0, 0, 1};
    return &outbounds;
}

CharacterPlaceHolder* CharacterPlaceHolder::Create(AllocatorIndex)
{
    return new CharacterPlaceHolder;
}

void CharacterPlaceHolder::Register()
{
    tCharacterPlaceHolder = new EntityType("CharacterPlaceHolder");
    tCharacterPlaceHolder->InheritFrom(tNode);
    tCharacterPlaceHolder->SetCreator((CREATOR)Create);

    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "ResourceType", (EntityGetterFunction)&GetResourceType, (EntitySetterFunction)&SetResourceType, nullptr);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Status", (EntityGetterFunction)&GetStatus, (EntitySetterFunction)&SetStatus, nullptr, 10);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "PopSensitiveType", (EntityGetterFunction)&GetPopSensitiveType, (EntitySetterFunction)&SetPopSensitiveType, "control=autodropdown|POPSENSITIVE_TYPES", 14);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Locked", (EntityGetterFunction)&IsLocked, (EntitySetterFunction)&SetIsLocked, nullptr, 11);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "IsVIP", (EntityGetterFunction)&IsVIP, (EntitySetterFunction)&SetIsVIP, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "VIPTimer", (EntityGetterFunction)&GetVIPTimer, (EntitySetterFunction)&SetVIPTimer, "control=slider|min=0.0|max=100.0", 12);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Priority", (EntityGetterFunction)&GetPriority, (EntitySetterFunction)&SetPriority, nullptr, 13);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Model", (EntityGetterFunction)&GetModel, (EntitySetterFunction)&SetModel, "control=autodropdown|MODEL_CHARACTER");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Real_Active_Texture_Set", (EntityGetterFunction)&GetRealActiveTextureSet, (EntitySetterFunction)&SetRealActiveTextureSet, "control=slider|min=0|max=10");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "StoredDamage", (EntityGetterFunction)&GetStoredDamage, (EntitySetterFunction)&SetStoredDamage, nullptr, 15);
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "OrgHealth", (EntityGetterFunction)&GetOrgHealth, (EntitySetterFunction)&SetOrgHealth, nullptr, 16);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "CallBackNode", (EntityGetterFunction)&GetCallBackNode, (EntitySetterFunction)&SetCallBackNode, "control=autodropdown|CALLBACK_NODES");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "PhysicModel", (EntityGetterFunction)&GetPhysicModel, (EntitySetterFunction)&SetPhysicModel, "control=autodropdown|PHYSIC_MODEL");
    tCharacterPlaceHolder->RegisterProperty(tSTRING, "ForceDynamicNodeName", (EntityGetterFunction)&GetForceDynamicNodeName, (EntitySetterFunction)&SetForceDynamicNodeName, "control=string");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "MissionObjectiveKill", (EntityGetterFunction)&GetMissionObjectiveKill, (EntitySetterFunction)&SetMissionObjectiveKill, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Use_AISubType_Data", (EntityGetterFunction)&UseAISubTypeData, (EntitySetterFunction)&SetUseAISubTypeData, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "AISubType", (EntityGetterFunction)&GetAISubType, (EntitySetterFunction)&SetAISubType, "control=autodropdown|SUB_CHARACTER");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Morale", (EntityGetterFunction)&GetMorale, (EntitySetterFunction)&SetMorale, "control=dropdown|UseDefault=1|ChickenShit=0|StandFast=2");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "Wait_until_Aggressive", (EntityGetterFunction)&GetWaitUntilAggressive, (EntitySetterFunction)&SetWaitUntilAggressive, "control=slider|min=0.0|max=10.0");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "DesiredRange", (EntityGetterFunction)&GetDesiredRange, (EntitySetterFunction)&SetDesiredRange, "control=dropdown|UseWeapon=0|Custom=1");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "CustomDesiredRange", (EntityGetterFunction)&GetCustomDesiredRange, (EntitySetterFunction)&SetCustomDesiredRange, "control=slider|min=0.0|max=100.0");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "DontHunt", (EntityGetterFunction)&DontHunt, (EntitySetterFunction)&SetDontHunt, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "When_Shot", (EntityGetterFunction)&GetWhenShot, (EntitySetterFunction)&SetwhenShot, "control=autodropdown|WHEN_SHOT_PROP");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "IgnoreActionBreaks", (EntityGetterFunction)&IgnoreActionBreaks, (EntitySetterFunction)&SetIgnoreActionBreaks, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "IgnoreWeaponDrop", (EntityGetterFunction)&IgnoreWeaponDrop, (EntitySetterFunction)&SetIgnoreWeaponDrop, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "IgnoreMagnet", (EntityGetterFunction)&IgnoreMagnet, (EntitySetterFunction)&SetIgnoreMagnet, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Ignore_Hit_Anim", (EntityGetterFunction)&GetIgnoreHitAnim, (EntitySetterFunction)&SetIgnoreHitAnim, "control=dropdown|Use_Default=0|True=1|False=2");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Goto_Movement", (EntityGetterFunction)&GetGotoMovement, (EntitySetterFunction)&SetGotomovement, "control=autodropdown|GOTO_MOVEMENT_TYPE");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Always_Complete_Goto", (EntityGetterFunction)&AlwaysCompleteGoto, (EntitySetterFunction)&SetAlwaysCompleteGoto, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Ignore_5M_rule", (EntityGetterFunction)&Ignore5MRule, (EntitySetterFunction)&SetIgnore5MRule, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "BackUpWeapon", (EntityGetterFunction)&GetBackUpWeapon, (EntitySetterFunction)&SetBackUpWeapon, "control=dropdown|Default=-2|None=-1|Beretta=0|Magnum=1|Marlin=2|Winchester=3|Benelli=4|Hudson=5|Mac10=6|MP5=7|AK47=8|M16=9|Hand_Grenade=11|Grenade_Launcher=12|Rocket_Launcher=13|Melee=14");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Is_ConeHead", (EntityGetterFunction)&IsConeHead, (EntitySetterFunction)&SetIsConeHead, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Is_Buddy", (EntityGetterFunction)&IsBuddy, (EntitySetterFunction)&SetIsBuddy, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "GotopointDamage_mod", (EntityGetterFunction)&GetGotopointDamageMod, (EntitySetterFunction)&SetGotopointDamageMod, "control=slider|min=0.0|max=100.0");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Health_modify", (EntityGetterFunction)&GetHealthModify, (EntitySetterFunction)&SetHealthModify, "control=dropdown|-99%=18|-95%=17|-90%=0|-75%=5|-50%=2|-25%=3|-10%=4|NORMAL=1|+10%=6|+25%=7|+50%=8|+75%=9|+100%=10|+150%=11|+200%=12|+300%=13|+500%=14|+750%=15|+1000%=16|+1500%=19|+2000%=20|+3000%=21|+5000%=22");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Damage_modify", (EntityGetterFunction)&GetDamageModify, (EntitySetterFunction)&SetDamageModify, "control=dropdown|-99%=14|-95%=15|-90%=0|-75%=3|-50%=4|-25%=5|-10%=6|NORMAL=1|+10%=6|+25%=7|+50%=2|+75%=9|+100%=10|+200%=11|+500%=12|+1000%=13");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "HeadShot_Damage", (EntityGetterFunction)&GetHeadShotDamage, (EntitySetterFunction)&SetHeadShotDamage, "control=dropdown|50%=1|Normal=0|150%=2|Instant=3");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "Health_mod", (EntityGetterFunction)&GetHealthMod, (EntitySetterFunction)&SetHealthMod, nullptr);
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "Damage_mod", (EntityGetterFunction)&GetDamageMod, (EntitySetterFunction)&SetDamageMod, nullptr);
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "Marco_sqr_dist", (EntityGetterFunction)&GetMarcoSqrDist, (EntitySetterFunction)&SetMarcoSqrDist, nullptr, 31);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "PointValue_mod", (EntityGetterFunction)&GetPointValueMod, (EntitySetterFunction)&SetPointValueMod, "control=slider|min=0|max=100");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "InflictorValue_mod", (EntityGetterFunction)&GetInflictorValueMod, (EntitySetterFunction)&SetInflictorValueMod, "control=slider|min=0|max=100");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "FactionID", (EntityGetterFunction)&GetFactionID, (EntitySetterFunction)&SetFactionID, "control=autodropdown|FactionTypes", 17);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "UsingModifiedFaction", (EntityGetterFunction)&IsUsingModifiedFaction, (EntitySetterFunction)&SetUsingModifiedFaction, nullptr, 18);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "UsingSecretFaction", (EntityGetterFunction)&IsUsingSecretFaction, (EntitySetterFunction)&SetUsingSecretFaction, nullptr, 19);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "SecretFaction", (EntityGetterFunction)&GetSecretFaction, (EntitySetterFunction)&SetSecretFaction, nullptr, 20);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "PedestrianGroup", (EntityGetterFunction)&GetPedestrianGroup, (EntitySetterFunction)&SetPedestrianGroup, nullptr, 21);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "LockedType", (EntityGetterFunction)&GetLockedType, (EntitySetterFunction)&SetLockedType, nullptr, 22);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "FallbackModel", (EntityGetterFunction)&GetFallbackModel, (EntitySetterFunction)&SetFallbackModel, nullptr, 24);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "CurrentModel", (EntityGetterFunction)&GetCurrentModel, (EntitySetterFunction)&SetCurrentModel, nullptr, 23);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "PreviousModel", (EntityGetterFunction)&GetPreviousModel, (EntitySetterFunction)&SetPreviousModel, nullptr);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "StoreTrig", (EntityGetterFunction)&GetStoreTrig, (EntitySetterFunction)&SetStoreTrig, nullptr, 25);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "StoreAlarm", (EntityGetterFunction)&GetStoreAlarm, (EntitySetterFunction)&SetStoreAlarm, nullptr, 26);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "StorePriority", (EntityGetterFunction)&GetStorePriority, (EntitySetterFunction)&SetStorePriority, nullptr, 27);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "StoreTrigType", (EntityGetterFunction)&GetStoreTrigType, (EntitySetterFunction)&SetStoreTrigType, nullptr, 28);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "passenger_seat", (EntityGetterFunction)&GetPassengerSeat, (EntitySetterFunction)&SetPassengerSet, nullptr, 29);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "was_passenger", (EntityGetterFunction)&WasPassenger, (EntitySetterFunction)&SetWasPassenger, nullptr, 30);

    GetPlaceholderModelCommand = RegisterGlobalCommand("get_placeholder_model:entity", true);

    tCharacterPlaceHolder->PropagateProperties();
}