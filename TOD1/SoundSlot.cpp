#include "SoundSlot.h"
#include "StringType.h"
#include "NumberType.h"
#include "TruthType.h"
#include "IntegerType.h"
#include "AssetManager.h"
#include "StreamedSoundBuffers.h"
#include "Scene.h"
#include "SoundEmitter.h"

int SoundSlot::StreamAllocateFailedCommand;
int SoundSlot::StreamAllocateFinishedCommand;
String SoundSlot::StreamingSoundPath;
float SoundSlot::TimePitchFactor = 1.f;
std::vector<float> SoundSlot::VolumeGroupsList;
std::vector<float> SoundSlot::VolumeModifiersList;
std::vector<float> SoundSlot::GroupVolume;
bool SoundSlot::GlobalSoundLooped;
SoundFile SoundSlot::GlobalSoundFile;
StreamedSoundBuffer* SoundSlot::GlobalStreamedSound;

EntityType* tSoundSlot;

const char* const SoundSlot::GetSound() const
{
    if (m_SoundResource)
        return m_SoundResource.m_AssetPtr->GetName();

    if (!m_Sound.Empty())
        return m_Sound.m_Str;

    return nullptr;
}

void SoundSlot::SetSound(const char* const sound)
{
    if (!m_StreamingSoundResource)
    {
        LogDump::LogA("ERROR: You cannot specify a NON-STREAMED sound-file on a SoundSlot, if you already have specified a STREAMED sound-file. Clear the STREAMED sound-file first, before selecting a NON-STREAMED sound-file.\n");
        return;
    }

    //  TODO: !
}

const float SoundSlot::GetMinRange() const
{
    return m_MinRange;
}

void SoundSlot::SetMinRange(const float minrange)
{
    m_MinRange = minrange;
}

const float SoundSlot::GetMaxRange() const
{
    return m_MaxRange;
}

void SoundSlot::SetMaxRange(const float maxrange)
{
    m_MaxRange = maxrange;
}

const float SoundSlot::GetRollOff() const
{
    return m_RollOff;
}

void SoundSlot::SetRollOff(const float rolloff)
{
    m_RollOff = rolloff;
}

const bool SoundSlot::IsDopplerEffectOn() const
{
    return m_Flags.DopplerEffectOn;
}

void SoundSlot::SetDopplerEffectOn(const bool on)
{
    m_Flags.DopplerEffectOn = on;
}

const float SoundSlot::GetDopplerFactor() const
{
    return (float)(m_DopplerFactor * (1.f / 255.f));
}

void SoundSlot::SetDopplerFactor(const float factor)
{
    if (factor >= 0.f)
        if (factor <= 1.f)
            m_DopplerFactor = (unsigned char)((factor * 255.f) + 0.5f);
        else
            m_DopplerFactor = (unsigned char)1.f;
    else
        m_DopplerFactor = (unsigned char)0.f;
}

const bool SoundSlot::CutAtMaxRange() const
{
    return m_Flags.CutAtMaxRange;
}

void SoundSlot::SetCutAtMaxRange(const bool cut)
{
    m_Flags.CutAtMaxRange = cut;
}

const float SoundSlot::GetVolume() const
{
    return (float)(m_Volume * (1.f / 1000.f));
}

void SoundSlot::SetVolume(const float volume)
{
    m_Volume = (unsigned short)(volume * 10000.f);
}

const float SoundSlot::GetLFELevel() const
{
    return (float)(m_LfeLevel * (1.f / 255.f));
}

void SoundSlot::SetLFELevel(const float level)
{
    if (level >= 0.f)
        if (level <= 1.f)
            m_LfeLevel = (unsigned char)((level * 255.f) + 0.5f);
        else
            m_LfeLevel = -1;
    else
        m_LfeLevel = 0;
}

const bool SoundSlot::CenterChannelOnly() const
{
    return m_Flags.CenterChannelOnly;
}

void SoundSlot::SetCenterChannelOnly(const bool centerchannelonly)
{
    m_Flags.CenterChannelOnly = centerchannelonly;
}

