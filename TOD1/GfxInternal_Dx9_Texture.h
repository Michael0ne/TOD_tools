﻿#pragma once
#include "stdafx.h"
#include "Types.h"
#include "GfxInternal_Dx9_Surface.h"
#include <map>

class GfxInternal_Dx9_Texture
{
    enum TextureFormat
    {
        DXT1 = 7,
        DXT2 = 8,
        DXT3 = 9,
        DXT4 = 10,
        DXT5 = 11
    };

    struct Texture
    {
        int             field_0;
        int             field_4;
        int             m_Levels;
    };

public:
    Texture             *m_Tex;
    LPDIRECT3DTEXTURE9  m_Texture;
    char               *m_TextureSurfaceBits;
    ScreenResolution    m_Resolution;
    ScreenResolution    m_SurfaceSize;
    TextureFormat       m_Format;
    int                 field_20;
    unsigned short      m_Levels;
    int                 m_MipMapLevels;

private:
    void                CreateDirect3DTexture(const ScreenResolution& res, unsigned int formatindex, unsigned int levels);	//	@463530

public:
    GfxInternal_Dx9_Texture(const ScreenResolution& resolution, unsigned int, unsigned int levels);	//	@463BA0
    GfxInternal_Dx9_Texture(void*);	//	@463C60
    ~GfxInternal_Dx9_Texture();

    unsigned int        GetSizeForLevel(const unsigned char lvl) const;	//	@4633D0
    void                SetTextureForStage(const unsigned int stage) const; //  @4634B0

    static void         DrawAllTextures();	//	@463850
    static void         InitTexturesMap();	//	@464120

    static std::map<unsigned int, GfxInternal_Dx9_Texture>*	TexturesMap;	//	@A39F50
    static D3DFORMAT	SupportedTextureFormats[];	//	@A0A5B0
    static unsigned int	_A08980[15];	//	@A08980
};

ASSERT_CLASS_SIZE(GfxInternal_Dx9_Texture, 44);