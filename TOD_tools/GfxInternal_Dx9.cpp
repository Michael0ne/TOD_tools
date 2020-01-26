#include "GfxInternal_Dx9.h"

GfxInternal_Dx9::GfxInternal_Dx9()
{
	debug("Renderer created...\n");
}

inline void PATCH_RENDERER()
{
	//	Override constructor for Renderer class.
}