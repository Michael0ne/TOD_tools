#pragma once
#include "Model.h"

class SkyBox : public Model
{
    struct Vector8f
    {
        Vector4f                m_Vec_1;
        Vector4f                m_Vec_2;
    };

    struct SkyBoxMesh
    {
        Vector8f                m_Pos_1;
        Vector8f                m_Pos_2;
        Vector8f                m_Pos_3;
        Vector8f                m_Pos_4;
    };

protected:
    int                         field_100;

public:
    SkyBox();
    virtual ~SkyBox(); // @8F2300

    virtual void                Render();   //  @8F1E10

    static void                 Register(); // @8F22A0

private:
    void               CreateStaticMesh() const; // @8F20B0

    static int                 *StaticMesh[5]; // @A3E0B0
    static const SkyBoxMesh     StaticMeshInfo[5]; //  @A12BC4
    static const unsigned short StaticMeshIndicies[12]; //  @A12E28

    static SkyBox*              Create(AllocatorIndex); // @8F23B0
};

extern EntityType* tSkyBox; // @A3E0AC

ASSERT_CLASS_SIZE(SkyBox, 260);