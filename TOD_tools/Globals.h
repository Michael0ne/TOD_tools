#pragma once

#include "List.h"

namespace Utils
{
	static char LastSavedCRCString[4] = {};	//	@9B6F84
	static unsigned int _A3A060 = NULL;	//	@A3A060

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

	static void crc32_init_default(int* str)	//	@A3A060
	{
		*str = NULL;
		++_A3A060;
	}

	static void crc32_gen(int* str)	//	@4658A0
	{
		generic_crc32(str, LastSavedCRCString, sizeof(LastSavedCRCString));
		--_A3A060;
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

	static bool(__cdecl* IsFileAvailable)(const char* szPath) = (bool(__cdecl*)(const char*))0x418B00;
	static void(__cdecl* CreateDirectoriesRecursive)(const char* szPath) = (void(__cdecl*)(const char*))0x4096E0;
	static const char* (__cdecl* GetErrorCodeDescription)(unsigned int code) = (const char* (__cdecl*)(unsigned int))0x99E9F8;	//	NOTE: DO NOT DECOMPILE!!!
	static bool (__cdecl* IsDirectoryValid)(const char* inPath) = (bool (__cdecl*)(const char*))0x418410;	//	NOTE: actual function at 437FD0.
	static void (__cdecl* DeleteAllFilesInFolder)(const char* inPath) = (void (__cdecl*)(const char*))0x418B50;
	static void (__cdecl* CreateDirectoryIfNotFound)(const char* inPath) = (void (__cdecl*)(const char*))0x4186F0;

	//	TODO: is this correct?
	static UINT64&	CreateUniqueId(UINT64& id_)
	{
		id_ = __rdtsc();
		id_ = time(NULL);

		return id_;
	}

	static void		GetBuildNumberString(char* str)	//	@401000
	{
		sprintf(str, "build %d.%d (%d)", KAPOW_ENGINE_VERSION_MAJOR, KAPOW_ENGINE_VERSION_MINOR, KAPOW_ENGINE_VERSION_BUILD);
	}

	static void		GetEngineAuthor(char* str)	//	@401020
	{
		sprintf(str, "by %s", "Kasper.Fauerby");
	}

	static unsigned int	GetEngineVersionBuild()
	{
		return KAPOW_ENGINE_VERSION_BUILD;
	}
}