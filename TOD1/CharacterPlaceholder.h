#pragma once
#include "Node.h"

enum class eIgnoreHitAnim
{
    USE_DEFAULT = 0,
    USE_TRUE = 1,
    USE_FALSE = 2
};

enum class eDesiredRange
{
    USE_WEAPON = 0,
    CUSTOM = 1
};

enum class eGotoMovementType
{};

enum class eBackupWeapon
{
    DEFAULT = -2,
    NONE = -1,
    BERETTA = 0,
    MAGNUM = 1,
    MARLIN = 2,
    WINCHESTER = 3,
    BERNELLI = 4,
    HUDSON = 5,
    MAC10 = 6,
    MP5 = 7,
    AK47 = 8,
    M16 = 9,
    HANDGRENADE = 11,
    GRENADELAUNCHER = 12,
    ROCKETLAUNCHER = 13,
    MELEE = 14
};

enum class eHealthModify
{
    MINUS_99 = 18,
    MINUS_95 = 17,
    MINUS_90 = 0,
    MINUS_75 = 5,
    MINUS_50 = 2,
    MINUS_25 = 3,
    MINUS_10 = 4,
    NORMAL = 1,
    PLUS_10 = 6,
    PLUS_25 = 7,
    PLUS_50 = 8,
    PLUS_75 = 9,
    PLUS_100 = 10,
    PLUS_150 = 11,
    PLUS_200 = 12,
    PLUS_300 = 13,
    PLUS_500 = 14,
    PLUS_750 = 15,
    PLUS_1000 = 16,
    PLUS_1500 = 19,
    PLUS_2000 = 20,
    PLUS_3000 = 21,
    PLUS_5000 = 22
};

enum class eDamageModify
{
    MINUS_99 = 14,
    MINUS_95 = 15,
    MINUS_90 = 0,
    MINUS_75 = 3,
    MINUS_50 = 4,
    MINUS_25 = 5,
    MINUS_10 = 6,
    NORMAL = 1,
    PLUS_10 = 6,
    PLUS_25 = 7,
    PLUS_50 = 2,
    PLUS_75 = 9,
    PLUS_100 = 10,
    PLUS_200 = 11,
    PLUS_500 = 12,
    PLUS_1000 = 13
};

enum class eHeadShotDamage
{
    HALF = 1,   //  50%
    NORMAL = 0,
    HUNDRED_AND_FIFTY = 2,  //  150%
    INSTANT = 3
};

enum class eFactionType
{};

class CharacterPlaceHolder : public Node
{
protected:
    union
    {
        unsigned    ResourceType : 4;
        unsigned    Status : 2;
        unsigned    PopSensitiveType : 2;
        unsigned    Locked : 1;
        unsigned    VIP : 1;
    }               m_Flags_1;
    float           m_VIPTimer;
    union
    {
        unsigned    Priority : 3;
        unsigned    Model : 5;
        unsigned    RealActiveTextureSet : 5;
    }               m_Flags_2;
    float           m_StoredDamage;
    float           m_OrgHealth;
    union
    {
        unsigned    CallBackNode : 1;
        unsigned    PhysicModel : 3;
    }               m_Flags_3;
    char           *m_DynamicNodeName;
    union
    {
        unsigned    MissionObjectiveKill : 1;
        unsigned    UseAISubTypeData : 1;
        unsigned    AISubType : 5;
        unsigned    Morale : 2;
    }               m_Flags_4;
    float           m_WaitUntilAggressiveTime;
    eDesiredRange   m_DesiredRange;
    float           m_CustomDesiredRange;
    union
    {
        unsigned    DontHunt : 1;
        unsigned    WhenShot : 2;
        unsigned    IgnoreActionBreaks : 1;
        unsigned    IgnoreWeaponDrop : 1;
        unsigned    IgnoreMagnet : 1;
        unsigned    IgnoreHitAnim : 2;
        unsigned    GotoMovement : 2;
        unsigned    AlwaysCompleteGoto : 1;
        unsigned    Ignore5MRule : 1;
        unsigned    BackUpWeapon : 5;
        unsigned    IsConeHead : 1;
        unsigned    IsBuddy : 1;
    }               m_Flags_5;
    float           m_GotopointDamage_mod;
    union
    {
        unsigned    HealthModify : 5;
        unsigned    DamageModify : 4;
        unsigned    HeadShotDamage : 2;
    }               m_Flags_6;
    float           m_Health_mod;
    float           m_Damage_mod;
    float           m_Marco_sqr_dist;
    union
    {
        unsigned    PointValueMod : 8;
        unsigned    InflictorValueMod : 8;
        unsigned    FactionID : 6;
        unsigned    UsingModifiedFaction : 1;
        unsigned    UsingSecretFaction : 1;
        unsigned    SecretFaction : 7;
    }               m_Flags_7;
    union
    {
        unsigned    PedestrianGroup : 4;
        unsigned    LockedType : 1;
        unsigned    FallbackModel : 6;
        unsigned    CurrentModel : 6;
        unsigned    PreviousModel : 6;
        unsigned    StoreTrig : 1;
        unsigned    StoreAlarm : 1;
        unsigned    StorePriority : 1;
        unsigned    StoreTrigType : 4;
    }               m_Flags_8;
    union
    {
        unsigned    PassengerSeat : 3;
        unsigned    WasPassenger : 1;
    }               m_Flags_9;

public:
    #pragma message(TODO_IMPLEMENTATION)
    inline CharacterPlaceHolder() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
    {
        MESSAGE_CLASS_CREATED(CharacterPlaceHolder);

        m_CustomDesiredRange = 25.0f;
        m_GotopointDamage_mod = 100.0f;
        m_VIPTimer = 0.0f;
        m_StoredDamage = 0.0f;
        m_OrgHealth = 0.0f;
        m_WaitUntilAggressiveTime = 0.0f;
        m_Health_mod = 1.0f;
        m_Damage_mod = 1.0f;
        m_Marco_sqr_dist = 0.0f;
    }

