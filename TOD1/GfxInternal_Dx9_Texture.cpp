#include "GfxInternal_Dx9_Texture.h"
#include "LogDump.h"
#include "GfxInternal_Dx9.h"

std::map<unsigned int, GfxInternal_Dx9_Texture>* GfxInternal_Dx9_Texture::TexturesMap;

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9_Texture::GfxInternal_Dx9_Texture(const Vector2<float>& resolution, unsigned int, unsigned int levels)
{}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9_Texture::GfxInternal_Dx9_Texture(void*)
{}

#pragma message(TODO_IMPLEMENTATION)
GfxInternal_Dx9_Texture::~GfxInternal_Dx9_Texture()
{
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9_Texture::DrawAllTextures()
{
	/*
	unsigned int memUsage = NULL;

	LogDump::LogA("rendering a frame with all (%i) textures!\n", TexturesMap->size());

	if (TexturesMap->cbegin() != TexturesMap->cend())
	{
		for (unsigned int i = 0; i < TexturesMap->size(); i++)
		{
			const GfxInternal_Dx9_Texture& tex = (GfxInternal_Dx9_Texture&)TexturesMap[i];
			Vector2f size_1(2.f, 2.f);
			Vector2f size_2;
			ColorRGB clr(1.f, 1.f, 1.f, 1.f);

			g_RendererDx->RenderTexturedQuad2D_1(&tex, size_1, size_2, clr);

			unsigned int texSize = NULL, stageSize = NULL;

			for (unsigned int j = tex.m_Levels >> 1; stageSize < j; texSize += tex.GetSizeForLevel(stageSize++));

			memUsage += texSize;
		}
	}

	LogDump::LogA("Current texture memory usage = %i\n", memUsage);
	*/
}

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9_Texture::InitTexturesMap()
{
	TexturesMap = new std::map<unsigned int, GfxInternal_Dx9_Texture>;	//	TODO: who is responsible to delete this?
}