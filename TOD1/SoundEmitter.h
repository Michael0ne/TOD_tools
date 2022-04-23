#pragma once
#include "Node.h"
#include "BuiltinType.h"

class SoundSlot;

class SoundEmitter : public Node
{
public:
    Vector4f        m_FollowCamOffset;
    SoundSlot      *m_SoundSlot;
    int             m_ActiveStreamSlot;
    int             m_PausedStreamSlot;
    float           m_MinRange;
    float           m_MaxRange;
    union
    {
        struct
        {
            unsigned    RollOff : 22;
            unsigned    NextPlayAs2D : 1;
            unsigned    FollowCamera : 1;
            unsigned    Looped : 1;
            unsigned    _26 : 1;
            unsigned    RandomPitchMode : 1;
            unsigned    RandomVolumeMode : 1;
            unsigned    NoUpdatePos : 1;
            unsigned    SendMessageToParent : 1;
            unsigned    DopplerEffect : 1;
        };
    }               m_Flags;
    union
    {
        unsigned    LFEChannel : 1;
        unsigned    Volume : 16;
    }               m_Flags_1;
    short           m_Pitch;
    float           m_DopplerFactor;
    int             m_StopCommand;
    int             m_StopMessage;
    short           m_RandomPitchLength;
    short           m_RandomVolumeLength;
    short           field_90;
    short           field_92;

public:
    SoundEmitter() : Node(NODE_MASK_POSITION) // NOTE: no constructor.
    {
        MESSAGE_CLASS_CREATED(SoundEmitter);

        m_PausedStreamSlot = -1;
        m_StopCommand = -1;
        m_StopMessage = -1;
        m_RandomPitchLength = 0;
        m_ActiveStreamSlot = 0;
        m_RandomVolumeLength = 0;
        m_SoundSlot = nullptr;
        field_90 = 10000;
        m_MaxRange = 100.f;
        m_Pitch = 10000;
        field_92 = 10000;
        m_MinRange = 1.f;
        m_Flags_1.LFEChannel = true;
        m_Flags_1.Volume = 10000;
        m_Flags.SendMessageToParent = true;
        m_Flags.RollOff = 10000;
        m_Flags.DopplerEffect = false;
        m_Flags.FollowCamera = false;
        m_Flags.NextPlayAs2D = false;
        m_FollowCamOffset = BuiltinType::ZeroVector;
        m_FollowCamOffset.y = 0.8f;
        m_DopplerFactor = 1.f;
    }
    virtual ~SoundEmitter();    //  @89C930

    const int               GetStopMessage() const; //  @89AA70
    void                    SetStopMessage(const int stopmessage);  //  @89AA60
    const float             GetMinRange() const;    //  @905850
    void                    SetMinRange(const float minrange);  //  @5EA6C0
    const float             GetMaxRange() const;    //  @89AAB0
    void                    SetMaxRange(const float maxrange);  //  @5A1B70
    const float             GetRollOff() const; //  @89AAC0
    void                    SetRollOff(const float rolloff);    //  @89AA80
    const bool              DopplerEffectOn() const;    //  @89AD00
    void                    SetDopplerEffectOn(const bool on);  //  @89ACE0
    const float             GetDopplerFactor() const;   //  @89AD10
    void                    SetDopplerFactor(const float factor);   //  @8D5040
    const float             GetVolume() const;  //  @89AAE0
    void                    SetVolume(const float volume);  //  @5EA6D0
    const bool              HasLFEChannel() const;  //  @89AB50
    void                    SetHasLFEChannel(const bool on);    //  @89AB30
    const bool              IsLooped() const;   //  @89AB00
    void                    SetIsLooped(const bool looped); //  @5EA700
    const float             GetPitch() const;   //  @89AB10
    void                    SetPitch(const float pitch);    //  @5EA720
    const bool              NoUpdatePos() const;    //   @89AB60
    void                    SetNoUpdatePos(const bool noupdate);    //  @4B8FB0
    const bool              IsPlaying() const;  //  @89AE70
    const bool              RandomPitchMode() const;    //  @89AC30
    void                    SetRandomPitchMode(const bool enabled); //  @89AC10
    const float             GetRandomPitchLength() const;   //  @89ABF0
    void                    SetRandomPitchLength(const float length);   //  @89ABD0
    const bool              RandomVolumeMode() const;   //  @89ACA0
    void                    SetRandomVolumeMode(const bool enabled);    //  @89AC80
    const float             GetRandomVolumeLength() const;  //  @89AC60
    void                    SetRandomVolumeLength(const float length);  //  @89AC40
    const bool              ShouldSendMessageToParent() const;    //  @89ABA0
    void                    SetShouldSendMessageToParent(const bool enabled);   //  @89ABB0
    const bool              ShouldFollowCamera() const;   //  @89ACB0
    void                    SetShouldFollowCamera(const bool enabled);    //  @89ACC0
    void                    GetFollowCamOffset(Vector4f& outOffset) const;  //  @91CC90
    void                    SetFollowCamOffset(const Vector4f& offset); //  @89AD20
    const float             GetEndBlockDelayTime() const;   //  @89AE60
    SoundSlot*              GetCurrentSoundSlot() const;    //  @8E3700
    const bool              NextPlayAs2D() const;   //  @89AB90
    void                    SetNextPlayAs2D(const bool enabled);    //  @89AB70
    const bool              IsCurrSoundSlotStreamedAndAlloc() const;    //  @89AD40

    void                    PlaySlot(int* args);    //  @89CB90
    void                    PlayEmitter(int* args); //  @89CBB0
    void                    Pause(int* args);   //  @89CB70
    void                    UnPause(int* args); //  @89CB80
    void                    Stop(int* args);    //  @89CB60
    void                    PlayModeStop(int* args);    //  @89CB60
    void                    DeallocCurrStream(int* args);   //  @89AD60

private:
    void                    PlaySlot_Impl(SoundSlot* soundslot, const int stopCommand);   //  @89BB00
    void                    PlayEmitter_Impl(SoundSlot* soundslot, const int stopCommand);  //  @89B5C0
    void                    Pause_Impl();   //  @89AFD0
    void                    UnPause_Impl(); //  @89B0B0
    void                    Stop_Impl();    //  @89AF30

public:
    static void     Register(); //  @89C090
    static SoundEmitter*    Create(AllocatorIndex); //  @89C960
};

extern EntityType* tSoundEmitter;   //  @A3DD78

ASSERT_CLASS_SIZE(SoundEmitter, 148);