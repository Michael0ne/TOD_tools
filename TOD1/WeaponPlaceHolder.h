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
    inline WeaponPlaceHolder() : PlaceHolder()
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

    const int       GetResourceType() const;    //  @8CD200
    void            SetResourceType(const int restype); //  @8CDA40
    const int       GetStatus() const;  //  @842C10
    void            SetStatus(const int status);    //  @67B8F0
    const bool      IsLocked() const;   //  @7607D0
    void            SetIsLocked(const bool locked); //  @68B150
    const int       GetPopSensitiveType() const;    //  @7607E0
    void            SetPopSensitiveType(const int ptype);   //  @8CD210
    const bool      IsVIP() const;  //  @842C20
    void            SetIsVIP(const bool vip);   //  @8CDA60
    const float     GetVIPTimer() const;    //  @8CD230
    void            SetVIPTimer(const float viptimer);  //  @8CDA80
    const int       GetPriority() const;    //  @67B940
    void            SetPriority(const int priority);    //  @842C30
    const int       GetWeaponSubType() const;   //  @68B1F0
    void            SetWeaponSubType(const int subtype);    //  @8CD240
    const int       GetMeleeWeaponSubType() const;  //  @842C80
    void            SetMeleeWeaponSubType(const int msubtype);  //  @5A2200
    const int       GetBulletDrop() const;  //  @8CDAE0
    void            SetBulletDrop(const int bdrop); //  @8CDAF0
    const bool      IsRespawnable() const;  //  @842C90
    void            SetIsRespawnable(const bool respawnable);   //  @8D1C70
    const float     GetRespawnTime() const; //  @68B210
    void            SetRespawnTime(const float resptime);   //  @8D1C90

    static void     Register(); //  @8D1D00

private:
    static WeaponPlaceHolder*   Create(AllocatorIndex); //  @8D2100
};

extern EntityType* tWeaponPlaceholder;  //  @A3DFEC

ASSERT_CLASS_SIZE(WeaponPlaceHolder, 284);