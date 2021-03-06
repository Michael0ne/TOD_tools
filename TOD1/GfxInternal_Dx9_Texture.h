#pragma once
#include "stdafx.h"
#include "Types.h"
#include <map>

#define GFX_TEXTURE_CLASS_SIZE 44

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

private:
	LPDIRECT3DSURFACE9	m_Direct3DSurface;
	LPDIRECT3DTEXTURE9	m_Texture;
	char*				m_TextureSurfaceBits;
	int					m_Width;
	int					m_Height;
	int					m_SurfaceWidth;
	int					m_SurfaceHeight;
	TextureFormat		m_Format;
	int					field_20;
	unsigned short		m_Levels;
	int					m_MipMapLevels;

	/*
	* NOTE: this is used by GfxInternal_Dx9 class, an extension to what is already above. Probably another class, contains 'unpacked' texture information.
	String				m_String_1;
	String				m_String_2;
	int					field_40;
	int					field_44;
	int					field_48;
	int					field_4C;
	short				field_50;
	int					field_54;
	int					field_58;
	int					field_5C;
	int					field_60;
	int					field_64;
	int					field_68;
	int					field_6C;
	int					field_70;
	ResType::Texture	m_TextureResource;
	*/

public:
	GfxInternal_Dx9_Texture(const Vector2<float>& resolution, unsigned int, unsigned int levels);	//	@463BA0
	GfxInternal_Dx9_Texture(void*);	//	@463C60
	~GfxInternal_Dx9_Texture();

	static void			DrawAllTextures();	//	@463850
	static void			InitTexturesMap();	//	@464120

	static std::map<unsigned int, GfxInternal_Dx9_Texture>*	TexturesMap;	//	@A39F50
};

static_assert(sizeof(GfxInternal_Dx9_Texture) == GFX_TEXTURE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GfxInternal_Dx9_Texture));