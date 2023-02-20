#include "StreamedSoundBuffer.h"
#include "LogDump.h"
#include "StreamedSoundBuffers.h"
#include "StreamedWAV.h"
#include "BuiltinType.h"
#include "Timer.h"
#include "Scene.h"

StreamedSoundBuffer::~StreamedSoundBuffer()
{
    MESSAGE_CLASS_DESTROYED(StreamedSoundBuffer);
    LogDump::LogA("Deleting streamed sound buffer....\n");

    ShutdownThread();

    LogDump::LogA("Streamed sound buffer deleted\n");
}

#pragma message(TODO_IMPLEMENTATION)
StreamedSoundBuffer::StreamedSoundBuffer(SoundFile* sndfile, int, int, int, char, char)
{
    MESSAGE_CLASS_CREATED(StreamedSoundBuffer);
}

#pragma message(TODO_IMPLEMENTATION)
StreamedSoundBuffer::StreamedSoundBuffer(bool a2, unsigned int totalchunks, int channels, unsigned int isstereo, float bytespersec, unsigned int frequency, bool ismonosound, unsigned int a9, char a10, const char* filepath)
{
    MESSAGE_CLASS_CREATED(StreamedSoundBuffer);
}

void StreamedSoundBuffer::stub2()
{
}

void StreamedSoundBuffer::stub3()
{
}

void StreamedSoundBuffer::SetSampledData(void*)
{
}

bool StreamedSoundBuffer::Is3DSound(const uint32_t streamIndex) const
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        return DieselPower3DStream != nullptr;

    if (DirectSound3DBuffer)
    {
        DWORD sound3DMode;
        DirectSound3DBuffer->GetMode(&sound3DMode);
        return sound3DMode != DS3DMODE_DISABLE;
    }

    return false;
}

void StreamedSoundBuffer::Get3DMode(unsigned char mode3d)
{
    if (mode3d && DirectSound3DBuffer)
        DirectSound3DBuffer->GetMode((LPDWORD)&mode3d);

    field_1C ^= (field_1C ^ (mode3d << 24)) & 0x1000000;
}

bool StreamedSoundBuffer::IsLooped(const uint32_t streamIndex) const
{
    return Flags.Looped;
}

int StreamedSoundBuffer::Play(const uint32_t streamIndex, bool looped, int)
{
    Flags._4 = 0;
    g_StreamedSoundBuffers->_43D200(DirectSoundBuffer, field_20, (field_1C >> 20) & 0xFFFFFF01, (field_1C >> 30) & 0xFFFFFF01, Is3DSound(0));
    field_54 = field_54 & 0xFFFF00FF | (((field_54 >> 8) + 1) << 8);

    if (!Flags.PreLoaded)
        LogDump::LogA("Play called on (%s), but has not yet finished pre-loading.\n", StreamedWAVSound->m_FileName.m_Str);

    if (!Flags.PlayRequest)
    {
        Flags.Looped = looped;
        Flags.PlayRequest = true;
    }

    return 0;
}

bool StreamedSoundBuffer::IsPlaying(const uint32_t streamIndex) const
{
    return (Flags.PlayRequest && Flags._7);
}

