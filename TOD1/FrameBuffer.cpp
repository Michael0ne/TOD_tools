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

void FrameBuffer::_431510()
{
    m_Flags.m_FlagBits._4 = 1;
}

void FrameBuffer::_4315A0(const DirectX::XMMATRIX& mat, const unsigned int index)
{
    m_RenderBuffer[0].PushMatrix(mat, index);
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

void FrameBuffer::SubmitRenderFullscreenTextureCommand(Texture* tex)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERFULLSCREENTEXTURE);
    m_RenderBuffer[0].push_back((int)tex);
}

void FrameBuffer::SubmitDrawNoiseCommand(Texture* tex, const float intensity, const int blendmode)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_DRAWNOISE);
    m_RenderBuffer[0].push_back(intensity);
    m_RenderBuffer[0].push_back(blendmode);
}

void FrameBuffer::SubmitDrawVignetteCommand(Texture* tex, const ColorRGB& clr, const float intensity, const float shape, const float size)
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

void FrameBuffer::SubmitRenderMeshBufferCommand_1(const MeshBuffer_Dx9* mesh)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERMESHBUFFER);
    m_RenderBuffer[0].push_back(mesh);
}

void FrameBuffer::SubmitRenderTriangleCommand(const Vector3f& pos1, const Vector3f& pos2, const Vector3f& pos3, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTRIANGLE);
    
    m_RenderBuffer[0].push_back(pos1.x);
    m_RenderBuffer[0].push_back(pos1.y);
    m_RenderBuffer[0].push_back(pos1.z);

    m_RenderBuffer[0].push_back(pos2.x);
    m_RenderBuffer[0].push_back(pos2.y);
    m_RenderBuffer[0].push_back(pos2.z);

    m_RenderBuffer[0].push_back(pos3.x);
    m_RenderBuffer[0].push_back(pos3.y);
    m_RenderBuffer[0].push_back(pos3.z);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

void FrameBuffer::SubmitRenderTriangleCommand(const Vector3f& pos1, const Vector3f& pos2, const Vector3f& pos3, const ColorRGB& clr1, const ColorRGB& clr2, const ColorRGB& clr3)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTRIANGLE);

    m_RenderBuffer[0].push_back(pos1.x);
    m_RenderBuffer[0].push_back(pos1.y);
    m_RenderBuffer[0].push_back(pos1.z);

    m_RenderBuffer[0].push_back(pos2.x);
    m_RenderBuffer[0].push_back(pos2.y);
    m_RenderBuffer[0].push_back(pos2.z);

    m_RenderBuffer[0].push_back(pos3.x);
    m_RenderBuffer[0].push_back(pos3.y);
    m_RenderBuffer[0].push_back(pos3.z);

    m_RenderBuffer[0].push_back(clr1.r);
    m_RenderBuffer[0].push_back(clr1.g);
    m_RenderBuffer[0].push_back(clr1.b);
    m_RenderBuffer[0].push_back(clr1.a);

    m_RenderBuffer[0].push_back(clr2.r);
    m_RenderBuffer[0].push_back(clr2.g);
    m_RenderBuffer[0].push_back(clr2.b);
    m_RenderBuffer[0].push_back(clr2.a);

    m_RenderBuffer[0].push_back(clr3.r);
    m_RenderBuffer[0].push_back(clr3.g);
    m_RenderBuffer[0].push_back(clr3.b);
    m_RenderBuffer[0].push_back(clr3.a);
}

void FrameBuffer::SubmitRenderLineCommand(const Vector3f& start, const Vector3f& end, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERLINE);

    m_RenderBuffer[0].push_back(start.x);
    m_RenderBuffer[0].push_back(start.y);
    m_RenderBuffer[0].push_back(start.z);

    m_RenderBuffer[0].push_back(end.x);
    m_RenderBuffer[0].push_back(end.y);
    m_RenderBuffer[0].push_back(end.z);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

void FrameBuffer::SubmitRenderLine2DCommand(const Vector2f& start, const Vector2f& end, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERLINE2D);

    m_RenderBuffer[0].push_back(start.x);
    m_RenderBuffer[0].push_back(start.y);

    m_RenderBuffer[0].push_back(end.x);
    m_RenderBuffer[0].push_back(end.y);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