const bool SoundSlot::HasBackChannels() const
{
    return m_Flags.BackChannels;
}

void SoundSlot::SetHasBackChannels(const bool hasbackchannels)
{
    m_Flags.BackChannels = hasbackchannels;
}

const bool SoundSlot::IsLooped() const
{
    return m_Flags.Looped;
}

void SoundSlot::SetIsLooped(const bool looped)
{
    m_Flags.Looped = looped;
}

const float SoundSlot::GetPitch() const
{
    return (float)(m_Pitch * (1.f / 10000.f));
}

void SoundSlot::SetPitch(const float pitch)
{
    m_Pitch = (unsigned short)(pitch * 10000.f);
}

const bool SoundSlot::HasHardPause() const
{
    return m_Flags.HardPause;
}

void SoundSlot::SetHasHardPause(const bool hardpause)
{
    m_Flags.HardPause = hardpause;
}

const bool SoundSlot::FollowCamera() const
{
    return m_Flags.FollowCamera;
}

void SoundSlot::SetFollowCamera(const bool follow)
{
    m_Flags.FollowCamera = follow;
}

const bool SoundSlot::HasRandomPitchMode() const
{
    return m_Flags.RandomPitchMode;
}

void SoundSlot::SetHasRandomPitchMode(const bool enabled)
{
    m_Flags.RandomPitchMode = enabled;
}

const float SoundSlot::GetRandomPitchLength() const
{
    return (float)(m_RandomPitchLength * (1.f / 10000.f));
}

void SoundSlot::SetRandomPitchLength(const float length)
{
    m_RandomPitchLength = (unsigned short)(length * 10000.f);
}

const bool SoundSlot::HasRandomVolumeMode() const
{
    return m_Flags.RandomVolumeMode;
}

void SoundSlot::SetHasRandomVolumeMode(const bool enabled)
{
    m_Flags.RandomVolumeMode = enabled;
}

const float SoundSlot::GetRandomVolumeLength() const
{
    return (float)(m_RandomVolumeLength * (1.f / 10000.f));
}

void SoundSlot::SetRandomVolumeLength(const float length)
{
    m_RandomVolumeLength = (unsigned short)(length * 10000.f);
}

const bool SoundSlot::HasStaticMode() const
{
    return m_Flags.StaticMode;
}

void SoundSlot::SetHasStaticMode(const bool enabled)
{
    m_Flags._6 = 1;

    if (!m_Flags.StaticMode && enabled)
        LoadRes(true);

    m_Flags.StaticMode = enabled;

    if (m_StreamingSoundResource)
        m_Flags.StaticMode = true;

    if (m_Flags._5)
        LoadSoundResource(m_Sound.m_Str);
}

const char* const SoundSlot::GetStreamingSound()
{
    delete[] m_StreamingSoundName;

    if (!m_StreamingSoundResource)
        return nullptr;

    m_StreamingSoundName = new char[52];
    strcpy(m_StreamingSoundName, m_StreamingSoundResource.m_AssetPtr->GetName());
    strcpy(strrchr(m_StreamingSoundName, '.'), ".wav");

    return m_StreamingSoundName;
}

void SoundSlot::SetStreamingSound(const char* const sound)
{
    if (sound && (m_SoundResource || !m_Sound.Empty()))
    {
        LogDump::LogA("ERROR: You cannot specify a STREAMED sound-file on a SoundSlot, if you already have specified a NON-STREAMED sound-file. Clear the NON-STREAMED sound-file first, before selecting a STREAMED sound-file.\n");
        return;
    }

    DeallocateStream_Impl();

    if (sound && sound[0])
    {
        String fileName, fileDir, fullFilePath;
        File::ExtractFileName(fileName, sound);
        File::ExtractFileDir(fileDir, sound);

        fullFilePath = fileDir;
        fullFilePath.Append(fileName.m_Str);
        fullFilePath.Append(".stream");

        m_StreamingSoundResource = AssetLoader(fullFilePath.m_Str);
    }
    else
    {
        m_StreamingSoundResource.m_AssetPtr = nullptr;
    }

    if (m_StreamingSoundResource)
        m_Flags.HasStreamedSound = 1;
}

