#include "FrameBuffer.h"
#include "GfxInternal.h"

FrameBuffer::FrameBuffer(unsigned int a1, unsigned char a2, unsigned int a3)
{
    MESSAGE_CLASS_CREATED(FrameBuffer);

    m_Flags.m_FlagBits._6 = a2;
    
    m_RenderBuffer[0] = { a1,   m_Flags.m_FlagBits._6 ? RENDERLIST : DEFAULT };
    m_RenderBuffer[1] = { 0,    m_Flags.m_FlagBits._6 ? RENDERLIST : DEFAULT };
    m_RenderBuffer[2] = { 0,    m_Flags.m_FlagBits._6 ? RENDERLIST : DEFAULT };

    m_Flags.m_FlagBits._0 = 0;
    m_Flags.m_FlagBits._3 = 0;
    m_Flags.m_Flags = m_Flags.m_Flags | a2 & 8 | ((a2 & 1) == 0);

    //  TODO: make this right.
    m_Flags.m_Flags = m_Flags.m_Flags ^ ((unsigned char)m_Flags.m_Flags ^ (unsigned char)(4 * (((a2 & 4) != 0) || ((m_Flags.m_Flags & 8) != 0)))) & 4;
    m_Flags.m_Flags = m_Flags.m_Flags & 0xFFFFFFCD | (2 * (((a2 & 2) != 0) || ((m_Flags.m_Flags & 4) == 0)));

    field_0 = NULL;
    field_54 = 0;
    field_58 = NULL;
}

FrameBuffer::~FrameBuffer()
{
    MESSAGE_CLASS_DESTROYED(FrameBuffer);

    if (m_RenderBuffer[2].m_ParamsArray != RenderBuffer::Buffer)
        delete m_RenderBuffer[2].m_ParamsArray;

    if (m_RenderBuffer[1].m_ParamsArray != RenderBuffer::Buffer)
        delete m_RenderBuffer[1].m_ParamsArray;

    if (m_RenderBuffer[0].m_ParamsArray != RenderBuffer::Buffer)
        delete m_RenderBuffer[0].m_ParamsArray;
}

void FrameBuffer::_436040(const int a1, const float a2)
{
    field_54 = a2;
    field_58 = g_GfxInternal->_41F8F0(this, a1);
    m_Flags.m_FlagBits._5 = 1;
}

#pragma message(TODO_IMPLEMENTATION)
void FrameBuffer::_436BF0()
{
}

bool FrameBuffer::_436030()
{
    m_Flags.m_FlagBits._4 = 0;
    return true;
}

#pragma message(TODO_IMPLEMENTATION)
void FrameBuffer::ExecuteRenderCommand(RenderBuffer& buf) const
{
    if (!buf.m_CurrentParamIndex)
        return;

    buf.m_PrevParamIndex = 0;
    do 
    {
        switch ((RenderBuffer::RenderCommand)buf.m_ParamsArray[buf.m_PrevParamIndex++])
        {
            //  TODO: handle all possible commands here.
        default:
            break;
        }
    } while (buf.m_PrevParamIndex != buf.m_CurrentParamIndex);
}

void FrameBuffer::CmdCall()
{
    m_Flags.m_FlagBits._5 = 0;

    ExecuteRenderCommand(m_RenderBuffer[1]);
    ExecuteRenderCommand(m_RenderBuffer[0]);
    ExecuteRenderCommand(m_RenderBuffer[2]);
}

void FrameBuffer::Reset()
{
    m_Flags.m_FlagBits._4 = 0;

    m_RenderBuffer[1].m_CurrentParamIndex = 0;
    m_RenderBuffer[1].m_PrevParamIndex = 0;

    m_RenderBuffer[0].m_CurrentParamIndex = 0;
    m_RenderBuffer[0].m_PrevParamIndex = 0;

    m_RenderBuffer[2].m_CurrentParamIndex = 0;
    m_RenderBuffer[2].m_PrevParamIndex = 0;

    field_0 = 0;
    m_LightsList.clear();
}

void FrameBuffer::SubmitRenderFullscreenTextureCommand(GfxInternal_Dx9_Texture* tex)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERFULLSCREENTEXTURE);
    m_RenderBuffer[0].push_back((int)tex);
}

void FrameBuffer::SubmitDrawNoiseCommand(GfxInternal_Dx9_Texture* tex, const float intensity, const int blendmode)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_DRAWNOISE);
    m_RenderBuffer[0].push_back(intensity);
    m_RenderBuffer[0].push_back(blendmode);
}

void FrameBuffer::SubmitDrawVignetteCommand(GfxInternal_Dx9_Texture* tex, const ColorRGB& clr, const float intensity, const float shape, const float size)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_DRAWVIGNETTE);
    m_RenderBuffer[0].push_back(intensity);
    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(shape);
    m_RenderBuffer[0].push_back(size);
}

