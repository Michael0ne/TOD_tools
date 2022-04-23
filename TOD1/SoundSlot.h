#pragma once
#include "Node.h"
#include "SoundAsset.h"
#include "StreamedSoundInfoAsset.h"
#include "SoundFile.h"

class SoundEmitter;

class SoundSlot : public Node
{
    friend class SoundEmitter;
protected:
    float                       m_MinRange;
    float                       m_MaxRange;
    float                       m_RollOff;
    char*                       m_StreamingSoundName;
    AssetLoader                 m_SoundResource;
    String                      m_Sound;
    AssetLoader                 m_StreamingSoundResource;
    StreamedSoundBuffer        *m_StreamBuffer;
    unsigned short              m_Volume;
    unsigned short              m_Pitch;
    int                         m_StreamBufferIndex;
    int                         m_PausedInstanceIndex;
    unsigned short              m_RandomPitchLength;
    unsigned short              m_RandomVolumeLength;
    Node                       *m_StreamAllocationCallback;
    std::vector<SoundEmitter*>  m_SoundEmittersList;
    union
    {
        struct
        {
            unsigned            CutAtMaxRange : 1;
            unsigned            Looped : 1;
            unsigned            RandomPitchMode : 1;
            unsigned            RandomVolumeMode : 1;
            unsigned            StaticMode : 1;
            unsigned            _5: 1;
            unsigned            _6 : 1;
            unsigned            FollowCamera : 1;
            unsigned            Priority : 4;
            unsigned            Group : 4;
            unsigned            _16 : 1;
            unsigned            Is3D : 1;
            unsigned            SoloMode : 1;
            unsigned            HasStreamedSound : 1;
            unsigned            DopplerEffectOn : 1;
            unsigned            _21 : 1;
            unsigned            HardPause : 1;
            unsigned            CenterChannelOnly : 1;
            unsigned            BackChannels : 1;
            unsigned            _25 : 1;
            unsigned            _26 : 1;
            unsigned            _27 : 1;
            unsigned            _28 : 1;
            unsigned            _29 : 1;
            unsigned            _30 : 1;
            unsigned            _31 : 1;
        };
    }                           m_Flags;
    unsigned char               m_DopplerFactor;
    unsigned char               m_LfeLevel;

public:
    SoundSlot(); // @89D590
    virtual                     ~SoundSlot(); // @89F290
    virtual void                Update();   //  @89CE50
    virtual String*             GetResourceName(String*); // @89F270

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

