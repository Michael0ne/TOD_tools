#pragma once

#include "List.h"

namespace Utils
{
	static const char* generic_crc32(int* t, char* base, size_t len)	//	@465840
	{
		char* base_ptr = base;
		unsigned int charsread = 0;
		char* result = nullptr;

		while (true)
		{
			if (*t < NULL)
				*t = ((2 * *t) | (*base_ptr >> charsread) & 1) ^ 0x4C11DB7;
			else
				*t = ((2 * *t) | (*base_ptr >> charsread) & 1);

			if (++charsread == 8)
			{
				charsread = NULL;
				++base_ptr;

				result = &base[len];
				if (base_ptr == &base[len])
					break;
			}
		}

		return result;
	}

	static int CalcCRC32(const char* str, unsigned int len)	//	@4657C0
	{
		char* str_ptr = (char*)str;
		int sum = NULL;

		do 
		{
			while (true)
			{
				for (int ind = 0; ind != 8; ++ind)
					if (sum < NULL)
						sum = sum ^ 0x4C11DB7;
					else
						sum = (*str_ptr >> ind) & 1 | (2 * sum);
				if (++str_ptr != &str[len])
					break;
				str_ptr = LastSavedCRCString;
			}
		} while (str_ptr != &LastSavedCRCString[5]);

		return sum;
	}

	static char LastSavedCRCString[4] = {};	//	@9B6F84

	static bool(__cdecl* IsFileAvailable)(const char* szPath) = (bool(__cdecl*)(const char*))0x418B00;
	static void(__cdecl* CreateDirectoriesRecursive)(const char* szPath) = (void(__cdecl*)(const char*))0x4096E0;
	static const char* (__cdecl* GetErrorCodeDescription)(unsigned int code) = (const char* (__cdecl*)(unsigned int))0x99E9F8;	//	NOTE: DO NOT DECOMPILE!!!
	static bool (__cdecl* IsDirectoryValid)(const char* inPath) = (bool (__cdecl*)(const char*))0x418410;	//	NOTE: actual function at 437FD0.
	static void (__cdecl* DeleteAllFilesInFolder)(const char* inPath) = (void (__cdecl*)(const char*))0x418B50;
	static void (__cdecl* CreateDirectoryIfNotFound)(const char* inPath) = (void (__cdecl*)(const char*))0x4186F0;
}

namespace Script
{
	static bool FileCheck = false;	//	@A35DE0
	static bool ForceFeedback = false;	//	@A35E70
	static bool SavePlatformPS2 = true;	//	@A090C8
	static unsigned int Ps2MaxTextureSize = 1024;	//	@A10FF0
	static const char* CountryCodes[] = {
		"uk", "fr", "it", "de", "es", "dk"
	};	//	@A089BC
	static bool _A1B98D = true;	//	@A1B98D
	static bool CheckOriginalAsset = true;	//	@A11540
	static bool WarningShow = true;	//	@A082FE
	static bool FrameConsoleMarker = true;	//	@A1B98C
	static unsigned int MinFadeDist = 15;	//	@A11C8C
	static bool LodAndFade = true;	//	@A11C88
	static bool FixDanglingRefs = true;	//	@A1207D
	static unsigned int LanguageStringsOffset = 0;	//	@A35E28
	static bool RelaxBuildVersionCheck;	//	@A5D5B0
	static bool CutsceneDisableAware;	//	@A3D892
	static bool CutsceneForceCompleteLodUpdates;	//	@A5D5A8
	static bool ShowHud;	//	@A3E0C8
	static bool CheckDataSanity;	//	@A5D5A9
	static bool CheckDivisionByZero;	//	@A5D5AA
	static bool Ps2PlayCtrl;	//	@A5D5B8
	static bool CheckDanglingRefs;	//	@A3DC4C
	static bool SimulateReleaseBuild;	//	@A3B584
	
	static bool LoadBlocks;
	static bool Fullscreen;
	static int IconResourceId;

	extern String Filesystem;
	extern String ControlType;
	extern String Region;
	extern String VersionName;	//	@A1B9C8
	extern String StreamedSoundExt;	//	@A35EE4
	extern String LanguageMode;	//	@A086A8
	extern String ScriptsPath;	//	@A0B434
	extern List<StringTuple> DirectoryMappings;	//	@A35DE4
}