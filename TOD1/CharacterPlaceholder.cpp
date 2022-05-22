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

const int CharacterPlaceHolder::GetResourceType() const
{
    return m_Flags_1.ResourceType;
}

void CharacterPlaceHolder::SetResourceType(const int restype)
{
    m_Flags_1.ResourceType = restype;
}

const int CharacterPlaceHolder::GetStatus() const
{
    return m_Flags_1.Status;
}

void CharacterPlaceHolder::SetStatus(const int status)
{
    const int currentStatus = m_Flags_1.Status;
    StoreProperty(10, &currentStatus, tINTEGER);

    m_Flags_1.Status = status;
}

const int CharacterPlaceHolder::GetPopSensitiveType() const
{
    return m_Flags_1.PopSensitiveType;
}

void CharacterPlaceHolder::SetPopSensitiveType(const int popsestype)
{
    const int currentPopSensitiveType = m_Flags_1.PopSensitiveType;
    StoreProperty(14, &currentPopSensitiveType, tINTEGER);

    m_Flags_1.PopSensitiveType = popsestype;
}

const bool CharacterPlaceHolder::IsLocked() const
{
    return m_Flags_1.Locked;
}

void CharacterPlaceHolder::SetIsLocked(const bool locked)
{
    const bool currentLockedStatus = m_Flags_1.Locked;
    StoreProperty(11, &currentLockedStatus, tTRUTH);

    m_Flags_1.Locked = locked;
}

const bool CharacterPlaceHolder::IsVIP() const
{
    return m_Flags_1.VIP;
}

void CharacterPlaceHolder::SetIsVIP(const bool vip)
{
    m_Flags_1.VIP = vip;
}

const float CharacterPlaceHolder::GetVIPTimer() const
{
    return m_VIPTimer;
}

void CharacterPlaceHolder::SetVIPTimer(const float timer)
{
    StoreProperty(12, &m_VIPTimer, tNUMBER);
    m_VIPTimer = timer;
}

const int CharacterPlaceHolder::GetPriority() const
{
    return m_Flags_2.Priority;
}

void CharacterPlaceHolder::SetPriority(const int priority)
{
    const int currentPriority = m_Flags_2.Priority;
    StoreProperty(13, &currentPriority, tINTEGER);

    m_Flags_2.Priority = priority;
}

const int CharacterPlaceHolder::GetModel() const
{
    return m_Flags_2.Model;
}

void CharacterPlaceHolder::SetModel(const int model)
{
    m_Flags_2.Model = model;
}

const int CharacterPlaceHolder::GetRealActiveTextureSet() const
{
    return m_Flags_2.RealActiveTextureSet;
}

void CharacterPlaceHolder::SetRealActiveTextureSet(const int texset)
{
    m_Flags_2.RealActiveTextureSet = texset;
}

const float CharacterPlaceHolder::GetStoredDamage() const
{
    return m_StoredDamage;
}

void CharacterPlaceHolder::SetStoredDamage(const float dam)
{
    StoreProperty(15, &m_StoredDamage, tNUMBER);

    m_StoredDamage = dam;
}

const float CharacterPlaceHolder::GetOrgHealth() const
{
    return m_OrgHealth;
}

void CharacterPlaceHolder::SetOrgHealth(const float health)
{
    StoreProperty(16, &m_OrgHealth, tNUMBER);

    m_OrgHealth = health;
}

const int CharacterPlaceHolder::GetCallBackNode() const
{
    return m_Flags_3.CallBackNode;
}

void CharacterPlaceHolder::SetCallBackNode(const int cbnode)
{
    m_Flags_3.CallBackNode = cbnode;
}

const int CharacterPlaceHolder::GetPhysicModel() const
{
    return m_Flags_3.PhysicModel;
}

void CharacterPlaceHolder::SetPhysicModel(const int physmodel)
{
    m_Flags_3.PhysicModel = physmodel;
}

const char* const CharacterPlaceHolder::GetDynamicNodeName() const
{
    return m_DynamicNodeName;
}

