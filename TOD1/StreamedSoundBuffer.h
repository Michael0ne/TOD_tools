#pragma once
#include "StringsPool.h"
#include "DieselPowerSound.h"
#include "Types.h"
#include <map>
#include <vector>

class StreamedWAV;
class SoundFile;

class IStreamBuffer
{
public:
    virtual ~IStreamBuffer(); // @43F3B0
};

class StreamBuffer : public IStreamBuffer
{
public:
    unsigned int        m_SamplesPerSec;
    char                m_Channels;
    char                field_9;
    char                field_A;
    char                field_B;
    unsigned int        m_TotalChunks;
    unsigned int        m_SampledDataSize;
    float               field_14;
    float               m_BytesPerSec;
    int                 field_1C;   //  NOTE: flags for soundbuffer?
    float               field_20;
    void               *m_SampledData;
    std::map<int, int>  field_28;
    int                 field_34;
    StreamedWAV*        m_AuxMonoStream_1;
    std::vector<int>    m_List;

public:
    virtual ~StreamBuffer(); // @441AC0
    StreamBuffer(); // @446690
};

class StreamedSoundBuffer : public StreamBuffer
{
    friend class StreamedSoundBuffers;
    friend class SoundSlot;
protected:
    int* field_4C;

    union
    {
        struct
        {
            unsigned _0 : 1;
            unsigned Started : 1;
            unsigned PlayRequest : 1;
            unsigned StopRequest : 1;
            unsigned _4 : 1;
            unsigned PreLoaded : 1;
            unsigned ThreadExited : 1;
            unsigned _7 : 1;
            unsigned LastChunkPlaying : 1;
            unsigned Looped : 1;
            unsigned HardPause : 1;
            unsigned Paused : 1;
            unsigned _12 : 1;
            unsigned _13 : 1;
            unsigned WaitMult : 1;
            unsigned _15 : 1;
            unsigned StopEvent : 1;
            unsigned PreEvent : 1;
            unsigned StartSent : 1;
            unsigned _19 : 1;
            unsigned _20 : 1;
            unsigned _21 : 1;
            unsigned _22 : 1;
            unsigned _23 : 1;
            unsigned _24 : 1;
            unsigned _25 : 1;
            unsigned _26 : 1;
            unsigned _27 : 1;
            unsigned _28 : 1;
            unsigned _29 : 1;
            unsigned StartEvent : 1;
        };
    }           m_Flags;

    int      field_54;
    int      field_58;
    int      field_5C;
    char     m_StoppingFinished;
    char                    field_61;
    void* m_SoundBufferBlockStartPtr;
    int      m_SoundBufferBlockSize;
    int      field_6C;
    int      field_70;
    class StreamedWAV* m_StreamedWAV;
    int      field_78;
    int      field_7C;
    LPDWORD     m_ThreadId;
    int      field_84;
    LPDIRECTSOUNDBUFFER  m_DirectSoundBuffer;
    StreamedSoundBuffer* field_8C;
    LPDIRECTSOUND3DBUFFER m_DirectSound3DBuffer;
    int* m_DieselPowerStream;
    DieselPower* m_DieselPowerSoundBuffer;
    HANDLE     field_9C;
    HANDLE     field_A0;
    HANDLE     m_EventNotify;
    HANDLE     m_EvHandle_2;
    HANDLE     m_TerminateThreadEvent;
    HANDLE     field_B0;
    HANDLE     m_StreamThread;
    int      m_CurrentAudioPosition;
    float     field_BC;
    int      m_FrequencyMultiplied;
    int      field_C4;
    Vector4f    m_Position;
    float     m_MaxDistance;
    float     m_Frequency;
    float     field_E0;
    float     m_Volume;
    int      field_E8;
    int      field_EC;

public:
    StreamedSoundBuffer(class SoundFile* sndfile, int, int, int, char, char); // @445770
    StreamedSoundBuffer(bool a2, unsigned int totalchunks, int channels, unsigned int isstereo, float bytespersec, unsigned int frequency, bool ismonosound, unsigned int a9, char a10, const char* filepath); // @441A10

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

    virtual             ~StreamedSoundBuffer(); // @446640
    virtual void        stub2();
    virtual void        stub3();
    virtual void        SetSampledData(void*);
    virtual bool        Is3DSound(int);  //  @443350
    virtual void        Get3DMode(unsigned char mode3d);  //  @4433A0
    virtual bool        IsLooped(int slot);   //  @4433E0
    virtual int         Play(int slot, bool looped, int);    //  @444B40
    virtual bool        IsPlaying(int slot) const;  //  @4433F0
    virtual bool        IsFirstChannelPlaying() const;  //  @443DF0
    virtual void        _440850(int);
    virtual void        Stop(int slot);
    virtual void        SetPause(int slot, bool hardpause); //  @443410
    virtual void        UnPause(int slot, bool hardpause);  //  @443440
    virtual bool        IsPaused(int slot) const;
    virtual bool        IsCreated() const;
    virtual char*       GetBufferDataPtr();
    virtual void        SetVolume(int, float);
    virtual float       GetVolume(int);
    virtual void        SetFrequencyMultiplier(int, float mul);
    virtual float       GetFrequencyMultiplier(int);
    virtual int         SetPan(int slot, const float pan);   //  @4435C0
    virtual float       GetPan(int slot);    //  @443650
    virtual void        SetSoundPosition(int, const Vector4f*);
    virtual void        GetPosition(Vector4f& outPos, const int);
    virtual void        _443990(int, int*);
    virtual Vector4f*   _4439E0(Vector4f*, int);
    virtual void        SetMaxDistance(int, float);
    virtual float       GetMaxDistance(int);
    virtual void        SetFrequency(int, float);
    virtual float       GetFrequency(int);
    virtual void        _443C20(int, float);
    virtual float       _443C90(int);
    virtual void        _443CD0(int, float);
    virtual float       _443D30(int);
    virtual void        SetSoundProperties(int slot, float frequency, float pan, float);   //  @443D40
    virtual int         GetChannelsNumber() const;   //  @442810
    virtual void        DumpInfo();  //  @444C00
    virtual void        StopFirstChannelSound(); //  @443E00

    void     FillSoundData(const bool overwrite); // @443FC0
    void                    ShutdownThread();   //  @444D60
};

ASSERT_CLASS_SIZE(StreamBuffer, 76);
ASSERT_CLASS_SIZE(StreamedSoundBuffer, 240);