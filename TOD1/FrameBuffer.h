#pragma once
#include "RenderList.h"
#include "Texture.h"
#include "MeshBuffer_Dx9.h"
#include "SkinnedMeshBuffer.h"
#include <vector>

class FrameBuffer
{
    friend class GfxInternal;
protected:
    union
    {
        struct
        {
            unsigned    ModelMatrix : 1;            //  0
            unsigned    Texture : 1;                //  1
            unsigned    EnableLighting : 1;         //  2
            unsigned    EnableLight : 1;            //  3
            unsigned    ZWrite : 1;                 //  4
            unsigned    Fog : 1;                    //  5
            unsigned    FogProperties : 1;          //  6
            unsigned    Filter : 1;                 //  7
            unsigned    CullMode : 1;               //  8
            unsigned    ZBias : 1;                  //  9
            unsigned    BlendMode : 1;              //  10
            unsigned    Opacity : 1;                //  11
            unsigned    Viewport : 1;               //  12
            unsigned    Projection : 1;             //  13
            unsigned    MipMapBias : 1;             //  14
            unsigned    MipMapping : 1;             //  15
            unsigned    EnvironmentMap : 1;         //  16
            unsigned    EnvironmentMapCoef : 1;     //  17
            unsigned    Brightness : 1;             //  18
            unsigned    ZTest : 1;                  //  19
            unsigned    TextureAddressMode : 1;     //  20
            unsigned    RenderTarget : 1;           //  21
            unsigned    AlphaChannel : 1;           //  22
            unsigned    SetAlphaTestThreshold : 1;  //  23
            unsigned    EnableAlphaTest : 1;        //  24
        };
    }                   m_CommandsBits;
    std::vector<int*>   m_LightsList;
    union
    {
        struct
        {
            unsigned    _0 : 1;
            unsigned    _1 : 1;
            unsigned    _2 : 1;
            unsigned    _3 : 1;
            unsigned    _4 : 1;
            unsigned    _5 : 1;
            unsigned    _6 : 1;
            unsigned    _7 : 1;
            unsigned    _8 : 1;
            unsigned    _9 : 1;
            unsigned    _10 : 1;
            unsigned    _11 : 1;
            unsigned    _12 : 1;
            unsigned    _13 : 1;
            unsigned    _14 : 1;
            unsigned    _15 : 1;
            unsigned    _16 : 1;
            unsigned    _17 : 1;
            unsigned    _18 : 1;
            unsigned    _19 : 1;
            unsigned    _20 : 1;
            unsigned    _21 : 1;
            unsigned    _22 : 1;
            unsigned    _23 : 1;
            unsigned    _24 : 1;
            unsigned    _25 : 1;
            unsigned    _26 : 1;
            unsigned    _27 : 1;
            unsigned    _28 : 1;
            unsigned    _29 : 1;
            unsigned    _30 : 1;
            unsigned    _31 : 1;
        }               m_FlagBits;
        unsigned int    m_Flags;
    }                   m_Flags;
    RenderList          m_RenderBuffer[3];
    float               field_54;
    FrameBuffer        *field_58;

public:
    FrameBuffer(unsigned int a1, unsigned char a2, unsigned int a3); // @436B00
    ~FrameBuffer(); // @4361E0

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

    void                _436040(const int a1, const float a2);  //  @436040
    void                _436BF0();  //  @436BF0
    bool                _436030();  //  @436030
    void                _431510();  //  @431510
    void                UpdateMatrixAtIndex(const DirectX::XMMATRIX& mat, const unsigned int index);    //  @4315A0
    void                PropagateRenderListPushCmd(RenderList* rb) const; //  @436240
    void                PropagateRenderListPopCmd(RenderList* rb) const;    //  @436690
    const int           GetRenderCommandsTotal() const; //  @4360C0

