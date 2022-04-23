#pragma once
#include "StreamedWAV.h"

// NOTE: possible name "MonoStream".
class SoundFile
{
private:
    int             m_WavChunkSize;
    int             field_4;
    int             m_Samples;
    float           m_BytesPerSample;
    int             m_AverageBytesPerSec;
    int             m_BlockAlign;
    int             m_Frequency;
    int             m_ChunkSize;
    int             m_SoundFormat;
    int             field_24;
    String          m_FileName;
    StreamedWAV*    m_StreamedWAV;

public:
    SoundFile(); // @4451C0
    ~SoundFile(); // @444F40

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    void   ChangeNameIfDifferent(const String& fn); // @444F90  //  NOTE: unused.
    void   CopySoundPropertiesFromStreamedWav(); // @444FE0
    void   CopySoundPropertiesFromStreamedWav_1(); // @445080
    void   ReadStreamedSoundFile(bool a1); // @445370
    void   Open(const char* const filename); // @445510
};

ASSERT_CLASS_SIZE(SoundFile, 60);