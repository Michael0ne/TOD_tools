#include "StreamedSoundBuffers.h"
#include "DieselPowerSound.h"
#include "Platform.h"
#include "Timer.h"
#include "LogDump.h"
#include "Scene.h"
#include "StreamedWAV.h"
#include "SoundFile.h"

StreamedSoundBuffers* g_StreamedSoundBuffers = nullptr;

std::vector<SoundBufferStatus> StreamedSoundBuffers::vSoundBuffers;
float StreamedSoundBuffers::DefaultFxVolume;
float StreamedSoundBuffers::DefaultAmbienceVolume;
float StreamedSoundBuffers::DefaultMusicVolume;
float StreamedSoundBuffers::DefaultSpeaksVolume;
SoundSystemType StreamedSoundBuffers::SoundRendererId;
HANDLE StreamedSoundBuffers::SemaphoreObject;
StreamedSoundBuffer* StreamedSoundBuffers::StaticStreamedSound = nullptr;
SoundBufferStatus SoundBufferStatus::EmptyInstance;

void StreamedSoundBuffers::SetDefaultFxVolume(float vol)
{
    DefaultFxVolume = vol;
}

void StreamedSoundBuffers::SetDefaultAmbienceVolume(float vol)
{
    DefaultAmbienceVolume = vol;
}

void StreamedSoundBuffers::SetDefaultMusicVolume(float vol)
{
    DefaultMusicVolume = vol;
}

void StreamedSoundBuffers::SetDefaultSpeaksVolume(float vol)
{
    DefaultSpeaksVolume = vol;
}

float StreamedSoundBuffers::GetDefaultFxVolume()
{
    return DefaultFxVolume;
}

float StreamedSoundBuffers::GetDefaultAmbienceVolume()
{
    return DefaultAmbienceVolume;
}

float StreamedSoundBuffers::GetDefaultMusicVolume()
{
    return DefaultMusicVolume;
}

float StreamedSoundBuffers::GetDefaultSpeaksVolume()
{
    return DefaultSpeaksVolume;
}

void StreamedSoundBuffers::RememberSoundRenderer(SoundSystemType soundRendererId)
{
    SoundRendererId = soundRendererId;

    if (RegCreateKeyEx(HKEY_CURRENT_USER, Platform::RegistryKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, (PHKEY)&soundRendererId, NULL) == ERROR_SUCCESS)
    {
        RegSetValueEx((HKEY)soundRendererId, "SoundRenderer", NULL, REG_DWORD, (const BYTE*)&SoundRendererId, sizeof(SoundRendererId));
        RegCloseKey((HKEY)soundRendererId);
    }
}

SoundSystemType StreamedSoundBuffers::GetSoundRenderer()
{
    if (SoundRendererId)
        return SoundRendererId;

    SoundRendererId = SOUND_SYSTEM_AUTOSELECT;

    HKEY phkResult;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, Platform::RegistryKey, NULL, KEY_QUERY_VALUE, &phkResult) == ERROR_SUCCESS)
    {
        DWORD lpType;
        BYTE Data[4];
        DWORD cbData = NULL;
        if (RegQueryValueEx(phkResult, "SoundRenderer", NULL, &lpType, Data, &cbData) == ERROR_SUCCESS &&
            lpType == REG_DWORD)
            SoundRendererId = *(SoundSystemType*)Data;
        RegCloseKey(phkResult);
    }

    return SoundRendererId;
}

SoundBufferStatus& StreamedSoundBuffers::FindSoundBufferInBuffersList(const StreamedSoundBuffer* ssb)
{
    if (!vSoundBuffers.size())
        return SoundBufferStatus::EmptyInstance;

    for (unsigned int i = 0; i < vSoundBuffers.size(); ++i)
        if (!vSoundBuffers[i].m_InUse || vSoundBuffers[i].m_StreamBufferPtr == ssb)
            return vSoundBuffers[i];

    return SoundBufferStatus::EmptyInstance;
}