void CharacterPlaceHolder::ForceDynamicNodeName(const char* const nodename)
{
    if (m_DynamicNodeName)
        delete[] m_DynamicNodeName;

    if (!nodename || !nodename[0])
        return;

    m_DynamicNodeName = _strdup(nodename);
}

const bool CharacterPlaceHolder::MissionObjectiveKill() const
{
    return m_Flags_4.MissionObjectiveKill;
}

void CharacterPlaceHolder::SetMissionObjectiveKill(const bool obj)
{
    m_Flags_4.MissionObjectiveKill = obj;
}

const bool CharacterPlaceHolder::UseAISubTypeData() const
{
    return m_Flags_4.UseAISubTypeData;
}

void CharacterPlaceHolder::SetUseAISubTypeData(const bool use)
{
    m_Flags_4.UseAISubTypeData = use;
}

const int CharacterPlaceHolder::GetAISubType() const
{
    return m_Flags_4.AISubType;
}

void CharacterPlaceHolder::SetAISubType(const int subtype)
{
    m_Flags_4.AISubType = subtype;
}

const int CharacterPlaceHolder::GetMorale() const
{
    return m_Flags_4.Morale;
}

void CharacterPlaceHolder::SetMorale(const int morale)
{
    m_Flags_4.Morale = morale;
}

const float CharacterPlaceHolder::GetWaitUntilAggressive() const
{
    return m_WaitUntilAggressiveTime;
}

void CharacterPlaceHolder::SetWaitUntilAggressive(const float waittime)
{
    m_WaitUntilAggressiveTime = waittime;
}

const eDesiredRange CharacterPlaceHolder::GetDesiredRange() const
{
    return m_DesiredRange;
}

void CharacterPlaceHolder::SetDesiredRange(const eDesiredRange range)
{
    m_DesiredRange = range;
}

const float CharacterPlaceHolder::GetCustomDesiredRange() const
{
    return m_CustomDesiredRange;
}

void CharacterPlaceHolder::SetCustomDesiredRange(const float range)
{
    m_CustomDesiredRange = range;
}

const bool CharacterPlaceHolder::DontHunt() const
{
    return m_Flags_5.DontHunt;
}

void CharacterPlaceHolder::SetDontHunt(const bool donthunt)
{
    m_Flags_5.DontHunt = donthunt;
}

const int CharacterPlaceHolder::WhenShot() const
{
    return m_Flags_5.WhenShot;
}

void CharacterPlaceHolder::SetWhenShot(const int whenshot)
{
    m_Flags_5.WhenShot = whenshot;
}

const bool CharacterPlaceHolder::IgnoreActionBreaks() const
{
    return m_Flags_5.IgnoreActionBreaks;
}

void CharacterPlaceHolder::SetIgnoreActionBreaks(const bool ignore)
{
    m_Flags_5.IgnoreActionBreaks = ignore;
}

const bool CharacterPlaceHolder::IgnoreWeaponDrop() const
{
    return m_Flags_5.IgnoreWeaponDrop;
}

void CharacterPlaceHolder::SetIgnoreWeaponDrop(const bool ignore)
{
    m_Flags_5.IgnoreWeaponDrop = ignore;
}

const bool CharacterPlaceHolder::IgnoreMagnet() const
{
    return m_Flags_5.IgnoreMagnet;
}

void CharacterPlaceHolder::SetIgnoreMagnet(const bool ignore)
{
    m_Flags_5.IgnoreMagnet = ignore;
}

const eIgnoreHitAnim CharacterPlaceHolder::GetIgnoreHitAnim() const
{
    return (eIgnoreHitAnim)m_Flags_5.IgnoreHitAnim;
}

void CharacterPlaceHolder::SetIgnoreHitAnim(const eIgnoreHitAnim hitanimignore)
{
    m_Flags_5.IgnoreHitAnim = (unsigned int)hitanimignore;
}

const int CharacterPlaceHolder::GotoMovement() const
{
    return m_Flags_5.GotoMovement;
}

