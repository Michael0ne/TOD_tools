#include "SoundEmitter.h"
#include "SoundSlot.h"
#include "StreamedSoundBuffer.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "NumberType.h"
#include "VectorType.h"

EntityType* tSoundEmitter;

SoundEmitter::~SoundEmitter()
{
    MESSAGE_CLASS_DESTROYED(SoundEmitter);

    if (m_SoundSlot)
        m_SoundSlot->RemoveSoundEmitter(this);
}

const int SoundEmitter::GetStopMessage() const
{
    return m_StopMessage;
}

void SoundEmitter::SetStopMessage(const int stopmessage)
{
    m_StopMessage = stopmessage;
}

const float SoundEmitter::GetMinRange() const
{
    return m_MinRange;
}

void SoundEmitter::SetMinRange(const float minrange)
{
    m_MinRange = minrange;
}

const float SoundEmitter::GetMaxRange() const
{
    return m_MaxRange;
}

void SoundEmitter::SetMaxRange(const float maxrange)
{
    m_MaxRange = maxrange;
}

const float SoundEmitter::GetRollOff() const
{
    return (float)(m_Flags.RollOff * (1.f / 10000.f));
}

void SoundEmitter::SetRollOff(const float rolloff)
{
    m_Flags.RollOff = (unsigned int)(rolloff * 10000.f);
}

const bool SoundEmitter::DopplerEffectOn() const
{
    return m_Flags.DopplerEffect;
}

void SoundEmitter::SetDopplerEffectOn(const bool on)
{
    m_Flags.DopplerEffect = on;
}

const float SoundEmitter::GetDopplerFactor() const
{
    return m_DopplerFactor;
}

void SoundEmitter::SetDopplerFactor(const float factor)
{
    m_DopplerFactor = factor;
}

const float SoundEmitter::GetVolume() const
{
    return (float)(m_Flags_1.Volume * (1.f / 10000.f));
}

void SoundEmitter::SetVolume(const float volume)
{
    m_Flags_1.Volume = (int)(volume * 10000.f);
}

const bool SoundEmitter::HasLFEChannel() const
{
    return m_Flags_1.LFEChannel;
}

void SoundEmitter::SetHasLFEChannel(const bool on)
{
    m_Flags_1.LFEChannel = on;
}

const bool SoundEmitter::IsLooped() const
{
    return m_Flags.Looped;
}

void SoundEmitter::SetIsLooped(const bool looped)
{
    m_Flags.Looped = looped;
}

const float SoundEmitter::GetPitch() const
{
    return (float)(m_Pitch * (1.f / 10000.f));
}

void SoundEmitter::SetPitch(const float pitch)
{
    m_Pitch = (short)(pitch * 10000.f);
}

const bool SoundEmitter::NoUpdatePos() const
{
    return m_Flags.NoUpdatePos;
}

void SoundEmitter::SetNoUpdatePos(const bool noupdate)
{
    m_Flags.NoUpdatePos = noupdate;
}

const bool SoundEmitter::IsPlaying() const
{
    if (!m_SoundSlot)
        return false;

    if (!m_SoundSlot->m_StreamBuffer)
        return false;

    return m_SoundSlot->m_StreamBuffer->IsPlaying(m_ActiveStreamSlot);
}

const bool SoundEmitter::RandomPitchMode() const
{
    return m_Flags.RandomPitchMode;
}

void SoundEmitter::SetRandomPitchMode(const bool enabled)
{
    m_Flags.RandomPitchMode = enabled;
}

const float SoundEmitter::GetRandomPitchLength() const
{
    return (float)(m_RandomPitchLength * (1.f / 10000.f));
}

void SoundEmitter::SetRandomPitchLength(const float length)
{
    m_RandomPitchLength = (short)(length * 10000.f);
}

const bool SoundEmitter::RandomVolumeMode() const
{
    return m_Flags.RandomVolumeMode;
}

void SoundEmitter::SetRandomVolumeMode(const bool enabled)
{
    m_Flags.RandomVolumeMode = enabled;
}

const float SoundEmitter::GetRandomVolumeLength() const
{
    return (float)(m_RandomVolumeLength * (1.f / 10000.f));
}

void SoundEmitter::SetRandomVolumeLength(const float length)
{
    m_RandomVolumeLength = (short)(length * 10000.f);
}

const bool SoundEmitter::ShouldSendMessageToParent() const
{
    return m_Flags.SendMessageToParent;
}

void SoundEmitter::SetShouldSendMessageToParent(const bool enabled)
{
    m_Flags.SendMessageToParent = enabled;
}

const bool SoundEmitter::ShouldFollowCamera() const
{
    return m_Flags.FollowCamera;
}

void SoundEmitter::SetShouldFollowCamera(const bool enabled)
{
    m_Flags.FollowCamera = enabled;
}

void SoundEmitter::GetFollowCamOffset(Vector4f& outOffset) const
{
    outOffset = m_FollowCamOffset;
}

void SoundEmitter::SetFollowCamOffset(const Vector4f& offset)
{
    m_FollowCamOffset = offset;
}

