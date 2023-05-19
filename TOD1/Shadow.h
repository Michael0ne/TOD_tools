#pragma once
#include "Node.h"
#include "CollisionProbe.h"
#include "TextureAsset.h"
#include "FrameBuffer.h"

enum ShadowComplexity
{
    SIMPLE = 0,
    MESHPRECIE = 1
};

class Shadow : public Node
{
protected:
    FrameBuffer    *m_FrameBuffer_1;
    FrameBuffer    *m_FrameBuffer_2;
    AssetLoader     m_Texture;
    CollisionProbe *m_CollisionProbe;
    float           m_ShadowRadiusX;
    float           m_ShadowRadiusY;
    String          m_BoneName;
    Node           *m_Bone;
    Node           *field_80;
    Vector4f        field_84;
    union
    {
        struct
        {
            unsigned    Opacity : 8;  //  NOTE: multiply by (1 / 255) to get float value.
            unsigned    Complexity : 4;
            unsigned    AlignRotation : 1;
        };
    }               m_Flags;
    Vector4f        m_CollisionClosestNormal;
    Vector4f        m_CollisionContactPos;
    Node           *m_HintNode;
    int            *m_HintCollisionVolume;

public:
    Shadow(); // @8EE3B0
    virtual ~Shadow();  //  @8F1CB0

    virtual void    Destroy(); //  @8EC330
    virtual void    Instantiate();  //  @8EC3C0
    virtual void    Render(); // @8EEF50

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

    static void     Register(); //  @8EEB70

    static int  TotalShadows; // @A3E094

private:
    void            ClearHints();   //  @8EBBC0
    void            SetOpacity(float opacity);    //  @8F1AC0
    const float     GetOpacity() const; //  @8EBBA0
    void            SetAlignRotation(const bool align); //  @668B30
    const bool      GetAlignRotation() const;   //  @8EBB90
    void            SetComplexity(const ShadowComplexity complexity);   //  @543B00
    const ShadowComplexity GetComplexity() const;   //  @8EBB80
    void            SetBoneName(const char* const bonename);    //  @8EC480
    const char* const   GetBoneName() const;    //  @8A6D00
    void            SetShadowRadiusY(const float radiusy);  //  @659E40
    const float     GetShadowRadiusY() const;   //  @905840
    void            SetShadowRadiusX(const float radiusx);  //  @659E20
    const float     GetShadowRadiusX() const;   //  @905830
    void            SetShadowRadius(const float radius);    //  @8EBB30
    const float     GetShadowRadius() const;    //  @905830
    void            SetTexture(const char* const texturepath);  //  @8F1D60
    const char* const   GetTexture() const; //  @8EB4F0

    static Vector4f _A12B70;    //  @A12B70
    static std::vector<Vector4f> _A12B60;    //  @A12B60    //  NOTE: a list of closest shadow points?
    static std::vector<int> _A3E09C;    //  @A3E09C
    static MeshBuffer      *ShadowMeshBuffer;    //  @A3E090

    static Shadow*  Create(AllocatorIndex);  //  @8F1D20
};

extern EntityType  *tShadow;    //  @A3E08C

ASSERT_CLASS_SIZE(Shadow, 192);