#pragma once
#include "Types.h"
#include "MemoryManager.h"
#include <vector>

class NodePosition;

class Particle
{
protected:
    int             field_4;
    int             field_8;
    int             field_C;
    int             field_10;
    int             field_14;
    NodePosition   *m_PositionMethods;
    Vector4f        field_1C;
    Vector4f        field_2C;
    int             field_3C;
    float           field_40;
    int             field_44;
    int             field_48;
    Orientation     field_4C;
    float           field_5C;
    int             field_60;
    float           field_64;
    int             m_StartTime;
    float           field_6C;
    int             field_70;
    int             field_74;

public:
    Particle();    //  @424E70

    virtual ~Particle();   //  @424DC0

    virtual void    SetParticlesListSize() = 0;
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

    virtual void    SetParticlesListSize() override; //  @42E350
    virtual const unsigned int  GetParticlesListSize() const override;  //  @42E300
};

ASSERT_CLASS_SIZE(ParticleA, 136);