#pragma once
#include "Types.h"
#include "MemoryManager.h"

class NodePosition;

struct Particle
{
protected:
    int             field_4;
    int             field_8;
    int             field_C;
    int             field_10;
    int             field_14;
    NodePosition   *m_PositionMethods;
    Vector4f        field_1C;
    int             field_2C;
    int             field_30;
    int             field_34;
    int             field_38;
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

    virtual void    stub2() = 0;
    virtual void    stub3() = 0;
};

ASSERT_CLASS_SIZE(Particle, 120);