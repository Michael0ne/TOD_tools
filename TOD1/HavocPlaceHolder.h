#pragma once
#include "Placeholder.h"

class HavocPlaceHolder : public PlaceHolder
{
protected:

    enum SUB_OBJECTS
    {
    };

    enum PARTICLE_EFFECT
    {
        NO_EFFECT = 0,
        FIRE_SMALL = 1,
        FLIES_SWARM = 2,
        AIRFIELD_LIGHT_FLASH = 3
    };

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
            SUB_OBJECTS ObjectSubType : 8;
            unsigned    RealActiveTextureSet : 3;
            PARTICLE_EFFECT DefaultParticleEffect : 2;
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
    inline HavocPlaceHolder() : PlaceHolder() // NOTE: constructor inlined.
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
        m_HavocFlags_2.ObjectSubType = (SUB_OBJECTS)0;
        m_HavocFlags_2.RealActiveTextureSet = 0;
        m_HavocFlags_2.DefaultParticleEffect = NO_EFFECT;

        m_HavocFlags_3.ReplacePivotID = 0;
    }
};

extern EntityType* tHavocPlaceHolder;   //  @A3DFD0

ASSERT_CLASS_SIZE(HavocPlaceHolder, 288);