const bool SoundSlot::Is3DEnabled() const
{
    return m_Flags.Is3D;
}

void SoundSlot::Set3DEnabled(const bool enabled)
{
    m_Flags.Is3D = enabled;
}

const int SoundSlot::GetPriority() const
{
    return m_Flags.Priority;
}

void SoundSlot::SetPriority(const int priority)
{
    m_Flags.Priority = priority;
}

const bool SoundSlot::IsSoloMode() const
{
    return m_Flags.SoloMode;
}

void SoundSlot::SetIsSoloMode(const bool solomode)
{
    m_Flags.SoloMode = solomode;
}

const bool SoundSlot::IsPlaying() const
{
    if ((m_SoundResource || m_StreamBuffer) && g_StreamedSoundBuffers)
    {
        if (m_StreamBuffer)
            return m_StreamBuffer->IsPlaying(m_StreamBufferIndex);

        if (!m_SoundResource)
            return false;

        //  NOTE: this seems to never be the case and the types are not related.
        return ((StreamedSoundBuffer*)((StreamedSoundInfoAsset*)m_SoundResource.m_AssetPtr)->m_StreamBuffer)->IsPlaying(m_StreamBufferIndex);
    }

    return false;
}

const char* const SoundSlot::GetStreamingSoundFullPathDirect() const
{
    if (!m_StreamingSoundResource)
        return nullptr;

    char buffer[512] = {};
    strcpy(buffer, m_StreamingSoundResource.m_AssetPtr->m_ResourcePath);
    strcpy(strrchr(buffer, '.'), ".wav");

    StreamingSoundPath = buffer;

    return StreamingSoundPath.m_Str;
}

const int SoundSlot::GetGroup() const
{
    return m_Flags.Group;
}

void SoundSlot::SetGroup(const int group)
{
    m_Flags.Group = group;
}

const float SoundSlot::GetGlobalTimePitchFactor() const
{
    return TimePitchFactor;
}

void SoundSlot::SetGlobalTimePitchFactor(const float factor) const
{
    TimePitchFactor = factor;
}

void SoundSlot::PlayAt(int* args)
{
    const Vector4f pos((float)args[0], (float)args[1], (float)args[2], 0.f);
    Play_Impl(pos);
}

void SoundSlot::Play(int* args)
{
    StopSound();

    m_Flags._16 = 1;
    Play_Impl({ 0, 0, 0, 0 });
    m_Flags._16 = 0;
}

void SoundSlot::EditorPlay(int* args)
{
    StopSound();
    DeallocateStreams(nullptr);

    m_Flags._16 = 1;

    if (Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_STOP)
        TriggerGlobalScript(GetCommandByName("play_pressed"), nullptr);

    Play_Impl({ 0, 0, 0, 0 });
    m_Flags._16 = 0;
}

void SoundSlot::Stop(int* args)
{
    StopSound();
}

void SoundSlot::Pause(int* args)
{
    SetPause();
}

void SoundSlot::UnPause(int* args)
{
    SetUnPause();
}

void SoundSlot::PlayModeStop(int* args)
{
    StopSound();
}

void SoundSlot::LoadResource(int* args)
{
    LoadRes(true);
}

void SoundSlot::UnLoadResource(int* args)
{
    UnloadRes(true);
}

void SoundSlot::AllocateStream(int* args)
{
    AllocateStream_Impl((Node*)args[0], args[1] != 0);
}

void SoundSlot::DeallocateStream(int* args)
{
    DeallocateStream_Impl();
}

const bool SoundSlot::IsStreamAllocated(int* args) const
{
    args[0] = m_StreamBuffer != nullptr;
    return (bool)args[0];
}

