#pragma once
#include "Types.h"
#include "MemoryManager.h"
#include "directxmath/include/DirectXMath.h"

#define RENDERBUFFER_DEFAULT_BUFFER_SIZE 128
#define RENDERBUFFER_MAX_PARAMS_SIZE 250

class RenderBuffer
{
    friend class FrameBuffer;
public:
    enum RenderCommand
    {
        CMD_SETMODELMATRIX = 0,
        CMD_SETCURRENTTEXTURE,
        CMD_SETTEXTURESCROLL,
        CMD_ENABLEENVIRONMENTMAP,
        CMD_SETENVIRONMENTMAPCOEF,
        CMD_SETBRIGHTNESS,
        CMD_RENDERTRIANGLE,
        CMD_RENDERTRIANGLE_2,
        CMD_RENDERTEXTUREDTRIANGLE,
        CMD_RENDERQUAD,
        CMD_RENDERLINE,
        CMD_RENDERLINE2D,
        CMD_RENDERTRIANGLE2D,
        CMD_RENDERQUAD2D,
        CMD_RENDERQUAD2D_2,
        CMD_RENDERFULLSCREENTEXTURE,
        CMD_RENDERVERTEXBUFFER,
        CMD_RENDERMESHBUFFER,
        CMD_RENDERMESHCOLORBUFFER,
        CMD_STARTCHARACTERRENDER,
        CMD_ENDCHARACTERRENDER,
        CMD_RENDERSKINNEDMESHBUFFER,
        CMD_SETBONEMATRIX,
        CMD_BEGINTEXT,
        CMD_RENDERTEXT,
        CMD_RENDERTEXT2,
        CMD_ENDTEXT,
        CMD_BEGINSHADOW,
        CMD_RENDERSHADOW,
        CMD_ENDSHADOW,
        CMD_CALL,
        CMD_ENABLELIGHTING,
        CMD_ENABLELIGHT,
        CMD_ENABLEZWRITE,
        CMD_ENABLEZTEST,
        CMD_ENABLEFOG,
        CMD_SETFOGPROPERTIES,
        CMD_SETFILTERMODE,
        CMD_SETCULLMODE,
        CMD_SETZBIAS,
        CMD_SETBLENDMODE,
        CMD_SETOPACITY,
        CMD_SETVIEWPORT,
        CMD_SETPROJECTION,
        CMD_SETMIPMAPBIAS,
        CMD_ENABLEMIPMAPPING,
        CMD_SETTEXTUREADDRESSMODE,
        CMD_SETRENDERTARGET,
        CMD_RENDERTEXTUREDQUAD2D_1,
        CMD_RENDERTEXTUREDQUAD2D_2,
        CMD_RENDERTEXTUREDQUAD2D_3,
        CMD_RENDERTEXTUREDQUAD2D_4,
        CMD_RENDERTEXTUREDQUAD_1,
        CMD_RENDERTEXTUREDQUAD_2,
        CMD_RENDERTEXTUREDQUAD_3,
        CMD_ENABLEALPHACHANNEL,
        CMD_SETALPHATESTTHRESHHOLD,
        CMD_ENABLEALPHATEST,
        CMD_PS2_CALLDMA,
        CMD_PS2_UPLOADVUCODE,
        CMD_PS2_RELOADCAMERAMATRIX,
        CMD_PS2_SETGUARDBAND,
        CMD_SETAXISALIGN,
        CMD_SETPARTICLE,
        CMD_BEGINPARTICLESYSTEM,
        CMD_ENDPARTICLESYSTEM,
        CMD_SETPARTICLESIZE,
        CMD_SETPARTICLEORIENT,
        CMD_RENDERPARTICLE,
        CMD_SETPARTICLEALIGN,
        CMD_SETPARTICLECOLOR,
        CMD_SETPARTICLEOPACITY,
        CMD_SETMIPMAPLEVELS,
        CMD_DRAWBRIGHTNESS,
        CMD_DRAWSATURATION,
        CMD_DRAWLIGHTBLEEDING,
        CMD_DRAWVIGNETTE,
        CMD_DRAWNOISE,
        CMD_PUSH_MODELMATRIX,
        CMD_PUSH_TEXTURE,
        CMD_PUSH_ENVIRONMENTMAP,
        CMD_PUSH_ENVIRONMENTMAPCOEF,
        CMD_PUSH_BRIGHTNESS,
        CMD_PUSH_ENABLELIGHTING,
        CMD_PUSH_ENABLELIGHT,
        CMD_PUSH_ZWRITE,
        CMD_PUSH_ZTEST,
        CMD_PUSH_FOG,
        CMD_PUSH_FOGPROPERTIES,
        CMD_PUSH_FILTER,
        CMD_PUSH_CULLMODE,
        CMD_PUSH_ZBIAS,
        CMD_PUSH_BLENDMODE,
        CMD_PUSH_OPACITY,
        CMD_PUSH_VIEWPORT,
        CMD_PUSH_PROJECTION,
        CMD_PUSH_MIPMAPBIAS,
        CMD_PUSH_MIPMAPPING,
        CMD_PUSH_TEXTUREADDRESSMODE,
        CMD_PUSH_RENDERTARGET,
        CMD_PUSH_ALPHACHANNEL,
        CMD_PUSH_SETALPHATESTTHRESHHOLD,
        CMD_PUSH_ENABLEALPHATEST,
        CMD_PUSH_PS2_SETGUARDBAND,
        CMD_POP_MODELMATRIX,
        CMD_POP_TEXTURE,
        CMD_POP_ENVIRONMENTMAP,
        CMD_POP_ENVIRONMENTMAPCOEF,
        CMD_POP_BRIGHTNESS,
        CMD_POP_ENABLELIGHTING,
        CMD_POP_ENABLELIGHT,
        CMD_POP_ZWRITE,
        CMD_POP_ZTEST,
        CMD_POP_FOG,
        CMD_POP_FOGPROPERTIES,
        CMD_POP_FILTER,
        CMD_POP_CULLMODE,
        CMD_POP_ZBIAS,
        CMD_POP_BLENDMODE,
        CMD_POP_OPACITY,
        CMD_POP_VIEWPORT,
        CMD_POP_PROJECTION,
        CMD_POP_MIPMAPBIAS,
        CMD_POP_MIPMAPPING,
        CMD_POP_TEXTUREADDRESSMODE,
        CMD_POP_RENDERTARGET,
        CMD_POP_ALPHACHANNEL,
        CMD_POP_SETALPHATESTTHRESHHOLD,
        CMD_POP_ENABLEALPHATEST,
        CMD_POP_PS2_SETGUARDBAND
    };

public:
    unsigned int    m_MaxParams;
    int            *m_ParamsArray;  //  NOTE: parameter at index 0 is always a 'RenderCommand' enum id.
    unsigned int    m_CurrentParamIndex;
    unsigned int    m_PrevParamIndex;
    AllocatorIndex  m_AllocatorId;

protected:
    void            SetBufferSize(const unsigned int size);	//	@415350

public:
    RenderBuffer(const unsigned int maxParams, AllocatorIndex allocatorType);	//	@415400
    RenderBuffer() {};