bool StreamedSoundBuffer::IsFirstChannelPlaying() const
{
    return IsPlaying(0);
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::_440850(int)
{
}

void StreamedSoundBuffer::Stop(const uint32_t streamIndex)
{
    field_54 = field_54 & 0xFF00FFFF | (((unsigned short)field_54 + 1) << 16) & 0xFF0000;
    Flags.StopRequest = true;
    Flags._4 = true;

    if (StreamedWAVSound)
    {
        StoppingFinished = !DirectSoundBuffer && !DieselPowerSoundBuffer;

        if (field_A0)
            ResetEvent(field_A0);
        if (field_B0)
            SetEvent(field_B0);
        if (EventNotify)
            ResetEvent(EventNotify);
        if (m_EvHandle_2)
            ResetEvent(m_EvHandle_2);

        Flags.LastChunkPlaying = false;
        field_5C = NULL;
    }
}

void StreamedSoundBuffer::SetPause(const uint32_t streamIndex, bool hardpause)
{
    Flags.HardPause = hardpause;
    Flags.Paused = true;
}

void StreamedSoundBuffer::UnPause(const uint32_t streamIndex, bool hardpause)
{
    Flags.HardPause = hardpause;
    g_StreamedSoundBuffers->m_GlobalPauseCalled = true;
}

bool StreamedSoundBuffer::IsPaused(const uint32_t streamIndex) const
{
    return CurrentAudioPosition >= 0;
}

bool StreamedSoundBuffer::IsCreated() const
{
    return StreamedWAVSound && ThreadId;
}

uint8_t* StreamedSoundBuffer::GetBufferDataPtr()
{
    return SoundBufferBlockStartPtr;
}

void StreamedSoundBuffer::SetVolume(const uint32_t streamIndex, const float_t volume)
{
    if (!g_StreamedSoundBuffers->m_Sound || volume <= 0.f)
        Volume = 0.0000099999997;
    else
        Volume = clamp<float_t>(volume, 0.0000099999997, 1.f);
}

float_t StreamedSoundBuffer::GetVolume(const uint32_t streamIndex) const
{
    return Volume;
}

void StreamedSoundBuffer::SetFrequency(const uint32_t streamIndex, const float_t frequency)
{
    Frequency = SamplesPerSec * frequency < 100 ? 100 : SamplesPerSec * frequency;

    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPowerSoundBuffer)
        {
            DieselPowerSoundBuffer->SetFrequency(Frequency);
            DieselPowerSoundBuffer->stub27();
        }
    }
    else
    {
        if (DirectSoundBuffer)
            DirectSoundBuffer->SetFrequency((DWORD)Frequency);
    }
}

float_t StreamedSoundBuffer::GetFrequency(const uint32_t streamIndex) const
{
    double_t frequency = Frequency;

    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER && DieselPowerSoundBuffer)
    {
        frequency = DieselPowerSoundBuffer->GetFrequency();
    }
    else
    {
        if (DirectSoundBuffer)
            DirectSoundBuffer->GetFrequency((LPDWORD)&Frequency);

        frequency = Frequency;
    }

    if (frequency < 0)
        frequency += 4294967300.0;

    return (float_t)frequency / SamplesPerSec;
}

void StreamedSoundBuffer::SetPan(const uint32_t streamIndex, const float_t pan)
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (!DieselPower3DStream)
        {
            Pan = (uint32_t)(pan * 20.f);   //  TODO: __FYL2X__(pan, lg10(2)) * 20
            if (DieselPowerSoundBuffer)
                DieselPowerSoundBuffer->SetPan(pan);
        }
    }
    else
    {
        if (!DirectSound3DBuffer)
        {
            Pan = (uint32_t)(pan * 20.f);   //  TODO: __FYL2X__(pan, lg10(2)) * 20
            DirectSoundBuffer->SetPan(Pan);
        }
    }
}

float StreamedSoundBuffer::GetPan(const uint32_t streamIndex)
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        if (!DieselPower3DStream)
            return DieselPowerSoundBuffer->GetPan();
        else
            return 0.f;

    if (DirectSound3DBuffer)
        return 0.f;

    DirectSoundBuffer->GetPan((LPLONG)&Pan);

    return powf(10.0f, Pan * 0.05f);
}

