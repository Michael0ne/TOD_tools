#pragma once
#include "GfxInternal_Dx9.h"
#include "AssetManager.h"

class Buffer276
{
public:
    Vector3f	m_BufferSize;
    class RenderBuffer92*	m_RenderBuffer;
    Buffer276*	field_10;
    char		field_14;
    DirectX::XMMATRIX	m_ViewMatrix;
    DirectX::XMMATRIX	m_MatrixUnknown_1;
    int			field_98;
    DirectX::XMMATRIX	m_MatrixUnknown_2;
    char		field_DC;
    Vector4f	m_ProjectionMatrixParams;	//	NOTE: fov, xy_ratio, near_clip, far_clip.
    Vector2f	m_ViewportDimensions_1;
    Vector2f	m_ViewportDimensions_2;
    int			m_ClearFlags;
    ColorRGB	m_ClearColor;

public:
    Buffer276() {};
    Buffer276(const Vector3f& bufferSize);	//	@41FE80

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
    }
};

struct Scene_Buffer108
{
private:
    int	field_0;
    int	field_4;
    int	field_8;
    int	field_C;
    int	field_10;
    int	field_14;
    int	field_18;
    int	field_1C;
    int	field_20;
    int	field_24;
    int	field_28;
    int	field_2C;
    int	field_30;
    int	field_34;
    int	field_38;
    int	field_3C;
    int	field_40;
    int	field_44;
    int	field_48;
    int	field_4C;
    int	field_50;
    int	field_54;
    int	field_58;
    int	field_5C;
    int	field_60;
    int	field_64;
    int	field_68;

public:
    Scene_Buffer108()
    {
        MESSAGE_CLASS_CREATED(Scene_Buffer108);
    }
    ~Scene_Buffer108()
    {
        MESSAGE_CLASS_DESTROYED(Scene_Buffer108);
    }

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, true);
    }

    void	Init(unsigned int unk1, unsigned char unk2, unsigned int unk3);	//	@422330
};

struct Scene_Buffer68
{
private:
    int	field_0;
    int	field_4;
    int	field_8;
    int	field_C;
    int	field_10;
    int	field_14;
    int	field_18;
    int	field_1C;
    int	field_20;
    int	field_24;
    int	field_28;
    int	field_2C;
    int	field_30;
    int	field_34;
    int	field_38;
    int	field_3C;
    int	field_40;

public:
    Scene_Buffer68()
    {
        MESSAGE_CLASS_CREATED(Scene_Buffer68);
    }
    ~Scene_Buffer68()
    {
        MESSAGE_CLASS_DESTROYED(Scene_Buffer68);
    }

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, true);
    }

    void	Init(const Scene_Buffer108& buf, unsigned int unk);	//	@4617D0

    static void		CreateMeshBufferMap();	//	@460AB0
    
    static std::map<int, int>*	MeshBuffersMap;	//	@A39F38
};

class GfxInternal
{
    friend class TextureAsset;
    friend class GfxInternal_Dx9;
protected:
    bool							m_RenderBufferEmpty;	//	NOTE: this is set when failed to allocate space for buffer from stack.
    std::vector<GfxInternal_Dx9_Texture*>	m_TexturesList;
    Scene_Buffer68*					m_Buffer68;
    Scene_Buffer108*				m_Buffer108;
    unsigned int					m_RenderBufferTotal;
    int								field_20;
    Buffer276*						m_RenderBufferArray;
    float							m_TimeDelta;
    int								m_TimeMilliseconds;
    int								m_FramesRendered;
    char							field_34;
    char							field_35;
    float							m_Time_1;
    char							(* m_SceneCallback)(int);
    __int64							m_FrameEndTime;

public:
    GfxInternal(const Vector2<unsigned int>& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int buffersCount, unsigned int unk1, const Vector3<float>* buffersDimens);	//	@421320
    ~GfxInternal();	//	@421470

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

    void                            Render(GfxInternal_Dx9_Surface& screenshotDumpSurface, const bool shouldRender, int a3, int a4);	//	@421B30
    void                            CallSceneCallback();	//	@420BA0
    void                            SetClearColorForBufferIndex(const ColorRGB& color, int index);	//	@41FDF0
    void                            SetClearFlagsForBufferIndex(const unsigned int flags, const int index);	//	@41FD90
    void                            SetRenderBufferIsEmpty(bool);	//	@420170
    void                            PrepareForNewLevel();	//	@420180
    void                            DumpScreenShot(class GfxInternal_Dx9_Surface& surf) const;	//	@420100
    ScreenResolution&               GetScreenResolution(ScreenResolution& res) const;	//	@41FD70
    bool                            IsScreenResolutionAvailable(unsigned int width, unsigned int height) const;	//	@485460
    void                            SetBufferRenderBufferPointerByIndex(unsigned int index, class RenderBuffer92* buf);	//	@41F9B0
    void                            _41F950();	//	@41F950
    void                            ExecuteRenderBuffer(int a1, int a2, int a3);    //  @421530

    static AssetManager::RegionCode		GetRegion();	//	@420160
    static bool                     IsWideScreen();	//	@420120

    static bool                     WideScreen;	//	@A39F12
    static bool                     FSAA;
    static float                    RatioXY;	//	@A119F4
    static float                    _A3A064;    //  @A3A064
    
    struct Renderer_Buffer2
    {
        unsigned int	field_0;
        unsigned int	field_4;
    };

    static Renderer_Buffer2         _A08704[28];	//	@A08704
};

extern GfxInternal* g_GfxInternal;

ASSERT_CLASS_SIZE(GfxInternal, 72);