void SoundSlot::GlobalDeallocateAllStreams(int* args)
{
    DeallocateStreams((std::vector<SoundSlot*>*)args[0]);
}

void SoundSlot::AddSoundEmitter(SoundEmitter* soundemitter)
{
    if (!m_SoundEmittersList.size())
    {
        m_SoundEmittersList.push_back(soundemitter);
    }
    else
    {
        auto it = std::find(m_SoundEmittersList.begin(), m_SoundEmittersList.end(), soundemitter);
        if (it == m_SoundEmittersList.end())
            m_SoundEmittersList.push_back(soundemitter);
    }
}

void SoundSlot::RemoveSoundEmitter(SoundEmitter* soundemitter)
{
    m_SoundEmittersList.erase(std::find(m_SoundEmittersList.begin(), m_SoundEmittersList.end(), soundemitter));
}

void SoundSlot::LoadSoundResource(const char* const sound)
{
    char soundPath[512] = {};
    if (m_Flags.StaticMode)
    {
        m_SoundResource = AssetLoader(sound);

        g_AssetManager->GetResourcePathSceneRelative(sound);
        m_Sound = soundPath;
    }
    else
    {
        if (m_StreamingSoundResource)
            return;

        g_AssetManager->GetResourcePathSceneRelative(sound);
        m_Sound = soundPath;
    }
}

void SoundSlot::StopSound()
{
    if ((m_SoundResource || m_StreamBuffer) && g_StreamedSoundBuffers)
    {
        StreamedSoundBuffer* sndBuffer = m_StreamBuffer ? m_StreamBuffer : (StreamedSoundBuffer*)((StreamedSoundInfoAsset*)m_SoundResource.m_AssetPtr)->m_StreamBuffer;
        if (!sndBuffer)
            return;

        sndBuffer->Stop(m_StreamBufferIndex);

        if (Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_STOP)
            DeallocateStream_Impl();
    }
}

void SoundSlot::SetPause()
{
    if ((m_SoundResource || m_StreamBuffer) && g_StreamedSoundBuffers)
    {
        StreamedSoundBuffer* sndBuffer = m_StreamBuffer ? m_StreamBuffer : (StreamedSoundBuffer*)((StreamedSoundInfoAsset*)m_SoundResource.m_AssetPtr)->m_StreamBuffer;
        if (m_PausedInstanceIndex != m_StreamBufferIndex && m_PausedInstanceIndex >= 0)
        {
            LogDump::LogA("WARNING: A paused sound-instance was released, because a new instance is paused.\n");
            sndBuffer->UnPause(m_PausedInstanceIndex, false);
        }

        m_PausedInstanceIndex = m_StreamBufferIndex;
        sndBuffer->SetPause(m_StreamBufferIndex, false);
    }
}

void SoundSlot::SetUnPause()
{
    if ((m_SoundResource || m_StreamBuffer) && g_StreamedSoundBuffers)
    {
        StreamedSoundBuffer* sndBuffer = m_StreamBuffer ? m_StreamBuffer : (StreamedSoundBuffer*)((StreamedSoundInfoAsset*)m_SoundResource.m_AssetPtr)->m_StreamBuffer;
        if (m_PausedInstanceIndex >= 0)
        {
            sndBuffer->UnPause(m_PausedInstanceIndex, false);
            m_PausedInstanceIndex = -1;
        }
    }
}

bool SoundSlot::LoadRes(const bool b)
{
    if (m_Flags.StaticMode)
        return false;

    if (m_StreamingSoundResource)
    {
        LogDump::LogA("SoundSlot::LoadRes() : SoundSlot is in Streaming mode, so Dynamic Mode does not have any effect.\n");
        return 0;
    }

    StreamedSoundBuffer* sndBuffer = m_StreamBuffer ? m_StreamBuffer : (StreamedSoundBuffer*)((StreamedSoundInfoAsset*)m_SoundResource.m_AssetPtr)->m_StreamBuffer;
    if (sndBuffer)
    {
        if (b)
            sndBuffer->_440850(0);

        if (sndBuffer->AreAnyInstancesPlaying())
        {
            LogDump::LogA("Can't LoadRes() because there are still instances of SoundSlot:'%s' playing.\n", GetSoundName());
            return false;
        }
    }

    m_SoundResource = AssetLoader(m_Sound.m_Str);
    if (m_SoundResource)
    {
        return true;
    }
    else
    {
        LogDump::LogA("Couldn't find Sound-file '%s'.\n", m_Sound.m_Str);
        return false;
    }
}

