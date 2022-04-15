#pragma once
#include "Placeholder.h"

enum class eObjectSubType
{
    UNKNOWN_0 = 0
};

enum class eParticleEffect
{
    NO_EFFECT = 0,
    FIRE_SMALL = 1,
    FLIES_SWARM = 2,
    AIRFIELD_LIGHT_FLASH = 3
};

class HavocPlaceHolder : public PlaceHolder
{
protected:
    union
    {
        struct
        {
            unsigned    ResourceType : 4;
            unsigned    Status : 3;
            unsigned    Locked : 1;
            unsigned    PopSensitiveType : 3;
            unsigned    IsVIP : 1;
        };
    }                   m_HavocFlags_1;
    float               m_VIPTimer;
    union
    {
        struct
        {
            unsigned    Priority : 3;
            eObjectSubType ObjectSubType : 8;
            unsigned    RealActiveTextureSet : 3;
            eParticleEffect DefaultParticleEffect : 2;
        };
    }                   m_HavocFlags_2;
    union
    {
        struct
        {
            unsigned    ReplacePivotID : 24;
        };
    }                   m_HavocFlags_3;
    union
    {
        struct
        {
            unsigned    ComID : 24;
            unsigned    Broken : 1;
        };
    }                   m_HavocFlags_4;

public:
    inline HavocPlaceHolder() : PlaceHolder()
    {
        MESSAGE_CLASS_CREATED(HavocPlaceHolder);

        m_VIPTimer = 0.0f;

        m_HavocFlags_1.ResourceType = 2;
        m_HavocFlags_1.Status = 0;
        m_HavocFlags_1.Locked = false;
        m_HavocFlags_1.PopSensitiveType = 4;
        m_HavocFlags_1.IsVIP = true;

        m_HavocFlags_4.ComID = 0;
        m_HavocFlags_4.Broken = false;

        m_HavocFlags_2.Priority = 0;
        m_HavocFlags_2.ObjectSubType = eObjectSubType::UNKNOWN_0;
        m_HavocFlags_2.RealActiveTextureSet = 0;
        m_HavocFlags_2.DefaultParticleEffect = eParticleEffect::NO_EFFECT;

        m_HavocFlags_3.ReplacePivotID = 0;
    }

    const int       GetResourceType() const;    //  @8CD200
    void            SetResourceType(const int restype); //  @8CDA40
    const int       GetStatus() const;  //  @842C10
    void            SetStatus(const int status);    //  @67B8F0
    const bool      IsLocked() const;   //  @7607D0
    void            SetIsLocked(const bool locked); //  @68B150
    const int       GetPopSensitiveType() const;    //  @7607E0
    void            SetPopSensitiveType(const int popsenstype); //  @8CD210
    const bool      IsVIP() const;  //  @842C20
    void            SetIsVIP(const bool vip);   //  @8CDA60
    const float     GetVIPTimer() const;    //  @8CD230
    void            SetVIPTimer(const float timer); //  @8CDA80
    const int       GetPriority() const;    //  @67B940
    void            SetPriority(const int priority);    //  @842C30
    const eObjectSubType    GetObjectSubType() const;   //  @67B950
    void            SetObjectSubType(const eObjectSubType subtype);    //  @8CDAB0
    const int       GetRealActiveTextureSet() const;    //  @8CDAE0
    void            SetRealActiveTextureSet(const int texset);  //  @8CDAF0
    const eParticleEffect   GetDefaultParticleEffect() const;   //  @676B40
    void            SetDefaultParticleEffect(const eParticleEffect effect); //  67B960
    const int       GetReplacePivotID() const;  //  @67B980
    void            SetReplacePivotID(const int pivotid);   //  @67B990
    const int       GetComID() const;   //  @67B9E0
    void            SetComID(const int comid);  //  @67B9F0
    const bool      IsBroken() const;   //  @668B60
    void            SetIsBroken(const bool broken); //  @67BA40

    static void     Register(); //  @8CDB90

private:
    static HavocPlaceHolder*    Create(AllocatorIndex); //  @8CDFD0
};

extern EntityType* tHavocPlaceHolder;   //  @A3DFD0

ASSERT_CLASS_SIZE(HavocPlaceHolder, 288);