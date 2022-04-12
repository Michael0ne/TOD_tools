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

    static void     Register(); //  @8CD2E0

private:
    static GoodiePlaceHolder*   Create(AllocatorIndex); //  @8CD6A0
};

extern EntityType*  tGoodiePlaceholder;  //  @A3DFC8

ASSERT_CLASS_SIZE(GoodiePlaceHolder, 284);