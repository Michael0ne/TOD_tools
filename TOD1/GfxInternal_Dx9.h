#pragma once
#ifdef DIRECTX
#include "Types.h"
#include "MemoryManager.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MeshBuffer_Dx9.h"
#include "Light.h"
#include <directxmath\include\DirectXMath.h>
#include <vector>

#define COLOR_BGRA(clr) (DWORD)( ((unsigned char)(clr.b * 255.f) << 8 | ((unsigned char)(clr.g * 255.f) << 8 |(((unsigned char)(clr.r * 255.f) << 8) | (unsigned char)(clr.a * 255.f) << 8))))

class GfxInternal_Dx9
{
    friend class GfxInternal;
    friend class VirtualHud;
    friend class FrameBuffer;

    struct Line2D
    {
        Vector2f                    m_Start;
        float                       field_8;
        float                       field_C;
        D3DCOLOR                    m_ColorStart;

        Vector2f                    m_End;
        float                       field_1C;
        float                       field_20;
        D3DCOLOR                    m_ColorEnd;

        static const unsigned int   VerticesTotal = 2;
    };

    struct Line3D
    {
        Vector3f                    m_Start;
        D3DCOLOR                    m_ColorStart;

        Vector3f                    m_End;
        D3DCOLOR                    m_ColorEnd;

        static const unsigned int   VerticesTotal = 2;
    };

    struct Triangle2D
    {
        Vector2f                    m_Top;
        Vector2f                    m_TopUV;
        int                         m_TopColor;

        Vector2f                    m_BottomLeft;
        Vector2f                    m_BottomLeftUV;
        int                         m_BottomLeftColor;

        Vector2f                    m_BottomRight;
        Vector2f                    m_BottomRightUV;
        int                         m_BottomRightColor;

        static const unsigned int   VerticesTotal = 3;
    };

    struct TriangleTextured3D
    {
        Vector3f                    m_Top;
        int                         m_TopColor;
        Vector2f                    m_TopUV;

        Vector3f                    m_BottomLeft;
        int                         m_BottomLeftColor;
        Vector2f                    m_BottomLeftUV;

        Vector3f                    m_BottomRight;
        int                         m_BottomRightColor;
        Vector2f                    m_BottomRightUV;

        static const unsigned int   VerticesTotal = 3;

    };

    struct DisplayModeInfo
    {
        unsigned int	            m_Width;
        unsigned int	            m_Height;
        bool			            m_Available;
        unsigned int	            m_RefreshRate;
        int				            m_Format;
    };

    struct VerticesBuffer
    {
        float	                    field_0;
        float	                    field_4;
        float	                    field_8;
        float	                    field_C;
        int		                    field_10;
        Vector2f                    field_14;
    };

    struct LightStatus
    {
        bool	                    m_Enabled;
        Light                      *m_Light;
    };

    struct Sampler
    {
        int                         m_Sampler;
        int                         m_AddressModeU;
        int                         m_AddressModeV;
        char                        m_MipMapped;
        char                        field_D;

        void                        SetAddressUMode(const unsigned int mode, const bool immediatechange);   //  @45EC50
        void                        SetAddressVMode(const unsigned int mode, const bool immediatechange);  //  @45ECC0
    };

    struct RenderProperties
    {
        int                         field_0;
        int                         field_4;
        int                         field_8;
        int                         field_C;
        int                         field_10;
        int                         field_14;
        int                         field_18;
        int                         field_1C;
        int                         field_20;
        char                        field_24;
        int                         field_28;
        int                         field_2C;
        int                         field_30;
        int                         field_34;
        int                         field_38;
        int                         field_3C;
        int                         field_40;
        int                         field_44;
        int                         field_48;
        char                        field_4C;
        Sampler                     m_Sampler[2];
        bool                        m_LightingEnabled;
        bool                        m_ZTest;
        bool                        m_ZWrite;
        char                        field_73;
        ColorRGB                    m_AmbientColor;
        int                         m_CullMode;
        bool                        m_AlphaTest;
        bool                        m_AlphaBlend;
        char                        field_8A;
        char                        field_8B;

        RenderProperties();	//	@45FF40
        RenderProperties(const RenderProperties& rhs);	//	@460150

        void	                    SetTextureAmbientColor(const ColorRGB& clr, bool flushdirectly);	//	@45ED60
        void	                    ToggleLighting(bool enabled, bool flushdirectly);	//	@45ED30
    };

