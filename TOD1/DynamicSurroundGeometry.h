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
    AssetLoader         m_Texture[4];
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

    const int           GetCategory() const;    //  @8D5160
    void                SetCategory(const int category);    //  @8D5170
    const int           GetMaxCount() const;    //  @8D5180
    void                SetMaxCount(const int maxcount);    //  @8D5190
    const float         GetRadius() const;  //  @8D4FF0
    void                SetRadius(const float radius);  //  @8D5000
    const float         GetObjectDensity() const;   //  @89AAB0
    void                SetObjectDensity(const float density);  //  @5A1B70
    const float         GetSpawnRate() const;   //  @905860
    void                SetSpawnRate(const float spawnrate);    //  @8D5020
    const char* const   GetTexture1() const;    //  @8D8160
    void                SetTexture1(const char* const texture1);    //  @8DCD90
    const char* const   GetTexture2() const;    //  @8D8170
    void                SetTexture2(const char* const texture2);    //  @8DCE00
    const char* const   GetTexture3() const;    //  @8D8180
    void                SetTexture3(const char* const texture3);    //  @8DCE70
    const char* const   GetTexture4() const;    //  @8D8190
    void                SetTexture4(const char* const texture4);    //  @8DCEE0
    const float         GetSize() const;    //  @905870
    void                SetSize(const float size);  //  @916E20
    const float         GetInitialHeight() const;   //  @905880
    void                SetInitialHeight(const float initialheight);    //  @905890
    const float         GetMass() const;    //  @89AD10
    void                SetMass(const float mass);  //  @8D5040
    const float         GetMassSize() const;    //  @916E30
    void                SetMassSize(const float masssize);  //  @8D5060
    const float         GetWindFactor() const;  //  @8CB4C0
    void                SetWindFactor(const float windfactor);  //  @8D5080
    const float         GetWindLift() const;    //  @501090
    void                SetWindLift(const float windlift);  //  @8D50A0
    const float         GetWindLiftHeight() const;  //  @5A1D40
    void                SetWindLiftHeight(const float windliftheight);  //  @8D50C0
    const float         GetAirResistanceY() const;  //  @8D50E0
    void                SetAirResistanceY(const float airresy); //  @8D50F0
    const float         GetAirResistanceXZ() const; //  @8D5110
    void                SetAirResistanceXZ(const float airresxz);   //  @88AAC0
    const float         GetRotResistanceY() const;  //  @916DF0
    void                SetRotResistanceY(const float rotresy); //  @88AAE0
    const float         GetRotResistanceXZ() const; //  @4B29A0
    void                SetRotResistanceXZ(const float rotresxz);   //  @88AB00
    const float         GetForceOffset() const; //  @8D5120
    void                SetForceOffset(const float offset); //  @8D5130
    const float         GetForceRotate() const; //  @8D5150
    void                SetForceRotate(const float rotate); //  @88AB60

    static void         Register(); //  @8DCF50
    static DynamicSurroundGeometry* Create(AllocatorIndex); //  @8DD9E0
    static void         _8FA270(int32_t* _this);    //  @8FA270

    static int          TotalCreated; // @A3E0E0
    static int32_t*     _A3E0DC;    //  @A3E0DC

private:
    void                CalculateWindAffection(GeometryNode*);  //  @8D51E0
};

extern EntityType* tDynamicSurroundGeometry;    //  @A3DFFC

ASSERT_CLASS_SIZE(DynamicSurroundGeometry, 276);