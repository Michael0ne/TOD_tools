#pragma once
#include "AnimSlot.h"

//  TODO: figure out meanings for each.
enum class ePassiveActionAnimType
{
    UNKNOWN = -1
};

enum class eAimActionAnimType
{
    UNKNOWN = -1
};

enum class eShootActionAnimType
{
    UNKNOWN = -1
};

enum class eDefaultAnimDictType
{
    UNKNOWN = -1
};

struct ActionAnimInfo
{
    union
    {
        struct
        {
            ePassiveActionAnimType       PassiveActionAnimType : 2;
            eAimActionAnimType           AimActionAnimType : 2;
            eShootActionAnimType         ShootActionAnimType : 2;
            eDefaultAnimDictType              DefaultAnimDict : 2;
            eDefaultAnimDictType              ShootAndAimDict : 2;

            unsigned                        AimAllowed : 1;
            unsigned                        AimHighLow : 1;
        };
    }               m_Flags;
};

struct ActionAnim
{
    char* m_WeaponClass;
    Entity* m_PassiveEntity;
    Entity* m_AimLowEntity;
    Entity* m_AimHighEntity;
    Entity* m_ShootLowEntity;
    Entity* m_ShootHighEntity;

    inline ActionAnim()
    {
        m_PassiveEntity = nullptr;
        m_AimLowEntity = nullptr;
        m_AimHighEntity = nullptr;
        m_ShootLowEntity = nullptr;
        m_ShootHighEntity = nullptr;
        m_WeaponClass = nullptr;
    }
};

class MotionAnimSlot : public AnimSlot
{
protected:
    char                        m_CurrAnimInfoListIndex;
    char                        m_CurrAnimListIndex;
    std::vector<ActionAnimInfo> m_ActionAnimInfoList;
    std::vector<ActionAnim>     m_ActionAnimList;

    union
    {
        struct
        {
            //  Melee
            ePassiveActionAnimType  MeleePassiveActionAnimType : 2;
            eAimActionAnimType      MeleeAimActionAnimType : 2;
            eShootActionAnimType    MeleeShootActionAnimType : 2;
            eDefaultAnimDictType    MeleeDefaultAnimDict : 2;
            eDefaultAnimDictType    MeleeShootAndAimDict : 2;

            unsigned        MeleeAimAllowed : 1;
            unsigned        MeleeAimHighLow : 1;

            //  Gun
            ePassiveActionAnimType  GunPassiveActionAnimType : 2;
            eAimActionAnimType      GunAimActionAnimType : 2;
            eShootActionAnimType    GunShootActionAnimType : 2;
            eDefaultAnimDictType    GunDefaultAnimDict : 2;
            eDefaultAnimDictType    GunShootAndAimDict : 2;

            unsigned        GunAimAllowed : 1;
            unsigned        GunAimHighLow : 1;

            //  AutoLong Action
            ePassiveActionAnimType  AutoLongPassiveActionAnimType : 2;
            eAimActionAnimType      AutoLongAimActionAnimType : 2;
            eShootActionAnimType    AutoLongShootActionAnimType : 2;
            eDefaultAnimDictType    AutoLongDefaultAnimDict : 2;
        };
    }                   m_Flags;
    union
    {
        struct
        {
            //  AutoLong Action ct-ed
            eDefaultAnimDictType    AutoLongShootAndAimDict : 2;

            unsigned        AutoLongAimAllowed : 1;
            unsigned        AutoLongAimHighLow : 1;

            //  Dual Action
            ePassiveActionAnimType  DualActionPassiveActionAnimType : 2;
            eAimActionAnimType      DualActionAimActionAnimType : 2;
            eShootActionAnimType    DualActionShootActionAnimType : 2;
            eDefaultAnimDictType    DualActionDefaultAnimDict : 2;
            eDefaultAnimDictType    DualActionShootAndAimDict : 2;

            unsigned        DualActionAimAllowed : 1;
            unsigned        DualActionAimHighLow : 1;

            //  Throwable
            ePassiveActionAnimType  ThrowablePassiveActionAnimType : 2;
            eAimActionAnimType      ThrowableAimActionAnimType : 2;
            eShootActionAnimType    ThrowableShootActionAnimType : 2;
            eDefaultAnimDictType    ThrowableDefaultAnimDict : 2;
            eDefaultAnimDictType    ThrowableShootAndAimDict : 2;

            unsigned        ThrowableAimAllowed : 1;
            unsigned        ThrowableAimHighLow : 1;

            //  Shields
            ePassiveActionAnimType  ShieldPassiveActionAnimType : 2;
            eAimActionAnimType      ShieldAimActionAnimType : 2;
        };
    }                   m_Flags_1;
    union
    {
        struct
        {
            //  Shields ct-ed
            eShootActionAnimType    ShieldShootActionAnimType : 2;
            eDefaultAnimDictType    ShieldDefaultAnimDict : 2;
            eDefaultAnimDictType    ShieldShootAndAimDict : 2;

            unsigned        ShieldAimAllowed : 1;
            unsigned        ShieldAimHighLow : 1;

            //  Guitar
            ePassiveActionAnimType  GuitarPassiveActionAnimType : 2;
            eAimActionAnimType      GuitarAimActionAnimType : 2;
            eShootActionAnimType    GuitarShootActionAnimType : 2;
            eDefaultAnimDictType    GuitarDefaultAnimDict : 2;
            eDefaultAnimDictType    GuitarShootAndAimDict : 2;

