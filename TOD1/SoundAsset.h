#pragma once
#include "BaseAsset.h"

class StreamedSoundBuffer;

class SoundAsset : public Asset
{
    friend class SoundSlot;

    class SoundFileInfo
    {
        friend class SoundAsset;
    protected:
        int             m_TotalChunks;
        int             m_SamplesPerSec;
        int             field_8;
        int             m_ChunkSize;
        void           *m_SampledData;
        short           m_InterleavedChannels;
        short           m_BytesPerSample;

    protected:
        SoundFileInfo(const char* const filepath);  //  @85D220
        ~SoundFileInfo();   //  @85D180

        const int       GetTotalChunks() const; //  @85D1C0
        const short     GetNumberOfChannels() const;    //  @85D1D0
        const short     GetBytesPerSample() const;  //  @85D1E0
        const int       GetSamplesPerSec() const;   //  @85D1F0
        const void*     GetSampledData() const; //  @85D200
    };

protected:
    int                 field_1C;
    StreamedSoundBuffer       *m_StreamBuffer;
    int                 field_24;

public:
    SoundAsset();	//	@85C3C0

    virtual             ~SoundAsset();  //  @85C450
    virtual AssetInstance* GetInstancePtr() const override;
    virtual void        LoadResource(const char* const resPath) override;    //  @85BE70

    void*               operator new(size_t size)
    {
        return MemoryManager::Released ? nullptr : MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void                operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    static void         CreateInstance();	//	@85C010
    static SoundAsset*  Create();	//	@85C430

private:
    static AssetInstance*   Instance;   //  @A3BE50
};

ASSERT_CLASS_SIZE(SoundAsset, 40);