const char* const SoundSlot::GetSoundName()
{
    char* sndResName = nullptr;

    if (m_SoundResource)
        sndResName = (char*)m_SoundResource.m_AssetPtr->GetName();

    if (!m_Sound.Empty())
        sndResName = m_Sound.m_Str;

    if (!sndResName)
        sndResName = (char*)GetStreamingSound();

    return sndResName;
}

bool SoundSlot::UnloadRes(const bool b)
{
    if (m_Flags.StaticMode || m_StreamingSoundResource)
        return false;

    StreamedSoundBuffer* sndBuffer = m_StreamBuffer ? m_StreamBuffer : (StreamedSoundBuffer*)((StreamedSoundInfoAsset*)m_SoundResource.m_AssetPtr)->m_StreamBuffer;
    if (sndBuffer)
    {
        if (b)
            sndBuffer->_440850(0);

        g_StreamedSoundBuffers->UpdateSoundBuffers();

        if (sndBuffer->AreAnyInstancesPlaying())
        {
            LogDump::LogA("Can't UnloadRes() because there are still instances of SoundSlot:'%s' playing.\n", GetSoundName());
            return false;
        }
    }

    m_SoundResource.m_AssetPtr = nullptr;
    return true;
}

void SoundSlot::AllocateStream_Impl(Node* allocationCallbackNode, const bool stereoStream)
{
    m_StreamAllocationCallback = allocationCallbackNode;
    if (m_StreamingSoundResource)
    {
        if (Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_STOP || !m_StreamBuffer)
        {
            if (m_StreamBuffer)
                DeallocateStream_Impl();

            m_StreamBuffer = new StreamedSoundBuffer(((StreamedSoundInfoAsset*)m_StreamingSoundResource.m_AssetPtr)->m_StreamBuffer, 0, m_Flags.Is3D, m_Flags.Looped & 0xFFFFFF01, 0, stereoStream);
            if (!m_StreamBuffer->IsMonoStreamCreated())
            {
                if (m_StreamAllocationCallback)
                {
                    m_StreamAllocationCallback->TriggerGlobalScript(StreamAllocateFailedCommand, nullptr);
                    m_StreamAllocationCallback->TriggerGlobalScript(StreamAllocateFinishedCommand, nullptr);
                    m_StreamAllocationCallback = nullptr;
                }
                DeallocateStream_Impl();
            }
        }
    }
    else
    {
        LogDump::LogA("SoundSlot::AllocateStream() : Was called with in invalid info.\n");
        if (m_StreamAllocationCallback)
        {
            m_StreamAllocationCallback->TriggerGlobalScript(StreamAllocateFailedCommand, nullptr);
            m_StreamAllocationCallback->TriggerGlobalScript(StreamAllocateFinishedCommand, nullptr);
            m_StreamAllocationCallback = nullptr;
        }
    }
}

void SoundSlot::DeallocateStream_Impl()
{
    if (!m_StreamBuffer)
        return;

    for (unsigned int i = 0; i < m_SoundEmittersList.size(); ++i)
        m_SoundEmittersList[i]->m_SoundSlot = nullptr;

    m_SoundEmittersList.clear();

    delete m_StreamBuffer;
}

void SoundSlot::DumpStreamedSoundsToConsole()
{
}

