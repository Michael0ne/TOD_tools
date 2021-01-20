#include "GfxInternal_Dx9_Texture.h"

std::map<unsigned int, GfxInternal_Dx9_Texture>* GfxInternal_Dx9_Texture::TexturesMap;

#pragma message(TODO_IMPLEMENTATION)
void GfxInternal_Dx9_Texture::InitTexturesMap()
{
	TexturesMap = new std::map<unsigned int, GfxInternal_Dx9_Texture>;	//	TODO: who is responsible to delete this?
}