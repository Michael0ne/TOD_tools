#include "StreamedSound.h"
#include "LogDump.h"


#pragma message(TODO_IMPLEMENTATION)
StreamedSound::~StreamedSound()
{
	MESSAGE_CLASS_DESTROYED(StreamedSound);
}

#pragma message(TODO_IMPLEMENTATION)
StreamedSound::StreamedSound(class SoundFile* sndfile, int, int, int, char, char)
{
	MESSAGE_CLASS_CREATED(StreamedSound);
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::stub2()
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::stub3()
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetSampledData(void*)
{
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSound::_443350(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::_4433A0(bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSound::IsLooped(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
int StreamedSound::Play(int, char, int)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSound::IsPlaying(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSound::AreAnyInstancesPlaying()
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::_440850(int)
{
}

void StreamedSound::Stop(int)
{
	field_54 = field_54 & 0xFF00FFFF | (((unsigned short)field_54 + 1) << 16) & 0xFF0000;
	m_Flags.m_FlagBits.StopRequest = m_Flags.m_FlagBits._4 = true;

	if (m_StreamedWAV)
	{
		m_StoppingFinished = !m_DirectSoundBuffer && !m_DieselPowerSoundBuffer;

		if (field_A0)
			ResetEvent(field_A0);
		if (field_B0)
			SetEvent(field_B0);
		if (m_EventNotify)
			ResetEvent(m_EventNotify);
		if (m_EvHandle_2)
			ResetEvent(m_EvHandle_2);

		m_Flags.m_FlagBits.LastChunkPlaying = false;
		field_5C = NULL;
	}
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetPause(int, bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetPause_A(int, bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSound::IsPaused(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSound::IsMonoStreamCreated()
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
int StreamedSound::_443480()
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetVolume(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSound::GetVolume(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetFrequencyMultiplier(int, float mul)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSound::GetFrequencyMultiplier(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
int StreamedSound::_4435C0(int, int)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSound::_443650(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetSoundPosition(int, const Vector4f*)
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::_4438B0(Vector4f*, int)
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::_443990(int, int*)
{
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* StreamedSound::_4439E0(Vector4f*, int)
{
	return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetPan(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSound::GetPan(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::SetFrequency(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSound::GetFrequency(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::_443C20(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSound::_443C90(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::_443CD0(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSound::_443D30(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::_443D40(int, float, float, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
int StreamedSound::_442810()
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSound::DumpInfo()
{
}

void StreamedSound::_443E00()
{
	Stop(0);
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

	m_SampledData = nullptr;
	m_AuxMonoStream_1 = nullptr;
}