#pragma once
#include "Types.h"
#include "Globals.h"
#include "StreamedSoundBuffer.h"
#include "StringsPool.h"
#include <vector>

#define STREAMEDSOUNDBUFFERS_MAX_CONCURRENT_SOUNDS 100
#define STREAMEDSOUNDBUFFERS_PREALLOCATED_COUNT 7

enum SoundSystemType
{
    SOUND_SYSTEM_AUTOSELECT = 1, // NOTE: when this is selected, it gets re-selected with directsound or dieselpower based on speaker configuration.
    SOUND_SYSTEM_DSOUND = 2,
    SOUND_SYSTEM_DIESELPOWER = 3,
    SOUND_SYSTEM_UNDEFINED = -1
};

const int SoundPriorityLevels[] = {
    0x3F000000,
    DSSCL_NORMAL,
    DSSCL_PRIORITY,
    DSSCL_EXCLUSIVE,
    DSSCL_WRITEPRIMARY
};

struct SoundBufferStatus
{
    StreamedSoundBuffer* m_StreamBufferPtr;
    bool m_InUse;

    static SoundBufferStatus    EmptyInstance;
};

//-------------- Handles streamed game sounds ----------------
// TODO: redo this class.
class StreamedSoundBuffers
{
public:
    int       m_MaxConcurrentSounds;
    int       m_CurrentlyPlaying;
    float      field_8;
    int       field_C;
    bool      m_Muted;
    bool      m_GlobalPauseCalled;
    bool      m_GlobalPause;
    bool      m_Sound;
    char      field_14;
    SoundSystemType    m_SoundSystem;
    enum {
        SPEAKER_CONFIG_DIRECT_OUT = 0,
        SPEAKER_CONFIG_HEADPHONES,
        SPEAKER_CONFIG_MONO,
        SPEAKER_CONFIG_QUAD,
        SPEAKER_CONFIG_SURROUND,
        SPEAKER_CONFIG_51,
        SPEAKER_CONFIG_71,
        SPEAKER_CONFIG_STEREO = -1
    }       m_SpeakerConfig;
    LPDIRECTSOUNDBUFFER   m_DirectSoundBuffer;
    DieselPower*    DieselPower2DStream; // NOTE: this and one below are something related to Diesel Power sound buffers. The 3D and 2D ones.
    DieselPower*    DieselPower3DStream;
    int       m_DirectSoundChannels;
    int       m_DirectSoundSampleRate;
    int       m_DirectSoundBits;
    int       m_MultichannelStreamedSoundsNum;
    int       m_MonoStreamsNumber;
    int       m_PlayingMonoStreamsNumber;
    LPDIRECTSOUND3DBUFFER  m_DirectSound3DBuffer;
    LPDIRECTSOUNDBUFFER   m_DirectSoundBuffer_1;
    class DieselPower*   m_DieselPower;
    int*      field_50; // NOTE: pointer to some DieselPower structure, size unknown.
    class DieselPower*      m_DieselPower_1;
    LPDIRECTSOUND    m_DirectSound;
    std::vector<DieselPower*>   m_DieselPowerSoundBuffers;
    std::vector<StreamedSoundBuffer*> m_StreamDataBufferList;
    std::vector<int>   m_SoundList_1;
    std::vector<int>   m_SoundList_2;
    std::vector<int>   m_SoundList_3;
    std::vector<int>   m_SoundList_4;
    std::vector<int>   m_SoundList_5;
    std::vector<int>   m_SoundList_6;
    std::vector<int>   m_SoundList_7;
    std::vector<int>   m_SoundList_8;
    std::vector<int>*   m_SoundList_1_Ptr;
    std::vector<int>*   m_SoundList_2_Ptr;
    std::vector<int>*   m_SoundList_3_Ptr;
    std::vector<int>*   m_SoundList_4_Ptr;
    std::vector<int>*   m_SoundList_5_Ptr;
    std::vector<int>*   m_SoundList_6_Ptr;
    std::vector<int>*   m_SoundList_7_Ptr;
    std::vector<int>*   m_SoundList_8_Ptr;
    String      m_Str_1;
public:
    StreamedSoundBuffers(char channels, int sampleRate); // @43E080
    ~StreamedSoundBuffers(); // @43E450

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    static void     SetDefaultFxVolume(float vol); // @43CDA0
    static void     SetDefaultAmbienceVolume(float vol); // @43CDB0
    static void     SetDefaultMusicVolume(float vol); // @43CDC0
    static void     SetDefaultSpeaksVolume(float vol); // @43CDD0
    static float    GetDefaultFxVolume(); // @43CDE0
    static float    GetDefaultAmbienceVolume(); // @43CDF0
    static float    GetDefaultMusicVolume(); // @43CE00
    static float    GetDefaultSpeaksVolume(); // @43CE10

    inline void     SetDefaultVolumeLevels(); // @43CE20
    void      InitDieselPower(); // @43CE90
    void      GetMaxDistance(Vector4f& outVec) const; // @43CFD0
    HRESULT      CreateSoundBuffer(DSBUFFERDESC* bufferDesc); // @43D060
    void      SetPrimarySoundFormat(int channels, int sampleRate, int bits); // @43D0D0
    float      GetDefaultVolumeForType(int type); // @43D180
    void      SetGlobalPause(bool pause); // @43D1D0
    void      _43D200(LPDIRECTSOUNDBUFFER, const float, const int, const int, const int); // @43D200 //  NOTE: used, but empty.

    static void     RememberSoundRenderer(SoundSystemType soundRendererId); // @43D210
    static SoundSystemType  GetSoundRenderer(); // @43D280
    static SoundBufferStatus&   FindSoundBufferInBuffersList(const StreamedSoundBuffer* ssb);   //  @444D20

    void      InitDirectSound(char channels, int sampleRate); // @43D310
    void      SetListener3DPos(const Vector4f& pos); // @43D560
    void      SetListener3DOrientation(const Orientation& orient); // @43D6F0
    void      GetListener3DOrientation(Orientation& outOrient); // @43DFD0
    void      UpdateSoundBuffers(); // @43E640
    int       GetPlayingSoundsNumber(); // @43E7B0 // NOTE: unused
    void      WaitForSoftPause(); // @43E800
    void      _43E850(); // @43E850 // NOTE: unused
    void      StopAllSounds(); // @43E880 // NOTE: unused
    bool      IsStreamBufferAlreadyExists(const StreamedSoundBuffer& streamedsound) const; // @43E8B0
    void      Dump() const; // @43EAD0
    void      AddStreamBufferToList(const StreamedSoundBuffer& streamedsound); // @43F240
    void      PreallocateStreamBuffersPool(); // @4462D0
    void      CreateStaticStreamedSoundBuffer(); // @441C00
    void                        RemoveSoundBufferFromList(StreamedSoundBuffer* soundbuffer);    //  @43F2B0

    static std::vector<SoundBufferStatus> vSoundBuffers; // @A09314

    static float    DefaultFxVolume; // @A09230
    static float    DefaultAmbienceVolume; // @A09234
    static float    DefaultMusicVolume; // @A09238
    static float    DefaultSpeaksVolume; // @A0923C
    static SoundSystemType  SoundRendererId; // @A35ECC
    static HANDLE    SemaphoreObject; // @A35EC8
    static StreamedSoundBuffer *StaticStreamedSound; // @A35ED0
};

extern StreamedSoundBuffers* g_StreamedSoundBuffers;    //  @A35EC0

ASSERT_CLASS_SIZE(StreamedSoundBuffers, 300);