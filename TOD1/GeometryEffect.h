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
        };
    }                   m_Flags;
    int                 field_10C;
    int                 field_110;
    int                 field_114;
    int                 field_118;
    Vector4f            field_11C;
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
    inline GeometryEffect() : Model()
    {
        MESSAGE_CLASS_CREATED(GeometryEffect);

        m_Flags.BlendMode = 0;
        m_Flags.Looping = false;
        m_Flags.TriggerAtStart = false;
        m_Flags.Frozen = false;
        m_Flags.TargetBracket = true;
        m_EffectLifeTime = 2;
        field_148 = nullptr;
        m_Effect = nullptr;
        m_TotalEffects = 0;
        m_FrameBuffer[0] = nullptr;
        field_134 = 0;

        m_Flags.BlendMode = 15; //  NOTE: or -1 if signed. The whole code is strange *shrug*.
        m_Flags.Looping = true;
        m_Flags.TriggerAtStart = true;
        m_Flags.Frozen = true;
        m_Flags.TargetBracket = false;

        m_EffectFadeThreshold = 0.02f;
        m_EffectRadius = 5;
        m_ModelFlags.m_FlagBits.PlaceInHud = false;
    }
    virtual ~GeometryEffect();  //  @8E2AB0

    Effect*             AddEffect();    //  @8DDD40
    void                SetTimeAndFreeze(float* args);  //  @8DDB50
    void                RemoveEffect(Effect* effect);   //  @8DDC10

    static void         Register(); //  @8E2210

private:
    static GeometryEffect*      Create(AllocatorIndex); //  @8E2A70
};

extern EntityType* tGeometryEffect; //  @A3E000

ASSERT_CLASS_SIZE(GeometryEffect, 364);