void CharacterPlaceHolder::SetGotoMovement(const int movementtype)
{
    m_Flags_5.GotoMovement = movementtype;
}

const bool CharacterPlaceHolder::AlwaysCompleteGoto() const
{
    return m_Flags_5.AlwaysCompleteGoto;
}

void CharacterPlaceHolder::SetAlwaysCompleteGoto(const bool complete)
{
    m_Flags_5.AlwaysCompleteGoto = complete;
}

const bool CharacterPlaceHolder::Ignore5MRule() const
{
    return m_Flags_5.Ignore5MRule;
}

void CharacterPlaceHolder::SetIgnore5MRule(const bool ignore)
{
    m_Flags_5.Ignore5MRule = ignore;
}

const eBackupWeapon CharacterPlaceHolder::GetBackupWeapon() const
{
    return (eBackupWeapon)m_Flags_5.BackUpWeapon;
}

void CharacterPlaceHolder::SetBackupWeapon(const eBackupWeapon bkweapon)
{
    m_Flags_5.BackUpWeapon = (unsigned int)bkweapon;
}

const bool CharacterPlaceHolder::IsConeHead() const
{
    return m_Flags_5.IsConeHead;
}

void CharacterPlaceHolder::SetIsConeHead(const bool conehead)
{
    m_Flags_5.IsConeHead = conehead;
}

const bool CharacterPlaceHolder::IsBuddy() const
{
    return m_Flags_5.IsBuddy;
}

void CharacterPlaceHolder::SetIsBuddy(const bool buddy)
{
    m_Flags_5.IsBuddy = buddy;
}

const float CharacterPlaceHolder::GetGotopointDamageMod() const
{
    return m_GotopointDamage_mod;
}

void CharacterPlaceHolder::SetGotopointDamageMod(const float mod)
{
    m_GotopointDamage_mod = mod;
}

const eHealthModify CharacterPlaceHolder::GetHealthModify() const
{
    return (eHealthModify)m_Flags_6.HealthModify;
}

void CharacterPlaceHolder::SetHealthModify(const eHealthModify mod)
{
    m_Flags_6.HealthModify = (unsigned int)mod;
}

const eDamageModify CharacterPlaceHolder::GetDamageModify() const
{
    return (eDamageModify)m_Flags_6.DamageModify;
}

void CharacterPlaceHolder::SetDamageModify(const eDamageModify mod)
{
    m_Flags_6.DamageModify = (unsigned int)mod;
}

const eHeadShotDamage CharacterPlaceHolder::GetHeadShotDamage() const
{
    return (eHeadShotDamage)m_Flags_6.HeadShotDamage;
}

void CharacterPlaceHolder::SetHeadShotDamage(const eHeadShotDamage dam)
{
    m_Flags_6.HeadShotDamage = (unsigned int)dam;
}

const float CharacterPlaceHolder::GetHealthMod() const
{
    return m_Health_mod;
}

void CharacterPlaceHolder::SetHealthMod(const float mod)
{
    m_Health_mod = mod;
}

const float CharacterPlaceHolder::GetDamageMod() const
{
    return m_Damage_mod;
}

void CharacterPlaceHolder::SetDamageMod(const float mod)
{
    m_Damage_mod = mod;
}

const float CharacterPlaceHolder::GetMarcoSqrDist() const
{
    return m_Marco_sqr_dist;
}

void CharacterPlaceHolder::SetMarcoSqrDist(const float dist)
{
    StoreProperty(31, &m_Marco_sqr_dist, tNUMBER);
    m_Marco_sqr_dist = dist;
}

const int CharacterPlaceHolder::GetPointValueMod() const
{
    return m_Flags_7.PointValueMod;
}

void CharacterPlaceHolder::SetPointValueMod(const int mod)
{
    m_Flags_7.PointValueMod = mod;
}

const int CharacterPlaceHolder::GetInflictorValueMod() const
{
    return m_Flags_7.InflictorValueMod;
}

