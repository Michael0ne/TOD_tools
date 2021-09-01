#pragma once
#include "Node.h"
#include <vector>

class FrameBuffer;
class ModelAsset;
class MeshBuffer_Dx9;
class PhysSystem;

class Cloth : public Node
{
protected:
    Vector4f            m_BoundingRadius;
    FrameBuffer        *m_FrameBuffer;
    ModelAsset         *m_ModelAsset;
    int                 field_68;
    MeshBuffer_Dx9     *m_MeshBuffer;
    PhysSystem         *m_PhysSystem;
    float               m_Weight;
    float               m_Damping;
    float               m_WindInfluence;
    float               m_Opacity;
    union
    {
        struct
        {
            unsigned    TextureSet : 7;
            unsigned    Iterations : 7;
            unsigned    AllwaysUpdate : 1;
            unsigned    AddBlend : 1;
        };
        unsigned int    m_Flags;
    }                   m_Flags;
    std::vector<int>    m_List_1;
    std::vector<int>    m_List_2;
    int                 field_A8;
    int                 field_AC;
    int                 field_B0;

public:
    Cloth();    //  @92A7E0
    virtual ~Cloth();   //  @92A980

    virtual void    Instantiate() override;  //  @929D70
    virtual void    Update() override;  //  @929DD0
    virtual void    Render() override;  //  @92A500

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

    static void         Register(); //  @92B0C0

private:
    void                ApplyImpulse(const Vector4f& v1, const Vector4f& v2, const float force);    //  @929E60

    static Cloth*       Create(AllocatorIndex); //  @92B5A0

    static const float  MaximumImpulse; //  @9D722C
};

ASSERT_CLASS_SIZE(Cloth, 180);