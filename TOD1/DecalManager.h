#pragma once
#include "Node.h"

class TextureAsset;
class FrameBuffer;
struct VertexBuffer;

class DecalManager : public Node
{
    struct DecalInfo
    {
        VertexBuffer   *m_VertexBuffer;
        int             m_BufferSize;
        int             m_PerFrameFillSize;
        int             m_DecalsInThisFrame;
        int             m_VBFreeSpace;

        DecalInfo(); //  @430740
        ~DecalInfo();    //  @430B20

        void            _430760(const DirectX::XMMATRIX& mat, const float& opacity);   //  @430760
        void            _430AA0(FrameBuffer* fb);   //  @430AA0
        void            _430B50(const char bufferSize, const char perFrameFillSize);    //  @430B50
    };

protected:
    FrameBuffer    *m_FrameBuffer;
    AssetLoader     m_Texture;
    char            m_BufferSize;
    char            m_PerFrameFillSize;
    short           field_5E;
    DecalInfo           *m_DecalInfo;

public:
    inline DecalManager() : Node(NODE_MASK_QUADTREE)
    {
        MESSAGE_CLASS_CREATED(DecalManager);

        m_DecalInfo = new DecalInfo;

        m_BufferSize = 30;
        m_PerFrameFillSize = 2;
        m_FrameBuffer = nullptr;

        m_QuadTree->m_UserType |= 0x8000000;
    }
    virtual ~DecalManager();    //  @8D4FC0

    const char*         GetTexture() const; //  @8D2B20
    void                SetTexture(const char* texturepath);    //  @8D4C90
    const int           GetBufferSize() const;  //  @8D2A00
    void                SetBufferSize(const char buffersize);   //  @8D2AE0
    const int           GetPerFrameFillSize() const;    //  @8D2A10
    void                SetPerFrameFillSize(const int size);    //  @8D2B00

    void                SetDecal(int* args);    //  @8D3D80
    void                SetDecal_A(int* args);  //  @8D2B30

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

    static DecalManager* Create(AllocatorIndex);    //  @8D4F90
    static void         Register(); //  @8D4D00
};

extern EntityType* tDecalManager;   //  @A3DFF8

ASSERT_CLASS_SIZE(DecalManager, 100);