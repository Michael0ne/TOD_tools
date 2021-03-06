#pragma once
#include <cstdio>

static void PrintHex(unsigned int number)
{
	printf("%X %X %X %X | %u",
		0x000000FF & (number >> 24),
		0x000000FF & (number >> 16),
		0x000000FF & (number >> 8),
		0x000000FF & (number),
		number
	);
}

static void PrintUsage()
{
	HMODULE modulehandle = GetModuleHandle(NULL);
	char usagestr1[128] = {}, usagestr2[128] = {}, usagestr3[128] = {}, usagestr4[128] = {};
	LoadString(modulehandle, 101, usagestr1, sizeof(usagestr1));
	LoadString(modulehandle, 102, usagestr2, sizeof(usagestr2));
	LoadString(modulehandle, 103, usagestr3, sizeof(usagestr3));
	LoadString(modulehandle, 107, usagestr4, sizeof(usagestr4));

	printf("%s%s%s%s", usagestr1, usagestr2, usagestr3, usagestr4);
}

static void ConvertBackslashes(char* str)
{
	if (str == nullptr || !*str)
		return;

	while (*str != NULL)
		if (*str == '\\')
			*str++ = '/';
		else
			str++;
}

enum PlatformDefinition
{
	PLATFORM_UNSPECIFIED = 0,
	PLATFORM_PC = 1,
	PLATFORM_PS2 = 2,
	PLATFORM_XBOX = 3
};