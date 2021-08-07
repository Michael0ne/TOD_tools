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

    struct Vertex
    {
        Vector4f	m_Position;
        Orientation m_Orientation;
        Vector4f    m_Normal;
        short       field_30;
        short       field_32;
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
    std::vector<Vertex>     m_VerticesList;
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

    void                    SetVertexPos(const unsigned int vertexindex, const Vector4f& pos);   //  @92FB90
    void                    SetNormalPos(const unsigned int vertexindex, const Vector4f& norm); //  @92FC80
};

ASSERT_CLASS_SIZE(PhysSystem, 120);