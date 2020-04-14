#include "GfxInternal_Dx9.h"

GfxInternal_Dx9* g_RendererDx = NULL;

Map<int, int>& GfxInternal_Dx9::g_UnkMap_1 = *(Map<int, int>*)0xA39F58;
Map<int, int>& GfxInternal_Dx9::g_UnkMap_2 = *(Map<int, int>*)0xA39F38;
Map<int, int>& GfxInternal_Dx9::g_RenderedTexturesMap = *(Map<int, int>*)0xA39F50;
void* GfxInternal_Dx9::g_RenderBuffer = (void*)0xA35E60;

void GfxInternal_Dx9::Init(void* resolution, int unk1, int unk2, int fsaa, int unk3)
{
	patch(0xA39F14, this, 4);

	(*(void(__thiscall*)(GfxInternal_Dx9*, void*, int, int, int, int))0x45E620)(this, resolution, unk1, unk2, fsaa, unk3);
}

inline void PATCH_RENDERER()
{
}