const float SoundEmitter::GetEndBlockDelayTime() const
{
    return 0.f;
}

SoundSlot* SoundEmitter::GetCurrentSoundSlot() const
{
    return m_SoundSlot;
}

const bool SoundEmitter::NextPlayAs2D() const
{
    return m_Flags.NextPlayAs2D;
}

void SoundEmitter::SetNextPlayAs2D(const bool enabled)
{
    m_Flags.NextPlayAs2D = enabled;
}

const bool SoundEmitter::IsCurrSoundSlotStreamedAndAlloc() const
{
    return m_SoundSlot && m_SoundSlot->m_StreamBuffer;
}

void SoundEmitter::PlaySlot(int* args)
{
    PlaySlot_Impl((SoundSlot*)args[0], args[1]);
}

void SoundEmitter::PlayEmitter(int* args)
{
    PlayEmitter_Impl((SoundSlot*)args[0], args[1]);
}

void SoundEmitter::Pause(int* args)
{
    Pause_Impl();
}

void SoundEmitter::UnPause(int* args)
{
    UnPause_Impl();
}

void SoundEmitter::Stop(int* args)
{
    Stop_Impl();
}

void SoundEmitter::PlayModeStop(int* args)
{
    Stop_Impl();
}

void SoundEmitter::DeallocCurrStream(int* args)
{
    if (m_SoundSlot)
        m_SoundSlot->DeallocateStream_Impl();
}

#pragma message(TODO_IMPLEMENTATION)
void SoundEmitter::PlaySlot_Impl(SoundSlot* soundslot, const int stopCommand)
{
}

#pragma message(TODO_IMPLEMENTATION)
void SoundEmitter::PlayEmitter_Impl(SoundSlot* soundslot, const int stopCommand)
{
}

void SoundEmitter::Pause_Impl()
{
    if (!m_SoundSlot)
        return;

    StreamedSoundBuffer* sndBuffer = m_SoundSlot->m_SoundResource.m_AssetPtr ? (StreamedSoundBuffer*)m_SoundSlot->m_SoundResource.GetAsset<StreamedSoundInfoAsset>()->m_StreamBuffer : m_SoundSlot->m_StreamBuffer;
    if (m_PausedStreamSlot != m_ActiveStreamSlot && m_PausedStreamSlot >= 0)
    {
        LogDump::LogA("Warning: A Sound-instance that was paused was released because another instance was paused.\n");
        if (sndBuffer)
            sndBuffer->UnPause(m_PausedStreamSlot, true);
        m_PausedStreamSlot = -1;
    }

    if (sndBuffer)
        sndBuffer->SetPause(m_ActiveStreamSlot, m_SoundSlot->m_Flags.HardPause);

    m_PausedStreamSlot = m_ActiveStreamSlot;
}

void SoundEmitter::UnPause_Impl()
{
    if (m_PausedStreamSlot < 0 || !m_SoundSlot)
        return;

    StreamedSoundBuffer* sndBuffer = m_SoundSlot->m_SoundResource.m_AssetPtr ? (StreamedSoundBuffer*)m_SoundSlot->m_SoundResource.GetAsset<StreamedSoundInfoAsset>()->m_StreamBuffer : m_SoundSlot->m_StreamBuffer;
    if (sndBuffer)
        sndBuffer->UnPause(m_PausedStreamSlot, m_SoundSlot->m_Flags.HardPause);

    m_PausedStreamSlot = -1;
}

void SoundEmitter::Stop_Impl()
{
    if (!m_SoundSlot)
        return;

    StreamedSoundBuffer* sndBuffer = m_SoundSlot->m_SoundResource.m_AssetPtr ? (StreamedSoundBuffer*)m_SoundSlot->m_SoundResource.GetAsset<StreamedSoundInfoAsset>()->m_StreamBuffer : m_SoundSlot->m_StreamBuffer;
    if (sndBuffer)
        sndBuffer->Stop(m_ActiveStreamSlot);

    m_ActiveStreamSlot = 0;
}

