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

namespace Utils
{
	static bool(__cdecl* FindFileEverywhere)(const char* szPath) = (bool(__cdecl*)(const char*))0x4182A0;
	static int(__cdecl* CalcCRC32)(const char* str, unsigned int strlength) = (int(__cdecl*)(const char*, unsigned int))0x4657C0;
	static bool(__cdecl* IsFileAvailable)(const char* szPath) = (bool(__cdecl*)(const char*))0x418B00;
	static void(__cdecl* CreateDirectoriesRecursive)(const char* szPath) = (void(__cdecl*)(const char*))0x4096E0;
	static void(__cdecl* GetFileAbsolutePath)(String* outStr) = (void(__cdecl*)(String*))0x437A70;	//	Also, at 437B80
	static void(__cdecl* SetWarningString)(const char* str) = (void(__cdecl*)(const char*))0x406E00;
	static const char* (__cdecl* GetErrorCodeDescription)(unsigned int code) = (const char* (__cdecl*)(unsigned int))0x45BF32;
	static void(__cdecl* ExtractFilePath)(const char* inPath, char* outDirectory, char* outFilename, char* outExtension) = (void(__cdecl*)(const char*, char*, char*, char*))0x4088E0;
	static String* (__cdecl* ExtractFilenameFromPath)(String* outFilename, const char* path) = (String * (__cdecl*)(String*, const char*))0x4093B0;
	static void(__cdecl* GetResourcePathRelative)(String* outPath, String resourceName, int blockTypeIndex, const char* languageCode) = (void(__cdecl*)(String*, String, int, const char*))0x882DF0;	//	It'll return "<languageCode>/<resourceName>_<languageCode>.<blockTypeExtension>"
	static bool (__cdecl* IsDirectoryValid)(const char* inPath) = (bool (__cdecl*)(const char*))0x418410;	//	NOTE: actual function at 437FD0.
	static void (__cdecl* DeleteAllFilesInFolder)(const char* inPath) = (void (__cdecl*)(const char*))0x418B50;
	static void (__cdecl* CreateDirectoryIfNotFound)(const char* inPath) = (void (__cdecl*)(const char*))0x4186F0;

	static inline void ToLowercase(char* str) {
		char* str_ = str;
		while (*(str_++))
			if (*str_ >= 65 && *str_ <= 90)
				*str_ += 32;
	}
}

namespace Control
{
	static void* (__cdecl* GetGamepadByIndex)(signed int index) = (void* (__cdecl*)(signed int))0x439660;
}