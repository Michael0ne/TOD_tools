#pragma once
#include "stdafx.h"

namespace Utils
{
    static char LastSavedCRCString[5] = {}; // @9B6F84
    static unsigned int _A3A060 = NULL; // @A3A060

    static const char* generic_crc32(int* t, char* base, size_t len) // @465840
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

    static void crc32_init_default(int* str) // @A3A060
    {
        *str = NULL;
        ++_A3A060;
    }

    static void crc32_gen(int* str) // @4658A0
    {
        generic_crc32(str, LastSavedCRCString, sizeof(LastSavedCRCString));
        --_A3A060;
    }

    static int CalcCRC32(const char* str, unsigned int len) // @4657C0
    {
        if (!*str || len == NULL)
            return NULL;

        int chround = 0;
        const char* _str = str;
        int sum = 0;

        do
        {
            while (true)
            {
                do
                {
                    bool nosum = sum < 0;
                    sum = (*_str >> chround) & 1 | (2 * sum);
                    if (nosum)
                        sum ^= 0x4C11DB7;
                    ++chround;
                } while (chround != 8);

                chround = 0;
                if (++_str != &str[len])
                    break;
                _str = LastSavedCRCString;
            }
        } while (_str != &LastSavedCRCString[5]);

        return sum;
    }

    static void GetDXErrorString(HRESULT hr, char* errorstr)
    {
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, (DWORD)hr, 0, errorstr, 0, 0);
    }

    static void(__cdecl* DeleteAllFilesInFolder)(const char* inPath) = (void(__cdecl*)(const char*))0x418B50;

    struct UniqueId
    {
        union
        {
            UINT64  m_Id64;
            struct
            {
                int     m_Id1;
                int     m_Id2;
            };
        }           m_Id;

        UniqueId() // @40FEA0
        {
            *(time_t*)&m_Id = time(NULL);
            *((unsigned int*)&m_Id + 1) = (unsigned int)__rdtsc();
        }

        static UniqueId  Instance; // @A3D8A8
    };

    __declspec(selectany) Utils::UniqueId UniqueId::Instance;

    static void  GetBuildNumberString(char* str) // @401000
    {
        sprintf(str, "build %d.%d (%d)", KAPOW_ENGINE_VERSION_MAJOR, KAPOW_ENGINE_VERSION_MINOR, KAPOW_ENGINE_VERSION_BUILD);
    }

    static void  GetEngineAuthor(char* str) // @401020
    {
        sprintf(str, "by %s", KAPOW_ENGINE_BUILDBY);
    }

    static unsigned int GetEngineVersionBuild() // @401040
    {
        return KAPOW_ENGINE_VERSION_BUILD;
    }
}