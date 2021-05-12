#pragma once
#include "RenderBuffer.h"
#include "GfxInternal_Dx9_Texture.h"
#include <vector>

class FrameBuffer
{
protected:
    int                 field_0;
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
    RenderBuffer        m_RenderBuffer[3];
    float               field_54;
    FrameBuffer        *field_58;

public:
    FrameBuffer(unsigned int a1, unsigned char a2, unsigned int a3);	//	@436B00
    ~FrameBuffer();	//	@4361E0

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

    void                ExecuteRenderCommand(RenderBuffer& buf) const;  //  @4342C0
    void                CmdCall();  //  @434290
    void                Reset();    //  @436AE0
    void                SubmitRenderFullscreenTextureCommand(GfxInternal_Dx9_Texture* tex);   //  @4320E0
    void                SubmitDrawNoiseCommand(GfxInternal_Dx9_Texture* tex, const float intensity, const int blendmode); //  @4341F0
    void                SubmitDrawVignetteCommand(GfxInternal_Dx9_Texture* tex, const ColorRGB& clr, const float intensity, const float shape, const float size);    //  @4340E0
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
    void                SubmitEndShadowCommand();   //  @433240

    void                SubmitIndexedParam(const void* param, const int index); //  @4337E0
};

ASSERT_CLASS_SIZE(FrameBuffer, 92);