void FrameBuffer::SubmitRenderTriangle2DCommand(const Vector2f& x, const Vector2f& y, const Vector2f& z, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTRIANGLE2D);

    m_RenderBuffer[0].push_back(x.x);
    m_RenderBuffer[0].push_back(x.y);

    m_RenderBuffer[0].push_back(y.x);
    m_RenderBuffer[0].push_back(y.y);

    m_RenderBuffer[0].push_back(z.y);
    m_RenderBuffer[0].push_back(z.y);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

void FrameBuffer::SubmitRenderQuad2D_2Command(const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const ColorRGB& clr1, const ColorRGB& clr2, const ColorRGB& clr3, const ColorRGB& clr4)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERQUAD2D_2);

    m_RenderBuffer[0].push_back(topleft.x);
    m_RenderBuffer[0].push_back(topleft.y);

    m_RenderBuffer[0].push_back(bottomleft.x);
    m_RenderBuffer[0].push_back(bottomleft.y);

    m_RenderBuffer[0].push_back(topright.x);
    m_RenderBuffer[0].push_back(topright.y);

    m_RenderBuffer[0].push_back(bottomright.x);
    m_RenderBuffer[0].push_back(bottomright.y);

    m_RenderBuffer[0].push_back(clr1.r);
    m_RenderBuffer[0].push_back(clr1.g);
    m_RenderBuffer[0].push_back(clr1.b);
    m_RenderBuffer[0].push_back(clr1.a);

    m_RenderBuffer[0].push_back(clr2.r);
    m_RenderBuffer[0].push_back(clr2.g);
    m_RenderBuffer[0].push_back(clr2.b);
    m_RenderBuffer[0].push_back(clr2.a);

    m_RenderBuffer[0].push_back(clr3.r);
    m_RenderBuffer[0].push_back(clr3.g);
    m_RenderBuffer[0].push_back(clr3.b);
    m_RenderBuffer[0].push_back(clr3.a);

    m_RenderBuffer[0].push_back(clr4.r);
    m_RenderBuffer[0].push_back(clr4.g);
    m_RenderBuffer[0].push_back(clr4.b);
    m_RenderBuffer[0].push_back(clr4.a);
}

void FrameBuffer::SubmitRenderQuad2D_Command(const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERQUAD2D);

    m_RenderBuffer[0].push_back(topleft.x);
    m_RenderBuffer[0].push_back(topleft.y);

    m_RenderBuffer[0].push_back(bottomleft.x);
    m_RenderBuffer[0].push_back(bottomleft.y);

    m_RenderBuffer[0].push_back(topright.x);
    m_RenderBuffer[0].push_back(topright.y);

    m_RenderBuffer[0].push_back(bottomright.x);
    m_RenderBuffer[0].push_back(bottomright.y);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

void FrameBuffer::SubmitRenderTexturedQuad2D_3Command(const Texture* tex, const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const Vector2f& a6, const Vector2f& a7, const Vector2f& a8, const Vector2f& a9, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD2D_3);

    m_RenderBuffer[0].push_back(tex);

    m_RenderBuffer[0].push_back(topleft.x);
    m_RenderBuffer[0].push_back(topleft.y);

    m_RenderBuffer[0].push_back(bottomleft.x);
    m_RenderBuffer[0].push_back(bottomleft.y);

    m_RenderBuffer[0].push_back(topright.x);
    m_RenderBuffer[0].push_back(topright.y);

    m_RenderBuffer[0].push_back(bottomright.x);
    m_RenderBuffer[0].push_back(bottomright.y);

    m_RenderBuffer[0].push_back(a6.x);
    m_RenderBuffer[0].push_back(a6.y);

    m_RenderBuffer[0].push_back(a7.x);
    m_RenderBuffer[0].push_back(a7.y);

    m_RenderBuffer[0].push_back(a8.x);
    m_RenderBuffer[0].push_back(a8.y);

    m_RenderBuffer[0].push_back(a9.x);
    m_RenderBuffer[0].push_back(a9.y);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

void FrameBuffer::SubmitRenderVertexBufferCommand(const void* vb, const int a2, const int a3)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERVERTEXBUFFER);
    m_RenderBuffer[0].push_back(vb);
    m_RenderBuffer[0].push_back(a2);
    m_RenderBuffer[0].push_back(a3);
}

