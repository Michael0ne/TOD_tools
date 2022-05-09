#pragma once
#include "Node.h"
#include "Particle.h"

class FrameBuffer;
class MeshBuffer;

struct ParticleSystemInfo
{
    char            m_FadeInTime;
    char            m_FadeOutTime;
    char            m_GlobalWindFactor;
    char            m_SizeVariation;
    char            m_LifeTimeVariation;
    char            field_5;
    char            field_6;
    char            field_7;
    char            m_Opacity;
    char            m_OpacityVariation;
    char            m_TumbleSpeed;
    char            m_TumbleAmp;
    char            m_Turbulence;
    char            field_D;
    char            field_E;
    char            field_F;
    char            m_BirthRateVariation;
    char            field_11;
    char            field_12;
    char            field_13;
    char            m_BirthDistance;
    char            field_15;
    char            m_EmitterLifeTime;
    char            field_17;
    char            m_SpeedVariation;
    char            field_19;
    char            field_1A;
    char            field_1B;
    char            m_DirVariation;
    char            m_DirScaleX;
    char            m_DirScaleY;
    char            m_DirScaleZ;
    char            m_RotationVariation;
    char            field_21;
    char            field_22;
    char            field_23;
    char            m_BirthDistFade;
    char            field_25;
    char            field_26;
    char            field_27;
    char            m_InheritEmitterSpeed;
    char            m_EmitterDelay;
    char            m_EmitterFadeThreshold;
    char            m_EmitterRadius;
    float           m_EmitterLength;
    int             m_Color;

    union
    {
        struct
        {
            unsigned    _0 : 1;
            unsigned    _1 : 1;
            unsigned    _2 : 1;
            unsigned    _3 : 1;
            unsigned    GrowthFade : 1;
            unsigned    LinkNext : 1;
            unsigned    DirScaleZRing : 1;
            unsigned    InitialTrigger : 1;
            unsigned    Is3D : 1;
            unsigned    Has2DEmitter : 1;
            unsigned    UsesVirtualHud : 1;
            unsigned    ExplicitBirthOnly : 1;
            unsigned    _12 : 1;
            unsigned    _13 : 1;
            unsigned    _14 : 1;
            unsigned    _15 : 1;
            unsigned    _16 : 1;
            unsigned    UseRealTime : 1;
            unsigned    _18 : 1;
            unsigned    _19 : 1;
            unsigned    _20 : 1;
            unsigned    _21 : 1;
            unsigned    _22 : 1;
            unsigned    _23 : 1;
            unsigned    _24 : 1;
            unsigned    _25 : 1;
            unsigned    _26 : 1;
            unsigned    _27 : 1;
            unsigned    _28 : 1;
            unsigned    _29 : 1;
            unsigned    _30 : 1;
            unsigned    _31 : 1;
        };
    }               m_Flags;

    int             m_PriorityLayer;
    short           m_MaxParticles;
    short           field_3E;
    int             field_40;
    ParticleA      *m_Particles;
    Particle       *m_ParticlesArray;
    Texture        *m_MeshTexture;
    MeshBuffer     *m_Mesh;
    int             field_54;
    int             field_58;
    short           field_5C;
    short           field_5E;

    ParticleSystemInfo(); // @425470
    inline ~ParticleSystemInfo()
    {
        MESSAGE_CLASS_DESTROYED(ParticleSystemInfo);
        KillAll();
    }

    void            CalculateEmitterLength();   //  @4250C0
    ParticleA*      CreateNewParticle();    //  @425620
    void            KillAll();  //  @425050
    void            RemoveParticle(ParticleA* particleRef); //  @425000
};

class ParticleSystem : public Node
{
protected:
    AssetLoader     m_Texture;
    AssetLoader     m_Model;
    ParticleSystemInfo  m_Properties;
    FrameBuffer    *m_FrameBuffer;
    std::vector<int>m_List_1;
    std::vector<int>m_List_2;
    int             field_E4;
    int             field_E8;
    int             field_EC;
    int             m_IndexInSceneList;

public:
    ParticleSystem(); // @8EB650
    virtual ~ParticleSystem();  //  @8E9FD0
    virtual void    Instantiate();  //  @8E9E70
    virtual void    Render();   //  @8E9BB0

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    const float     GetEmitterFadeThreshold() const;    //  @8E6A40
    void            SetEmitterFadeThreshold(const float threshold); //  @8E6DB0
    const float     GetEmitterRadius() const;   //  @8E6A60
    void            SetEmitterRadius(const float radius);   //  @8E6E50
    const bool      UseRealTime() const;    //  @8E6B00
    void            SetUseRealTime(const bool enabled); //  @8E6AE0
    const bool      GetSep6() const;    //  @42F4F0
    void            SetSep6(const bool);    //  @883EC0
    const int       GetMaxParticles() const;    //  @8E6B50
    void            SetMaxParticles(int maxparticles);    //  @8E6B30
    const float     GetEmitterLifeTime() const; //  @793A80
    void            SetEmitterLifeTime(const float emitterlifetime);    //  @826180

    const bool      GetSep3() const;    //  @42F4F0
    void            SetSep3(const bool);    //  @883EC0
    const float     GetParticleGravity() const; //  @8E58D0
    void            SetParticleGravity(const float gravity);    //  @8E5870

    void            TriggerWithPosition(int* args); //  @8E9750
    void            TriggerWithEntity(int* args);   //  @8E98B0
    void            TriggerWithEntityPosition(int* args);   //  @8E9A10
    void            TriggerDefault(int* args);  //  @8EB5C0
    void            KillEmitters(int* args);    //  @8EB540
    void            StopEmitters(int* args);    //  @8EB580
    void            SpawnParticles(int* args);  //  @8E6C10
    void            KillDefault(int* args); //  @8EB500
    void            KillAll(int* args); //  @8E6B60

    static void     Register(); //  @8EA090
    static ParticleSystem*  Create(AllocatorIndex);  //  @8EB750
    static float    CharToFloatLimited(const unsigned char valueFrom, const float limitValue);  //  @424CB0
    static float    ShortToFloatLimited(const unsigned short valueFrom, const float limitValue);    //  @424D60
    static char     FloatToChar(float value);   //  @424C60
    static void     KillNewEffects(const float gameTime);   //  @8E7280

    static bool     LodAndFade; // @A08944
    static Vector4f CameraOrigin;   //  @A35E18
};

extern EntityType* tParticleSystem; //  @A3E088

ASSERT_CLASS_SIZE(ParticleSystem, 244);
ASSERT_CLASS_SIZE(ParticleSystemInfo, 96);