    ASSERT_CLASS_SIZE(RenderProperties, 140);

public:
    IDirect3DDevice9               *m_Direct3DDevice;
protected:
    IDirect3DQuery9                *m_FramesyncQuery;
    char                            m_DeviceResetIssued;
    char                            field_9;
    char                            field_A;
    int                             m_TotalMeshesDrawn;
    int                             m_TotalPrimitivesDrawn;
    int                             m_TotalVerticiesDrawn;
    std::vector<DisplayModeInfo>	m_DisplayModesList;
    union {
        unsigned char               m_VertexShader11 : 1;
        unsigned char               m_VertexShader20 : 1;
        unsigned char               m_PixelShader13 : 1;
        unsigned char               m_PixelShader20 : 1;
    }                               m_ShaderCapabilities;
    unsigned int                    m_MaxVertexShaderConst;
    unsigned char                   m_TextureRequiresPow2;
    unsigned char                   m_MipmapLodBias;
    unsigned char                   m_Anisotropy;
    unsigned char                   m_MinAnisotropy;
    unsigned int                    m_MagAnisotropy;
    unsigned int                    m_MaxAnisotropy;
    unsigned char                   m_BlendOP;
    unsigned char                   m_FogAndSpecularAlpha;
    unsigned char                   m_RangeFog;
    unsigned char                   m_SeparateAlphaBlend;
    IDirect3D9                     *m_Direct3DInterface;
    D3DCAPS9                        m_DeviceCaps;
    int                             m_FSAA;
    D3DPRESENT_PARAMETERS           m_PresentParameters;
    bool                            m_DeviceLost;
    bool                            m_ShouldCreateVerticies;
    bool                            field_1B2;
    bool                            field_1B3;
    RenderProperties                m_TexProperties[2];
    bool                            m_FlushDirectly;
    IDirect3DTexture9              *m_TexturesArray[16];
public:
    bool                            m_Windowed;
protected:
    bool                            m_RenderingScene;
    D3DFORMAT                       m_DisplayModeFormat;
    ScreenResolution                m_ViewportResolution_1;	//	NOTE: backbuffer resolution?
    ScreenResolution                m_ViewportResolution;
    ScreenResolution                m_DisplayModeResolution;

    VerticesBuffer                  m_VerticesBuffer[750];
    bool                            m_DrawingText;
    int                             m_VerticiesToDrawTotal;
    int                             m_ActiveTextureColor;
    float                           m_FOV;
    float                           m_AspectRatio;
    float                           m_NearPlane;
    float                           m_FarPlane;
    VertexBuffer                   *m_VertexBuffer[4];
    DirectX::XMMATRIX               m_ProjectionMatrix;
    DirectX::XMMATRIX               m_ViewMatrix;
    DirectX::XMMATRIX               m_IdentityMatrix;
    DirectX::XMMATRIX               m_WorldMatrix;
    DirectX::XMMATRIX               m_MatrixUnknown;

    DirectX::XMMATRIX               m_BoneMatrix[255];

    char                            field_9664;
    int                             field_9668;
    int                             m_WireframeColor;
    int                             field_9670;
    int                             field_9674;
    float                           m_EnvironmentMapOpacity;
    float                           m_EnvironmentMapCoefficient;
    ColorRGB                        m_EnvironmentMapColors;
    bool                            m_AlphaChannelEnabled;
    bool                            m_AlphaTestEnabled;
    bool                            m_LightingEnabled;
    int                             m_BlendMode;
    float                           m_MipMapBias;
    bool                            m_MipMappingEnabled;
    int                             m_Filter;
    int                             m_TextureAddressModes[4];
    unsigned int                    m_ZBias;
    bool                            m_FogEnabled;
    float                           m_AlphaTestThreshhold;
    bool                            m_EnvironmentMapEnabled;
    int                             m_FVF;
    IDirect3DVertexDeclaration9    *m_Direct3DVertexDeclaration;
    CameraMatrix                   *m_GameCameraMatrix;
    int                             m_FogMode;
    float                           m_FogStart;
    float                           m_FogEnd;
    float                           m_FogDensity;
    ColorRGB                        m_FogColor;
    IndexBuffer                    *m_IndexBuffer;
    VertexBuffer                   *m_CurrentVertexBuffer;
    Texture                        *m_TexturesArray_2[2];
    int                             field_9700;
    int                             field_9704;
    bool                            m_TextureStageActive[4];
    Texture                        *m_RenderTarget;
    bool                            m_SurfaceDoubleSized;
    int                             m_ActiveViewportSurfaceIndex;
    Texture                        *m_ViewportTexturesArray[4];
    IDirect3DSurface9              *m_DepthStencilSurface;
    int                             m_AxisAlign;
    char                            m_DontColorParticles;
    int                             m_ParticleSize;
    Vector4f                        m_ParticleOrient;
    int                             m_ParticleAlign;
    int                             m_ParticleColor;
    int                            *m_ParticleMeshBuffer;	//	TODO: is this correct name?
    int                             m_ParticleSystemEnded;
public:
    float                           field_9758;
protected:
    int                             field_975C;
    std::vector<LightStatus>        m_SceneLights;
    std::map<int, int>              field_9770;	//	NOTE: related to lighting.
    int                             field_977C;
    int                             field_9780;
    int                             field_9784;
    int                             field_9788;
    int                             field_978C;
    int                             field_9790;
    int                             field_9794;
    int                             field_9798;
    int                             field_979C;

public:
    GfxInternal_Dx9(const ScreenResolution& resolution, unsigned int unused1, unsigned int unused2, unsigned int FSAA, unsigned int unk1);	//	@45E620
    ~GfxInternal_Dx9();	//	@45DA20