void FrameBuffer::SubmitRenderMeshColorBufferCommand(const MeshBuffer_Dx9* mesh)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERMESHCOLORBUFFER);
    m_RenderBuffer[0].push_back(mesh);
}

void FrameBuffer::SubmitStartCharacterRenderCommand()
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_STARTCHARACTERRENDER);
}

void FrameBuffer::SubmitSetBoneMatrixCommand(const int boneid, DirectX::XMMATRIX& mat)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETBONEMATRIX);
    m_RenderBuffer[0].push_back(boneid);

    m_RenderBuffer[0].PushModelMatrix(mat);
}

void FrameBuffer::SubmitBeginTextCommand(const int textlen, const void* textprop, const Vector4f& textdimens)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_BEGINTEXT);

    m_RenderBuffer[0].push_back(textlen);
    m_RenderBuffer[0].push_back(textprop);

    m_RenderBuffer[0].push_back(textdimens.x);
    m_RenderBuffer[0].push_back(textdimens.y);
    m_RenderBuffer[0].push_back(textdimens.z);
    m_RenderBuffer[0].push_back(textdimens.a);
}

void FrameBuffer::SubmitRenderTextCommand(const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const bool a5)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTEXT);

    m_RenderBuffer[0].push_back(topleft.x);
    m_RenderBuffer[0].push_back(topleft.y);

    m_RenderBuffer[0].push_back(bottomleft.x);
    m_RenderBuffer[0].push_back(bottomleft.y);

    m_RenderBuffer[0].push_back(topright.x);
    m_RenderBuffer[0].push_back(topright.y);

    m_RenderBuffer[0].push_back(bottomright.x);
    m_RenderBuffer[0].push_back(bottomright.y);

    m_RenderBuffer[0].push_back(a5 != false);
}

void FrameBuffer::SubmitRenderText2Command(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4, const Vector2f& p5, const Vector2f& p6, const Vector2f& p7, const Vector2f& p8, const bool a9)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTEXT2);

    m_RenderBuffer[0].push_back(p1.x);
    m_RenderBuffer[0].push_back(p1.y);

    m_RenderBuffer[0].push_back(p2.x);
    m_RenderBuffer[0].push_back(p2.y);

    m_RenderBuffer[0].push_back(p3.x);
    m_RenderBuffer[0].push_back(p3.y);

    m_RenderBuffer[0].push_back(p4.x);
    m_RenderBuffer[0].push_back(p4.y);

    m_RenderBuffer[0].push_back(p5.x);
    m_RenderBuffer[0].push_back(p5.y);

    m_RenderBuffer[0].push_back(p6.x);
    m_RenderBuffer[0].push_back(p6.y);

    m_RenderBuffer[0].push_back(p7.x);
    m_RenderBuffer[0].push_back(p7.y);

    m_RenderBuffer[0].push_back(p8.x);
    m_RenderBuffer[0].push_back(p8.y);

    m_RenderBuffer[0].push_back(a9 != false);
}

void FrameBuffer::SubmitRenderSkinnedMeshBufferCommand(const SkinnedMeshBuffer* skinnedmeshbuf)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERSKINNEDMESHBUFFER);
    m_RenderBuffer[0].push_back(skinnedmeshbuf);
}

void FrameBuffer::SubmitEndCharacterRenderCommand()
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENDCHARACTERRENDER);
}

void FrameBuffer::SubmitUpdateSpritePositionCommand(int a1, const int, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4)
{
    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1++] = p1.x;
    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1++] = p1.y;

    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1++] = p2.x;
    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1++] = p2.y;

    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1++] = p3.x;
    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1++] = p3.y;

    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1++] = p4.x;
    *(float*)&m_RenderBuffer[0].m_ParamsArray[a1] = p4.y;
}

void FrameBuffer::SubmitRenderTexturedQuad2D_2Command(const Texture* tex, const Vector2f& topleft, const Vector2f& bottomleft, const Vector2f& topright, const Vector2f& bottomright, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD2D_2);

    m_RenderBuffer[0].push_back(tex);

    m_RenderBuffer[0].push_back(topleft.x);
    m_RenderBuffer[0].push_back(topleft.y);

    m_RenderBuffer[0].push_back(bottomleft.x);
    m_RenderBuffer[0].push_back(bottomleft.y);

    m_RenderBuffer[0].push_back(topright.x);
    m_RenderBuffer[0].push_back(topright.y);

    m_RenderBuffer[0].push_back(bottomright.x);
    m_RenderBuffer[0].push_back(bottomright.y);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