    void            AdjustBufferSize(const unsigned int size);	//	@415510
    template <typename T>
    inline void     push_back(T val)	//	NOTE: this does not exist in original code as-is, because it's always inlined.
    {
        //  TODO: unoptimised variant. In most cases we know how much parameters are being submitted, re-write and take it into account.
        if (m_MaxParams < m_CurrentParamIndex + 1)
            AdjustBufferSize(m_CurrentParamIndex + 1);

        *(T*)&m_ParamsArray[m_CurrentParamIndex++] = val;
    }
    void            PopMatrix(DirectX::XMMATRIX& mat); //  @431430
    void            PopVector2i(Vector2<int>& vec);    //  @431400
    void            PopVector2i(Vector2<unsigned int>& vec);
    void            PopVector2f(Vector2f& vec); //  @42E870
    void            PopVector3f(Vector3f& vec); //  @42E770
    void            PopVector4f(Vector4f& vec); //  @431390
    inline void     PopInt(int& i)
    {
        m_PrevParamIndex = m_CurrentParamIndex - 1;
        i = m_ParamsArray[m_PrevParamIndex];
        m_PrevParamIndex = m_CurrentParamIndex;
        m_CurrentParamIndex--;
    };
    inline void     PopInt(unsigned int& i)
    {
        m_PrevParamIndex = m_CurrentParamIndex - 1;
        i = *(unsigned int*)&m_ParamsArray[m_PrevParamIndex];
        m_PrevParamIndex = m_CurrentParamIndex;
        m_CurrentParamIndex--;
    }
    void            PopFloat(float& f); //  @431320
    void            PopBool(bool& b);   //  @431290
    void            PopQuaternion(Orientation& q);    //  @430CD0
    void            PopColor(ColorRGB& clr); //  @42E700

    void            PushColor(const ColorRGB& clr);  //  @431340
    void            PushFloat(const float& f);  //  @4312F0
    void            PushInt(const int& i);  //  @4312C0
    void            PushBool(const bool& b);    //  @431260
    void            PushVector2i(const Vector2<int>& vec);  //  @431200
    void            PushVector2i(const ScreenResolution& vec);
    void            PushVector2f(const Vector2f& vec);  //  @431180
    void            PushMatrix(const DirectX::XMMATRIX& mat, const unsigned int ind);   //  @430F80
    void            PushModelMatrix(const DirectX::XMMATRIX& mat);  //  @430D60

    static void     CreateRenderBuffer();	//	@436070

    static int      Buffer[RENDERBUFFER_DEFAULT_BUFFER_SIZE];	//	@A35B98
    static int      MeshBuffersDrawn;   //  @A35E64
};

extern RenderBuffer*    g_RenderBuffer;	//	@A35E60

ASSERT_CLASS_SIZE(RenderBuffer, 20);