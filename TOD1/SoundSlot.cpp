#include "SoundSlot.h"
#include "Scene.h"
#include "StreamedSoundBuffers.h"
#include "SoundFile.h"

int SoundSlot::_A3DD7C;

#pragma message(TODO_IMPLEMENTATION)
const char* SoundSlot::GetStreamingSound()
{
    delete m_StreamingSoundName;

    if (!m_StreamingSoundResource)
        return nullptr;

    char buf[MAX_PATH] = {};
    strcpy(buf, m_StreamingSoundResource->AddResToOpenListAndReturnName());
    strcpy(strstr(buf, ".stream"), ".wav");

    m_StreamingSoundName = new char[strlen(buf) + 1];
    strcpy(m_StreamingSoundName, buf);

    return m_StreamingSoundName;
}

#pragma message(TODO_IMPLEMENTATION)
void SoundSlot::DeallocateStream()
{
    if (!m_StreamBuffer)
        return;

    //for (; m_SoundEmittersList.m_Elements;)
        //*(m_SoundEmittersList.m_Elements++).field_24 = NULL;
    //m_SoundEmittersList.m_CurrIndex = NULL;

    //delete m_StreamBuffer;
}

#pragma message(TODO_IMPLEMENTATION)
void SoundSlot::StopSound()
{
    /*
    if (!m_SoundResource ||
        !m_StreamBuffer ||
        !g_StreamedSoundBuffers ||
        !m_SoundResource->m_MonoStream)
        return;

    if (m_StreamBuffer)
        m_StreamBuffer->Stop(m_StreamBufferIndex);
    else
        if (m_SoundResource->m_MonoStream)
            m_SoundResource->m_MonoStream->Stop(m_StreamBufferIndex);

    if (Scene::SceneInstance->m_PlayMode == 1)
        DeallocateStream();
    */
}

void SoundSlot::GetSoundName(String& outName)
{
    if (m_SoundResource)
        outName = m_SoundResource->AddResToOpenListAndReturnName();
    else
        if (m_Sound.m_Length)
            outName = m_Sound;
        else
            outName = GetStreamingSound();
}

void SoundSlot::UnPause()
{
    if ((m_SoundResource || m_StreamBuffer) && g_StreamedSoundBuffers && m_PausedInstanceIndex >= 0)
    {
        if (m_SoundResource && m_SoundResource->m_StreamBuffer)
            m_SoundResource->m_StreamBuffer->SetPause_A(m_PausedInstanceIndex, false);
        else if (m_StreamBuffer)
            m_StreamBuffer->SetPause_A(m_PausedInstanceIndex, false);

        m_PausedInstanceIndex = -1;
    }
}

bool SoundSlot::IsPlaying() const
{
    if ((m_SoundResource || m_StreamBuffer) && g_StreamedSoundBuffers)
    {
        if (m_SoundResource && m_SoundResource->m_StreamBuffer)
            return m_SoundResource->m_StreamBuffer->IsPlaying(m_StreamBufferIndex);
        else if (m_StreamBuffer)
            return m_StreamBuffer->IsPlaying(m_StreamBufferIndex);
    }

    return false;
}

#pragma message(TODO_IMPLEMENTATION)
SoundSlot::~SoundSlot()
{
    MESSAGE_CLASS_DESTROYED(SoundSlot);

    StopSound();
    DeallocateStream();	//	NOTE: original code has this function inlined, so no need to dup it here.

    delete m_StreamingSoundName;

    //	TODO: delete lists...
}

String* SoundSlot::GetResourceName(String* strResName)
{
    return (*strResName = GetStreamingSound(), strResName);
}

#pragma message(TODO_IMPLEMENTATION)
SoundSlot::SoundSlot() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(SoundSlot);

    m_SoundResource = nullptr;
    m_StreamingSoundResource = nullptr;
    field_64 = 1;
    m_Sound;
    field_7C = 1;

    //m_SoundEmittersList = List<class SoundEmitter>(0x1A300);

    m_Flags = m_Flags & 0xFF230111 | 0x1020111;
    m_Volume = 10000;
    m_Randompitch_Length = 0;
    m_MinRange = 1.0f;
    m_MaxRange = 100.0f;
    m_RollOff = 1.0f;
    m_Pitch = 10000;
    m_Randomvolume_Length = 0;
    m_StreamBuffer = nullptr;
    m_StreamingSoundName = nullptr;
    m_DopplerFactor = -1;
    m_StreamAllocationCallback = 0;
    m_LfeLevel = 0;
    m_PausedInstanceIndex = -1;

    //	TODO: initialize more lists here.
}

bool SoundSlot::AllocateGlobalStreamedSound(const char* const filename, const bool ismono, const int a3)
{
    if (StreamedSoundBuffer::GlobalStreamedSound)
        delete StreamedSoundBuffer::GlobalStreamedSound;

    _A3DD7C = a3;

    SoundFile::GlobalMonoStream.Open(filename);
    StreamedSoundBuffer::GlobalStreamedSound = new StreamedSoundBuffer(&SoundFile::GlobalMonoStream, 0, ismono, a3, 0, 1);

    if (StreamedSoundBuffer::GlobalStreamedSound->IsMonoStreamCreated())
        return true;

    delete StreamedSoundBuffer::GlobalStreamedSound;

    return false;
}

void SoundSlot::DeallocateGlobalStreamedSound()
{
    delete StreamedSoundBuffer::GlobalStreamedSound;
}