void CharacterPlaceHolder::SetInflictorValueMod(const int mod)
{
    m_Flags_7.InflictorValueMod = mod;
}

const eFactionType CharacterPlaceHolder::GetFactionID() const
{
    return (eFactionType)m_Flags_7.FactionID;
}

void CharacterPlaceHolder::SetFactionID(const eFactionType factype)
{
    const int currentFactionID = m_Flags_7.FactionID;
    StoreProperty(17, &currentFactionID, tINTEGER);

    m_Flags_7.FactionID = (unsigned int)factype;
}

const bool CharacterPlaceHolder::UsingModifiedFaction() const
{
    return m_Flags_7.UsingModifiedFaction;
}

void CharacterPlaceHolder::SetUsingModifiedFaction(const bool use)
{
    const bool currentModifiedFaction = m_Flags_7.UsingModifiedFaction;
    StoreProperty(18, &currentModifiedFaction, tTRUTH);

    m_Flags_7.UsingModifiedFaction = use;
}

const bool CharacterPlaceHolder::UsingSecretFaction() const
{
    return m_Flags_7.UsingSecretFaction;
}

void CharacterPlaceHolder::SetUsingSecretFaction(const bool use)
{
    const bool currentUsingSecretFaction = m_Flags_7.UsingSecretFaction;
    StoreProperty(19, &currentUsingSecretFaction, tTRUTH);

    m_Flags_7.UsingSecretFaction = use;
}

const int CharacterPlaceHolder::GetSecretFaction() const
{
    return m_Flags_7.SecretFaction;
}

void CharacterPlaceHolder::SetSecretFaction(const int faction)
{
    const int currentSecretFaction = m_Flags_7.SecretFaction;
    StoreProperty(20, &currentSecretFaction, tINTEGER);

    m_Flags_7.SecretFaction = faction;
}

const int CharacterPlaceHolder::GetPedestrianGroup() const
{
    return m_Flags_8.PedestrianGroup;
}

void CharacterPlaceHolder::SetPedestrianGroup(const int group)
{
    const int currentPedestrianGroup = m_Flags_8.PedestrianGroup;
    StoreProperty(21, &currentPedestrianGroup, tINTEGER);

    m_Flags_8.PedestrianGroup = group;
}

const bool CharacterPlaceHolder::IsLockedType() const
{
    return m_Flags_8.LockedType;
}

void CharacterPlaceHolder::SetLockedType(const bool locked)
{
    const bool currentLockedType = m_Flags_8.LockedType;
    StoreProperty(22, &currentLockedType, tTRUTH);

    m_Flags_8.LockedType = locked;
}

const int CharacterPlaceHolder::GetFallbackModel() const
{
    return m_Flags_8.FallbackModel;
}

void CharacterPlaceHolder::SetFallbackModel(const int model)
{
    const int currentFallbackModel = m_Flags_8.FallbackModel;
    StoreProperty(24, &currentFallbackModel, tINTEGER);

    m_Flags_8.FallbackModel = model;
}

const int CharacterPlaceHolder::GetCurrentModel() const
{
    return m_Flags_8.CurrentModel;
}

void CharacterPlaceHolder::SetCurrentModel(const int model)
{
    const int currentCurrentModel = m_Flags_8.CurrentModel;
    StoreProperty(23, &currentCurrentModel, tINTEGER);

    m_Flags_8.CurrentModel = model;
}

const int CharacterPlaceHolder::GetPreviousModel() const
{
    return m_Flags_8.PreviousModel;
}

void CharacterPlaceHolder::SetPreviousModel(const int model)
{
    m_Flags_8.PreviousModel = model;
}

const bool CharacterPlaceHolder::StoreTrig() const
{
    return m_Flags_8.StoreTrig;
}

void CharacterPlaceHolder::SetStoreTrig(const bool store)
{
    const bool currentStoreTrig = m_Flags_8.StoreTrig;
    StoreProperty(25, &currentStoreTrig, tTRUTH);

    m_Flags_8.StoreTrig = store;
}