void SoundSlot::Register()
{
    tSoundSlot = new EntityType("SoundSlot");
    tSoundSlot->InheritFrom(tNode);
    tSoundSlot->SetCreator((CREATOR)Create);

    tSoundSlot->RegisterProperty(tSTRING, "sound", (EntityGetterFunction)&GetSound, (EntitySetterFunction)&SetSound, "control=resource|type=*.wav");
    tSoundSlot->RegisterProperty(tNUMBER, "minrange", (EntityGetterFunction)&GetMinRange, (EntitySetterFunction)&SetMinRange, "control=slider|min=1|max=5000");
    tSoundSlot->RegisterProperty(tNUMBER, "maxrange", (EntityGetterFunction)&GetMaxRange, (EntitySetterFunction)&SetMaxRange, "control=slider|min=1|max=5000");
    tSoundSlot->RegisterProperty(tNUMBER, "rolloff", (EntityGetterFunction)&GetRollOff, (EntitySetterFunction)&SetRollOff, "control=slider|min=0|max=20");
    tSoundSlot->RegisterProperty(tTRUTH, "doppler_effect_on", (EntityGetterFunction)&IsDopplerEffectOn, (EntitySetterFunction)&SetDopplerEffectOn, "control=truth");
    tSoundSlot->RegisterProperty(tNUMBER, "doppler_factor", (EntityGetterFunction)&GetDopplerFactor, (EntitySetterFunction)&SetDopplerFactor, "control=slider|min=0|max=1");
    tSoundSlot->RegisterProperty(tTRUTH, "cut_at_maxrange", (EntityGetterFunction)&CutAtMaxRange, (EntitySetterFunction)&SetCutAtMaxRange, "control=truth");
    tSoundSlot->RegisterProperty(tNUMBER, "volume", (EntityGetterFunction)&GetVolume, (EntitySetterFunction)&SetVolume, "control=slider|min=0|max=1");
    tSoundSlot->RegisterProperty(tNUMBER, "lfe_level", (EntityGetterFunction)&GetLFELevel, (EntitySetterFunction)&SetLFELevel, "control=slider|min=0|max=1");
    tSoundSlot->RegisterProperty(tTRUTH, "center_channel_only", (EntityGetterFunction)&CenterChannelOnly, (EntitySetterFunction)&SetCenterChannelOnly, "control=truth");
    tSoundSlot->RegisterProperty(tTRUTH, "back_channels", (EntityGetterFunction)&HasBackChannels, (EntitySetterFunction)&SetHasBackChannels, "control=truth");
    tSoundSlot->RegisterProperty(tTRUTH, "looped", (EntityGetterFunction)&IsLooped, (EntitySetterFunction)&SetIsLooped, "control=truth");
    tSoundSlot->RegisterProperty(tNUMBER, "pitch", (EntityGetterFunction)&GetPitch, (EntitySetterFunction)&SetPitch, "control=slider|min=0|max=5");
    tSoundSlot->RegisterProperty(tTRUTH, "hard_pause", (EntityGetterFunction)&HasHardPause, (EntitySetterFunction)&SetHasHardPause, "control=truth");
    tSoundSlot->RegisterProperty(tTRUTH, "follow_camera", (EntityGetterFunction)&FollowCamera, (EntitySetterFunction)&SetFollowCamera, "control=truth");
    tSoundSlot->RegisterProperty(tTRUTH, "randompitch_mode", (EntityGetterFunction)&HasRandomPitchMode, (EntitySetterFunction)&SetHasRandomPitchMode, "control=truth");
    tSoundSlot->RegisterProperty(tNUMBER, "randompitch_length", (EntityGetterFunction)&GetRandomPitchLength, (EntitySetterFunction)&SetRandomPitchLength, "control=slider|min=0|max=2");
    tSoundSlot->RegisterProperty(tTRUTH, "randomvolume_mode", (EntityGetterFunction)&HasRandomVolumeMode, (EntitySetterFunction)&SetHasRandomVolumeMode, "control=truth");
    tSoundSlot->RegisterProperty(tNUMBER, "randomvolume_length", (EntityGetterFunction)&GetRandomVolumeLength, (EntitySetterFunction)&SetRandomVolumeLength, "control=slider|min=0|max=2");
    tSoundSlot->RegisterProperty(tTRUTH, "static_mode", (EntityGetterFunction)&HasStaticMode, (EntitySetterFunction)&SetHasStaticMode, "control=truth");
    tSoundSlot->RegisterProperty(tSTRING, "streaming_sound", (EntityGetterFunction)&GetStreamingSound, (EntitySetterFunction)&SetStreamingSound, "control=resource|type=*.wav");
    tSoundSlot->RegisterProperty(tTRUTH, "enable3d", (EntityGetterFunction)&Is3DEnabled, (EntitySetterFunction)&Set3DEnabled, "control=truth");
    tSoundSlot->RegisterProperty(tINTEGER, "priority", (EntityGetterFunction)&GetPriority, (EntitySetterFunction)&SetPriority, "control=slider|min=1|max=8");
    tSoundSlot->RegisterProperty(tTRUTH, "solomode", (EntityGetterFunction)&IsSoloMode, (EntitySetterFunction)&SetIsSoloMode, "control=truth");
    tSoundSlot->RegisterProperty(tTRUTH, "isplaying", (EntityGetterFunction)&IsPlaying, nullptr, nullptr);
    tSoundSlot->RegisterProperty(tSTRING, "streaming_sound_fullpath_direct", (EntityGetterFunction)&GetStreamingSoundFullPathDirect, nullptr, nullptr);
    tSoundSlot->RegisterProperty(tINTEGER, "group", (EntityGetterFunction)&GetGroup, (EntitySetterFunction)&SetGroup, "control=string");
    tSoundSlot->RegisterProperty(tNUMBER, "global_time_pitchfactor", (EntityGetterFunction)&GetGlobalTimePitchFactor, (EntitySetterFunction)&SetGlobalTimePitchFactor, "control=slider|min=0.01|max=2.0");

    tSoundSlot->RegisterScript("play(vector)", (EntityFunctionMember)&PlayAt);
    tSoundSlot->RegisterScript("play", (EntityFunctionMember)&Play);
    tSoundSlot->RegisterScript("editor_play", (EntityFunctionMember)&EditorPlay, 0, 0, 0, "control=button|text=play");
    tSoundSlot->RegisterScript("stop", (EntityFunctionMember)&Stop, 0, 0, 0, "control=button|text=stop");
    tSoundSlot->RegisterScript("pause", (EntityFunctionMember)&Pause, 0, 0, 0, "control=button|text=pause");
    tSoundSlot->RegisterScript("unpause", (EntityFunctionMember)&UnPause, 0, 0, 0, "control=button|text=unpause");
    tSoundSlot->RegisterScript("playmode_stop", (EntityFunctionMember)&PlayModeStop);
    tSoundSlot->RegisterScript("load_resource", (EntityFunctionMember)&LoadResource, 0, 0, 0, "control=button|text=load_resource");
    tSoundSlot->RegisterScript("unload_resource", (EntityFunctionMember)&UnLoadResource, 0, 0, 0, "control=button|text=unload_resource");
    tSoundSlot->RegisterScript("allocate_stream(entity,truth)", (EntityFunctionMember)&AllocateStream);
    tSoundSlot->RegisterScript("deallocate_stream", (EntityFunctionMember)&DeallocateStream_Impl);
    tSoundSlot->RegisterScript("is_stream_allocated:truth", (EntityFunctionMember)&IsStreamAllocated);
    tSoundSlot->RegisterScript("global_dealloc_allstreams(list(entity))", (EntityFunctionMember)&GlobalDeallocateAllStreams);
    tSoundSlot->RegisterScript("dump_streamed_sounds_to_console", (EntityFunctionMember)&DumpStreamedSoundsToConsole, 0, 0, 0, "control=button|text=console_dump_stream_sounds");

    GetCommandId("subtitle_update", true);
    StreamAllocateFailedCommand = GetCommandId("stream_allocate_failed", true);
    StreamAllocateFinishedCommand = GetCommandId("stream_allocate_finished", true);

    tSoundSlot->PropagateProperties();
}

