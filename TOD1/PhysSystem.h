#pragma once
#include "Cloth.h"
#include <vector>

class PhysSystem
{
    struct List3
    {
        unsigned int        field_0;
        unsigned int        field_4;
    };

    struct List2
    {
        int	field_0;
        int	field_4;
        int	field_8;
        int	field_C;
        int	field_10;
        int	field_14;
        int	field_18;
        int	field_1C;
        int	field_20;
        int	field_24;
        int	field_28;
        int	field_2C;
        int	field_30;

    };

    struct List4
    {
        int	field_0;
        int	field_4;
        int	field_8;
        int	field_C;
        int	field_10;
    };

protected:
    std::vector<Vector4f>   m_List_1;
    std::vector<List2>      m_List_2;
    std::vector<List3>      m_List_3;
    Cloth                  *m_Cloth;
    Vector4f                m_GravityVec;
    int                     field_44;
    int                     field_48;
    int                     field_4C;
    int                     field_50;
    float                   m_Weight;
    float                   m_Damping;
    float                   field_5C;
    float                   field_60;
    std::vector<List4>      m_List_4;
    short                   m_NumIterations;

public:
    PhysSystem(unsigned int, Cloth*, bool);	//	@931240
    ~PhysSystem();  //  @92FB30
};

ASSERT_CLASS_SIZE(PhysSystem, 120);