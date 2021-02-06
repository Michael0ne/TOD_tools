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
	int					field_0;
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

public:
	GfxInternal_Dx9_Texture(const Vector2<float>& resolution, unsigned int, unsigned int levels);	//	@463BA0
	GfxInternal_Dx9_Texture(void*);	//	@463C60
	~GfxInternal_Dx9_Texture();

	static void			DrawAllTextures();	//	@463850
	static void			InitTexturesMap();	//	@464120

	static std::map<unsigned int, GfxInternal_Dx9_Texture>*	TexturesMap;	//	@A39F50
};

static_assert(sizeof(GfxInternal_Dx9_Texture) == GFX_TEXTURE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GfxInternal_Dx9_Texture));