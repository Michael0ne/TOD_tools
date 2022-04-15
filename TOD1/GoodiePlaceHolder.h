#pragma once
#include "Placeholder.h"

class GoodiePlaceHolder : public PlaceHolder
{
protected:
    union
    {
        unsigned    ResourceType : 4;
        unsigned    Status : 2;
        unsigned    Locked : 1;
        unsigned    PopSensitiveType : 2;
        unsigned    VIP : 1;
    }               m_Flags_13;
    float           m_VIPTimer;
    union
    {
        unsigned    Priority : 3;
        unsigned    GoodieSubType : 5;
        unsigned    Respawnable : 1;
    }               m_Flags_14;
    float           m_RespawnTime;

public:
    inline GoodiePlaceHolder() : PlaceHolder() //  NOTE: ctor at 8CD290, but compiler inlined it.
    {
        MESSAGE_CLASS_CREATED(GoodiePlaceHolder);

        m_Flags_13.ResourceType = 5;
        m_Flags_13.Status = 0;
        m_Flags_13.Locked = false;
        m_Flags_13.PopSensitiveType = 0;
        m_Flags_13.VIP = false;

        m_Flags_14.Priority = 0;
        m_Flags_14.GoodieSubType = 0;
        m_Flags_14.Respawnable = 0;

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
    void            SetPopSensitiveType(const int popsenstype); //  @8CD210
    const bool      IsVIP() const;  //  @842C20
    void            SetIsVIP(const bool vip);   //  @8CDA60
    const float     GetVIPTimer() const;    //  @8CD230
    void            SetVIPTimer(const float timer); //  @8CDA80
    const int       GetPriority() const;    //  @67B940
    void            SetPriority(const int priority);    //  @68B1A0
    const int       GetGoodieSubType() const;   //  @68B1F0
    void            SetGoodieSubType(const int subtype);    //  @8CD240
    const bool      IsRespawnable() const;  //  @68B200
    void            SetIsRespawnable(const bool resp);  //  @8CD270
    const float     GetRespawnTime() const; //  @68B210
    void            SetRespawnTime(const float resptime);   //  @8D1C90

    static void     Register(); //  @8CD2E0

private:
    static GoodiePlaceHolder*   Create(AllocatorIndex); //  @8CD6A0
};

extern EntityType*  tGoodiePlaceholder;  //  @A3DFC8

ASSERT_CLASS_SIZE(GoodiePlaceHolder, 284);