SoundSlot* SoundSlot::Create(AllocatorIndex)
{
    return new SoundSlot;
}

void SoundSlot::DeallocateStreams(std::vector<SoundSlot*>* soundSlotsList)
{
    for (Entity* node = g_AssetManager->FindFirstEntity(); node; node = g_AssetManager->FindNextEntity(node))
    {
        EntityType* entScript = node->m_ScriptEntity;
        if (!entScript)
            continue;

        while (tSoundSlot != entScript)
        {
            entScript = entScript->m_Parent;
            if (!entScript)
                break;
        }

        if (!entScript)
            continue;

        if (soundSlotsList && soundSlotsList->size())
        {
            bool cont = false;
            for (unsigned int i = 0; i < soundSlotsList->size(); ++i)
            {
                if (node == (*soundSlotsList)[i])
                {
                    cont = true;
                    break;
                }
            }

            if (!cont)
            {
                if (((SoundSlot*)node)->m_StreamBuffer)
                    ((SoundSlot*)node)->DeallocateStream_Impl();
            }
        }
        else
        {
            if (((SoundSlot*)node)->m_StreamBuffer)
                ((SoundSlot*)node)->DeallocateStream_Impl();
        }
    }
}

void SoundSlot::UpdateSoundGroups(const float timePassed)
{
    if (!VolumeGroupsList.size())
        return;

    for (unsigned int group = 0; group < VolumeGroupsList.size(); ++group)
    {
        GroupVolume[group] += timePassed * 2.f;
        if (GroupVolume[group] >= 1.f)
            GroupVolume[group] = 1.f;
    }
}

