#pragma once
#include "GfxInternal_Dx9.h"
#include "AssetManager.h"
#include "FrameBuffer.h"
#include "MeshBuffer_Dx9.h"
#include "Mesh.h"

using Matrix = DirectX::XMMATRIX;

//  TODO: move this somewhere else.
class RenderLayer
{
public:
    Vector3f            FrameResolution;
    FrameBuffer        *FrameBufferPtr;
    FrameBuffer        *_f10;
    uint32_t            _f14;
    Matrix              ViewMatrix;
    Matrix              MatrixUnk;
    uint32_t            _f98;
    Matrix              MatrixUnk_1;
    uint32_t            _fDC;
    union
    {
        struct
        {
            float_t         Fov;
            float_t         XYRatio;
            float_t         NearClip;
            float_t         FarClip;
        }                   ProjectionMatrixParams;

        Vector4f            ProjectionMatrixParamsVector;
    };
    ScreenResolution    ViewportResolution_1;
    ScreenResolution    ViewportResolution_2;
    uint32_t            ClearFlags;
    ColorRGB            ClearColor;

public:
    RenderLayer() = default;
    RenderLayer(const Vector3f& bufferSize); // @41FE80

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }
};

//  NOTE: old class size with old 'D3DMATRIX' structs.
//ASSERT_CLASS_SIZE(RenderLayer, 276);
ASSERT_CLASS_SIZE(RenderLayer, 304);

//  NOTE: possible name 'gfx2d'. Has layers that can have 3d stuff in it. Can draw textures.
class GfxInternal
{
public:
    bool                            m_RenderBufferEmpty; // NOTE: this is set when failed to allocate space for buffer from stack.
    std::vector<Texture*>           m_CheckerboardTextures;
    MeshBuffer                     *m_MeshBuffer;
    Mesh                           *m_Mesh;
    unsigned int                    m_RenderBufferTotal;
    int                             field_20;
    RenderLayer                    *m_RenderLayers;
    float                           m_TimeDelta;
    int                             m_TimeStart;
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
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
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
    void                            GetRenderLayerViewMatrix(DirectX::XMMATRIX& mat, const unsigned int ind) const;  //  @45EE30
    void                            _420390();  //  @420390
    void                            GetBackBufferResolution(ScreenResolution& outResolution, const unsigned int bufferindex) const; //  @41FD20
    const ScreenResolution&         GetViewportResolution() const;  //  @41FD50
    void                            SetBufferProjectionMatrixParams(const float fov, const float ratio, const float nearClip, const float farClip, const int32_t bufferIndex);  //  @41FB20
    void                            SetBufferViewMatrixByIndex(const DirectX::XMMATRIX& mat, const int32_t bufferIndex);    //  @41F9D0
    void                            SetGameCameraMatrix(CameraMatrix* mat);   //  @41F9A0

    static AssetManager::RegionCode  GetRegion(); // @420160
    static bool                     IsWideScreen(); // @420120

    static bool                     WideScreen; // @A39F12
    static bool                     FSAA;
    static float                    RatioXY; // @A119F4
    static float                    _A3A064;    //  @A3A064
    static DirectX::XMMATRIX        _A3A268;    //  @A3A268
};

extern GfxInternal* g_GfxInternal;

ASSERT_CLASS_SIZE(GfxInternal, 72);