    void* operator new(size_t size)
    {
        return MemoryManager::Released ? nullptr : MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::AllocatorsList[DEFAULT]->Free(ptr);
        ptr = nullptr;
    }

    void                            RememberResolution();	//	@44CFF0
    bool                            GetRegistryResolution(Vector2<unsigned int>&);	//	@44D080
    bool                            ProcessGameInput();	//	@44D140
    void                            stub1() {};	//	@44D1A0
    void                            SetFVF(unsigned int);	//	@44D1B0
    void                            SetVertexDeclaration(IDirect3DVertexDeclaration9*);	//	@44D1F0
    void                            IssueDeviceReset();	//	@44D230
    void                            ProcessFramesyncQuery();	//	@44D260
    void                            EndScene();	//	@44D2A0
    void                            stub2() {};	//	@44D2D0
    void                            SetTextureStage1TransformationMatrix() const;	//	@44D2E0
    void                            ToggleEnvironmentMap(bool);	//	@44D5D0
    void                            SetEnvironmentMapCoef(float);	//	@44D8C0
    void                            LoadDDSTexture(unsigned int, const char*);	//	@44D920	//	NOTE: unused.
    HRESULT                         SetCurrentTextureIndex(unsigned int) const;	//	@44D9B0
    void                            EnableZTest(bool);	//	@44D9D0
    void                            EnableZWrite(bool);	//	@44DA10
    void                            EnableFog(bool);	//	@44DA50
    void                            GetFogParams(unsigned int* state, ColorRGB* color, float* start, float* end, float* density) const;	//	@44DA70
    void                            EnableLighting(bool);	//	@44DAE0
    void                            SetMipMapBias(float);	//	@44DB20
    void                            SetMipMapping(bool);	//	@44DB50
    void                            SetFilterMode(unsigned int);	//	@44DBE0
    void                            SetViewport(const ScreenResolution&, const ScreenResolution&) const {};	//	@44DCA0	//	NOTE: unused, but referenced.
    void                            GetProjectionParams(float* fov, float* aspectratio, float* nearplane, float* farplane) const;	//	@44DCB0
    void                            SetRenderStateWireframe(bool);	//	@44DD00
    void                            SetWireFrameColor(const ColorRGB& clr);	//	@44DE80
    void                            SetWorldMatrix(const DirectX::XMMATRIX*);	//	@44DEF0
    void                            SetEnvironmentMapOpacity(float);	//	@44DF90
    void                            EnableAlphaChannel(bool);	//	@44E090
    void                            SetBlendMode(unsigned int);	//	@44E160
    void                            EnableAlphaTest(bool);	//	@44E1A0
    void                            SetAlphaTestThreshold(float);	//	@44E1E0
    void                            SetRenderTarget(Texture*);	//	@44E220
    void                            TransformStateView(DirectX::XMMATRIX*);	//	@44E400
    void                            SetProjection(float fov, float aspectratio, float nearplane, float farplane);	//	@44E580
    void                            _44E680(const DirectX::XMMATRIX*, Vector2<float>*, unsigned int*);	//	@44E680
    unsigned int                    GetAvailableTextureMemory() const;	//	@44E960	//	NOTE: unused completely.
    void                            DumpScreenShot(class Surface* surf);	//	@44E970
    void                            SetTextureAddressMode(const unsigned int mode, const unsigned int ind);	//	@44EB30
    void                            SetCullMode(unsigned int);	//	@44EB80
    HRESULT                         GetDeviceCaps(IDirect3D9*, D3DCAPS9*);	//	@44EBC0
    void                            DestroySurfaces();	//	@44ED40
    void                            stub3(int) {};	//	@44EE30
    void                            _44EE40();	//	@44EE40	//	NOTE: unused.
    void                            SetVertexBuffer(VertexBuffer* vb);	//	@44EE50
    void                            SetIndexBuffer(IndexBuffer* ib);	//	@44EE60
    void                            SetTextureScroll(const Texture* tex, const float scroll);	//	@44EE70
    void                            SetTextureIndex(Texture*, const unsigned int index);	//	@44EF70
    void                            BeginText(const unsigned int, Texture* tex, const ColorRGB& clr);	//	@44EFA0
    void                            RenderText(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, bool usingcolor);	//	@44F050
    void                            RenderText2(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, bool);	//	@44F310
    void                            BeginShadow(Texture*);	//	@44F5B0
    void                            EndShadow();	//	@44F600
    void                            SetBoneMatrix(const unsigned int index, const DirectX::XMMATRIX&);	//	@44F640
    void                            SetParticleSize(unsigned int);	//	@44F790
    void                            SetParticleOrient(const Quaternion<float>&);	//	@44F7B0
    void                            SetParticleAlign(unsigned int);	//	@44F7E0
    void                            SetParticleColor(unsigned int);	//	@44F7F0
    IDirect3DVertexBuffer9*         CreateVertexBuffer(unsigned int length, unsigned int usage, unsigned int fvf, IDirect3DVertexBuffer9* pool);	//	@44F800	//	NOTE: unused completely.
    IDirect3DIndexBuffer9*          CreateIndexBuffer(unsigned int length, unsigned int usage, IDirect3DIndexBuffer9* pool);	//	@44F830	//	NOTE: unused completely.
    IDirect3DTexture9*              CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned int usage, D3DFORMAT format, IDirect3DTexture9* outTexture);	//	@44F860
    void                            ResetTextures();	//	@44F8A0
    void                            SetFlushDirectly();	//	@44F910	//	NOTE: unused completely.
    void                            SetupViewportSurface();	//	@44F940
    void                            HandleDeviceLost();	//	@44F960
    void                            Clear(unsigned char flags, const ColorRGB& clearcolor);	//	@44F9D0
    void                            SetZBias(unsigned int);	//	@44FAA0
    void                            SetFogProperties(unsigned int, const ColorRGB& color, float start, float end, float density);	//	@44FAE0
    void                            DrawIndexedPrimitive(int startindex, const int a2, int minvertexindex, const int a4);	//	@44FC40
    void                            SetupRenderer();	//	@44FD00	//	NOTE: setup initial parameters for Direct 3D renderer.
    void                            CreateVertexBuffersObjects(const unsigned int size);	//	@450610
    void                            DestroyVertexBuffersObjects();	//	@450710
    void                            RenderIndexedGeometry(MeshBuffer_Dx9* meshbuffer);	//	@4507B0
    void                            ResetStream();	//	@450810
    DisplayModeInfo*                IsScreenResolutionAvailable(unsigned int width, unsigned int height, bool dontignoreunavailable);	//	@450890
    void                            EnumDisplayModes();	//	@4508F0
    char                            GetLightStatus(const LightStatus*) const;	//	@4509F0
    void                            CreateSurfaces();	//	@450A30
    void                            CreateParticleMeshBuffer();	//	@450DB0
    void                            Reset();	//	@451020
    bool                            BeginScene();	//	@451080
    void                            CreateRenderDevice();	//	@451110
    void                            RenderLine2D(const Vector2<float>& start, const Vector2<float>& end, const ColorRGB& clr);	//	@451270
    void                            RenderLine(const Vector3<float>& start, const Vector3<float>& end, const ColorRGB& clr);	//	@4514B0
    void                            _4516A0(const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector3<float>&, unsigned int, unsigned int);	//	@4516A0
    void                            RenderTriangle2D(const Vector2<float>& top, const Vector2<float>& bottomleft, const Vector2<float>& bottomright, const ColorRGB& clr);	//	@451920
    void                            RenderTriangle_2(const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const ColorRGB&, const ColorRGB&, const ColorRGB&);	//	@451BC0
    void                            RenderTexturedTriangle(const Texture* tex, const Vector3<float>& top, const Vector3<float>& bottomleft, const Vector3<float>& bottomright, const Vector2<float>& textop, const Vector2<float>& texbottomleft, const Vector2<float>& texbottomright, const ColorRGB& clrtop, const ColorRGB& clrbottomleft, const ColorRGB& clrbottomright);	//	@451E90
    void                            RenderQuad2D(const Vector2<float>& tl, const Vector2<float>& bl, const Vector2<float>& tr, const Vector2<float>& br, const ColorRGB& clr);	//	@452220
    void                            RenderTexturedQuad2D_4(const Texture&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&, const ColorRGB&, const ColorRGB&, const ColorRGB&);	//	@452260
    void                            RenderTexturedQuad_3(const Texture*, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&, const ColorRGB&, const ColorRGB&, const ColorRGB&);	//	@452750
    void                            EndText();	//	@453D60
    void                            RenderShadow(const Vector3<float>& top, const Vector3<float>& bottomleft, const Vector3<float>& bottomright, const Vector2<float>& textop, const Vector2<float>& texbottomleft, const Vector2<float>& texbottomright, float r, float g, float b);	//	@453F20
    void                            RenderMeshBuffer(const MeshBuffer_Dx9* meshbuffer);	//	@4540E0
    void                            RenderMeshColorBuffer(const void* meshcolorbuffer);	//	@454660
    void                            RenderVertexBuffer(const VertexBuffer* vb, unsigned int startvertex, unsigned int primitivecount);	//	@454C80
    void                            EndParticleSystem(bool);	//	@454D40
    void                            RenderParticle(const Vector3f& pos);	//	@454D70
    void                            DrawBrightness(const float brightness);	//	@45A9A0
    void                            DrawSaturation(const float sat);	//	@45AC00
    void                            DrawVignette(const Texture*, const Vector3<float>&, float, float, float);	//	@45AE60
    void                            SetupWindowParamsAntialiased(unsigned int width, unsigned int height);	//	@45BE30
    void                            SetupWindowParamsNoAntiAliasing(const ScreenResolution resolution);	//	@45BEF0
    bool                            SetScreenResolution(unsigned int width, unsigned int height);	//	@45BF90
    bool                            SetupScreenRes();	//	@45C0D0
    void                            RenderTriangle(const Vector3<float>& top, const Vector3<float>& bottomleft, const Vector3<float>& bottomright, const ColorRGB& color);	//	@45C250
    void                            RenderQuad(const Vector3<float>& tl, const Vector3<float>& bl, const Vector3<float>& tr, const Vector3<float>& br, const ColorRGB& clr);	//	@45C270
    void                            RenderTexturedQuad2D_3(const Texture&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@45C2B0
    void                            RenderTexturedQuad_2(const Texture&, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector3<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@45C2F0
    void                            DrawNoise(const Texture*, const float opacity, const unsigned int blend);	//	@45C330
    void                            RenderTexturedQuad2D_2(const Texture&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const Vector2<float>&, const ColorRGB&);	//	@45C610
    void                            RenderTexturedQuad_1(const Texture* tex, const Vector3<float>& tl, const Vector3<float>& bl, const Vector3<float>& tr, const Vector3<float>& br, const ColorRGB& clr);	//	@45C6D0
    void                            _45C790(float);	//	@45C790
    void                            DrawLightBleeding(const float clr);	//	@45C9A0
    void                            RenderTexturedQuad2D_1(const Texture& tex, const Vector2<float>& top, const Vector2<float>& bottom, const ColorRGB&);	//	@45D4A0
    void                            RenderViewport();	//	@45D5E0
    void                            RenderFullscreenTexture(const Texture& tex);	//	@45D940
    void                            ToggleLight(Light_Properties* l, const bool enabled);	//	@45DBA0
    void                            RemoveLightFromScene(Light_Properties* l);	//	@45E5D0
    void                            GetWorldMatrix(DirectX::XMMATRIX& mat) const;   //  @430C50

    static std::map<int, Texture*>  RenderedTexturesMap;	//	@A39F50
    static const DirectX::XMMATRIX  IdentityMatrix;	//	@A0AD38
    static bool                     ProcessingInput;	//	@A39F10
    static D3DCULL                  CullModes[3];	//	@9B6674
    static HMENU                    WindowMenu;	//	@A39F28
    static D3DFORMAT                SupportedFormats[];	//	@A0A99C
    static float                    NoiseTime;	//	@A39F2C
    static int                      NoiseState;	//	@A39F30	//	NOTE: used when rendering noise.
    static bool                     ParticlesEnabled;   //  @A0A99B

    static void                     GetScreenResolution(ScreenResolution& outRes);	//	@41FD70
};

extern GfxInternal_Dx9*             g_GfxInternal_Dx9;	//	@A39F14
extern LPDIRECT3DDEVICE9            g_Direct3DDevice;	//	@A39F34

ASSERT_CLASS_SIZE(GfxInternal_Dx9, 38832);//38816);
#endif