void SoundEmitter::Register()
{
    tSoundEmitter = new EntityType("SoundEmitter");
    tSoundEmitter->InheritFrom(tNode);
    tSoundEmitter->SetCreator((CREATOR)Create);

    tSoundEmitter->RegisterProperty(tINTEGER, "stopmessage", (EntityGetterFunction)&GetStopMessage, (EntitySetterFunction)&SetStopMessage, nullptr, 10);
    tSoundEmitter->RegisterProperty(tNUMBER, "minrange", (EntityGetterFunction)&GetMinRange, (EntitySetterFunction)&SetMinRange, "control=slider|min=1|max=5000");
    tSoundEmitter->RegisterProperty(tNUMBER, "maxrange", (EntityGetterFunction)&GetMaxRange, (EntitySetterFunction)&SetMaxRange, "control=slider|min=1|max=5000");
    tSoundEmitter->RegisterProperty(tNUMBER, "rolloff", (EntityGetterFunction)&GetRollOff, (EntitySetterFunction)&SetRollOff, "control=slider|min=0|max=20");
    tSoundEmitter->RegisterProperty(tTRUTH, "doppler_effect_on", (EntityGetterFunction)&DopplerEffectOn, (EntitySetterFunction)&SetDopplerEffectOn, "control=truth");
    tSoundEmitter->RegisterProperty(tNUMBER, "doppler_factor", (EntityGetterFunction)&GetDopplerFactor, (EntitySetterFunction)&SetDopplerFactor, "control=slider|min=0|max=4");
    tSoundEmitter->RegisterProperty(tNUMBER, "volume", (EntityGetterFunction)&GetVolume, (EntitySetterFunction)&SetVolume, "control=slider|min=0|max=1");
    tSoundEmitter->RegisterProperty(tTRUTH, "lfe_channel", (EntityGetterFunction)&HasLFEChannel, (EntitySetterFunction)&SetHasLFEChannel, "control=truth");
    tSoundEmitter->RegisterProperty(tTRUTH, "looped", (EntityGetterFunction)&IsLooped, (EntitySetterFunction)&SetIsLooped, "control=truth");
    tSoundEmitter->RegisterProperty(tNUMBER, "pitch", (EntityGetterFunction)&GetPitch, (EntitySetterFunction)&SetPitch, "control=slider|min=0|max=5");
    tSoundEmitter->RegisterProperty(tTRUTH, "no_update_pos", (EntityGetterFunction)&NoUpdatePos, (EntitySetterFunction)&SetNoUpdatePos, nullptr);
    tSoundEmitter->RegisterProperty(tTRUTH, "isplaying", (EntityGetterFunction)&IsPlaying, nullptr, nullptr);
    tSoundEmitter->RegisterProperty(tTRUTH, "randompitch_mode", (EntityGetterFunction)&RandomPitchMode, (EntitySetterFunction)&SetRandomPitchMode, "control=truth");
    tSoundEmitter->RegisterProperty(tNUMBER, "randompitch_length", (EntityGetterFunction)&GetRandomPitchLength, (EntitySetterFunction)&SetRandomPitchLength, "control=slider|min=0|max=2");
    tSoundEmitter->RegisterProperty(tTRUTH, "randomvolume_mode", (EntityGetterFunction)&RandomVolumeMode, (EntitySetterFunction)&SetRandomVolumeMode, "control=truth");
    tSoundEmitter->RegisterProperty(tNUMBER, "randomvolume_length", (EntityGetterFunction)&GetRandomVolumeLength, (EntitySetterFunction)&SetRandomVolumeLength, "control=slider|min=0|max=2");
    tSoundEmitter->RegisterProperty(tTRUTH, "send_message_to_parent", (EntityGetterFunction)&ShouldSendMessageToParent, (EntitySetterFunction)&SetShouldSendMessageToParent, "control=truth");
    tSoundEmitter->RegisterProperty(tTRUTH, "follow_camera", (EntityGetterFunction)&ShouldFollowCamera, (EntitySetterFunction)&SetShouldFollowCamera, "control=truth");
    tSoundEmitter->RegisterProperty(tVECTOR, "follow_cam_offset", (EntityGetterFunction)&GetFollowCamOffset, (EntitySetterFunction)&SetFollowCamOffset, "control=string");
    tSoundEmitter->RegisterProperty(tNUMBER, "end_block_delay_time", (EntityGetterFunction)&GetEndBlockDelayTime, nullptr, nullptr);
    tSoundEmitter->RegisterProperty(tEntity, "current_soundslot", (EntityGetterFunction)&GetCurrentSoundSlot, nullptr, nullptr);
    tSoundEmitter->RegisterProperty(tTRUTH, "next_play_as_2d", (EntityGetterFunction)&NextPlayAs2D, (EntitySetterFunction)&SetNextPlayAs2D, nullptr);
    tSoundEmitter->RegisterProperty(tTRUTH, "is_curr_soundslot_streamed_and_alloc", (EntityGetterFunction)&IsCurrSoundSlotStreamedAndAlloc, nullptr, nullptr);

    tSoundEmitter->RegisterScript("playslot(entity,integer)", (EntityFunctionMember)&PlaySlot);
    tSoundEmitter->RegisterScript("playemitter(entity,integer)", (EntityFunctionMember)&PlayEmitter);
    tSoundEmitter->RegisterScript("pause", (EntityFunctionMember)&Pause);
    tSoundEmitter->RegisterScript("unpause", (EntityFunctionMember)&UnPause);
    tSoundEmitter->RegisterScript("stop", (EntityFunctionMember)&Stop);
    tSoundEmitter->RegisterScript("playmode_stop", (EntityFunctionMember)&PlayModeStop);
    tSoundEmitter->RegisterScript("dealloc_curr_stream", (EntityFunctionMember)&DeallocCurrStream);

    tSoundEmitter->PropagateProperties();
}

SoundEmitter* SoundEmitter::Create(AllocatorIndex)
{
    return new SoundEmitter;
}