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

void FrameBuffer::ExecuteRenderCommand(RenderBuffer& buf) const
{
    if (!buf.m_CurrentParamIndex)
        return;

    buf.m_PrevParamIndex = 0;
    do 
    {
        switch ((RenderBuffer::RenderCommand)buf.m_ParamsArray[buf.m_PrevParamIndex++])
        {
        case RenderBuffer::RenderCommand::CMD_SETMODELMATRIX:
            {
                DirectX::XMMATRIX mat;
                buf.PopMatrix(mat);
                g_GfxInternal_Dx9->SetWorldMatrix(&mat);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETCURRENTTEXTURE:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                int texind = buf.m_ParamsArray[buf.m_PrevParamIndex++];

                if (texind < 0 || texind > 1)
                    return;

                g_GfxInternal_Dx9->SetTextureIndex(tex, texind);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETTEXTURESCROLL:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                float sy = (float)buf.m_ParamsArray[buf.m_PrevParamIndex++];

                g_GfxInternal_Dx9->SetTextureScroll(tex, sy);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLEENVIRONMENTMAP:
            {
                bool envmap;
                buf.PopBool(envmap);
                g_GfxInternal_Dx9->ToggleEnvironmentMap(envmap);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETENVIRONMENTMAPCOEF:
            g_GfxInternal_Dx9->SetEnvironmentMapCoef((float)buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTRIANGLE:
            {
                Vector3f top, btmleft, btmright;
                ColorRGB clr;
            
                buf.PopVector3f(top);
                buf.PopVector3f(btmleft);
                buf.PopVector3f(btmright);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderTriangle(top, btmleft, btmright, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTRIANGLE_2:
            {
                Vector3f top, btmleft, btmright;
                ColorRGB clrtop, clrbtml, clrbtmr;

                buf.PopVector3f(top);
                buf.PopVector3f(btmleft);
                buf.PopVector3f(btmright);
                buf.PopColor(clrtop);
                buf.PopColor(clrbtml);
                buf.PopColor(clrbtmr);

                g_GfxInternal_Dx9->RenderTriangle_2(top, btmleft, btmright, clrtop, clrbtml, clrbtmr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDTRIANGLE:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector3f top, btmleft, btmright;
                Vector2f topuv, btmleftuv, btmrightuv;
                ColorRGB clrtop, clrbtml, clrbtmr;

                buf.PopVector3f(top);
                buf.PopVector3f(btmleft);
                buf.PopVector3f(btmright);
                buf.PopVector2f(topuv);
                buf.PopVector2f(btmleftuv);
                buf.PopVector2f(btmrightuv);
                buf.PopColor(clrtop);
                buf.PopColor(clrbtml);
                buf.PopColor(clrbtmr);

                g_GfxInternal_Dx9->RenderTexturedTriangle(tex, top, btmleft, btmright, topuv, btmleftuv, btmrightuv, clrtop, clrbtml, clrbtmr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERQUAD:
            {
                Vector3f topleft, topright, bottomleft, bottomright;
                ColorRGB clr;

                buf.PopVector3f(topleft);
                buf.PopVector3f(bottomleft);
                buf.PopVector3f(topright);
                buf.PopVector3f(bottomright);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderQuad(topleft, bottomleft, topright, bottomright, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERLINE:
            {
                Vector3f start, end;
                ColorRGB clr;

                buf.PopVector3f(start);
                buf.PopVector3f(end);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderLine(start, end, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERLINE2D:
            {
                Vector2f start, end;
                ColorRGB clr;

                buf.PopVector2f(start);
                buf.PopVector2f(end);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderLine2D(start, end, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTRIANGLE2D:
            {
                Vector2f top, bottomleft, bottomright;
                ColorRGB clr;

                buf.PopVector2f(top);
                buf.PopVector2f(bottomleft);
                buf.PopVector2f(bottomright);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderTriangle2D(top, bottomleft, bottomright, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERQUAD2D:
            {
                Vector2f topleft, topright, bottomleft, bottomright;
                ColorRGB clr;

                buf.PopVector2f(topleft);
                buf.PopVector2f(bottomleft);
                buf.PopVector2f(topright);
                buf.PopVector2f(bottomright);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderQuad2D(topleft, bottomleft, topright, bottomright, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERQUAD2D_2:
            {
                Vector2f topleft, topright, bottomleft, bottomright;
                ColorRGB clrtopleft, clrtopright, clrbottomleft, clrbottomright;

                buf.PopVector2f(topleft);
                buf.PopVector2f(bottomleft);
                buf.PopVector2f(topright);
                buf.PopVector2f(bottomright);
                buf.PopColor(clrtopleft);
                buf.PopColor(clrbottomleft);
                buf.PopColor(clrtopright);
                buf.PopColor(clrbottomright);

                //  NOTE: not used.
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERFULLSCREENTEXTURE:
            g_GfxInternal_Dx9->RenderFullscreenTexture(*(Texture*)&buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERVERTEXBUFFER:
            {
                VertexBuffer   *vb = (VertexBuffer*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                unsigned int    startvert = buf.m_ParamsArray[buf.m_PrevParamIndex++];
                unsigned int    primcount = buf.m_ParamsArray[buf.m_PrevParamIndex++];

                g_GfxInternal_Dx9->RenderVertexBuffer(vb, startvert, primcount);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERMESHBUFFER:
            {
                MeshBuffer_Dx9* mb = (MeshBuffer_Dx9*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                RenderBuffer::MeshBuffersDrawn++;

                g_GfxInternal_Dx9->RenderMeshBuffer(mb);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERMESHCOLORBUFFER:
            {
                int* mcb = (int*)buf.m_ParamsArray[buf.m_PrevParamIndex++];

                g_GfxInternal_Dx9->RenderMeshColorBuffer(mcb);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERSKINNEDMESHBUFFER:
            {
                MeshBuffer_Dx9* smb = (MeshBuffer_Dx9*)buf.m_ParamsArray[buf.m_PrevParamIndex++];

                g_GfxInternal_Dx9->RenderIndexedGeometry(smb);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETBONEMATRIX:
            {
                int boneid = buf.m_ParamsArray[buf.m_PrevParamIndex++];
                DirectX::XMMATRIX mat;
            
                buf.PopMatrix(mat);

                g_GfxInternal_Dx9->SetBoneMatrix(boneid, mat);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_BEGINTEXT:
            {
                unsigned int len = buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                ColorRGB clr;

                buf.PopColor(clr);

                g_GfxInternal_Dx9->BeginText(len, tex, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXT:
            {
                Vector2f topleft, topright, bottomleft, bottomright;
                bool b;

                buf.PopVector2f(topleft);
                buf.PopVector2f(bottomleft);
                buf.PopVector2f(topright);
                buf.PopVector2f(bottomright);
                buf.PopBool(b);

                g_GfxInternal_Dx9->RenderText(topleft, bottomleft, topright, bottomright, b);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXT2:
            {
                Vector2f v[8];
                bool b;

                for (unsigned int i = 0; i < 8; ++i)
                    buf.PopVector2f(v[i]);
                buf.PopBool(b);

                g_GfxInternal_Dx9->RenderText2(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], b);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENDTEXT:
            {
                g_GfxInternal_Dx9->EndText();
                g_GfxInternal_Dx9->SetMipMapping(true);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_BEGINSHADOW:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];

                g_GfxInternal_Dx9->BeginShadow(tex);
                g_GfxInternal_Dx9->SetMipMapping(false);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERSHADOW:
            {
                Vector3f top, bottomleft, bottomright;
                Vector2f topuv, bottomleftuv, bottomrightuv;
                Vector3f clr;

                buf.PopVector3f(top);
                buf.PopVector3f(bottomleft);
                buf.PopVector3f(bottomright);
                buf.PopVector2f(topuv);
                buf.PopVector2f(bottomleftuv);
                buf.PopVector2f(bottomrightuv);
                buf.PopVector3f(clr);

                g_GfxInternal_Dx9->RenderShadow(top, bottomleft, bottomright, topuv, bottomleftuv, bottomrightuv, clr.x, clr.y, clr.z);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENDSHADOW:
            g_GfxInternal_Dx9->EndShadow();
            g_GfxInternal_Dx9->SetMipMapping(true);
            break;
        case RenderBuffer::RenderCommand::CMD_CALL:
            ((FrameBuffer*)(buf.m_ParamsArray[buf.m_PrevParamIndex++]))->CmdCall();
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLELIGHTING:
            {
                bool l;
                buf.PopBool(l);

                g_GfxInternal_Dx9->EnableLighting(l);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLELIGHT:
            {
                void* light = (void*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                bool enabled;

                buf.PopBool(enabled);

                g_GfxInternal_Dx9->ToggleLight((Light_Properties*)light, enabled);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLEZWRITE:
            {
                bool enabled;

                buf.PopBool(enabled);

                g_GfxInternal_Dx9->EnableZWrite(enabled);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLEZTEST:
            {
                bool enabled;

                buf.PopBool(enabled);

                g_GfxInternal_Dx9->EnableZTest(enabled);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLEFOG:
            {
                bool enabled;
            
                buf.PopBool(enabled);

                g_GfxInternal_Dx9->EnableFog(enabled);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETFOGPROPERTIES:
            {
                int fogmode = buf.m_ParamsArray[buf.m_PrevParamIndex++];
                ColorRGB fogclr;
                buf.PopColor(fogclr);
                float fogstart = *(float*)&buf.m_ParamsArray[buf.m_PrevParamIndex++];
                float fogend = *(float*)&buf.m_ParamsArray[buf.m_PrevParamIndex++];

                g_GfxInternal_Dx9->SetFogProperties(fogmode, fogclr, fogstart, fogend, 0);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETFILTERMODE:
            g_GfxInternal_Dx9->SetFilterMode(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETCULLMODE:
            g_GfxInternal_Dx9->SetCullMode(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETZBIAS:
            g_GfxInternal_Dx9->SetZBias(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETBLENDMODE:
            g_GfxInternal_Dx9->SetBlendMode(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETOPACITY:
            g_GfxInternal_Dx9->SetEnvironmentMapOpacity((float)buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETVIEWPORT:
            {
                ScreenResolution left, right;

                buf.PopVector2i(left);
                buf.PopVector2i(right);

                g_GfxInternal_Dx9->SetViewport(left, right);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETPROJECTION:
            {
                float fov, aspectratio, nearplane, farplane;

                buf.PopFloat(fov);
                buf.PopFloat(aspectratio);
                buf.PopFloat(nearplane);
                buf.PopFloat(farplane);

                g_GfxInternal_Dx9->SetProjection(fov, aspectratio, nearplane, farplane);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETMIPMAPBIAS:
            {
                float bias;
            
                buf.PopFloat(bias);

                g_GfxInternal_Dx9->SetMipMapBias(bias);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLEMIPMAPPING:
            {
                bool mipmapping;
            
                buf.PopBool(mipmapping);

                g_GfxInternal_Dx9->SetMipMapping(mipmapping);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETTEXTUREADDRESSMODE:
            {
                int u, v;
                u = buf.m_ParamsArray[buf.m_PrevParamIndex++];
                v = buf.m_ParamsArray[buf.m_PrevParamIndex++];

                g_GfxInternal_Dx9->SetTextureAddressMode(u, v);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETRENDERTARGET:
            g_GfxInternal_Dx9->SetRenderTarget((Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD2D_1:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector2f top, bottom;
                ColorRGB clr;

                buf.PopVector2f(top);
                buf.PopVector2f(bottom);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderTexturedQuad2D_1(*tex, top, bottom, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD2D_2:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector2f topleft, topright, bottomleft, bottomright;
                ColorRGB clr;

                buf.PopVector2f(topleft);
                buf.PopVector2f(bottomleft);
                buf.PopVector2f(topright);
                buf.PopVector2f(bottomright);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderTexturedQuad2D_2(*tex, topleft, bottomleft, topright, bottomright, clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD2D_3:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector2f v[8];
                ColorRGB clr;

                for (unsigned int i = 0; i < 8; ++i)
                    buf.PopVector2f(v[i]);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderTexturedQuad2D_3(*tex, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD2D_4:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector2f v[8];
                ColorRGB clr[4];

                for (unsigned int i = 0; i < 8; ++i)
                    buf.PopVector2f(v[i]);

                buf.PopColor(clr[0]);
                buf.PopColor(clr[1]);
                buf.PopColor(clr[2]);
                buf.PopColor(clr[3]);

                g_GfxInternal_Dx9->RenderTexturedQuad2D_4(*tex, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], clr[0], clr[1], clr[2], clr[3]);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD_1:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector3f v[4];
                ColorRGB clr;

                buf.PopVector3f(v[0]);
                buf.PopVector3f(v[1]);
                buf.PopVector3f(v[2]);
                buf.PopVector3f(v[3]);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderTexturedQuad_1(tex, v[0], v[1], v[2], v[3], clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD_2:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector3f v[4];
                Vector2f v2[4];
                ColorRGB clr;

                buf.PopVector3f(v[0]);
                buf.PopVector3f(v[1]);
                buf.PopVector3f(v[2]);
                buf.PopVector3f(v[3]);
                buf.PopVector2f(v2[0]);
                buf.PopVector2f(v2[1]);
                buf.PopVector2f(v2[2]);
                buf.PopVector2f(v2[3]);
                buf.PopColor(clr);

                g_GfxInternal_Dx9->RenderTexturedQuad_2(*tex, v[0], v[1], v[2], v[3], v2[0], v2[1], v2[2], v2[3], clr);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERTEXTUREDQUAD_3:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                Vector3f v[4];
                Vector2f v2[4];
                ColorRGB clr[4];

                buf.PopVector3f(v[0]);
                buf.PopVector3f(v[1]);
                buf.PopVector3f(v[2]);
                buf.PopVector3f(v[3]);
                buf.PopVector2f(v2[0]);
                buf.PopVector2f(v2[1]);
                buf.PopVector2f(v2[2]);
                buf.PopVector2f(v2[3]);
                buf.PopColor(clr[0]);
                buf.PopColor(clr[1]);
                buf.PopColor(clr[2]);
                buf.PopColor(clr[3]);

                g_GfxInternal_Dx9->RenderTexturedQuad_3(tex, v[0], v[1], v[2], v[3], v2[0], v2[1], v2[2], v2[3], clr[0], clr[1], clr[2], clr[3]);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLEALPHACHANNEL:
            {
                bool b;
            
                buf.PopBool(b);

                g_GfxInternal_Dx9->EnableAlphaChannel(b);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETALPHATESTTHRESHHOLD:
            {
                float t;
            
                buf.PopFloat(t);

                g_GfxInternal_Dx9->SetAlphaTestThreshold(t);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_ENABLEALPHATEST:
            {
                bool b;
            
                buf.PopBool(b);

                g_GfxInternal_Dx9->EnableAlphaTest(b);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETAXISALIGN:
            g_GfxInternal_Dx9->m_AxisAlign = buf.m_ParamsArray[buf.m_PrevParamIndex++];
            break;
        case RenderBuffer::RenderCommand::CMD_SETPARTICLE:
            buf.m_PrevParamIndex++;
            g_GfxInternal_Dx9->SetMipMapping(false);
            break;
        case RenderBuffer::RenderCommand::CMD_BEGINPARTICLESYSTEM:
            g_GfxInternal_Dx9->SetMipMapping(true);
            break;
        case RenderBuffer::RenderCommand::CMD_ENDPARTICLESYSTEM:
            g_GfxInternal_Dx9->EndParticleSystem(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETPARTICLESIZE:
            g_GfxInternal_Dx9->SetParticleSize(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETPARTICLEORIENT:
            {
                Orientation orient;

                buf.PopQuaternion(orient);

                g_GfxInternal_Dx9->SetParticleOrient(orient);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_RENDERPARTICLE:
            {
                Vector3f pos;

                buf.PopVector3f(pos);

                g_GfxInternal_Dx9->RenderParticle(pos);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_SETPARTICLEALIGN:
            g_GfxInternal_Dx9->SetParticleAlign(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_SETPARTICLECOLOR:
            g_GfxInternal_Dx9->SetParticleColor(buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            break;
        case RenderBuffer::RenderCommand::CMD_DRAWBRIGHTNESS:
            g_GfxInternal_Dx9->SetMipMapping(false);
            g_GfxInternal_Dx9->DrawBrightness((float)buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            g_GfxInternal_Dx9->SetMipMapping(true);
            break;
        case RenderBuffer::RenderCommand::CMD_DRAWSATURATION:
            g_GfxInternal_Dx9->SetMipMapping(false);
            g_GfxInternal_Dx9->DrawSaturation((float)buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            g_GfxInternal_Dx9->SetMipMapping(true);
            break;
        case RenderBuffer::RenderCommand::CMD_DRAWLIGHTBLEEDING:
            g_GfxInternal_Dx9->SetMipMapping(false);
            g_GfxInternal_Dx9->DrawLightBleeding(*(float*)&buf.m_ParamsArray[buf.m_PrevParamIndex++]);
            g_GfxInternal_Dx9->SetMipMapping(true);
            break;
        case RenderBuffer::RenderCommand::CMD_DRAWVIGNETTE:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                float a3, a4, a5;
                Vector3f v;
                buf.PopFloat(a3);
                buf.PopVector3f(v);
                buf.PopFloat(a4);
                buf.PopFloat(a5);

                g_GfxInternal_Dx9->DrawVignette(tex, v, a3, a4, a5);
                g_GfxInternal_Dx9->SetMipMapping(true);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_DRAWNOISE:
            {
                Texture* tex = (Texture*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                float a2;
                int a3;
                buf.PopFloat(a2);
                buf.PopInt(a3);

                g_GfxInternal_Dx9->SetMipMapping(false);
                g_GfxInternal_Dx9->DrawNoise(tex, a2, a3);
                g_GfxInternal_Dx9->SetMipMapping(true);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_MODELMATRIX:
            {
                DirectX::XMMATRIX mat;
                g_GfxInternal_Dx9->GetWorldMatrix(mat);
                g_RenderBuffer->PushModelMatrix(mat);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_TEXTURE:
            g_RenderBuffer->PushInt((int)g_GfxInternal_Dx9->m_TexturesArray_2[0]);
            g_RenderBuffer->PushInt((int)g_GfxInternal_Dx9->m_TexturesArray_2[1]);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ENVIRONMENTMAP:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_EnvironmentMapEnabled);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ENVIRONMENTMAPCOEF:
            g_RenderBuffer->PushFloat(g_GfxInternal_Dx9->m_EnvironmentMapCoefficient);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ENABLELIGHTING:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_LightingEnabled);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ENABLELIGHT:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->GetLightStatus((const GfxInternal_Dx9::LightStatus*)buf.m_ParamsArray[buf.m_PrevParamIndex++]));
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ZWRITE:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_TexProperties[0].m_ZWrite);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ZTEST:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_TexProperties[0].m_ZTest);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_FOG:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_FogEnabled);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_FOGPROPERTIES:
            {
                unsigned int mode;
                ColorRGB clr;
                float start, end, density;
                g_GfxInternal_Dx9->GetFogParams(&mode, &clr, &start, &end, &density);

                g_RenderBuffer->PushInt(mode);
                g_RenderBuffer->PushColor(clr);
                g_RenderBuffer->PushFloat(start);
                g_RenderBuffer->PushFloat(end);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_FILTER:
            g_RenderBuffer->PushInt(g_GfxInternal_Dx9->m_Filter);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_CULLMODE:
            g_RenderBuffer->PushInt(g_GfxInternal_Dx9->m_TexProperties[0].m_CullMode);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ZBIAS:
            g_RenderBuffer->PushInt(g_GfxInternal_Dx9->m_ZBias);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_BLENDMODE:
            break;
            g_RenderBuffer->PushInt(g_GfxInternal_Dx9->m_BlendMode);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_OPACITY:
            g_RenderBuffer->PushFloat(g_GfxInternal_Dx9->m_EnvironmentMapOpacity);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_VIEWPORT:
            g_RenderBuffer->PushVector2i(g_GfxInternal_Dx9->m_ViewportResolution_1);
            g_RenderBuffer->PushVector2i(g_GfxInternal_Dx9->m_ViewportResolution);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_PROJECTION:
            float fov, aspectratio, nearplane, farplane;
            g_GfxInternal_Dx9->GetProjectionParams(&fov, &aspectratio, &nearplane, &farplane);

            g_RenderBuffer->PushFloat(fov);
            g_RenderBuffer->PushFloat(aspectratio);
            g_RenderBuffer->PushFloat(nearplane);
            g_RenderBuffer->PushFloat(farplane);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_MIPMAPBIAS:
            g_RenderBuffer->PushFloat(g_GfxInternal_Dx9->m_MipMapBias);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_MIPMAPPING:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_MipMappingEnabled);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_TEXTUREADDRESSMODE:
            g_RenderBuffer->PushInt(g_GfxInternal_Dx9->m_TextureAddressModes[0]);
            g_RenderBuffer->PushInt(g_GfxInternal_Dx9->m_TextureAddressModes[1]);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_RENDERTARGET:
            g_RenderBuffer->PushInt((int)g_GfxInternal_Dx9->m_RenderTarget);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ALPHACHANNEL:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_AlphaChannelEnabled);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_SETALPHATESTTHRESHHOLD:
            g_RenderBuffer->PushFloat(g_GfxInternal_Dx9->m_AlphaTestThreshhold);
            break;
        case RenderBuffer::RenderCommand::CMD_PUSH_ENABLEALPHATEST:
            g_RenderBuffer->PushBool(g_GfxInternal_Dx9->m_AlphaTestEnabled);
            break;
        case RenderBuffer::RenderCommand::CMD_POP_MODELMATRIX:
            {
                DirectX::XMMATRIX mat;
                g_RenderBuffer->PopMatrix(mat);
                g_GfxInternal_Dx9->SetWorldMatrix(&mat);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_TEXTURE:
            {
                int tex1, tex2;
                g_RenderBuffer->PopInt(tex1);
                g_RenderBuffer->PopInt(tex2);

                g_GfxInternal_Dx9->SetTextureIndex((Texture*)tex1, 1);
                g_GfxInternal_Dx9->SetTextureIndex((Texture*)tex2, 0);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ENVIRONMENTMAP:
            {
                bool b;
                g_RenderBuffer->PopBool(b);
                g_GfxInternal_Dx9->ToggleEnvironmentMap(b);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ENVIRONMENTMAPCOEF:
            {
                float c;
                g_RenderBuffer->PopFloat(c);
                g_GfxInternal_Dx9->SetEnvironmentMapCoef(c);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ENABLELIGHTING:
            {
                bool b;
                g_RenderBuffer->PopBool(b);
                g_GfxInternal_Dx9->EnableLighting(b);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ENABLELIGHT:
            {
                void* l = (void*)buf.m_ParamsArray[buf.m_PrevParamIndex++];
                bool b;
                g_RenderBuffer->PopBool(b);
                g_GfxInternal_Dx9->ToggleLight((Light_Properties*)l, b);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ZWRITE:
            {
                bool zw;
                g_RenderBuffer->PopBool(zw);
                g_GfxInternal_Dx9->EnableZWrite(zw);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ZTEST:
            {
                bool zt;
                g_RenderBuffer->PopBool(zt);
                g_GfxInternal_Dx9->EnableZTest(zt);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_FOG:
            {
                bool fe;
                g_RenderBuffer->PopBool(fe);
                if (g_GfxInternal_Dx9->m_RenderingScene)
                    g_GfxInternal_Dx9->EnableFog(fe);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_FOGPROPERTIES:
            {
                float fe, fs;
                int fm;
                ColorRGB fc;

                g_RenderBuffer->PopFloat(fe);
                g_RenderBuffer->PopFloat(fs);
                g_RenderBuffer->PopColor(fc);
                g_RenderBuffer->PopInt(fm);

                if (g_GfxInternal_Dx9->m_RenderingScene)
                    g_GfxInternal_Dx9->SetFogProperties(fm, fc, fs, fe, 0);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_FILTER:
            {
                int m;
                g_RenderBuffer->PopInt(m);

                g_GfxInternal_Dx9->SetFilterMode(m);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_CULLMODE:
            {
                int cm;
                g_RenderBuffer->PopInt(cm);

                g_GfxInternal_Dx9->SetCullMode(cm);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ZBIAS:
            {
                unsigned int zb;
                g_RenderBuffer->PopInt(zb);

                g_GfxInternal_Dx9->SetZBias(zb);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_BLENDMODE:
            {
                unsigned int bm;
                g_RenderBuffer->PopInt(bm);

                g_GfxInternal_Dx9->SetBlendMode(bm);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_OPACITY:
            {
                float op;
                g_RenderBuffer->PopFloat(op);
                g_GfxInternal_Dx9->SetEnvironmentMapOpacity(op);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_VIEWPORT:
            {
                ScreenResolution sr1, sr2;
                g_RenderBuffer->PopVector2i(sr1);
                g_RenderBuffer->PopVector2i(sr2);

                g_GfxInternal_Dx9->SetViewport(sr2, sr1);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_PROJECTION:
            {
                float fov, aspectratio, nearplane, farplane;
                g_RenderBuffer->PopFloat(farplane);
                g_RenderBuffer->PopFloat(nearplane);
                g_RenderBuffer->PopFloat(aspectratio);
                g_RenderBuffer->PopFloat(fov);

                g_GfxInternal_Dx9->SetProjection(fov, aspectratio, nearplane, farplane);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_MIPMAPBIAS:
            {
                float mb;
                g_RenderBuffer->PopFloat(mb);
                g_GfxInternal_Dx9->SetMipMapBias(mb);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_MIPMAPPING:
            {
                bool me;
                g_RenderBuffer->PopBool(me);
                g_GfxInternal_Dx9->SetMipMapping(me);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_TEXTUREADDRESSMODE:
            {
                unsigned int am1, am2;
                g_RenderBuffer->PopInt(am1);
                g_RenderBuffer->PopInt(am2);

                g_GfxInternal_Dx9->SetTextureAddressMode(am1, 1);
                g_GfxInternal_Dx9->SetTextureAddressMode(am2, 0);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_RENDERTARGET:
            {
                int rt;
                g_RenderBuffer->PopInt(rt);
                g_GfxInternal_Dx9->SetRenderTarget((Texture*)rt);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ALPHACHANNEL:
            {
                bool ac;
                g_RenderBuffer->PopBool(ac);
                g_GfxInternal_Dx9->EnableAlphaChannel(ac);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_SETALPHATESTTHRESHHOLD:
            {
                float att;
                g_RenderBuffer->PopFloat(att);
                g_GfxInternal_Dx9->SetAlphaTestThreshold(att);
            }
            break;
        case RenderBuffer::RenderCommand::CMD_POP_ENABLEALPHATEST:
            {
                bool alte;
                g_RenderBuffer->PopBool(alte);
                g_GfxInternal_Dx9->EnableAlphaTest(alte);
            }
            break;
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
