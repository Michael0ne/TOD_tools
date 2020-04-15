#pragma once

class String;

//	TODO: maybe put these into different namespaces?

static HINSTANCE& g_hInstance = *(HINSTANCE*)0xA35EB0;
static LPSTR& g_CmdLine = *(LPSTR*)0xA35EB4;

static DWORD* StringResourceBaseAddr = (DWORD*)0xA35E28;
static UINT* uType = (UINT*)0xA091A8;
static STICKYKEYS* sSTICKYKEYS = (STICKYKEYS*)0xA0917C;
static TOGGLEKEYS* sTOGGLEKEYS = (TOGGLEKEYS*)0xA09184;
static FILTERKEYS* sFILTERKEYS = (FILTERKEYS*)0xA0918C;

static LPCSTR& RegistryKey = *(LPCSTR*)0xA09178;

static int& g_ZipSlotId = *(int*)0xA35DDC;
static char* g_ZipStatus[16] = {
	(char*)0xA0862C,
	(char*)0xA0862D,
	(char*)0xA0862E,
	(char*)0xA0862F,
	(char*)0xA08630,
	(char*)0xA08631,
	(char*)0xA08632,
	(char*)0xA08633,
	(char*)0xA08634,
	(char*)0xA08635,
	(char*)0xA08636,
	(char*)0xA08637,
	(char*)0xA08638,
	(char*)0xA08639,
	(char*)0xA0863A,
	(char*)0xA0863B
};
static String* g_ZipNames[8] = {
	(String*)0xA085A8,
	(String*)0xA085B8,
	(String*)0xA085C8,
	(String*)0xA085D8,
	(String*)0xA085E8,
	(String*)0xA085F8,
	(String*)0xA08608,
	(String*)0xA08618
};

namespace Utils
{
	static bool(__cdecl* FindFileEverywhere)(const char* szPath) = (bool(__cdecl*)(const char*))0x4182A0;
	static int(__cdecl* CalcCRC32)(const char* str, unsigned int strlength) = (int(__cdecl*)(const char*, unsigned int))0x4657C0;
	static bool(__cdecl* IsFileAvailable)(const char* szPath) = (bool(__cdecl*)(const char*))0x418B00;
	static void(__cdecl* CreateDirectoriesRecursive)(const char* szPath) = (void(__cdecl*)(const char*))0x4096E0;
	static void(__cdecl* GetFileAbsolutePath)(String* outStr) = (void(__cdecl*)(String*))0x437A70;	//	Also, at 437B80
	static void(__cdecl* SetWarningString)(const char* str) = (void(__cdecl*)(const char*))0x406E00;
	static const char* (__cdecl* GetErrorCodeDescription)(unsigned int code) = (const char* (__cdecl*)(unsigned int))0x45BF32;
}

namespace Control
{
	static void* (__cdecl* GetGamepadByIndex)(signed int index) = (void* (__cdecl*)(signed int))0x439660;
}