#pragma once
#include <cstdio>
#include <array>

#include "resource.h"

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

    charcopied += LoadString(modulehandle, IDS_USAGESTRING1, usagestrptr, sizeof(usagestr));
    charcopied += LoadString(modulehandle, IDS_USAGESTRING2, usagestrptr + charcopied, sizeof(usagestr));
    charcopied += LoadString(modulehandle, IDS_USAGESTRING3, usagestrptr + charcopied, sizeof(usagestr));
    charcopied += LoadString(modulehandle, IDS_USAGESTRING4, usagestrptr + charcopied, sizeof(usagestr));

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

static void MakeMissingFolders(const char* const fullPath)
{
    char* context = nullptr;
    char* fullPathCopy = _strdup(fullPath);

    //  Remove filename from path.
    *(strrchr(fullPathCopy, '/') + 1) = NULL;

    char* pathPart = strtok_s(fullPathCopy, "/", &context);
    size_t pathOffset = 0;
    while (pathPart)
    {
        size_t tokenLength = context - pathPart - 1;
        char* currentPath = new char[pathOffset + tokenLength + 1];
        strncpy_s(currentPath, pathOffset + tokenLength + 1, fullPath, pathOffset + tokenLength);
        pathOffset += tokenLength + 1;

        //  Skip drive letter.
        if (pathPart[1] == ':')
        {
            delete[] currentPath;
            pathPart = strtok_s(nullptr, "/", &context);
            continue;
        }

        //  Current path now has full path including current folder, pathPart is only the folder name.
        //  If this folder already exists, skip next.
        if (CreateDirectory(currentPath, nullptr) != NULL)
        {
            DWORD creationError = GetLastError();
            if (creationError == ERROR_ALREADY_EXISTS)
            {
                delete[] currentPath;
                pathPart = strtok_s(nullptr, "/", &context);
                continue;
            }
        }

        delete[] currentPath;
        pathPart = strtok_s(nullptr, "/", &context);
    }
}