#pragma once
#include "Node.h"

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
    enum
    {
        UseWeapon = 0,
        Custom = 1
    }               m_DesiredRange;
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

    static void                     Register(); //  @8CB680

    static int                      GetPlaceholderModelCommand;    //  @A12710

private:
    static CharacterPlaceHolder*    Create(AllocatorIndex); //  @8CC8A0
};

extern EntityType* tCharacterPlaceHolder;

ASSERT_CLASS_SIZE(CharacterPlaceHolder, 160);