void StreamedSoundBuffer::SetSoundPosition(const uint32_t streamIndex, const Vector4f& position)
{
    const float_t frameDelta = Scene::TimePassed > 0.0000099999997f ? 0.0000099999997f : Scene::TimePassed;
    const float_t invFrameDelta = 1.f / frameDelta;
    Vector4f currentPosition;
    GetPosition(currentPosition, 0);

    Position = position;

    Vector4f velocity = position - currentPosition;

    if (DopplerFactor == 0.f)
        velocity = BuiltinType::ZeroVector;
    else
        velocity = velocity * invFrameDelta;

    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPower3DStream)
        {
            DieselPower3DStream->SetPosition(position.x, position.y, position.z);
            DieselPower3DStream->SetVelocity(velocity.x, velocity.y, velocity.z);
        }
    }
    else
    {
        if (DirectSound3DBuffer)
        {
            DirectSound3DBuffer->SetPosition(position.x, position.y, position.z, DS3D_DEFERRED);
            HRESULT velocitySetResult = DirectSound3DBuffer->SetVelocity(velocity.x, velocity.y, velocity.z, DS3D_DEFERRED);
            if (FAILED(velocitySetResult))
                LogDump::LogA("Failed to set 3D sound position (%d)", velocitySetResult);
        }
    }
}

void StreamedSoundBuffer::GetPosition(Vector4f& outPos, const uint32_t streamIndex) const
{
    outPos = BuiltinType::ZeroVector;

    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPowerSoundBuffer)
        {
            DieselPowerSoundBuffer->GetPosition(outPos.x, outPos.y, outPos.z);
            return;
        }
        else
            return;
    }

    if (!DirectSound3DBuffer)
        return;

    DirectSound3DBuffer->GetPosition((D3DVECTOR*)&Position);
    outPos = Position;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::_443990(int, int*)
{
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* StreamedSoundBuffer::_4439E0(Vector4f*, int)
{
    return nullptr;
}

void StreamedSoundBuffer::SetMaxDistance(const uint32_t streamIndex, const float_t maxdistance)
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPower3DStream)
        {
            MaxDistance = maxdistance;
            DieselPower3DStream->SetAudioEnvironmentParams(MinDistance, MaxDistance, RollOff);
            DieselPower3DStream->stub27();
        }
    }
    else
    {
        if (DirectSound3DBuffer)
        {
            MaxDistance = maxdistance;
            DirectSound3DBuffer->SetMaxDistance(maxdistance, DS3D_DEFERRED);
        }
    }
}

float_t StreamedSoundBuffer::GetMaxDistance(const uint32_t streamIndex) const
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        if (DieselPowerSoundBuffer)
            return DieselPowerSoundBuffer->GetMaxDistance();
        else
            return NULL;

    if (!DirectSound3DBuffer)
        return NULL;

    DirectSound3DBuffer->GetMaxDistance((D3DVALUE*)&MaxDistance);
    return MaxDistance;
}

void StreamedSoundBuffer::SetMinDistance(const uint32_t streamIndex, const float_t minDistance)
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPower3DStream)
        {
            MinDistance = minDistance;
            DieselPower3DStream->SetAudioEnvironmentParams(minDistance, MaxDistance, RollOff);
            DieselPower3DStream->stub27();
        }
    }
    else
    {
        if (DirectSound3DBuffer)
        {
            MinDistance = minDistance;
            DirectSound3DBuffer->SetMinDistance(minDistance, DS3D_DEFERRED);
        }
    }
}

float_t StreamedSoundBuffer::GetMinDistance(const uint32_t streamIndex) const
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        if (DieselPower3DStream)
            return DieselPower3DStream->GetMinDistance();
        else
            return 0.f;

    D3DVALUE minDistance = 0.f;
    if (!DirectSound3DBuffer)
        return 0.f;
    else
        DirectSound3DBuffer->GetMinDistance(&minDistance);

    return minDistance;
}

void StreamedSoundBuffer::SetRollOff(const uint32_t streamIndex, const float_t rollOff)
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPower3DStream)
        {
            RollOff = rollOff;
            DieselPower3DStream->SetAudioEnvironmentParams(MinDistance, MaxDistance, rollOff);
            DieselPower3DStream->stub27();
        }
    }
    else
        if (DirectSound3DBuffer)
            RollOff = rollOff;
}