const bool CharacterPlaceHolder::StoreAlarm() const
{
    return m_Flags_8.StoreAlarm;
}

void CharacterPlaceHolder::SetStoreAlarm(const bool store)
{
    const bool currentStoreAlarm = m_Flags_8.StoreAlarm;
    StoreProperty(26, &currentStoreAlarm, tTRUTH);

    m_Flags_8.StoreAlarm = store;
}

const bool CharacterPlaceHolder::StorePriority() const
{
    return m_Flags_8.StorePriority;
}

void CharacterPlaceHolder::SetStorePriority(const bool store)
{
    const bool currentStorePriority = m_Flags_8.StorePriority;
    StoreProperty(27, &currentStorePriority, tTRUTH);

    m_Flags_8.StorePriority = store;
}

const int CharacterPlaceHolder::StoreTrigType() const
{
    return m_Flags_8.StoreTrigType;
}

void CharacterPlaceHolder::SetStoreTrigType(const int store)
{
    const int currentStoreTrigType = m_Flags_8.StoreTrigType;
    StoreProperty(28, &currentStoreTrigType, tINTEGER);

    m_Flags_8.StoreTrigType = store;
}

const int CharacterPlaceHolder::GetPassengerSeat() const
{
    return m_Flags_9.PassengerSeat;
}

void CharacterPlaceHolder::SetPassengerSeat(const int seat)
{
    const int currentPassengerSeat = m_Flags_9.PassengerSeat;
    StoreProperty(29, &currentPassengerSeat, tINTEGER);

    m_Flags_9.PassengerSeat = seat;
}

const bool CharacterPlaceHolder::WasPassenger() const
{
    return m_Flags_9.WasPassenger;
}

