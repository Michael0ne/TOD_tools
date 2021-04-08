#include "GfxInternal_Dx9_Texture.h"
#include "LogDump.h"
#include "GfxInternal_Dx9.h"

std::map<unsigned int, GfxInternal_Dx9_Texture>* GfxInternal_Dx9_Texture::TexturesMap;
D3DFORMAT GfxInternal_Dx9_Texture::SupportedTextureFormats[] = {D3DFMT_R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, D3DFMT_A4R4G4B4, D3DFMT_P8};
unsigned int GfxInternal_Dx9_Texture::_A08980[] = { 0x180888, 0x208888, 0x100565, 0x101555, 0x104444, 0x8088888, 0x8048888, 0x40000, 0, 0, 0, 0, 0x208888, 0x100565, 0 };

void GfxInternal_Dx9_Texture::CreateDirect3DTexture(const ScreenResolution& res, unsigned int formatindex, unsigned int levels)
{
	if (levels == 4 || levels == 3)
	{
		m_SurfaceResolution = res;
	}
	else
	{
		m_SurfaceResolution = { 1, 1 };

		if (res.x > 1)
			for (; m_SurfaceResolution.x < res.x; m_SurfaceResolution.x *= 2);
		if (res.y > 1)
			for (; m_SurfaceResolution.y < res.y; m_SurfaceResolution.y *= 2);
	}

	m_MipMapLevels = levels;
	m_Levels = m_Levels & 0xFE03 | 2;

	g_GfxInternal_Dx9->m_Direct3DDevice->CreateTexture(
		m_SurfaceResolution.x,
		m_SurfaceResolution.y,
		levels != 1,
		levels == 3 ? 1 : 0,
		SupportedTextureFormats[formatindex],
		levels ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT,
		&m_Texture,
		nullptr);
	
	D3DSURFACE_DESC surfdesc;
	m_Texture->GetLevelDesc(0, &surfdesc);

	m_SurfaceResolution = { surfdesc.Width, surfdesc.Height };
	
	switch (surfdesc.Format)
	{
	case D3DFMT_DXT5:
		m_Format = DXT5;
		break;
	case D3DFMT_DXT4:
		m_Format = DXT4;
		break;
	case D3DFMT_DXT3:
		m_Format = DXT3;
		break;
	case D3DFMT_DXT2:
		m_Format = DXT2;
		break;
	case D3DFMT_DXT1:
		m_Format = DXT1;
		break;
	}

	m_Resolution = res;
	field_20 = _A08980[m_Format];
	*(int*)&m_Format = formatindex;
}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9_Texture::GfxInternal_Dx9_Texture(const ScreenResolution& resolution, unsigned int, unsigned int levels)
{
	MESSAGE_CLASS_CREATED(GfxInternal_Dx9_Texture);

	m_TextureInfo = nullptr;
	m_Texture = nullptr;
	m_TextureSurfaceBits = nullptr;
	m_Levels = 0xFDFE;
	CreateDirect3DTexture(resolution, 1, levels);
	GfxInternal_Dx9::RenderedTexturesMap.emplace(this);
}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9_Texture::GfxInternal_Dx9_Texture(void*)
{}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9_Texture::~GfxInternal_Dx9_Texture()
{
}

unsigned int GfxInternal_Dx9_Texture::GetSizeForLevel(const unsigned char lvl) const
{
	unsigned int widthn = m_SurfaceResolution.x >> lvl;
	unsigned int heightn = m_SurfaceResolution.y >> lvl;

	if (widthn < 1) widthn = 1;
	if (heightn < 1) heightn = 1;

	if (!(m_Levels & 1))
		return (widthn * heightn * (*((char*)&field_20 + 2) & 63)) >> 3;
	else
		return (8 * (widthn / 4 >= 1 ? widthn / 4 : 1) * (heightn / 4 >= 1 ? heightn : 1));
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9_Texture::DrawAllTextures()
{
	unsigned int memoryusage = 0;
	LogDump::LogA("rendering a frame with all (%i) textures!\n", TexturesMap->size());

	if (TexturesMap->cbegin() != TexturesMap->cend())
	{
		for (std::map<unsigned int, GfxInternal_Dx9_Texture>::const_iterator it = TexturesMap->cbegin(); it != TexturesMap->cend(); ++it)
		{
			g_GfxInternal_Dx9->RenderTexturedQuad2D_1(it->second, {}, { 2.f, 2.f }, { 1.f, 1.f, 1.f, 1.f });
			unsigned int levelsize = 0;
			unsigned int texlevel = 0;

			for (unsigned int level = (unsigned int)it->second.m_Levels >> 1; texlevel < level; levelsize += it->second.GetSizeForLevel(texlevel++));
			memoryusage += texlevel;
		}
	}

	LogDump::LogA("Current texture memory usage = %i\n", memoryusage);
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9_Texture::InitTexturesMap()
{
	TexturesMap = new std::map<unsigned int, GfxInternal_Dx9_Texture>;	//	TODO: who is responsible to delete this?
}