#pragma once

#include "stdafx.h"

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *-------------- Main wrapper for Direct3D9 ------------------
 *------------------------------------------------------------
 *----- Really huge, will take a long time to reverse --------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class GfxInternal_Dx9
{
	//	Size is 0x97A0 (38816) bytes. Huge!
public:
	IDirect3DDevice9 *m_pDirect3DDevice9;
	IDirect3DQuery9 *m_pFramesyncQuery;
	BYTE _pad1[16];
	int m_pDisplayModes;
	int m_nNumDisplayModes;
	BYTE _pad1c[2];
	BYTE _pad2[15];
	BYTE _pad2a[14];
	BYTE m_bRangeFogEnabled;
	IDirect3D9 *m_pDirect3D9Interface;
	D3DCAPS9 m_pDeviceCaps;
	int _pad4;
	D3DPRESENT_PARAMETERS m_pD3DPRESENT_PARAMETERS;
	BYTE _pad5[200];
	int m_nResolutionWidth;
	int m_nResolutionHeight;
	D3DDISPLAYMODE m_DisplayMode;
	BYTE _pad6[128];
	BYTE m_bResolutionDetected;
	BYTE m_bSceneBegan;
	BYTE _pad8[2];
	int m_BackBufferFormat;
	BYTE _pad9[14];
	D3DDISPLAYMODE m_DisplayModeActual;
	BYTE _pad10[10000];
	BYTE _pad11[10000];
	BYTE _pad12[1020];
	int *m_pUnkBuff1;
	int *m_pUnkBuff2;
	int *m_pUnkBuff3;
	BYTE _pad13[8968];
	BYTE _pad14[7872];
	IDirect3DSurface9 *m_pSurface;
	BYTE _pad15[116];

public:
	GfxInternal_Dx9();
};

