#pragma once
#include "Node.h"
#include "CollisionProbe.h"
#include "TextureAsset.h"
#include "FrameBuffer.h"

class Shadow : public Node
{
protected:
    FrameBuffer    *m_FrameBuffer_1;
    FrameBuffer    *m_FrameBuffer_2;
    TextureAsset   *m_Texture;
    int            *field_5C;
    CollisionProbe *m_CollisionProbe;
    float           m_ShadowRadiusX;
    float           m_ShadowRadiusY;
    String          m_BoneName;
    Node           *m_Bone;
    Node           *field_80;
    Vector4f        field_84;
    unsigned int    m_Flags;
    Vector4f        m_CollisionClosestNormal;
    Vector4f        m_CollisionContactPos;
    Node           *m_HintNode;
    int            *m_HintCollisionVolume;

public:
    Shadow(); // @8EE3B0
    virtual ~Shadow();  //  @8F1CB0

    virtual void    Destroy(); //  @8EC330
    virtual void    Instantiate();  //  @8EC3C0
    virtual void Render(); // @8EEF50

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

    static void     Register(); //  @8EEB70

    static int  TotalShadows; // @A3E094

private:
    void            ClearHints();   //  @8EBBC0

    static Vector4f _A12B70;    //  @A12B70
    static std::vector<Vector4f> _A12B60;    //  @A12B60    //  NOTE: a list of closest shadow points?
    static std::vector<int> _A3E09C;    //  @A3E09C
    static MeshBuffer      *ShadowMeshBuffer;    //  @A3E090

    static Shadow*  Create(AllocatorIndex);  //  @8F1D20
};

extern EntityType  *tShadow;    //  @A3E08C

ASSERT_CLASS_SIZE(Shadow, 192);