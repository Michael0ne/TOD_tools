#pragma once
#include "Types.h"
#include "MemoryManager.h"
#include <vector>

class NodePosition;

class Particle
{
    friend struct ParticleSystemInfo;
    friend class ParticleSystem;
protected:
    int             field_4;
    int             field_8;
    int             field_C;
    Particle       *m_Previous;
    Particle       *m_Next;
    NodePosition   *m_PositionMethods;
    Vector4f        m_ScreenPosition;
    Vector4f        m_CameraPosition;
    int             field_3C;
    float           field_40;
    int             field_44;
    int             field_48;
    Orientation     m_Orientation;
    float           m_BirthDistFade;
    int             field_60;
    float           field_64;
    int             m_StartTime;
    float           m_EmitterDelay;
    int             field_70;
    int             m_Amount;

public:
    Particle();    //  @424E70

    virtual ~Particle();   //  @424DC0

    virtual void    SetParticlesListSize(const unsigned int size) = 0;
    virtual const unsigned int  GetParticlesListSize() const = 0;
};

ASSERT_CLASS_SIZE(Particle, 120);

class ParticleA : public Particle
{
protected:
    std::vector<int>    m_Particles;

public:
    ParticleA();    //  @4255D0

    virtual ~ParticleA();   //  @42E310

    virtual void            SetParticlesListSize(const unsigned int size) override; //  @42E350
    virtual const size_t    GetParticlesListSize() const override;  //  @42E300
};

ASSERT_CLASS_SIZE(ParticleA, 136);