    const char* const           GetSound() const;   //  @89CF70
    void                        SetSound(const char* const sound);  //  @89E0E0
    const float                 GetMinRange() const;    //  @89A880
    void                        SetMinRange(const float minrange);  //  @8A6C90
    const float                 GetMaxRange() const;    //  @89A890
    void                        SetMaxRange(const float maxrange);  //  @8A6CA0
    const float                 GetRollOff() const; //  @89A8A0
    void                        SetRollOff(const float rolloff);    //  @8E3720
    const bool                  IsDopplerEffectOn() const;  //  @89A960
    void                        SetDopplerEffectOn(const bool on);  //  @89CD00
    const float                 GetDopplerFactor() const;   //  @89A910
    void                        SetDopplerFactor(const float factor);   //  @89CC50
    const bool                  CutAtMaxRange() const;  //  @89A8B0
    void                        SetCutAtMaxRange(const bool cut);   //  @89CBD0
    const float                 GetVolume() const;  //  @89A8C0
    void                        SetVolume(const float volume);  //  @89CBF0
    const float                 GetLFELevel() const;    //  @89A980
    void                        SetLFELevel(const float level); //  @89CD40
    const bool                  CenterChannelOnly() const;  //  @89A9A0
    void                        SetCenterChannelOnly(const bool centerchannelonly); //  @89CDA0
    const bool                  HasBackChannels() const;    //  @89A9B0
    void                        SetHasBackChannels(const bool hasbackchannels); //  @89CDC0
    const bool                  IsLooped() const;   //  @89A8E0
    void                        SetIsLooped(const bool looped); //  @89CC10
    const float                 GetPitch() const;   //  @89A8F0
    void                        SetPitch(const float pitch);    //  @89CC30
    const bool                  HasHardPause() const;   //  @89A970
    void                        SetHasHardPause(const bool hardpause);  //  @89CD20
    const bool                  FollowCamera() const;   //  @89CCD0
    void                        SetFollowCamera(const bool follow); //  @89CCB0
    const bool                  HasRandomPitchMode() const; //  @89AA00
    void                        SetHasRandomPitchMode(const bool enabled);  //  @89CDE0
    const float                 GetRandomPitchLength() const;   //  @89A9E0
    void                        SetRandomPitchLength(const float length);   //  @89A9C0
    const bool                  HasRandomVolumeMode() const;    //  @89AA50
    void                        SetHasRandomVolumeMode(const bool enabled); //  @89CE00
    const float                 GetRandomVolumeLength() const;  //  @89AA30
    void                        SetRandomVolumeLength(const float length);  //  @89AA10
    const bool                  HasStaticMode() const;  //  @89CE20
    void                        SetHasStaticMode(const bool enabled);   //  @89E200
    const char* const           GetStreamingSound();  //  @89CA40
    void                        SetStreamingSound(const char* const sound); //  @89E270
    const bool                  Is3DEnabled() const;  //  @4B8F80
    void                        Set3DEnabled(const bool enabled);   //  @4B8F60
    const int                   GetPriority() const;    //  @89A940
    void                        SetPriority(const int priority);    //  @89CCE0
    const bool                  IsSoloMode() const; //  @89A950
    void                        SetIsSoloMode(const bool solomode);    //  @4B8F90
    const bool                  IsPlaying() const;  //  @89D380
    const char* const           GetStreamingSoundFullPathDirect() const;    //  @6D8220
    const int                   GetGroup() const;   //  @89A930
    void                        SetGroup(const int group);  //  @4B8F40
    const float                 GetGlobalTimePitchFactor() const;   //  @89A870
    void                        SetGlobalTimePitchFactor(const float factor) const;   //  @5B9E80
    void                        PlayAt(int* args);  //  @89F1F0
    void                        Play(int* args);    //  @89DD30
    void                        EditorPlay(int* args);  //  @89DD90
    void                        Stop(int* args);    //  @89F020
    void                        Pause(int* args);   //  @89F030
    void                        UnPause(int* args); //  @89F040
    void                        PlayModeStop(int* args);    //  @89F020
    void                        LoadResource(int* args);    //  @89F2C0
    void                        UnLoadResource(int* args);  //  @89F2D0
    void                        AllocateStream(int* args);  //  @89F050
    void                        DeallocateStream(int* args);    //  @89F010
    const bool                  IsStreamAllocated(int* args) const;   //  @89CE30
    void                        GlobalDeallocateAllStreams(int* args);  //  @89F070
    void                        AddSoundEmitter(SoundEmitter* soundemitter);    //  @89D8D0
    void                        RemoveSoundEmitter(SoundEmitter* soundemitter); //  @89D040

private:
    void                        LoadSoundResource(const char* const sound); //  @89E000
    void                        Play_Impl(const Vector4f& pos); //  @89D970
    void                        StopSound();    //  @89D320
    void                        SetPause(); //  @89D230
    void                        SetUnPause();   //  @89D2C0
    bool                        LoadRes(const bool b);  //  @89DE10
    const char* const           GetSoundName();   //  @89F080
    bool                        UnloadRes(const bool b);    //  @89DF30
    void                        AllocateStream_Impl(Node* allocationCallbackNode, const bool stereoStream); //  @89D450
    void                        DeallocateStream_Impl(); //  @89D0C0
    void                        DumpStreamedSoundsToConsole();   //  @883EC0

public:
    static void                 Register(); //  @89E460
    static SoundSlot*           Create(AllocatorIndex); //  @89F230

    static void                 DeallocateStreams(std::vector<SoundSlot*>* soundSlotsList);   //  @89D3D0
    static void                 UpdateSoundGroups(const float timePassed);    //  @89D120
    static bool                 AllocateGlobalStreamedSound(const char* const soundname, const bool monosound, const bool loopedsound);   //  @89CF90
    static void                 DeallocateGlobalStreamedSound();    //  @89CEB0
    static bool                 PlayGlobalStreamedSound();  //  @89CED0
    static bool                 StopGlobalStreamedSound();  //  @89CF50
    static bool                 SetVolumePitchGlobalStreamedSound(const float volume, const float pitch);   //  @89D190

    static int                  StreamAllocateFailedCommand;    //  @A3DD84
    static int                  StreamAllocateFinishedCommand;  //  @A3DD88
    static String               StreamingSoundPath; //  @A12160
    static float                TimePitchFactor;    //  @A12124
    static std::vector<float>   VolumeGroupsList;   //  @A12150
    static std::vector<float>   VolumeModifiersList;    //  @A3DDDC
    static std::vector<float>   GroupVolume;    //  @A3DDCC
    static bool                 GlobalSoundLooped;  //  @A3DD7C
    static SoundFile            GlobalSoundFile;    //  @A3DD90
    static StreamedSoundBuffer* GlobalStreamedSound;    //  @A3DD8C
};

extern EntityType* tSoundSlot;  //  @A3DD80

ASSERT_CLASS_SIZE(SoundSlot, 176);