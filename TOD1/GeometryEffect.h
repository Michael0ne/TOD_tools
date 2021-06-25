#pragma once
#include "Model.h"
#include "Camera.h"
#include "BuiltinType.h"

class GeometryEffect;

struct Effect
{
    Vector4f            m_Position;
    Orientation         m_Orient;
    GeometryEffect     *m_Parent;
    float               m_Lifetime;
    Effect             *m_Sibling;
    Effect             *m_ParentEffect;
    int                 field_30;
    int                 field_34;
    int                 field_38;
    int                 field_3C;
    int                 field_40;
    int                 field_44;
    int                 field_48;
    int                 field_4C;
    int                 field_50;
    int                 field_54;
    int                 field_58;
    int                 field_5C;
    int                 field_60;
    int                 field_64;
    int                 field_68;
    int                 m_StartTime;
    int                 field_70;
    float               field_74;

    inline Effect()
    {
        MESSAGE_CLASS_CREATED(Effect);

        m_Orient = BuiltinType::Orient;
        m_Lifetime = 0;
        field_70 = (field_70 & 0xFFFFFFF0) | 0x10;
        field_74 = 1;
        m_Parent = nullptr;
        m_Sibling = nullptr;
        m_Parent = nullptr;
        m_StartTime = 0;
    }
};

class GeometryEffect : public Model
{
protected:
    int                 m_TotalEffects;
    float               m_EffectLifeTime;
    union
    {
        struct
        {
            unsigned BlendMode : 4;
            unsigned Looping : 1;
            unsigned TriggerAtStart : 1;
            unsigned Frozen : 1;
            unsigned TargetBracket : 1;
            unsigned _8 : 1;
            unsigned _9 : 1;
            unsigned _10 : 1;
            unsigned _11 : 1;
            unsigned _12 : 1;
            unsigned _13 : 1;
            unsigned _14 : 1;
            unsigned _15 : 1;
            unsigned _16 : 1;
            unsigned _17 : 1;
            unsigned _18 : 1;
            unsigned _19 : 1;
            unsigned _20 : 1;
            unsigned _21 : 1;
            unsigned _22 : 1;
            unsigned _23 : 1;
            unsigned _24 : 1;
            unsigned _25 : 1;
            unsigned _26 : 1;
            unsigned _27 : 1;
            unsigned _28 : 1;
            unsigned _29 : 1;
            unsigned _30 : 1;
        }				m_FlagBits;
        unsigned int	m_Flags;
    }					m_GeometryEffectFlags;
    int                 field_10C;
    int                 field_110;
    int                 field_114;
    int                 field_118;
    int                 field_11C;
    int                 field_120;
    int                 field_124;
    int                 field_128;
    int                 field_12C;
    int                 field_130;
    int                 field_134;
    std::vector<int>    m_List_1;
    Effect             *field_148;
    Effect             *m_Effect;
    float               m_EffectRadius;
    CameraMatrix       *m_ActiveCameraMatrix;
    Vector4f            m_CameraPos;
    float               m_EffectFadeThreshold;

public:
    GeometryEffect();
    virtual ~GeometryEffect();

    Effect*             AddEffect();    //  @8DDD40
    void                SetTimeAndFreeze(float* args);  //  @8DDB50
};

ASSERT_CLASS_SIZE(GeometryEffect, 364);