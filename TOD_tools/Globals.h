#pragma once

//	TODO: maybe put these into different namespaces?

static HINSTANCE& g_hInstance = *(HINSTANCE*)0xA35EB0;
static LPSTR& g_CmdLine = *(LPSTR*)0xA35EB4;

static DWORD* StringResourceBaseAddr = (DWORD*)0xA35E28;
static UINT* uType = (UINT*)0xA091A8;
static STICKYKEYS* sSTICKYKEYS = (STICKYKEYS*)0xA0917C;
static TOGGLEKEYS* sTOGGLEKEYS = (TOGGLEKEYS*)0xA09184;
static FILTERKEYS* sFILTERKEYS = (FILTERKEYS*)0xA0918C;

static LPCSTR& RegistryKey = *(LPCSTR*)0xA09178;

namespace Utils
{
	static bool(__cdecl* FindFileEverywhere)(const char* szPath) = (bool(__cdecl*)(const char*))0x4182A0;
	static int(__cdecl* CalcCRC32)(const char* str, unsigned int strlength) = (int(__cdecl*)(const char*, unsigned int))0x4657C0;
	static bool(__cdecl* IsFileAvailable)(const char* szPath) = (bool(__cdecl*)(const char*))0x418B00;
	static void(__cdecl* CreateDirectoriesRecursive)(const char* szPath) = (void(__cdecl*)(const char*))0x4096E0;
}

namespace Control
{
	static void* (__cdecl* GetGamepadByIndex)(signed int index) = (void* (__cdecl*)(signed int))0x439660;
}