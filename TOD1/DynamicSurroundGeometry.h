#pragma once
#include "Node.h"
#include <vector>

class FrameBuffer;
class MeshBuffer;

struct GeometryNode
{
    int                 m_TextureIndex;
    float               m_Opacity;
    float               field_8;
    float               field_C;
    int                 field_10;
    int                 field_14;
    int                 field_18;
    int                 field_1C;
    int                 field_20;
    int                 field_24;
    int                 field_28;
    int                 field_2C;
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
};

class DynamicSurroundGeometry : public Node
{
public:
    AssetLoader         m_Texture1[4];
    float               m_ObjectDensity;
    float               m_SpawnRate;
    float               m_Size;
    float               m_InitialHeight;
    float               m_Mass;
    float               m_MassSize;
    float               m_WindFactor;
    float               m_WindLift;
    float               m_WindLiftHeight;
    float               m_AirResistanceY;
    float               m_AirResistanceXZ;
    float               m_RotResistanceY;
    float               m_RotResistanceXZ;
    float               m_ForceOffset;
    float               m_ForceRotate;
    float               m_fAC;
    float               m_fB0;
    float               m_fB4;
    float               m_fB8;
    int                 m_Category;
    int                 m_MaxCount;
    float               m_Radius; // NOTE: range is from 0.f to 250.f
    float               m_TimePassed;
    FrameBuffer        *m_FrameBuffer;
    std::vector<int>    m_LightingPropsList;
    std::vector<int>    m_List_2;
    int                 field_F0;
    int                 field_F4;
    int                 field_F8;
    std::vector<GeometryNode>    m_List_3;
    MeshBuffer         *m_MeshBuffer;
    float               m_f110;

public:
    DynamicSurroundGeometry(); // @8D8FF0
    virtual             ~DynamicSurroundGeometry(); //  @8D9250
    virtual void        Render();   //  @8DC270

    static void         Register(); //  @8DCF50
    static DynamicSurroundGeometry* Create(AllocatorIndex); //  @8DD9E0

    static int          TotalCreated; // @A3E0E0

private:
    void                CalculateWindAffection(GeometryNode*);  //  @8D51E0
};

extern EntityType* tDynamicSurroundGeometry;    //  @A3DFFC

ASSERT_CLASS_SIZE(DynamicSurroundGeometry, 276);