    void                ExecuteRenderCommand(RenderList& buf) const;  //  @4342C0
    void                CmdCall();  //  @434290
    void                Reset();    //  @436AE0
    void                SubmitRenderFullscreenTextureCommand(Texture* tex);   //  @4320E0
    void                SubmitDrawNoiseCommand(Texture* tex, const float intensity, const int blendmode); //  @4341F0
    void                SubmitDrawVignetteCommand(Texture* tex, const ColorRGB& clr, const float intensity, const float shape, const float size);    //  @4340E0
    void                SubmitDrawLightBleedingCommand(const float lightbleeding);  //  @434080
    void                SubmitDrawSaturationCommand(const float saturation);    //  @434020
    void                SubmitDrawBrightnessCommand(const float brightness);    //  @433FC0
    void                SubmitSetParticleCommand(const unsigned short particle);    //  @433D00
    void                SubmitBeginParticleSystemCommand(); //  @433D50
    void                SubmitSetOpacityCommand(const float opacity, short* a2);   //  @433750
    void                SubmitSetParticleColorCommand(const int color); //  @433F60
    void                SubmitSetParticleAlignCommand(const int alignment); //  @433F10
    void                SubmitRenderParticleCommand(const Vector4f& pos);   //  @433EA0
    void                SubmitSetParticleOrientCommand(const Vector4f& orient); //  @433E30
    void                SubmitSetParticleSizeCommand(const int size);   //  @433DD0
    void                SubmitEndParticleSystemCommand(const int a1);   //  @433D80
    void                SubmitSetCullModeCommand(const int cullmode);   //  @4335D0
    void                SubmitSetAxisAlignCommand(const int alignment); //  @433CB0
    void                SubmitEnableAlphaTestCommand(const bool alphatest); //  @433C30
    void                SubmitEnableLightingCommand(const bool lighting);   //  @4332C0
    void                SubmitEnableAlphaChannelCommand(const bool alphachannel);   //  @433B30
    void                SubmitSetBlendModeCommand(const int blendmode); //  @4336D0
    void                SubmitSetZBiasCommand(const int zbias); //  @433650
    void                SubmitEnableZTestCommand(const bool ztest); //  @433A30
    void                SubmitSetAlphaThreshholdCommand(const float threshhold);    //  @433BB0
    void                SubmitSetRenderTargetCommand(const int* rt);    //  @433AB0 //  NOTE: unused.
    void                SubmitSetTextureAddressModeCommand(const int a1, const int a2);   //  @433990
    void                SubmitEnableZWriteCommand(const bool zwrite);   //  @433340
    void                SubmitEnableMipMappingCommand(const bool mipmapping);   //  @433810
    void                SubmitSetEnvironmentMapCoefCommand(const float coeff); //  @433910
    void                SubmitEnableLightCommand(int* light, const bool enabled); //  @436C90
    void                SubmitRenderMeshBufferCommand(void* meshbuffer, const float opacity);   //  @436180
    void                SubmitEnableEnvironmentMapCommand(const bool enabled);  //  @433890
    void                SubmitSetFilterModeCommand(const int filtermode);   //  @433550
    void                SubmitSetFogPropertiesCommand(const int fogtype, const ColorRGB& fogcolor, const float fogstart, const float fogend, const float fogdensity);  //  @433440
    void                SubmitEnableFogCommand(const bool enabled); //  @4333C0
    void                SubmitCallCommand(const int callcmd);   //  @433270
    void                SubmitBeginShadowCommand(Texture* tex); //  @433080
    void                SubmitRenderShadowCommand(const Vector4f& pos1, const Vector4f& pos2, const Vector4f& pos3, const Vector2f& uv1, const Vector2f& uv2, const Vector2f& uv3, const float rot1, const float rot2, const float rot3);    //  @4330D0
    void                SubmitEndShadowCommand();   //  @433240
    void                SubmitEndTextCommand(); //  @433050
    void                SubmitSetCurrentTextureCommand(const Texture* tex, const int stage);    //  @431660
    void                SubmitSetTextureScrollCommand(const float a1, const int a2);  //  @4315E0
    void                SubmitRenderMeshBufferCommand_1(const MeshBuffer_Dx9* mesh);    //  @432C70
    void                SubmitRenderTriangleCommand(const Vector3f& pos1, const Vector3f& pos2, const Vector3f& pos3, const ColorRGB& clr); //  @431700
    void                SubmitRenderTriangleCommand(const Vector3f& pos1, const Vector3f& pos2, const Vector3f& pos3, const ColorRGB& clr1, const ColorRGB& clr2, const ColorRGB& clr3);    //  @431820
    void                SubmitRenderLineCommand(const Vector3f& start, const Vector3f& end, const ColorRGB& clr);   //  @431CF0
    void                SubmitRenderLine2DCommand(const Vector2f& start, const Vector2f& end, const ColorRGB& clr);    //  @431DD0
    void                SubmitRenderTriangle2DCommand(const Vector2f& x, const Vector2f& y, const Vector2f& z, const ColorRGB& clr);    //  @431E50
    void                SubmitRenderQuad2D_2Command(const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const ColorRGB& clr1, const ColorRGB& clr2, const ColorRGB& clr3, const ColorRGB& clr4);  //  @431EE0
    void                SubmitRenderQuad2D_Command(const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const ColorRGB& clr);   //  @432040
    void                SubmitRenderTexturedQuad2D_3Command(const Texture* tex, const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const Vector2f& a6, const Vector2f& a7, const Vector2f& a8, const Vector2f& a9, const ColorRGB& clr);   //  @4322A0
    void                SubmitRenderVertexBufferCommand(const void* vb, const int a2, const int a3);  //  @432BD0
    void                SubmitRenderMeshColorBufferCommand(const MeshBuffer_Dx9* mesh);    //  @432CC0
    void                SubmitStartCharacterRenderCommand();   //  @432D10
    void                SubmitSetBoneMatrixCommand(const int boneid, DirectX::XMMATRIX& mat);  //  @432DC0
    void                SubmitBeginTextCommand(const int textlen, const void* textprop, const Vector4f& textdimens);   //  @432E40
    void                SubmitRenderTextCommand(const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const bool a5);  //  @432F00
    void                SubmitRenderText2Command(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4, const Vector2f& p5, const Vector2f& p6, const Vector2f& p7, const Vector2f& p8, const bool a9);  //  @432F90
    void                SubmitRenderSkinnedMeshBufferCommand(const SkinnedMeshBuffer* skinnedmeshbuf);  //  @432D70
    void                SubmitEndCharacterRenderCommand(); //  @432D40
    void                SubmitUpdateSpritePositionCommand(int a1, const int, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4); //  @432560
    void                SubmitRenderTexturedQuad2D_2Command(const Texture* tex, const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const ColorRGB& clr);  //  @4321E0
    void                SubmitRenderTexturedQuad2D_1Command(const Texture* tex, const Vector2f& top, const Vector2f& bottom, const ColorRGB& clr);  //  @432130
    unsigned int        SubmitSetModelMatrixCommand(const DirectX::XMMATRIX& mat);  //  @431540

    void                SubmitIndexedParam(const void* param, const int index); //  @4337E0

    static int          _A08704[57];    //  @A08704

private:
    void                SetRenderBufferSize(const unsigned int size);   //  @4314F0
};

ASSERT_CLASS_SIZE(FrameBuffer, 92);