#pragma message(TODO_IMPLEMENTATION)
StreamedSoundBuffers::StreamedSoundBuffers(char channels, int sampleRate)
{
    MESSAGE_CLASS_CREATED(StreamedSoundBuffers);

    LPDIRECTSOUND directSound;

    m_DieselPowerSoundBuffers;
    m_StreamDataBufferList;
    m_SoundList_1;
    m_SoundList_2;
    m_SoundList_3;
    m_SoundList_4;
    m_SoundList_5;
    m_SoundList_6;
    m_SoundList_7;
    m_SoundList_8;
    m_Str_1;
    m_SoundList_1_Ptr = &m_SoundList_1;
    m_SoundList_4_Ptr = &m_SoundList_4;
    m_SoundList_5_Ptr = &m_SoundList_5;
    m_SoundList_6_Ptr = &m_SoundList_6;
    field_8 = 0.001f;
    field_C = 1;
    m_MultichannelStreamedSoundsNum = NULL;
    m_PlayingMonoStreamsNumber = NULL;
    m_MonoStreamsNumber = 0;
    m_SoundList_2_Ptr = &m_SoundList_2;
    m_SoundList_3_Ptr = &m_SoundList_3;
    m_SoundList_7_Ptr = &m_SoundList_7;
    m_SoundList_8_Ptr = &m_SoundList_8;
    m_Sound = true;
    field_14 = NULL;
    SemaphoreObject = CreateSemaphore(NULL, 1, 1, NULL);
    m_MaxConcurrentSounds = STREAMEDSOUNDBUFFERS_MAX_CONCURRENT_SOUNDS;
    m_CurrentlyPlaying = NULL;
    m_DirectSoundBuffer = nullptr;
    DieselPower2DStream = nullptr;
    DieselPower3DStream = nullptr;
    m_DieselPower = nullptr;

    if (FAILED(DirectSoundCreate(NULL, &directSound, NULL)))
        IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_SOUNDCARD, false);

    if (FAILED(directSound->GetSpeakerConfig((LPDWORD)&m_SpeakerConfig)))
        IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_SOUNDCARD, false);

    RELEASE_SAFE(directSound);

    switch (m_SpeakerConfig)
    {
    case SPEAKER_CONFIG_DIRECT_OUT:
        LogDump::LogA("direct out speaker configuration\n");
        break;
    case SPEAKER_CONFIG_HEADPHONES:
        LogDump::LogA("headphone speaker configuration\n");
        break;
    case SPEAKER_CONFIG_MONO:
        LogDump::LogA("mono speaker configuration\n");
        break;
    case SPEAKER_CONFIG_QUAD:
        LogDump::LogA("quad speaker configuration\n");
        break;
    case SPEAKER_CONFIG_SURROUND:
        LogDump::LogA("surround speaker configuration\n");
        break;
    case SPEAKER_CONFIG_51:
        LogDump::LogA("5.1 speaker configuration\n");
        break;
    case SPEAKER_CONFIG_71:
        LogDump::LogA("7.1 speaker configuration\n");
        break;
    default:
        LogDump::LogA("stereo speaker configuration\n");
        break;
    }

    m_SoundSystem = GetSoundRenderer();
    switch (m_SoundSystem)
    {
    case SOUND_SYSTEM_AUTOSELECT:
        if (m_SpeakerConfig >= SPEAKER_CONFIG_71)
        {
            m_SoundSystem = SOUND_SYSTEM_DSOUND;
            LogDump::LogA("auto selecting DirectSound audio renderer\n");
        }
        else {
            m_SoundSystem = SOUND_SYSTEM_DIESELPOWER;
            LogDump::LogA("auto selecting Diesel Power audio renderer\n");
        }
        break;
    case SOUND_SYSTEM_DSOUND:
        LogDump::LogA("using DirectSound audio renderer\n");
        break;
    case SOUND_SYSTEM_DIESELPOWER:
        LogDump::LogA("using Diesel Power audio renderer\n");
        break;
    }

    //if (m_nSoundSystem == SOUND_SYSTEM_DIESELPOWER)
     //InitDieselPower();
    //else
     //InitDirectSound(channels, sampleRate);

    //PreallocateStreamBuffersPool();
    //CreateStaticStreamBuffer();
    //SetListener3DPos(Vector4f());

    m_GlobalPauseCalled = false;
    m_GlobalPause = false;
}