void FrameBuffer::SubmitDrawLightBleedingCommand(const float lightbleeding)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_DRAWLIGHTBLEEDING);
    m_RenderBuffer[0].push_back(lightbleeding);
}

void FrameBuffer::SubmitDrawSaturationCommand(const float saturation)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_DRAWSATURATION);
    m_RenderBuffer[0].push_back(saturation);
}

void FrameBuffer::SubmitDrawBrightnessCommand(const float brightness)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_DRAWBRIGHTNESS);
    m_RenderBuffer[0].push_back(brightness);
}

void FrameBuffer::SubmitSetParticleCommand(const unsigned short particle)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETPARTICLE);
    m_RenderBuffer[0].push_back(particle);
}

void FrameBuffer::SubmitBeginParticleSystemCommand()
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_BEGINPARTICLESYSTEM);
}

void FrameBuffer::SubmitSetOpacityCommand(const float opacity, short* a2)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETOPACITY);

    if (a2)
        *a2 = m_RenderBuffer[0].m_CurrentParamIndex;

    m_RenderBuffer[0].push_back(opacity);

    field_0 ^= ((unsigned short)field_0 ^ ((GfxInternal::_A08704[18].field_0 == 1) << 12)) & 0x1000;
}

void FrameBuffer::SubmitSetParticleColorCommand(const int color)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETPARTICLECOLOR);
    m_RenderBuffer[0].push_back(color);
}

void FrameBuffer::SubmitSetParticleAlignCommand(const int alignment)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETPARTICLEALIGN);
    m_RenderBuffer[0].push_back(alignment);
}

void FrameBuffer::SubmitRenderParticleCommand(const Vector4f& pos)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERPARTICLE);
    m_RenderBuffer[0].push_back(pos.x);
    m_RenderBuffer[0].push_back(pos.y);
    m_RenderBuffer[0].push_back(pos.z);
}

void FrameBuffer::SubmitSetParticleOrientCommand(const Vector4f& orient)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETPARTICLEORIENT);
    m_RenderBuffer[0].push_back(orient.x);
    m_RenderBuffer[0].push_back(orient.y);
    m_RenderBuffer[0].push_back(orient.z);
    m_RenderBuffer[0].push_back(orient.a);
}

void FrameBuffer::SubmitSetParticleSizeCommand(const int size)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETPARTICLESIZE);
    m_RenderBuffer[0].push_back(size);
}

void FrameBuffer::SubmitEndParticleSystemCommand(const int a1)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENDPARTICLESYSTEM);
    m_RenderBuffer[0].push_back(a1);
}

void FrameBuffer::SubmitSetCullModeCommand(const int cullmode)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETCULLMODE);
    m_RenderBuffer[0].push_back(cullmode);

    field_0 ^= ((unsigned char)field_0 ^ (unsigned char)((GfxInternal::_A08704[14].field_0 == 1) << 7)) & 0x80;
}

void FrameBuffer::SubmitSetAxisAlignCommand(const int alignment)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETAXISALIGN);
    m_RenderBuffer[0].push_back(alignment);
}

void FrameBuffer::SubmitEnableAlphaTestCommand(const bool alphatest)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLEALPHATEST);
    m_RenderBuffer[0].push_back(alphatest != false);

    field_0 ^= (field_0 ^ ((GfxInternal::_A08704[27].field_0 == 1) << 23)) & 0x800000;
}

void FrameBuffer::SubmitEnableLightingCommand(const bool lighting)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLELIGHTING);
    m_RenderBuffer[0].push_back(lighting != false);

    field_0 ^= ((unsigned char)field_0 ^ (unsigned char)(4 * (GfxInternal::_A08704[6].field_0 == 1))) & 4;
}

void FrameBuffer::SubmitEnableAlphaChannelCommand(const bool alphachannel)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLEALPHACHANNEL);
    m_RenderBuffer[0].push_back(alphachannel != false);

    field_0 ^= (field_0 ^ ((GfxInternal::_A08704[25].field_0 == 1) << 21)) & 0x200000;
}

void FrameBuffer::SubmitSetBlendModeCommand(const int blendmode)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETBLENDMODE);
    m_RenderBuffer[0].push_back(blendmode);

    field_0 ^= ((unsigned short)field_0 ^ ((GfxInternal::_A08704[17].field_0 == 1) << 11)) & 0x800;
}

void FrameBuffer::SubmitSetZBiasCommand(const int zbias)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETZBIAS);
    m_RenderBuffer[0].push_back(zbias);

    field_0 ^= ((unsigned short)field_0 ^ ((GfxInternal::_A08704[15].field_0 == 1) << 8)) & 0x100;
}

void FrameBuffer::SubmitEnableZTestCommand(const bool ztest)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLEZTEST);
    m_RenderBuffer[0].push_back(ztest != false);

    field_0 ^= (field_0 ^ ((GfxInternal::_A08704[10].field_0 == 1) << 19)) & 0x80000;
}