float_t StreamedSoundBuffer::GetRollOff(const uint32_t streamIndex) const
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        if (DieselPower3DStream)
            return DieselPower3DStream->GetRollOff();
        else
            return 0.f;

    if (!DirectSound3DBuffer)
        return 0.f;
    else
        return RollOff;
}

void StreamedSoundBuffer::SetDopplerFactor(const uint32_t streamIndex, const float_t factor)
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPower3DStream)
        {
            DopplerFactor = factor;
            DieselPower3DStream->SetDopplerFactor(factor);
            DieselPower3DStream->stub27();
        }
    }
    else
        if (DirectSound3DBuffer)
            DopplerFactor = factor;
}

float_t StreamedSoundBuffer::GetDopplerFactor(const uint32_t streamIndex) const
{
    return DopplerFactor;
}

void StreamedSoundBuffer::SetSoundProperties(const uint32_t streamIndex, const float_t minDistance, const float_t maxDistance, const float_t rollOff)
{
    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        if (DieselPower3DStream)
        {
            DieselPower3DStream->SetAudioEnvironmentParams(minDistance, maxDistance, rollOff);
            DieselPower3DStream->stub27();
        }
    }
    else
    {
        if (DirectSound3DBuffer)
        {
            RollOff = rollOff;
            MinDistance = minDistance;
            MaxDistance = maxDistance;
        }
    }
}

int StreamedSoundBuffer::GetChannelsNumber() const
{
    return Channels;
}

void StreamedSoundBuffer::DumpInfo()
{
    Vector4f sndpos;
    GetPosition(sndpos, NULL);

    LogDump::LogA("   %s, vol=%.1f, playing=%s, looped=%s, pos=(%.1f,%.1f,%.1f)\n",
        Is3DSound(0)  ? "3d" : "2d",
        GetVolume(0),
        IsPlaying(0),
        Flags.Looped,
        sndpos.x, sndpos.y, sndpos.z);
}

void StreamedSoundBuffer::StopFirstChannelSound()
{
    Stop(0);
}

void StreamedSoundBuffer::FillSoundData(const bool overwrite)
{
    char* soundbufferptr = nullptr;
    unsigned int soundbuffersize = NULL;

    if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        soundbufferptr = DieselPowerSoundBuffer->GetSoundBufferPtr();
        soundbuffersize = DieselPowerSoundBuffer->stub9();
    }
    else
    {
        unsigned int i = NULL;
        HRESULT hr = NULL;

        for (; i < 100; ++i)
        {
            hr = DirectSoundBuffer->Lock(NULL, SoundBufferBlockSize, (LPVOID*)&soundbufferptr, (LPDWORD)&soundbuffersize, nullptr, nullptr, DSBLOCK_ENTIREBUFFER);

            if (SUCCEEDED(hr))
                break;

            // NOTE: this method is executed from another thread, so SoundSystem MAY change at any moment.
            if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
            {
                soundbufferptr = DieselPowerSoundBuffer->GetSoundBufferPtr();
                soundbuffersize = DieselPowerSoundBuffer->stub9();
            }
        }

        if (i >= 100)
        {
            char* errmsgbuf = nullptr;
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, hr, 0x400, errmsgbuf, NULL, nullptr);
            delete errmsgbuf;
        }
    }

    if (overwrite)
    {
        if (StreamedWAVSound->m_SoundBufferPtr)
        {
            memcpy(soundbufferptr, StreamedWAVSound->m_SoundBufferPtr, 4 * (SoundBufferBlockSize >> 2));
            memcpy(&soundbufferptr[4 * (SoundBufferBlockSize >> 2)], &StreamedWAVSound->m_SoundBufferPtr[4 * (SoundBufferBlockSize >> 2)], SoundBufferBlockSize & 3);
        }
        else
            memset(soundbufferptr, NULL, 4 * (SoundBufferBlockSize >> 2) + (SoundBufferBlockSize & 3));
    }
    else
    {
        if (StreamedWAVSound->m_SoundBufferPtr)
            memcpy(soundbufferptr + SoundBufferBlockSize, StreamedWAVSound->m_SoundBufferPtr, SoundBufferBlockSize);
        else
            memset(soundbufferptr + SoundBufferBlockSize, NULL, 4 * (SoundBufferBlockSize >> 2) + (SoundBufferBlockSize & 3));
    }

    if (g_StreamedSoundBuffers->m_SoundSystem != SOUND_SYSTEM_DIESELPOWER)
    {
        unsigned int i = NULL;
        HRESULT hr = NULL;

        for (; i < 100; ++i)
        {
            hr = DirectSoundBuffer->Unlock(soundbufferptr, soundbuffersize, nullptr, NULL);

            if (SUCCEEDED(hr))
                break;
        }

        if (i >= 100)
        {
            char* errmsgbuf = nullptr;
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, hr, LANG_USER_DEFAULT, (LPSTR)&errmsgbuf, NULL, nullptr);
            delete errmsgbuf;
        }
    }
}