void FrameBuffer::SubmitRenderTexturedQuad2D_1Command(const Texture* tex, const Vector2f& top, const Vector2f& bottom, const ColorRGB& clr)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD2D_1);

    m_RenderBuffer[0].push_back(tex);

    m_RenderBuffer[0].push_back(top.x);
    m_RenderBuffer[0].push_back(top.y);

    m_RenderBuffer[0].push_back(bottom.x);
    m_RenderBuffer[0].push_back(bottom.y);

    m_RenderBuffer[0].push_back(clr.r);
    m_RenderBuffer[0].push_back(clr.g);
    m_RenderBuffer[0].push_back(clr.b);
    m_RenderBuffer[0].push_back(clr.a);
}

unsigned int FrameBuffer::SubmitSetModelMatrixCommand(const DirectX::XMMATRIX& mat)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETMODELMATRIX);
    unsigned int ret = m_RenderBuffer[0].m_CurrentParamIndex;
    m_RenderBuffer[0].PushModelMatrix(mat);

    field_0 ^= ((unsigned char)field_0 ^ (GfxInternal::_A08704[0].field_0 == 1)) & 1;

    return ret;
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

void FrameBuffer::SubmitBeginShadowCommand(Texture* tex)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_BEGINSHADOW);
    m_RenderBuffer[0].push_back(tex);
}

void FrameBuffer::SubmitRenderShadowCommand(const Vector4f& pos1, const Vector4f& pos2, const Vector4f& pos3, const Vector2f& uv1, const Vector2f& uv2, const Vector2f& uv3, const float rot1, const float rot2, const float rot3)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_RENDERSHADOW);

    m_RenderBuffer[0].push_back(pos1.x);
    m_RenderBuffer[0].push_back(pos1.y);
    m_RenderBuffer[0].push_back(pos1.z);

    m_RenderBuffer[0].push_back(pos2.x);
    m_RenderBuffer[0].push_back(pos2.y);
    m_RenderBuffer[0].push_back(pos2.z);

    m_RenderBuffer[0].push_back(pos3.x);
    m_RenderBuffer[0].push_back(pos3.y);
    m_RenderBuffer[0].push_back(pos3.z);

    m_RenderBuffer[0].push_back(uv1.x);
    m_RenderBuffer[0].push_back(uv1.y);

    m_RenderBuffer[0].push_back(uv2.x);
    m_RenderBuffer[0].push_back(uv2.y);

    m_RenderBuffer[0].push_back(uv3.x);
    m_RenderBuffer[0].push_back(uv3.y);

    m_RenderBuffer[0].push_back(rot1);
    m_RenderBuffer[0].push_back(rot2);
    m_RenderBuffer[0].push_back(rot3);
}

void FrameBuffer::SubmitEndShadowCommand()
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENDSHADOW);
}

void FrameBuffer::SubmitEndTextCommand()
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_ENDTEXT);
}

void FrameBuffer::SubmitSetCurrentTextureCommand(const Texture* tex, const int stage)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETCURRENTTEXTURE);
    m_RenderBuffer[0].push_back(tex);
    m_RenderBuffer[0].push_back(stage);

    field_0 ^= ((unsigned char)field_0 ^ (unsigned char)(2 * (GfxInternal::_A08704[1].field_0 == 1))) & 2;
}

void FrameBuffer::SubmitSetTextureScrollCommand(const float a1, const int a2)
{
    m_RenderBuffer[0].push_back(RenderBuffer::RenderCommand::CMD_SETTEXTURESCROLL);
    m_RenderBuffer[0].push_back(a1);
    m_RenderBuffer[0].push_back(a2);
}

void FrameBuffer::SubmitIndexedParam(const void* param, const int index)
{
    m_RenderBuffer[0].m_ParamsArray[index] = (int)param;
}

void FrameBuffer::SetRenderBufferSize(const unsigned int size)
{
    m_RenderBuffer[0].SetBufferSize(size);
}
