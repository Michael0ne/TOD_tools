#pragma once

#include "List.h"

//	TODO: maybe put these into different namespaces?

static HINSTANCE& g_hInstance = *(HINSTANCE*)0xA35EB0;
static LPSTR& g_CmdLine = *(LPSTR*)0xA35EB4;

static DWORD* StringResourceBaseAddr = (DWORD*)0xA35E28;
static UINT* uType = (UINT*)0xA091A8;
static STICKYKEYS* sSTICKYKEYS = (STICKYKEYS*)0xA0917C;
static TOGGLEKEYS* sTOGGLEKEYS = (TOGGLEKEYS*)0xA09184;
static FILTERKEYS* sFILTERKEYS = (FILTERKEYS*)0xA0918C;

static LPCSTR& RegistryKey = *(LPCSTR*)0xA09178;

static bool& GameDiscFound = *(bool*)0xA35E68;
static String& WorkingDirectory = *(String*)0xA08FA0;
static String& GameWorkingDirectory = *(String*)0xA08FB0;

namespace Utils
{
	static bool(__cdecl* FindFileEverywhere)(const char* szPath) = (bool(__cdecl*)(const char*))0x4182A0;
	static int(__cdecl* CalcCRC32)(const char* str, unsigned int strlength) = (int(__cdecl*)(const char*, unsigned int))0x4657C0;
	static bool(__cdecl* IsFileAvailable)(const char* szPath) = (bool(__cdecl*)(const char*))0x418B00;
	static void(__cdecl* CreateDirectoriesRecursive)(const char* szPath) = (void(__cdecl*)(const char*))0x4096E0;
	static void(__cdecl* SetWarningString)(const char* str) = (void(__cdecl*)(const char*))0x406E00;
	static const char* (__cdecl* GetErrorCodeDescription)(unsigned int code) = (const char* (__cdecl*)(unsigned int))0x45BF32;
	static void(__cdecl* ExtractFilePath)(const char* inPath, char* outDirectory, char* outFilename, char* outExtension) = (void(__cdecl*)(const char*, char*, char*, char*))0x4088E0;
	static String* (__cdecl* ExtractFilenameFromPath)(String* outFilename, const char* path) = (String * (__cdecl*)(String*, const char*))0x4093B0;
	static void(__cdecl* GetResourcePathRelative)(String* outPath, String resourceName, int blockTypeIndex, const char* languageCode) = (void(__cdecl*)(String*, String, int, const char*))0x882DF0;	//	It'll return "<languageCode>/<resourceName>_<languageCode>.<blockTypeExtension>"
	static bool (__cdecl* IsDirectoryValid)(const char* inPath) = (bool (__cdecl*)(const char*))0x418410;	//	NOTE: actual function at 437FD0.
	static void (__cdecl* DeleteAllFilesInFolder)(const char* inPath) = (void (__cdecl*)(const char*))0x418B50;
	static void (__cdecl* CreateDirectoryIfNotFound)(const char* inPath) = (void (__cdecl*)(const char*))0x4186F0;
	static String* (__cdecl* ExtractFileDir)(String* outDir, const char* inPath) = (String * (__cdecl*)(String * outDir, const char*))0x409360;
	static String* (__cdecl* GetPathFromDirectoryMappings)(String* outPath, const char* inPath) = (String * (__cdecl*)(String*, const char*))0x41A360;
	static bool(__cdecl* ReadFileAttributes)(const char* path) = (bool (__cdecl*)(const char*))0x437E90;
	static void (__cdecl* GetWorkingDirRelativePath)(String* outPath) = (void (__cdecl*)(String*))0x437A70;
	static void (__cdecl* GetGameWorkingDirRelativePath)(String* outPath) = (void (__cdecl*)(String*))0x437B80;

	static inline void ToLowercase(char* str) {
		char* str_ = str;
		while (*(str_++))
			if (*str_ >= 65 && *str_ <= 90)
				*str_ += 32;
	}
}

namespace Script
{
	static String& LanguageMode = *(String*)0xA086A8;
	static bool& FileCheck = *(bool*)0xA35DE0;
	static bool& ForceFeedback = *(bool*)0xA35E70;
	static bool LoadBlocks;
	static int& Ps2MaxTextureSize = *(int*)0xA10FF0;
	static bool Fullscreen;
	static List<String>& DirectoryMappings = *(List<String>*)0xA35DE4;
	static String& ScriptsPath = *(String*)0xA0B434;
	static bool& RelaxBuildVersionCheck = *(bool*)0xA5D5B0;
	static int IconResourceId;
	static int& LanguageStringsOffset = *(int*)0xA35E28;
	static const char** CountryCodes[6] = {
		(const char**)0xA089BC,	//	UK
		(const char**)0xA089C0,	//	FR
		(const char**)0xA089C4,	//	IT
		(const char**)0xA089C8,	//	DE
		(const char**)0xA089CC,	//	ES
		(const char**)0xA089D0	//	DK
	};
	static bool& SavePlatformPS2 = *(bool*)0xA090C8;
	static bool& CutsceneDisableAware = *(bool*)0xA3D892;
	static bool& CutsceneForceCompleteLodUpdates = *(bool*)0xA5D5A8;
	static String& StreamedSoundExt = *(String*)0xA35EE4;
	static String& VersionName = *(String*)0xA1B9C8;
	static char& _A1B98D = *(char*)0xA1B98D;
	static bool& ShowHud = *(bool*)0xA3E0C8;
	static bool& CheckOriginalAsset = *(bool*)0xA11540;
	static bool& WarningShow = *(bool*)0xA082FE;
	static bool& FrameConsoleMarker = *(bool*)0xA1B98C;
	static bool& CheckDataSanity = *(bool*)0xA5D5A9;
	static bool& CheckDivisionByZero = *(bool*)0xA5D5AA;
	static bool& Ps2PlayCtrl = *(bool*)0xA5D5B8;
	static float& MinFadeDist = *(float*)0xA11C8C;
	static bool& LodAndFade = *(bool*)0xA11C88;
	static bool& CheckDanglingRefs = *(bool*)0xA11C88;
	static bool& FixDanglingRefs = *(bool*)0xA1207D;
	static bool& SimulateReleaseBuild = *(bool*)0xA3B584;

	extern String Filesystem;
	extern String ControlType;
	extern String Region;
}