void StreamedSoundBuffer::ShutdownThread()
{
    Stop(NULL);

    if (TerminateThreadEvent)
        SetEvent(TerminateThreadEvent);

    if (m_StreamThread)
    {
        LogDump::LogA("Shutting down thread - hTerminateThreadEvent = %i\n", this->TerminateThreadEvent);

        const unsigned int currtime = Timer::GetMilliseconds();
        constexpr unsigned int timeouttime = 5000;
        constexpr unsigned int sleeptime = 5;
        DWORD exitcode = NULL;
        GetExitCodeThread(m_StreamThread, &exitcode);

        while (exitcode == STILL_ACTIVE)
        {
            if (Timer::GetMilliseconds() - currtime > timeouttime)
                break;

            GetExitCodeThread(m_StreamThread, &exitcode);
            Sleep(sleeptime);
        }

        LogDump::LogA("Thread closed...\n");
    }

    if (StreamedWAVSound)
        StreamedWAVSound->field_34 = -1;

    RELEASE_SAFE(DirectSoundBuffer);
    RELEASE_SAFE(DirectSound3DBuffer);

    if (DieselPowerSoundBuffer)
    {
        DieselPowerSoundBuffer->stub24();
        DieselPowerSoundBuffer->stub27();
        DieselPowerSoundBuffer->stub28();

        DieselPowerSoundBuffer = nullptr;
        DieselPower3DStream = nullptr;
    }

    field_58 = 0xDEA110CA;

    if (IsCreated())
        g_StreamedSoundBuffers->RemoveSoundBufferFromList(this);

    if (m_StreamThread)
        CloseHandle(m_StreamThread);

    if (EventNotify)
        CloseHandle(EventNotify);

    if (m_EvHandle_2)
        CloseHandle(m_EvHandle_2);

    if (TerminateThreadEvent)
        CloseHandle(TerminateThreadEvent);

    if (field_9C)
        CloseHandle(field_9C);

    if (field_A0)
        CloseHandle(field_A0);

    if (field_B0)
        CloseHandle(field_B0);

    if (field_61 && StreamedWAVSound)
        delete StreamedWAVSound;
    else
        if (StreamedWAVSound)
            StreamedWAVSound->RemoveSoundBuffer();

    StreamedWAVSound = nullptr;

    SoundBufferStatus& sbs = StreamedSoundBuffers::FindSoundBufferInBuffersList(field_8C);
    if (sbs.m_StreamBufferPtr)
        sbs.m_InUse;

    ThreadId = NULL;
}

IStreamBuffer::~IStreamBuffer()
{
    MESSAGE_CLASS_DESTROYED(IStreamBuffer);
}

#pragma message(TODO_IMPLEMENTATION)
StreamBuffer::~StreamBuffer()
{
    MESSAGE_CLASS_DESTROYED(StreamBuffer);
}

StreamBuffer::StreamBuffer()
{
    MESSAGE_CLASS_CREATED(StreamBuffer);

    SampledData = nullptr;
    AuxMonoStream_1 = nullptr;
}