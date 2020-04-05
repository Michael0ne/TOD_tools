#pragma once

//	TODO: maybe put these into different namespaces?

static HINSTANCE& g_hInstance = *(HINSTANCE*)0xA35EB0;
static LPSTR& g_CmdLine = *(LPSTR*)0xA35EB4;

static DWORD* StringResourceBaseAddr = (DWORD*)0xA35E28;
static UINT* uType = (UINT*)0xA091A8;
static STICKYKEYS* sSTICKYKEYS = (STICKYKEYS*)0xA0917C;
static TOGGLEKEYS* sTOGGLEKEYS = (TOGGLEKEYS*)0xA09184;
static FILTERKEYS* sFILTERKEYS = (FILTERKEYS*)0xA0918C;

namespace Utils
{
	static bool(__cdecl* FindFileEverywhere)(const char* szPath) = (bool(__cdecl*)(const char*))0x4182A0;
}