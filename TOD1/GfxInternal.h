#pragma once
#include "GfxInternal_Dx9.h"
#include "AssetManager.h"
#include "FrameBuffer.h"
#include "MeshBuffer_Dx9.h"
#include "Mesh.h"

//  TODO: move this out here.
class Buffer276
{
public:
    Vector3f            m_BufferSize;
    FrameBuffer        *m_RenderBuffer;
    Buffer276          *field_10;
    char                field_14[4];
    DirectX::XMMATRIX   m_ViewMatrix;
    DirectX::XMMATRIX   m_MatrixUnknown_1;
    int                 field_98;
    DirectX::XMMATRIX   m_MatrixUnknown_2;
    char                field_DC[4];
    Vector4f            m_ProjectionMatrixParams; // NOTE: fov, xy_ratio, near_clip, far_clip.
    ScreenResolution    m_ViewportDimensions_1;
    ScreenResolution    m_ViewportDimensions_2;
    int                 m_ClearFlags;
    ColorRGB            m_ClearColor;

public:
    Buffer276() {};
    Buffer276(const Vector3f& bufferSize); // @41FE80

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }
};

class GfxInternal
{
    friend class TextureAsset;
    friend class GfxInternal_Dx9;
    friend class LoadScreenInfo;
    friend class ModelAsset;
protected:
    bool                            m_RenderBufferEmpty; // NOTE: this is set when failed to allocate space for buffer from stack.
    std::vector<Texture*>           m_CheckerboardTextures;
    MeshBuffer                     *m_MeshBuffer;
    Mesh                           *m_Mesh;
    unsigned int                    m_RenderBufferTotal;
    int                             field_20;
    Buffer276                      *m_RenderBufferArray;
    float                           m_TimeDelta;
    int                             m_TimeMilliseconds;
    int                             m_FramesRendered;
    char                            field_34;
    char                            field_35;
    float                           m_Time_1;
    char                            (* m_SceneCallback)(int);
    __int64                         m_FrameEndTime;

public:
    GfxInternal(const Vector2<unsigned int>& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1, const Vector3<float>* buffersDimens); // @421320
    ~GfxInternal(); // @421470

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

    void                            Render(Surface* screenshotDumpSurface, const bool shouldRender, int a3, int a4); // @421B30
    void                            CallSceneCallback(); // @420BA0
    void                            SetClearColorForBufferIndex(const ColorRGB& color, int index); // @41FDF0
    void                            SetClearFlagsForBufferIndex(const unsigned int flags, const int index); // @41FD90
    void                            SetRenderBufferIsEmpty(bool); // @420170
    void                            PrepareForNewLevel(); // @420180
    void                            DumpScreenShot(class Surface* surf) const; // @420100
    ScreenResolution&               GetScreenResolution(ScreenResolution& res) const; // @41FD70
    bool                            IsScreenResolutionAvailable(unsigned int width, unsigned int height) const; // @485460
    void                            SetBufferRenderBufferPointerByIndex(unsigned int index, FrameBuffer* buf); // @41F9B0
    void                            _41F950(); // @41F950
    void                            ExecuteRenderBuffer(int a1, int a2, int a3);    //  @421530
    FrameBuffer*                    _41F8F0(FrameBuffer* fb, unsigned int index);   //  @41F8F0
    void                            CreateCheckerboardTextures();    //  @4210E0
    void                            GetViewMatrixForBufferIndex(DirectX::XMMATRIX& mat, const unsigned int ind) const;  //  @45EE30
    void                            _420390();  //  @420390

    static AssetManager::RegionCode  GetRegion(); // @420160
    static bool                     IsWideScreen(); // @420120

    static bool                     WideScreen; // @A39F12
    static bool                     FSAA;
    static float                    RatioXY; // @A119F4
    static float                    _A3A064;    //  @A3A064
    static DirectX::XMMATRIX        _A3A268;    //  @A3A268
    
    struct Renderer_Buffer2
    {
        unsigned int field_0;
        unsigned int field_4;
    };

    static Renderer_Buffer2         _A08704[28]; // @A08704
};

extern GfxInternal* g_GfxInternal;

ASSERT_CLASS_SIZE(GfxInternal, 72);