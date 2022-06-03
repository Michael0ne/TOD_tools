#pragma once
#include "stdafx.h"

//  TODO: get rid of this stuff!
namespace Utils
{
    static char HashSalt[5] = {}; // @9B6F84
    static unsigned int _A3A060 = NULL; // @A3A060

    static void generic_crc32(int* checksum, const char* string, const unsigned int stringlen)  //  @465840
    {
        unsigned int charactersRead = 0;
        const char* stringBase = string;

        while (true)
        {
            *checksum = 2 * *checksum;
            *checksum = *checksum | (*stringBase >> charactersRead) & 1;

            if (*checksum < 0)
                *checksum = *checksum ^ 0x4C11DB7;

            if (++charactersRead == 8)
            {
                charactersRead = 0;
                if (++stringBase == &string[stringlen])
                    break;
            }
        }
    }

    static void crc32_init_default(int* str) // @A3A060
    {
        *str = NULL;
        ++_A3A060;
    }

    static void crc32_gen(int* str) // @4658A0
    {
        generic_crc32(str, HashSalt, sizeof(HashSalt));
        --_A3A060;
    }

    static int CalcCRC32(const char* str, const size_t len) // @4657C0
    {
        int32_t crc = NULL;
        const char* buffer = str;
        uint8_t blockIndex = 0;

        do
        {
            while (true)
            {
                do
                {
                    const bool sumNegative = crc < 0;
                    crc = ((*buffer >> blockIndex) & 1) | (2 * crc);

                    if (sumNegative)
                        crc ^= 0x4C11DB7u;
                } while (++blockIndex != 8);

                blockIndex = NULL;

                if (++buffer != str + len)
                    break;

                buffer = HashSalt;
            }
        } while (buffer != &HashSalt[4]);

        return crc;
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
        sprintf_s(str, 28, "build %d.%d (%d)", KAPOW_ENGINE_VERSION_MAJOR, KAPOW_ENGINE_VERSION_MINOR, KAPOW_ENGINE_VERSION_BUILD);
    }

    static void  GetEngineAuthor(char* str) // @401020
    {
        sprintf_s(str, 28, "by %s", KAPOW_ENGINE_BUILDBY);
    }

    static unsigned int GetEngineVersionBuild() // @401040
    {
        return KAPOW_ENGINE_VERSION_BUILD;
    }

    static bool IsSSESupported()
    {
        int sse = 0;

        __asm   push eax
        __asm   mov eax, 1
        __asm   cpuid
        __asm   mov eax, edx
        __asm   and eax, 2000000h
        __asm   shr eax, 19h
        __asm   mov sse, eax
        __asm   pop eax

        return sse;
    }
}