#include "GfxInternal_Dx9.h"

GfxInternal_Dx9* g_RendererDx = NULL;

Map<int, int>& GfxInternal_Dx9::g_UnkMap_1 = *(Map<int, int>*)0xA39F58;
Map<int, int>& GfxInternal_Dx9::g_UnkMap_2 = *(Map<int, int>*)0xA39F38;
Map<int, int>& GfxInternal_Dx9::g_RenderedTexturesMap = *(Map<int, int>*)0xA39F50;
void* GfxInternal_Dx9::g_RenderBuffer = (void*)0xA35E60;

inline void PATCH_RENDERER()
{
}