    virtual ~CharacterPlaceHolder(); // @8CC860
    virtual Vector4f* GetBounds(Vector4f& outbounds) override; // @8CC800

    const int                       GetResourceType() const;    //  @8CB2C0
    void                            SetResourceType(const int restype); //  @8CB2D0
    const int                       GetStatus() const;  //  @5A18D0
    void                            SetStatus(const int status);    //  @5A18E0
    const int                       GetPopSensitiveType() const;    //  @7607B0
    void                            SetPopSensitiveType(const int popsestype);  //  @5A1920
    const bool                      IsLocked() const;   //  @7607C0
    void                            SetIsLocked(const bool locked); //  @5A1970
    const bool                      IsVIP() const;  //  @5A19B0
    void                            SetIsVIP(const bool vip);   //  @5A19C0
    const float                     GetVIPTimer() const;    //  @89A890
    void                            SetVIPTimer(const float timer); //  @8CB2F0
    const int                       GetPriority() const;    //  @5A19E0
    void                            SetPriority(const int priority);    //  @5A19F0
    const int                       GetModel() const;   //  @5A1A30
    void                            SetModel(const int model); //  @5A1A40
    const int                       GetRealActiveTextureSet() const;    //  @5A1A60
    void                            SetRealActiveTextureSet(const int texset);  //  @5A1A70
    const float                     GetStoredDamage() const;    //  @905800
    void                            SetStoredDamage(const float dam);   //  @5A1A90
    const float                     GetOrgHealth() const;   //  @8A6D40
    void                            SetOrgHealth(const float health);   //  @5A1AB0
    const int                       GetCallBackNode() const;    //  @5A1AD0
    void                            SetCallBackNode(const int cbnode);  //  @8CB310
    const int                       GetPhysicModel() const; //  @8CB330
    void                            SetPhysicModel(const int physmodel);    //  @5A1AE0
    const char* const               GetDynamicNodeName() const;    //  @5A1B00
    void                            ForceDynamicNodeName(const char* const nodename);  //  @8CC8E0
    const bool                      MissionObjectiveKill() const;   //  @5A1B10
    void                            SetMissionObjectiveKill(const bool obj);    //  @8CB340
    const bool                      UseAISubTypeData() const;   //  @5C90D0
    void                            SetUseAISubTypeData(const bool use);    //  @8CB360
    const int                       GetAISubType() const;   //  @5A1B20
    void                            SetAISubType(const int subtype);    //  @5A1B30
    const int                       GetMorale() const;  //  @8CB380
    void                            SetMorale(const int morale);    //  @5A1B50
    const float                     GetWaitUntilAggressive() const; //  @89AAB0
    void                            SetWaitUntilAggressive(const float waittime);   //  @5A1B70
    const eDesiredRange             GetDesiredRange() const;    //  @8CB390
    void                            SetDesiredRange(const eDesiredRange range);   //  @8CB3A0
    const float                     GetCustomDesiredRange() const;  //  @905870
    void                            SetCustomDesiredRange(const float range); //  @916E20
    const bool                      DontHunt() const;   //  @91CCC0
    void                            SetDontHunt(const bool donthunt);   //  @5A1B80
    const int                       WhenShot() const;   //  @8CB3C0
    void                            SetWhenShot(const int whenshot);  //  @5A1BA0
    const bool                      IgnoreActionBreaks() const; //  @8CB3D0
    void                            SetIgnoreActionBreaks(const bool ignore);   //  @5A1BC0
    const bool                      IgnoreWeaponDrop() const;   //  @8CB3E0
    void                            SetIgnoreWeaponDrop(const bool ignore); //  @5A1BE0
    const bool                      IgnoreMagnet() const;   //  @8CB3F0
    void                            SetIgnoreMagnet(const bool ignore); //  @5A1C00
    const eIgnoreHitAnim            GetIgnoreHitAnim() const;  //  @8CB400
    void                            SetIgnoreHitAnim(const eIgnoreHitAnim hitanimignore);  //  @5A1C20
    const int                       GotoMovement() const;   //  @8CB410
    void                            SetGotoMovement(const int movementtype);    //  @5A1C40
    const bool                      AlwaysCompleteGoto() const; //  @8CB420
    void                            SetAlwaysCompleteGoto(const bool complete); //  @5A1C60
    const bool                      Ignore5MRule() const;   //  @8CB430
    void                            SetIgnore5MRule(const bool ignore); //  @8CB440
    const eBackupWeapon             GetBackupWeapon() const;    //  @5A1C80
    void                            SetBackupWeapon(const eBackupWeapon bkweapon);  //  @8CB460
    const bool                      IsConeHead() const;   //  @5A1C90
    void                            SetIsConeHead(const bool conehead); //  @8CB480
    const bool                      IsBuddy() const;    //  @8CB4A0
    void                            SetIsBuddy(const bool buddy);   //  @5A1CA0
    const float                     GetGotopointDamageMod() const;  //  @89AD10
    void                            SetGotopointDamageMod(const float mod); //  @8D5040
    const eHealthModify             GetHealthModify() const;    //  @5A1CC0
    void                            SetHealthModify(const eHealthModify mod);   //  @5A1CD0
    const eDamageModify             GetDamageModify() const;    //  @5A1CF0
    void                            SetDamageModify(const eDamageModify mod);   //  @5A1D00
    const eHeadShotDamage           GetHeadShotDamage() const;  //  @8CB4B0
    void                            SetHeadShotDamage(const eHeadShotDamage dam);   //  @5A1D20
    const float                     GetHealthMod() const;   //  @8CB4C0
    void                            SetHealthMod(const float mod);  //  @8D5080
    const float                     GetDamageMod() const;   //  @501090
    void                            SetDamageMod(const float mod);  //  @8D50A0
    const float                     GetMarcoSqrDist() const;    //  @5A1D40
    void                            SetMarcoSqrDist(const float dist);  //  @5A1D50
    const int                       GetPointValueMod() const;   //  @8CB4D0
    void                            SetPointValueMod(const int mod);    //  @8E37F0
    const int                       GetInflictorValueMod() const;    //  @8CB4E0
    void                            SetInflictorValueMod(const int mod);    //  @5A1D80
    const eFactionType              GetFactionID() const; //  @5A1D90
    void                            SetFactionID(const eFactionType factype); //  @5A1DA0
    const bool                      UsingModifiedFaction() const;   //  @5A1DF0
    void                            SetUsingModifiedFaction(const bool use);    //  @5A1E00
    const bool                      UsingSecretFaction() const; //  @5C90E0
    void                            SetUsingSecretFaction(const bool use);  //  @51D250
    const int                       GetSecretFaction() const;   //  @5A1E50
    void                            SetSecretFaction(const int faction);    //  @5A1E60
    const int                       GetPedestrianGroup() const; //  @5A1EB0
    void                            SetPedestrianGroup(const int group);    //  @5A1EC0
    const bool                      IsLockedType() const;   //  @87FDB0
    void                            SetLockedType(const bool locked);   //  @5A1F10
    const int                       GetFallbackModel() const;   //  @8CB4F0
    void                            SetFallbackModel(const int model);  //  @5A1F60
    const int                       GetCurrentModel() const;    //  @5A1FB0
    void                            SetCurrentModel(const int model);   //  @5A1FC0
    const int                       GetPreviousModel() const;   //  @8CB500
    void                            SetPreviousModel(const int model);  //  @8CB510
    const bool                      StoreTrig() const;  //  @5A2010
    void                            SetStoreTrig(const bool store); //  @5A2020
    const bool                      StoreAlarm() const; //  @5A2070
    void                            SetStoreAlarm(const bool store);    //  @5A2080
    const bool                      StorePriority() const;  //  @5A20D0
    void                            SetStorePriority(const bool store); //  @5A20E0
    const int                       StoreTrigType() const;  //  @5A2130
    void                            SetStoreTrigType(const int store);  //  @5A2140
    const int                       GetPassengerSeat() const;   //  @5A2190
    void                            SetPassengerSeat(const int seat);   //  @5A21A0
    const bool                      WasPassenger() const;   //  @5A21F0
    void                            SetWasPassenger(const bool set);    //  @8151A0

    static void                     Register(); //  @8CB680

    static int                      GetPlaceholderModelCommand;    //  @A12710

private:
    static CharacterPlaceHolder*    Create(AllocatorIndex); //  @8CC8A0
};

extern EntityType* tCharacterPlaceHolder;

ASSERT_CLASS_SIZE(CharacterPlaceHolder, 160);