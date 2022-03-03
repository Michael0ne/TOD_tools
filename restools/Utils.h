#pragma once
#include <cstdio>
#include <array>

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
    char usagestr[1024] = {};
    char* usagestrptr = usagestr;
    int charcopied = 0;

    charcopied += LoadString(modulehandle, 101, usagestrptr, sizeof(usagestr));
    charcopied += LoadString(modulehandle, 102, usagestrptr + charcopied, sizeof(usagestr));
    charcopied += LoadString(modulehandle, 103, usagestrptr + charcopied, sizeof(usagestr));
    charcopied += LoadString(modulehandle, 107, usagestrptr + charcopied, sizeof(usagestr));

    printf(usagestr);
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