StreamedSoundBuffers::~StreamedSoundBuffers()
{
    MESSAGE_CLASS_DESTROYED(StreamedSoundBuffers);

    CloseHandle(SemaphoreObject);

    if (m_SoundSystem == SOUND_SYSTEM_DSOUND)
    {
        RELEASE_SAFE(m_DirectSoundBuffer_1);
        RELEASE_SAFE(m_DirectSound);
    }
    else
    {
        if (m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        {
            for (unsigned int ind = 0; ind < m_DieselPowerSoundBuffers.size(); ++ind)
                if (m_DieselPowerSoundBuffers[ind])
                    delete m_DieselPowerSoundBuffers[ind];
            for (unsigned int ind = 0; ind < m_StreamDataBufferList.size(); ++ind)
                if (m_StreamDataBufferList[ind])
                    delete m_StreamDataBufferList[ind];

            m_DieselPower->stub33();
            Sleep(2000);

            delete m_DieselPower;
            m_DieselPower = nullptr;
        }
    }

    g_StreamedSoundBuffers = nullptr;
}

inline void StreamedSoundBuffers::SetDefaultVolumeLevels()
{
    if (m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        DefaultFxVolume = 1.0f;
        DefaultAmbienceVolume = 1.0f;
        DefaultMusicVolume = 0.75f;
        DefaultSpeaksVolume = 0.75f;
    }
    else {
        DefaultFxVolume = 0.89999998f;
        DefaultAmbienceVolume = 0.89999998f;
        DefaultMusicVolume = 1.0f;
        DefaultSpeaksVolume = 1.0f;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffers::InitDieselPower()
{
    LogDump::LogA("Initializing Sound System.....\n");

    SetDefaultVolumeLevels();

    //m_DieselPower = DieselPower::CallFactory(1, 2, 1, 3.0f, g_Platform->m_WindowHandle, 1, 0, 0);

    if (!m_DieselPower)
        return;

    //m_DieselPower->stub25(1);
    //m_DieselPower->stub26(1);

    //field_50 = m_DieselPower->stub24();

    switch (m_SpeakerConfig - 1)
    {
    case SPEAKER_CONFIG_DIRECT_OUT:
        (*(void(__stdcall*)(int))(field_50 + 8))(0); // FIXME: what is stub24 exactly?
        break;
    case SPEAKER_CONFIG_MONO:
    case SPEAKER_CONFIG_SURROUND:
    case SPEAKER_CONFIG_51:
    case SPEAKER_CONFIG_71:
        (*(void(__stdcall*)(int))(field_50 + 8))(2);
        break;
    default:
        (*(void(__stdcall*)(int))(field_50 + 8))(1);
        break;
    }

    (*(void(__stdcall*)())(field_50 + 80))();
    //field_54 = m_DieselPower->stub23();
    //m_DirectSound = m_DieselPower->GetDirectSound();
    m_Muted = false;

    //LogDump::LogA("Sound Rendering System is '%s'\n", m_DieselPower->GetSystemName());

    g_StreamedSoundBuffers = this;
}

void StreamedSoundBuffers::GetMaxDistance(Vector4f& outVec) const
{
    if (m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        (*(void(__stdcall*)(Vector4f&))(m_DieselPower_1 + 16))(outVec);
    else
        m_DirectSound3DBuffer->GetMaxDistance((D3DVALUE*)&outVec);
}

#pragma message(TODO_IMPLEMENTATION)
HRESULT StreamedSoundBuffers::CreateSoundBuffer(DSBUFFERDESC* bufferDesc)
{
    m_DirectSoundBuffer = nullptr;
    DieselPower2DStream = NULL;
    DieselPower3DStream = NULL;

    if (m_SoundSystem != SOUND_SYSTEM_DIESELPOWER)
        return m_DirectSound->CreateSoundBuffer(bufferDesc, &m_DirectSoundBuffer, NULL);

    //if (bufferDesc->dwFlags & DSBCAPS_CTRL3D)
     //AssetDataPtr = AssetInstanceDataPtr_1 = m_DieselPower->stub21(bufferDesc->dwBufferBytes, bufferDesc->lpwfxFormat);
    //else
     //AssetDataPtr = m_DieselPower->stub16(bufferDesc->dwBufferBytes, 11, &bufferDesc->lpwfxFormat);

    return S_OK;
}

void StreamedSoundBuffers::SetPrimarySoundFormat(int channels, int sampleRate, int bits)
{
    WAVEFORMATEX waveFormat;

    waveFormat.nSamplesPerSec = 0;
    waveFormat.cbSize = NULL;
    waveFormat.wBitsPerSample = 8 * bits;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = channels;
    waveFormat.nAvgBytesPerSec = sampleRate * (channels * (bits & 0x1FFF));
    waveFormat.nBlockAlign = channels * (bits & 0x1FFF);
    waveFormat.nSamplesPerSec = sampleRate;

    m_DirectSoundBuffer_1->SetFormat(&waveFormat);

    LogDump::LogA("Primary sound format set to %dHz, %dbit, %s \n", waveFormat.nSamplesPerSec, waveFormat.wBitsPerSample, channels == 2 ? "Stereo" : "Mono");

    m_DirectSoundChannels = channels;
    m_DirectSoundSampleRate = sampleRate;
    m_DirectSoundBits = bits;
}

float StreamedSoundBuffers::GetDefaultVolumeForType(int type)
{
    switch (type)
    {
    case 0:
        return DefaultFxVolume;
        break;
    case 1:
        return DefaultAmbienceVolume;
        break;
    case 2:
        return DefaultMusicVolume;
        break;
    case 3:
        return DefaultSpeaksVolume;
        break;
    default:
        return 1.0f;
        break;
    }
}

void StreamedSoundBuffers::SetGlobalPause(bool pause)
{
    LogDump::LogA("global pause %s\n", pause ? "on" : "off");

    m_GlobalPause = pause;
    m_GlobalPauseCalled = true;
}

void StreamedSoundBuffers::_43D200(LPDIRECTSOUNDBUFFER, const float, const int, const int, const int)
{
    return;
}

void StreamedSoundBuffers::InitDirectSound(char channels, int sampleRate)
{
    LogDump::LogA("Initializing Sound System.....\n");

    SetDefaultVolumeLevels();

    if (FAILED(DirectSoundCreate(NULL, &m_DirectSound, NULL)))
        IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_SOUNDCARD, false);

    LogDump::LogA("DirectSound created successfully\n");

    DSCAPS dsCaps;
    ZeroMemory(&dsCaps, sizeof(DSCAPS));
    dsCaps.dwSize = sizeof(DSCAPS);

    if (FAILED(m_DirectSound->GetCaps(&dsCaps)))
        IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_SOUNDCARD, false);

    LogDump::LogA("%d HW mixing buffers (%d free)\n", dsCaps.dwMaxHwMixingStaticBuffers, dsCaps.dwFreeHwMixingStaticBuffers);

    int soundPriorityLevel = 1;
    if (FAILED(m_DirectSound->SetCooperativeLevel(g_Platform->m_WindowHandle, SoundPriorityLevels[2])))
    {
        for (; soundPriorityLevel < 5; soundPriorityLevel++)
            if (SUCCEEDED(m_DirectSound->SetCooperativeLevel(g_Platform->m_WindowHandle, SoundPriorityLevels[soundPriorityLevel])))
                break;

        if (soundPriorityLevel == 5)
        {
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_SOUNDCARD, false);
            RELEASE_SAFE(m_DirectSound);

            return;
        }
    }

    LogDump::LogA("Sound priority level set to %d\n", soundPriorityLevel);

    DSBUFFERDESC dsBuffer;
    ZeroMemory(&dsBuffer, sizeof(DSBUFFERDESC));

    dsBuffer.dwSize = sizeof(DSBUFFERDESC);
    dsBuffer.dwReserved = NULL;
    dsBuffer.dwBufferBytes = 0;
    dsBuffer.lpwfxFormat = NULL;
    dsBuffer.guid3DAlgorithm = GUID_NULL;
    dsBuffer.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

    if (SUCCEEDED(m_DirectSound->CreateSoundBuffer(&dsBuffer, &m_DirectSoundBuffer_1, NULL)))
        SetPrimarySoundFormat((channels != NULL) + 1, sampleRate, 2);

    g_StreamedSoundBuffers = this;

    m_Muted = false;

    if (FAILED(m_DirectSoundBuffer_1->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID*)&m_DirectSound3DBuffer)))
        IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_SOUNDCARD, false);

    m_DirectSound3DBuffer->SetConeOutsideVolume(0, 0);
    m_DirectSound3DBuffer->SetAllParameters(nullptr, DS3D_IMMEDIATE);

    LogDump::LogA("Sound Rendering System is 'DirectSound'\n");
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffers::SetListener3DPos(const Vector4f& pos)
{
    float dist = Scene::TimePassed >= 0.0099999998f ? Scene::TimePassed : 0.0099999998f;
    Vector4f maxDistanceVec;
    GetMaxDistance(maxDistanceVec);

    maxDistanceVec.x -= pos.x;
    maxDistanceVec.y -= pos.y;
    maxDistanceVec.z -= pos.z;
    maxDistanceVec.a -= pos.a;

    if (m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
    {
        (*(void(__stdcall*)(float, float, float))m_DieselPower_1)(pos.x, pos.y, pos.z);
        (*(void(__stdcall*)(float, float, float))(m_DieselPower_1 + 8))(maxDistanceVec.x * (1.0f / dist), maxDistanceVec.y * (1.0f / dist), maxDistanceVec.z * (1.0f / dist));
        (*(void(__stdcall*)())(m_DieselPower_1 + 40))();
    }
    else
    {
        m_DirectSound3DBuffer->SetConeOrientation(pos.x, pos.y, pos.z, 1);
        HRESULT sposres = m_DirectSound3DBuffer->SetPosition(maxDistanceVec.x * (1.0f / dist), maxDistanceVec.y * (1.0f / dist), maxDistanceVec.z * (1.0f / dist), 1);
        if (FAILED(sposres))
            LogDump::LogA("Failed to set 3D listener position (%d)\n", sposres);
    }
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffers::SetListener3DOrientation(const Orientation& orient)
{
    // TODO: hell no, too many calculations here. Will do later...
}

void StreamedSoundBuffers::GetListener3DOrientation(Orientation& outOrient)
{
    if (m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
        (*(void(__stdcall*)(Orientation&))(m_DieselPower_1 + 20))(outOrient);
    else
        if (FAILED(m_DirectSound3DBuffer->GetConeOrientation((D3DVECTOR*)&outOrient)))
            LogDump::LogA("Failed to get 3D listener orientation\n");
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffers::UpdateSoundBuffers()
{
    // TODO: SoundBuffer class is not fully researched yet, so no implementation for now...
}

int StreamedSoundBuffers::GetPlayingSoundsNumber()
{
    int res = 0;
    for (unsigned int ind = 0; ind < m_StreamDataBufferList.size(); ++ind)
        if ((*(bool(__stdcall*)(int))(m_StreamDataBufferList[ind] + 32))(0) &&
            !(*(bool(__stdcall*)(int))(m_StreamDataBufferList[ind] + 56))(0))
            ++res;
    return res;
}

void StreamedSoundBuffers::WaitForSoftPause()
{
    int startTime = Timer::GetMilliseconds();

    while (m_GlobalPauseCalled)
    {
        UpdateSoundBuffers();
        Sleep(10);
    }

    int interval = Timer::GetMilliseconds() - startTime;

    if (interval > 10)
        LogDump::LogA("MeasureWaitForSoftPause slept %dms\n", interval);
}

void StreamedSoundBuffers::_43E850()
{
    for (unsigned int ind = 0; ind < m_DieselPowerSoundBuffers.size(); ++ind)
        (*(void(__stdcall*)(int))(m_DieselPowerSoundBuffers[ind] + 40))(0);
}

void StreamedSoundBuffers::StopAllSounds()
{
    for (unsigned int i = 0; i < m_StreamDataBufferList.size(); ++i)
        m_StreamDataBufferList[i]->StopFirstChannelSound();
}

void StreamedSoundBuffers::PreallocateStreamBuffersPool()
{
    LogDump::LogA(" ++ Preparing/pre-allocating %d Streambuffers to pool.\n", STREAMEDSOUNDBUFFERS_PREALLOCATED_COUNT);
    std::vector<StreamedSoundBuffer*> tempStreamedSoundVector(STREAMEDSOUNDBUFFERS_PREALLOCATED_COUNT);

    for (unsigned int i = 0; i < STREAMEDSOUNDBUFFERS_PREALLOCATED_COUNT; ++i)
    {
        SoundFile sndfile;
        tempStreamedSoundVector.push_back(new StreamedSoundBuffer(&sndfile, 0, 0, 0, 1, 0));
    }

    tempStreamedSoundVector.clear();

    LogDump::LogA(" ++ vSoundBuffers.size() = %d\n", vSoundBuffers.size());
    LogDump::LogA(" ++ Clearing all pre-allocated Stream buffers to bInUse = false\n");

    for (std::vector<SoundBufferStatus>::iterator it = vSoundBuffers.begin(); it != vSoundBuffers.end(); ++it)
        it->m_InUse = false;

    LogDump::LogA(" ++ Done.\n");
}

void StreamedSoundBuffers::CreateStaticStreamedSoundBuffer()
{
    StaticStreamedSound = new StreamedSoundBuffer(false, 1024, 1, false, 2.f, 44100, false, NULL, true, nullptr);
}

void StreamedSoundBuffers::RemoveSoundBufferFromList(StreamedSoundBuffer* soundbuffer)
{
    bool sndbuffound = false;
    std::vector<StreamedSoundBuffer*> soundbufferstemplist(m_StreamDataBufferList.size());

    if (m_StreamDataBufferList.size())
    {
        for (unsigned int i = 0; i < m_StreamDataBufferList.size(); ++i)
            if (m_StreamDataBufferList[i] == soundbuffer)
                sndbuffound = true;
            else
                soundbufferstemplist.push_back(m_StreamDataBufferList[i]);
    }

    m_StreamDataBufferList.clear();
    m_StreamDataBufferList = soundbufferstemplist;
    soundbufferstemplist.clear();

    if (sndbuffound)
    {
        if (soundbuffer->Channels <= 1)
        {
            if (soundbuffer->Channels == 1)
                if (soundbuffer->Flags.Started)
                    m_PlayingMonoStreamsNumber--;
                else
                    m_MonoStreamsNumber--;
        }
        else
            m_MultichannelStreamedSoundsNum--;
    }
}