bool SoundSlot::AllocateGlobalStreamedSound(const char* const soundname, const bool monosound, const bool loopedsound)
{
    delete GlobalStreamedSound;

    GlobalSoundLooped = loopedsound;
    GlobalSoundFile.Open(soundname);
    GlobalStreamedSound = new StreamedSoundBuffer(&GlobalSoundFile, 0, monosound, loopedsound, false, true);

    if (GlobalStreamedSound->IsMonoStreamCreated())
        return true;

    delete GlobalStreamedSound;
    return false;
}

void SoundSlot::DeallocateGlobalStreamedSound()
{
    delete GlobalStreamedSound;
}

bool SoundSlot::PlayGlobalStreamedSound()
{
    if (GlobalStreamedSound && !GlobalStreamedSound->IsMonoStreamCreated())
            return false;

    GlobalStreamedSound->field_B = GlobalStreamedSound->field_A;
    GlobalStreamedSound->field_A = 0;
    GlobalStreamedSound->field_1C |= 0x10000000;
    GlobalStreamedSound->field_1C &= ~0x4000000;

    GlobalStreamedSound->SetVolume(0, 1.f);
    GlobalStreamedSound->SetFrequencyMultiplier(0, 1.f);
    GlobalStreamedSound->Play(0, GlobalSoundLooped, 0);

    return true;
}

bool SoundSlot::StopGlobalStreamedSound()
{
    if (!GlobalStreamedSound)
        return false;

    GlobalStreamedSound->Stop(0);

    return true;
}

bool SoundSlot::SetVolumePitchGlobalStreamedSound(const float volume, const float pitch)
{
    if (!GlobalStreamedSound)
        return false;

    float volumeModifier = 0;
    if (GroupVolume[2] < 1.f)
        volumeModifier = ((1.f - GroupVolume[2]) * VolumeModifiersList[2] + VolumeGroupsList[2] * GroupVolume[2]);
    else
        volumeModifier = VolumeGroupsList[2];

    volumeModifier = g_StreamedSoundBuffers->GetDefaultVolumeForType(2) * volumeModifier * volume;

    GlobalStreamedSound->SetVolume(0, volumeModifier);
    GlobalStreamedSound->SetFrequencyMultiplier(0, pitch);

    return true;
}