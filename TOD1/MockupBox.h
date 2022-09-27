#pragma once
#include "Node.h"
#include "MeshBuffer.h"
#include "FrameBuffer.h"
#include "TextureAsset.h"
#include <vector>

class MockupBox : public Node
{
private:
    float               m_Left;
    float               m_Right;
    float               m_Front;
    float               m_Back;
    float               m_Height;
    AssetLoader         m_TextureAsset;
    float               m_Opacity;
    FrameBuffer        *m_FrameBuffer;
    MeshBuffer         *m_MeshBuffer;
    char                m_MetricTextureMode;
    float               m_MetricTextureMultiplier;
    int                *field_80;
    union
    {
        struct
        {
            unsigned    Solid : 1;
            unsigned    BulletProof : 1;
            unsigned    Opaque : 1;
            unsigned    CameraBlock : 1;
            unsigned    Trigger : 1;
            unsigned    NavigationBlock : 1;
            unsigned    Physics : 1;
            unsigned    ActivateTriggers : 1;

            unsigned    DynamicallyLit : 1;
            unsigned    StaticallyLit : 1;
        };
    }                   m_Flags;
    Vector4f            m_Dimensions;
    std::vector<int>    m_LightsList;
    std::vector<int>    m_List_2;
    int        field_B8;
    int        field_BC;
    int        field_C0;

public:
    MockupBox(); // @8CF870
    virtual            ~MockupBox();   //  @8D1170
    virtual void        Instantiate() override; //  @8D0420
    virtual void        Render() override;  //  @8CE930
    virtual char        ProcessCollision(int, int) override;    //  @8CF6A0
    virtual float       _8F8650(int, int) override; //  @8CE380
    virtual void        DestroyFrameBuffers() override; //  @8CE2F0
    virtual void        nullsub_3(int) override;    //  @8D1670
    virtual Vector4f*   GetBounds(Vector4f& bounds) override;   //  @8CE330

    const float         GetLeft() const;    //  @89A880
    void                SetLeft(const float left);  //  @8CE050
    const float         GetRight() const;   //  @89A890
    void                SetRight(const float right);    //  @8CE070
    const float         GetFront() const;   //  @89A8A0
    void                SetFront(const float front);    //  @8CE090
    const float         GetBack() const;    //  @905800
    void                SetBack(const float back);  //  @8CE0B0
    const float         GetHeight() const;  //  @8A6D40
    void                SetHeight(const float height);  //  @8CE0D0
    const char*         GetTexture() const; //  @8CE920
    void                SetTexture(const char* const texturename);  //  @8D03B0
    const float         GetOpacity() const; //  @905850
    void                SetOpacity(const float opacity);    //  @8CE1E0
    const int           GetCollisionFlags() const;  //  @8CE110
    void                SetCollisionFlags(const int flags); //  @8CE0F0
    const bool          GetMetricTextureMode() const;   //  @8CE010
    void                SetMetricTextureMode(const bool enabled);   //  @8CE020
    const float         GetMetricTextureMultiplier() const; //  @905880
    void                SetMetricTextureMultiplier(const float mult);   //  @8CE030
    const bool          IsDynamicallyLit() const;   //  @8CE120
    void                SetDynamicallyLit(const bool lit);   //  @8CE130
    const bool          IsStaticallyLit() const;    //  @8CE180
    void                SetStaticallyLit(const bool lit); //  @8CE190

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete (void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    static unsigned int TotalCreated; // @A3DFD8
    static MeshBuffer* MeshBufferPtr;   //  @A3DFD4

    static void         Register(); //  @8D1260

private:
    void                CalculateDimensions();  //  @8CE200

    static MockupBox*   Create(AllocatorIndex);   //  @8D1720
};

extern EntityType* tMockupBox;  //  @A3DFDC

ASSERT_CLASS_SIZE(MockupBox, 196);