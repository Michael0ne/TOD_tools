#pragma once
#include "Placeholder.h"

class WeaponPlaceHolder : public PlaceHolder
{
protected:
    union
    {
        struct
        {
            unsigned    ResourceType : 4;
            unsigned    Status : 2;
            unsigned    Locked : 1;
            unsigned    PopSensitiveType : 2;
            unsigned    IsVIP : 1;
        };
    }           m_Flags_15;
    float       m_VIPTimer;
    union
    {
        struct
        {
            unsigned    Priority : 3;
            unsigned    WeaponSubType : 5;
            unsigned    MeleeWeaponSubType : 3;
            unsigned    BulletDropType : 3;
            unsigned    Respawnable : 1;
        };
    }           m_Flags_16;
    float       m_RespawnTime;

public:
    WeaponPlaceHolder() : PlaceHolder()
    {
        MESSAGE_CLASS_CREATED(WeaponPlaceHolder);

        m_Flags_15.ResourceType = 3;
        m_Flags_15.Status = 0;
        m_Flags_15.Locked = false;
        m_Flags_15.PopSensitiveType = 0;
        m_Flags_15.IsVIP = false;

        m_Flags_16.Priority = 0;
        m_Flags_16.WeaponSubType = 0;
        m_Flags_16.MeleeWeaponSubType = 0;
        m_Flags_16.BulletDropType = 0;
        m_Flags_16.Respawnable = true;

        m_VIPTimer = 0.f;
        m_RespawnTime = 0.f;
    }

    static void     Register(); //  @8D1D00

private:
    static WeaponPlaceHolder*   Create(AllocatorIndex); //  @8D2100
};

extern EntityType* tWeaponPlaceholder;  //  @A3DFEC

ASSERT_CLASS_SIZE(WeaponPlaceHolder, 284);