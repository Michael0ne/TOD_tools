#pragma once

#include "stdafx.h"

class GfxInternal_Dx9_Texture
{
private:
	int field_0;
	IDirect3DTexture9* m_pTexture;
	int field_8;
	int field_C;
	int field_10;
	int m_nWidth;
	int m_nHeight;
	int field_1C;
	int field_20;
	int m_nLevels;
	int field_28;
};

static_assert(sizeof(GfxInternal_Dx9_Texture) == 0x2C, MESSAGE_WRONG_CLASS_SIZE("GfxInternal_Dx9_Texture"));