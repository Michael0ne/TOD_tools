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
    uint32_t            SamplesPerSec;
    uint8_t             Channels;
    uint8_t             field_9;
    uint8_t             field_A;
    uint8_t             field_B;
    uint32_t            TotalChunks;
    uint32_t            SampledDataSize;
    float               DopplerFactor;
    float_t             BytesPerSec;
    int                 field_1C;   //  NOTE: flags for soundbuffer?
    float_t             field_20;
    uint8_t            *SampledData;
    std::map<int, int>  field_28;
    int                 field_34;
    StreamedWAV*        AuxMonoStream_1;
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
    uint32_t   *field_4C;

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
    }           Flags;

    uint32_t    field_54;
    uint32_t    field_58;
    uint32_t    field_5C;
    bool        StoppingFinished;
    bool        field_61;
    uint8_t    *SoundBufferBlockStartPtr;
    uint32_t    SoundBufferBlockSize;
    uint32_t    field_6C;
    uint32_t    field_70;
    StreamedWAV*StreamedWAVSound;
    uint32_t    field_78;
    uint32_t    field_7C;
    LPDWORD     ThreadId;
    uint32_t    field_84;
    LPDIRECTSOUNDBUFFER     DirectSoundBuffer;
    StreamedSoundBuffer    *field_8C;
    LPDIRECTSOUND3DBUFFER   DirectSound3DBuffer;
    DieselPower* DieselPower3DStream;
    DieselPower* DieselPowerSoundBuffer;
    HANDLE      field_9C;
    HANDLE      field_A0;
    HANDLE      EventNotify;
    HANDLE      m_EvHandle_2;
    HANDLE      TerminateThreadEvent;
    HANDLE      field_B0;
    HANDLE      m_StreamThread;
    uint32_t    CurrentAudioPosition;
    float_t     field_BC;
    float_t     Frequency;
    uint32_t    Pan;
    Vector4f    Position;
    float_t     MaxDistance;
    float_t     MinDistance;
    float_t     RollOff;
    float_t     Volume;
    uint32_t    field_E8;
    uint32_t    field_EC;

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
    virtual bool        Is3DSound(const uint32_t streamIndex) const;  //  @443350
    virtual void        Get3DMode(unsigned char mode3d);  //  @4433A0
    virtual bool        IsLooped(const uint32_t streamIndex) const;   //  @4433E0
    virtual int         Play(const uint32_t streamIndex, bool looped, int);    //  @444B40
    virtual bool        IsPlaying(const uint32_t streamIndex) const;  //  @4433F0
    virtual bool        IsFirstChannelPlaying() const;  //  @443DF0
    virtual void        _440850(int);   //  @440850
    virtual void        Stop(const uint32_t streamIndex);   //  @4431D0
    virtual void        SetPause(const uint32_t streamIndex, bool hardpause); //  @443410
    virtual void        UnPause(const uint32_t streamIndex, bool hardpause);  //  @443440
    virtual bool        IsPaused(const uint32_t streamIndex) const; //  @443470
    virtual bool        IsCreated() const;  //  @443150
    virtual uint8_t*    GetBufferDataPtr(); //  @443480
    virtual void        SetVolume(const uint32_t streamIndex, const float_t volume);    //  @443490
    virtual float_t     GetVolume(const uint32_t streamIndex) const; //  @4434E0
    virtual void        SetFrequency(const uint32_t streamIndex, const float_t frequency); //  @4434F0
    virtual float_t     GetFrequency(const uint32_t streamIndex) const;    //  @443560
    virtual int         SetPan(const uint32_t streamIndex, const float_t pan);   //  @4435C0
    virtual float       GetPan(const uint32_t streamIndex);    //  @443650
    virtual void        SetSoundPosition(const uint32_t streamIndex, const Vector4f& position); //  @4436C0
    virtual void        GetPosition(Vector4f& outPos, const uint32_t streamIndex) const;   //  @4438B0
    virtual void        _443990(int, int*);
    virtual Vector4f*   _4439E0(Vector4f*, int);
    virtual void        SetMaxDistance(const uint32_t streamIndex, const float_t maxdistance); //  @443A80
    virtual float_t     GetMaxDistance(const uint32_t streamIndex) const;    //  @443B00
    virtual void        SetMinDistance(const uint32_t streamIndex, const float_t frequency);   //  @443B50
    virtual float_t     GetMinDistance(const uint32_t streamIndex) const; //  @443BD0
    virtual void        SetRollOff(const uint32_t streamIndex, const float_t rollOff);  //  @443C20
    virtual float_t     GetRollOff(const uint32_t streamIndex) const;   //  @443C90
    virtual void        SetDopplerFactor(const uint32_t streamIndex, const float_t factor);    //  @443CD0
    virtual float_t     GetDopplerFactor(const uint32_t streamIndex) const; //  @443D30
    virtual void        SetSoundProperties(const uint32_t streamIndex, const float_t minDistance, const float_t maxDistance, const float_t rollOff);   //  @443D40
    virtual int         GetChannelsNumber() const;   //  @442810
    virtual void        DumpInfo();  //  @444C00
    virtual void        StopFirstChannelSound(); //  @443E00

    void     FillSoundData(const bool overwrite); // @443FC0
    void                    ShutdownThread();   //  @444D60
};

ASSERT_CLASS_SIZE(StreamBuffer, 76);
ASSERT_CLASS_SIZE(StreamedSoundBuffer, 240);