            unsigned        GuitarAimAllowed : 1;
            unsigned        GuitarAimHighLow : 1;
        };
    }                   m_Flags_2;

public:
    inline MotionAnimSlot() : AnimSlot()
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

    void                    ActionAnimListPushBackElement(int* args);    //  @918540
    void                    ActionAnimInfoListPushBackElement(int* args);   //  @918580

    const bool              GetDummy0() const;  //  @42F4F0
    void                    SetDummy0(const bool);  //  @883EC0
    const ePassiveActionAnimType    GetMeleePassiveAnimType() const;    //  @630280
    void                    SetMeleePassiveAnimType(const ePassiveActionAnimType animtype); //  @917DF0
    const eAimActionAnimType    GetMeleeAimAnimType() const;    //  @630290
    void                    SetMeleeAimAnimType(const eAimActionAnimType animtype); //  @917E10
    const eShootActionAnimType  GetMeleeShootAnimType() const;  //  @6302A0
    void                    SetMeleeShootAnimType(const eShootActionAnimType animtype); //  @917E40
    const eDefaultAnimDictType  GetMeleeDefaultAnimDict() const;    //  @6302B0
    void                    SetMeleeDefaultAnimDict(const eDefaultAnimDictType animtype);   //  @917E60
    const eDefaultAnimDictType  GetMeleeShootAndAimDict() const;    //  @6302C0
    void                    SetMeleeShootAndAimDict(const eDefaultAnimDictType animtype);   //  @917E80
    const bool              MeleeAimAllowed() const;    //  @6302D0
    void                    SetMeleeAimAllowed(const bool allowed); //  @917EA0
    const bool              MeleeAimHighLow() const;    //  @6302E0
    void                    SetMeleeAimHighLow(const bool allowed); //  @917EC0

    const bool              GetDummy1() const;  //  @42F4F0
    void                    SetDummy1(const bool);  //  @883EC0
    const ePassiveActionAnimType    GetGunPassiveAnimType() const;  //  @6302F0
    void                    SetGunPassiveAnimType(const ePassiveActionAnimType animtype);   //  @917EE0
    const eAimActionAnimType    GetGunAimAnimType() const;  //  @630300
    void                    SetGunAimAnimType(const eAimActionAnimType animtype);   //  @917F00
    const eShootActionAnimType  GetGunShootAnimType() const;    //  @630310
    void                    SetGunShootAnimType(const eShootActionAnimType animtype);   //  @917F20
    const eDefaultAnimDictType  GetGunDefaultAnimDict() const;  //  @630320
    void                    SetGunDefaultAnimDict(const eDefaultAnimDictType animtype); //  @917F40
    const eDefaultAnimDictType  GetGunShootAndAimDict() const;  //  @630330
    void                    SetGunShootAndAimDict(const eDefaultAnimDictType animtype); //  @917F60
    const bool              GunAimAllowed() const;  //  @630340
    void                    SetGunAimAllowed(const bool allowed);   //  @917F80
    const bool              GunAimHighLow() const;  //  @630350
    void                    SetGunAimHighLow(const bool allowed);   //  @917FA0

    const int               GetCurrAnimInfoListIndex() const;   //  @918430
    void                    SetCurrAnimInfoListIndex(const char index);  //  @5011D0
    const int               GetCurrAnimListIndex() const;   //  @918440
    void                    SetCurrAnimListIndex(const char index); //  @50B4D0
    const ePassiveActionAnimType    GetActionAnimInfoListPassiveAnimType() const;   //  @631540
    void                    SetActionAnimInfoListPassiveAnimType(const ePassiveActionAnimType animtype);    //  @631560
    const eAimActionAnimType    GetActionAnimInfoListAimAnimType() const;   //  @631590
    void                    SetActionAnimInfoListAimAnimType(const eAimActionAnimType animtype);    //  @6315C0
    const eShootActionAnimType  GetActionAnimInfoListShootAnimType() const; //  @6315F0
    void                    SetActionAnimInfoListShootAnimType(const eShootActionAnimType animtype);    //  @631620
    const eDefaultAnimDictType  GetActionAnimInfoListDefaultAnimDict() const;   //  @631650
    void                    SetActionAnimInfoListDefaultAnimDict(const eDefaultAnimDictType animtype);  //  @631680
    const eShootActionAnimType  GetActionAnimInfoListShootAndAimDict() const;   //  @6316B0
    void                    SetActionAnimInfoListShootAndAimDict(const eShootActionAnimType animtype);  //  @6316E0
    const bool              ActionAnimInfoListAimAllowed() const;    //  @50B3D0
    void                    SetActionAnimInfoListAimAllowed(const bool allowed); //  @631710
    const bool              ActionAnimInfoListAimHighLow() const;   //  @50B3F0
    void                    SetActionAnimInfoListAimHighLow(const bool allowed);    //  @631740

private:

public:
    static void             Register(); //  @918700
    static MotionAnimSlot*  Create(AllocatorIndex); //  @9186C0
};

extern EntityType* tMotionAnimSlot; //  @A3E134

ASSERT_CLASS_SIZE(MotionAnimSlot, 188);