void CharacterPlaceHolder::SetWasPassenger(const bool set)
{
    const bool currentWasPassenger = m_Flags_9.WasPassenger;
    StoreProperty(30, &currentWasPassenger, tTRUTH);

    m_Flags_9.WasPassenger = set;
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
    tCharacterPlaceHolder->RegisterProperty(tSTRING, "ForceDynamicNodeName", (EntityGetterFunction)&GetDynamicNodeName, (EntitySetterFunction)&ForceDynamicNodeName, "control=string");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "MissionObjectiveKill", (EntityGetterFunction)&MissionObjectiveKill, (EntitySetterFunction)&SetMissionObjectiveKill, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Use_AISubType_Data", (EntityGetterFunction)&UseAISubTypeData, (EntitySetterFunction)&SetUseAISubTypeData, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "AISubType", (EntityGetterFunction)&GetAISubType, (EntitySetterFunction)&SetAISubType, "control=autodropdown|SUB_CHARACTER");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Morale", (EntityGetterFunction)&GetMorale, (EntitySetterFunction)&SetMorale, "control=dropdown|UseDefault=1|ChickenShit=0|StandFast=2");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "Wait_until_Aggressive", (EntityGetterFunction)&GetWaitUntilAggressive, (EntitySetterFunction)&SetWaitUntilAggressive, "control=slider|min=0.0|max=10.0");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "DesiredRange", (EntityGetterFunction)&GetDesiredRange, (EntitySetterFunction)&SetDesiredRange, "control=dropdown|UseWeapon=0|Custom=1");
    tCharacterPlaceHolder->RegisterProperty(tNUMBER, "CustomDesiredRange", (EntityGetterFunction)&GetCustomDesiredRange, (EntitySetterFunction)&SetCustomDesiredRange, "control=slider|min=0.0|max=100.0");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "DontHunt", (EntityGetterFunction)&DontHunt, (EntitySetterFunction)&SetDontHunt, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "When_Shot", (EntityGetterFunction)&WhenShot, (EntitySetterFunction)&SetWhenShot, "control=autodropdown|WHEN_SHOT_PROP");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "IgnoreActionBreaks", (EntityGetterFunction)&IgnoreActionBreaks, (EntitySetterFunction)&SetIgnoreActionBreaks, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "IgnoreWeaponDrop", (EntityGetterFunction)&IgnoreWeaponDrop, (EntitySetterFunction)&SetIgnoreWeaponDrop, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "IgnoreMagnet", (EntityGetterFunction)&IgnoreMagnet, (EntitySetterFunction)&SetIgnoreMagnet, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Ignore_Hit_Anim", (EntityGetterFunction)&GetIgnoreHitAnim, (EntitySetterFunction)&SetIgnoreHitAnim, "control=dropdown|Use_Default=0|True=1|False=2");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "Goto_Movement", (EntityGetterFunction)&GotoMovement, (EntitySetterFunction)&SetGotoMovement, "control=autodropdown|GOTO_MOVEMENT_TYPE");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Always_Complete_Goto", (EntityGetterFunction)&AlwaysCompleteGoto, (EntitySetterFunction)&SetAlwaysCompleteGoto, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "Ignore_5M_rule", (EntityGetterFunction)&Ignore5MRule, (EntitySetterFunction)&SetIgnore5MRule, "control=truth");
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "BackUpWeapon", (EntityGetterFunction)&GetBackupWeapon, (EntitySetterFunction)&SetBackupWeapon, "control=dropdown|Default=-2|None=-1|Beretta=0|Magnum=1|Marlin=2|Winchester=3|Benelli=4|Hudson=5|Mac10=6|MP5=7|AK47=8|M16=9|Hand_Grenade=11|Grenade_Launcher=12|Rocket_Launcher=13|Melee=14");
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
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "UsingModifiedFaction", (EntityGetterFunction)&UsingModifiedFaction, (EntitySetterFunction)&SetUsingModifiedFaction, nullptr, 18);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "UsingSecretFaction", (EntityGetterFunction)&UsingSecretFaction, (EntitySetterFunction)&SetUsingSecretFaction, nullptr, 19);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "SecretFaction", (EntityGetterFunction)&GetSecretFaction, (EntitySetterFunction)&SetSecretFaction, nullptr, 20);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "PedestrianGroup", (EntityGetterFunction)&GetPedestrianGroup, (EntitySetterFunction)&SetPedestrianGroup, nullptr, 21);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "LockedType", (EntityGetterFunction)&IsLockedType, (EntitySetterFunction)&SetLockedType, nullptr, 22);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "FallbackModel", (EntityGetterFunction)&GetFallbackModel, (EntitySetterFunction)&SetFallbackModel, nullptr, 24);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "CurrentModel", (EntityGetterFunction)&GetCurrentModel, (EntitySetterFunction)&SetCurrentModel, nullptr, 23);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "PreviousModel", (EntityGetterFunction)&GetPreviousModel, (EntitySetterFunction)&SetPreviousModel, nullptr);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "StoreTrig", (EntityGetterFunction)&StoreTrig, (EntitySetterFunction)&SetStoreTrig, nullptr, 25);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "StoreAlarm", (EntityGetterFunction)&StoreAlarm, (EntitySetterFunction)&SetStoreAlarm, nullptr, 26);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "StorePriority", (EntityGetterFunction)&StorePriority, (EntitySetterFunction)&SetStorePriority, nullptr, 27);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "StoreTrigType", (EntityGetterFunction)&StoreTrigType, (EntitySetterFunction)&SetStoreTrigType, nullptr, 28);
    tCharacterPlaceHolder->RegisterProperty(tINTEGER, "passenger_seat", (EntityGetterFunction)&GetPassengerSeat, (EntitySetterFunction)&SetPassengerSeat, nullptr, 29);
    tCharacterPlaceHolder->RegisterProperty(tTRUTH, "was_passenger", (EntityGetterFunction)&WasPassenger, (EntitySetterFunction)&SetWasPassenger, nullptr, 30);

    GetPlaceholderModelCommand = GetMessage("get_placeholder_model:entity", true);

    tCharacterPlaceHolder->PropagateProperties();
}