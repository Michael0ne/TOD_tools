#pragma once
#include "Model.h"
#include "Camera.h"
#include "BuiltinType.h"

class GeometryEffect;
class GEKeyFrame;

enum class eEffectBlendMode
{
    NORMAL = 0,
    ADD = 1
};

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
    friend class ParticleSystem;
protected:
    int                 m_TotalEffects;
    float               m_EffectLifeTime;
    union
    {
        struct
        {
            eEffectBlendMode BlendMode : 4;
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
    std::vector<GEKeyFrame*>    m_KeyFramesList;
    Effect             *m_ActiveEffect;
    Effect             *m_Effect;
    float               m_EffectRadius;
    CameraMatrix       *m_ActiveCameraMatrix;
    Vector4f            m_CameraPos;
    float               m_EffectFadeThreshold;

public:
    inline GeometryEffect() : Model()
    {
        MESSAGE_CLASS_CREATED(GeometryEffect);

        m_Flags.BlendMode = eEffectBlendMode::NORMAL;
        m_Flags.Looping = false;
        m_Flags.TriggerAtStart = false;
        m_Flags.Frozen = false;
        m_Flags.TargetBracket = true;
        m_EffectLifeTime = 2;
        m_ActiveEffect = nullptr;
        m_Effect = nullptr;
        m_TotalEffects = 0;
        m_FrameBuffer[0] = nullptr;
        field_134 = 0;

        m_Flags.Looping = true;
        m_Flags.TriggerAtStart = true;
        m_Flags.Frozen = true;
        m_Flags.TargetBracket = false;

        m_EffectFadeThreshold = 0.02f;
        m_EffectRadius = 5;
        m_ModelFlags.m_FlagBits.PlaceInHud = false;
    }
    virtual ~GeometryEffect();  //  @8E2AB0

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

private:
    Effect*             AddEffect();    //  @8DDD40
    void                RemoveEffect(Effect* effect);   //  @8DDC10
    void                AddKeyFrame(GEKeyFrame* keyframe);  //  @8E2AE0
    void                KillAllEffects_Impl(const Effect* effect);  //  @8DDC70
    void                MakeNewActiveEffect();  //  @8E27D0

public:
    const float         GetEffectFadeThreshold() const; //  @907E30
    void                SetEffectFadeThreshold(const float th); //  @8DDD00
    const float         GetEffectRadius() const;    //  @8DDAE0
    void                SetEffectRadius(const float radius);    //  @8DDD20
    const bool          GetGesep0() const;   //  @42F4F0
    void                SetGesep0(const int);    //  @883EC0
    const float         GetEffectLifeTime() const;  //  @8C9200
    void                SetEffectLifeTime(const float lt);  //  @698CE0
    const eEffectBlendMode    GetBlendMode() const;   //  @8DDA70
    void                SetBlendMode(const eEffectBlendMode blmode); //  @8DDA80
    const bool          IsLooping() const;  //  @8DDAD0
    void                SetIsLooping(const bool looping);   //  @8DDAB0
    const bool          TriggerAtStart() const; //  @8DDB10
    void                SetTriggerAtStart(const bool trig); //  @8DDAF0
    const bool          IsTargetBracket() const;    //  @8DDB20
    void                SetIsTargetBracket(const bool target);  //  @6962F0

    void                TriggerEffect(int* args);   //  @8E2B70
    void                TriggerAtPos(int* args);    //  @8E2860
    void                TriggerEntity(int* args);   //  @8E2930
    void                TriggerEntityPos(int* args);    //  @8E29B0
    void                KillAllEffects(int* args);  //  @8E2760
    void                PauseAll(int* args);    //  @8DDB30
    void                SetTimeAndFreeze(float* args);  //  @8DDB50
    void                Start(int* args);   //  @8E21C0

    static void         Register(); //  @8E2210

private:
    static GeometryEffect*      Create(AllocatorIndex); //  @8E2A70
};

extern EntityType* tGeometryEffect; //  @A3E000

ASSERT_CLASS_SIZE(GeometryEffect, 364);