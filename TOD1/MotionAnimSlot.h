#pragma once
#include "AnimSlot.h"

class MotionAnimSlot : public AnimSlot
{
protected:

    //  NOTE: original name taken from EXE.
    //  TODO: figure out meanings for each.
    enum PASSIVE_ACTION_ANIM_TYPES
    {
    };

    enum AIM_ACTION_ANIM_TYPES
    {
    };

    enum SHOOT_ACTION_ANIM_TYPES
    {
    };

    enum DEFAULT_ANIM_DICTS
    {
    };

    struct ActionAnimInfo
    {
        union
        {
            struct
            {
                PASSIVE_ACTION_ANIM_TYPES       PassiveActionAnimType : 2;
                AIM_ACTION_ANIM_TYPES           AimActionAnimType : 2;
                SHOOT_ACTION_ANIM_TYPES         ShootActionAnimType : 2;
                DEFAULT_ANIM_DICTS              DefaultAnimDict : 2;
                DEFAULT_ANIM_DICTS              ShootAndAimDict : 2;

                unsigned                        AimAllowed : 1;
                unsigned                        AimHighLow : 1;
            };
        }               m_Flags;
    };

    struct ActionAnim
    {
        char           *m_WeaponClass;
        Entity         *m_PassiveEntity;
        Entity         *m_AimLowEntity;
        Entity         *m_AimHighEntity;
        Entity         *m_ShootLowEntity;
        Entity         *m_ShootHighEntity;
    };

protected:
    char                m_CurrAnimInfoListIndex;
    char                m_CurrAnimListIndex;
    std::vector<ActionAnimInfo>    m_ActionAnimInfoList;
    std::vector<ActionAnim>    m_ActionAnimList;
    union
    {
        struct
        {
            //  Melee
            PASSIVE_ACTION_ANIM_TYPES       MeleePassiveActionAnimType : 2;
            AIM_ACTION_ANIM_TYPES           MeleeAimActionAnimType : 2;
            SHOOT_ACTION_ANIM_TYPES         MeleeShootActionAnimType : 2;
            DEFAULT_ANIM_DICTS              MeleeDefaultAnimDict : 2;
            DEFAULT_ANIM_DICTS              MeleeShootAndAimDict : 2;

            unsigned                        MeleeAimAllowed : 1;
            unsigned                        MeleeAimHighLow : 1;

            //  Gun
            PASSIVE_ACTION_ANIM_TYPES       GunPassiveActionAnimType : 2;
            AIM_ACTION_ANIM_TYPES           GunAimActionAnimType : 2;
            SHOOT_ACTION_ANIM_TYPES         GunShootActionAnimType : 2;
            DEFAULT_ANIM_DICTS              GunDefaultAnimDict : 2;
            DEFAULT_ANIM_DICTS              GunShootAndAimDict : 2;

            unsigned                        GunAimAllowed : 1;
            unsigned                        GunAimHighLow : 1;

            //  AutoLong Action
            PASSIVE_ACTION_ANIM_TYPES       AutoLongPassiveActionAnimType : 2;
            AIM_ACTION_ANIM_TYPES           AutoLongAimActionAnimType : 2;
            SHOOT_ACTION_ANIM_TYPES         AutoLongShootActionAnimType : 2;
            DEFAULT_ANIM_DICTS              AutoLongDefaultAnimDict : 2;
        };
    }                   m_Flags;
    union
    {
        struct
        {
            //  AutoLong Action ct-ed
            DEFAULT_ANIM_DICTS              AutoLongShootAndAimDict : 2;

            unsigned                        AutoLongAimAllowed : 1;
            unsigned                        AutoLongAimHighLow : 1;

            //  Dual Action
            PASSIVE_ACTION_ANIM_TYPES       DualActionPassiveActionAnimType : 2;
            AIM_ACTION_ANIM_TYPES           DualActionAimActionAnimType : 2;
            SHOOT_ACTION_ANIM_TYPES         DualActionShootActionAnimType : 2;
            DEFAULT_ANIM_DICTS              DualActionDefaultAnimDict : 2;
            DEFAULT_ANIM_DICTS              DualActionShootAndAimDict : 2;

            unsigned                        DualActionAimAllowed : 1;
            unsigned                        DualActionAimHighLow : 1;

            //  Throwable
            PASSIVE_ACTION_ANIM_TYPES       ThrowablePassiveActionAnimType : 2;
            AIM_ACTION_ANIM_TYPES           ThrowableAimActionAnimType : 2;
            SHOOT_ACTION_ANIM_TYPES         ThrowableShootActionAnimType : 2;
            DEFAULT_ANIM_DICTS              ThrowableDefaultAnimDict : 2;
            DEFAULT_ANIM_DICTS              ThrowableShootAndAimDict : 2;

            unsigned                        ThrowableAimAllowed : 1;
            unsigned                        ThrowableAimHighLow : 1;

            //  Shields
            PASSIVE_ACTION_ANIM_TYPES       ShieldPassiveActionAnimType : 2;
            AIM_ACTION_ANIM_TYPES           ShieldAimActionAnimType : 2;
        };
    }                   m_Flags_1;
    union
    {
        struct
        {
            //  Shields ct-ed
            SHOOT_ACTION_ANIM_TYPES         ShieldShootActionAnimType : 2;
            DEFAULT_ANIM_DICTS              ShieldDefaultAnimDict : 2;
            DEFAULT_ANIM_DICTS              ShieldShootAndAimDict : 2;

            unsigned                        ShieldAimAllowed : 1;
            unsigned                        ShieldAimHighLow : 1;

            //  Guitar
            PASSIVE_ACTION_ANIM_TYPES       GuitarPassiveActionAnimType : 2;
            AIM_ACTION_ANIM_TYPES           GuitarAimActionAnimType : 2;
            SHOOT_ACTION_ANIM_TYPES         GuitarShootActionAnimType : 2;
            DEFAULT_ANIM_DICTS              GuitarDefaultAnimDict : 2;
            DEFAULT_ANIM_DICTS              GuitarShootAndAimDict : 2;

            unsigned                        GuitarAimAllowed : 1;
            unsigned                        GuitarAimHighLow : 1;
        };
    }                   m_Flags_2;

public:
    inline MotionAnimSlot() : AnimSlot()   //  NOTE: constructor inlined.
    {
        MESSAGE_CLASS_CREATED(MotionAnimSlot);

        m_CurrAnimInfoListIndex = m_CurrAnimListIndex = -1;
    }

    virtual ~MotionAnimSlot();  //  @918600

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    static void         Register(); //  @918700
    static MotionAnimSlot*  Create(AllocatorIndex); //  @9186C0
};

extern EntityType* tMotionAnimSlot; //  @A3E134

ASSERT_CLASS_SIZE(MotionAnimSlot, 188);