void FrameBuffer::SubmitSetAlphaThreshholdCommand(const float threshhold)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETALPHATESTTHRESHHOLD);
    m_RenderBuffer[0].push_back(threshhold);

    field_0 ^= (field_0 ^ ((GfxInternal::_A08704[26].field_0 == 1) << 22)) & 0x400000;
}

void FrameBuffer::SubmitSetRenderTargetCommand(const int* rt)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETRENDERTARGET);
    m_RenderBuffer[0].push_back((int)rt);

    field_0 ^= (field_0 ^ ((GfxInternal::_A08704[24].field_0 == 1) << 20)) & 0x100000;
}

void FrameBuffer::SubmitSetTextureAddressModeCommand(const int a1, const int a2)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETTEXTUREADDRESSMODE);
    m_RenderBuffer[0].push_back(a2);
    m_RenderBuffer[0].push_back(a1);

    field_0 ^= (field_0 ^ ((GfxInternal::_A08704[23].field_0 == 1) << 18)) & 0x40000;
}

void FrameBuffer::SubmitEnableZWriteCommand(const bool zwrite)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLEZWRITE);
    m_RenderBuffer[0].push_back(zwrite);

    field_0 ^= ((unsigned char)field_0 ^ (unsigned char)(8 * (GfxInternal::_A08704[9].field_0 == 1))) & 8;
}

void FrameBuffer::SubmitEnableMipMappingCommand(const bool mipmapping)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLEMIPMAPPING);
    m_RenderBuffer[0].push_back(mipmapping != false);

    field_0 ^= ((unsigned short)field_0 ^ ((GfxInternal::_A08704[22].field_0 == 1) << 14)) & 0x4000;
}

void FrameBuffer::SubmitSetEnvironmentMapCoefCommand(const float coeff)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETENVIRONMENTMAPCOEF);
    m_RenderBuffer[0].push_back(coeff);

    field_0 ^= (field_0 ^ ((GfxInternal::_A08704[3].field_0 == 1) << 16)) & 0x10000;
}

void FrameBuffer::SubmitEnableLightCommand(int* light, const bool enabled)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLELIGHT);
    m_RenderBuffer[0].push_back((int)light);
    m_RenderBuffer[0].push_back(enabled != false);

    if (GfxInternal::_A08704[7].field_0 == 1)
        m_LightsList.push_back(light);
}

void FrameBuffer::SubmitRenderMeshBufferCommand(void* meshbuffer, const float opacity)
{
    SubmitSetOpacityCommand(opacity, nullptr);

    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERMESHBUFFER);
    m_RenderBuffer[0].push_back((int)meshbuffer);
}

void FrameBuffer::SubmitEnableEnvironmentMapCommand(const bool enabled)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLEENVIRONMENTMAP);
    m_RenderBuffer[0].push_back(enabled != false);

    field_0 ^= ((unsigned short)field_0 ^ (unsigned short)((GfxInternal::_A08704[2].field_0 == 1) << 15)) & 0x8000;
}

void FrameBuffer::SubmitSetFilterModeCommand(const int filtermode)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETFILTERMODE);
    m_RenderBuffer[0].push_back(filtermode);

    field_0 ^= ((unsigned char)field_0 ^ ((GfxInternal::_A08704[13].field_0 == 1) << 6)) & 0x40;
}

void FrameBuffer::SubmitSetFogPropertiesCommand(const int fogtype, const ColorRGB& fogcolor, const float fogstart, const float fogend, const float fogdensity)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETFOGPROPERTIES);
    m_RenderBuffer[0].push_back(fogtype);
    m_RenderBuffer[0].push_back(fogcolor.r);
    m_RenderBuffer[0].push_back(fogcolor.g);
    m_RenderBuffer[0].push_back(fogcolor.b);
    m_RenderBuffer[0].push_back(fogcolor.a);
    m_RenderBuffer[0].push_back(fogstart);
    m_RenderBuffer[0].push_back(fogend);

    field_0 ^= ((unsigned char)field_0 ^ (unsigned char)(32 * (GfxInternal::_A08704[12].field_0 == 1))) & 0x20;
}

void FrameBuffer::SubmitEnableFogCommand(const bool enabled)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENABLEFOG);
    m_RenderBuffer[0].push_back(enabled != false);

    field_0 ^= ((unsigned char)field_0 ^ (unsigned char)(16 * (GfxInternal::_A08704[11].field_0 == 1))) & 0x10;
}

void FrameBuffer::SubmitCallCommand(const int callcmd)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_CALL);
    m_RenderBuffer[0].push_back(callcmd);
}

void FrameBuffer::SubmitEndShadowCommand()
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENDSHADOW);
}

void FrameBuffer::SubmitIndexedParam(const void* param, const int index)
{
    m_RenderBuffer[0].m_ParamsArray[index] = (int)param;
}