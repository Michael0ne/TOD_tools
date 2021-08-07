#pragma once
#include "stdafx.h"
#include "Types.h"
#include "MutableTextureBuffer.h"
#include <map>
#include <vector>

#pragma pack(4)
class Texture
{
    enum TextureFormat
    {
        DXT1 = 7,
        DXT2 = 8,
        DXT3 = 9,
        DXT4 = 10,
        DXT5 = 11
    };

public:
    MutableTextureBuffer     *m_Mutable;
    LPDIRECT3DTEXTURE9  m_Texture;
    char               *m_TextureSurfaceBits;
    ScreenResolution    m_Resolution;
    ScreenResolution    m_SurfaceSize;
    TextureFormat       m_Format;
    char                field_20;
    char                field_21;
    char                m_BitsPerPixel;
    char                field_23;
    unsigned short      m_Levels;
    unsigned short      field_26;   //  NOTE: not used, only here for alignment.
    unsigned char       m_MipMapLevels;

private:
    void                CreateDirect3DTexture(const ScreenResolution& res, unsigned int formatindex, unsigned int levels);	//	@463530

    static TextureFormat    GetTextureFormat(D3DFORMAT fmt);   //  @463360

    static int          Formats[];  //  @A0A5F0

public:
    Texture(const ScreenResolution& resolution, unsigned int, unsigned int levels);	//	@463BA0
    Texture(MutableTextureBuffer* surf);	//	@463C60
    ~Texture(); //  @463D20

    unsigned int        GetSizeForLevel(const unsigned char lvl) const;	//	@4633D0
    void                SetTextureForStage(const unsigned int stage) const; //  @4634B0
    void                AdjustColors(); //  @463970
    const unsigned int  GetTextureSizeBytes() const;    //  @463450
    LPDIRECT3DSURFACE9  GetSurfaceLevel(const unsigned int level) const;    //  @463480
    LPDIRECT3DTEXTURE9  GetDirect3DTexture() const; //  @4634A0
    bool                FillTextureInfo();  //  @4634D0

    static void         DrawAllTextures();	//	@463850
    static void         InitTexturesMap();	//	@464120

    static std::map<unsigned int, Texture*>*	TexturesMap;	//	@A39F50
    static D3DFORMAT	SupportedTextureFormats[];	//	@A0A5B0
    static unsigned int	_A08980[15];	//	@A08980
};

ASSERT_CLASS_SIZE(Texture, 44);