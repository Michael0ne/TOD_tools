#include "StreamedSoundBuffer.h"
#include "LogDump.h"
#include "StreamedSoundBuffers.h"
#include "StreamedWAV.h"
#include "BuiltinType.h"
#include "Performance.h"

StreamedSoundBuffer::~StreamedSoundBuffer()
{
	MESSAGE_CLASS_DESTROYED(StreamedSoundBuffer);
	LogDump::LogA("Deleting streamed sound buffer....\n");

	ShutdownThread();

	LogDump::LogA("Streamed sound buffer deleted\n");
}

#pragma message(TODO_IMPLEMENTATION)
StreamedSoundBuffer::StreamedSoundBuffer(class SoundFile* sndfile, int, int, int, char, char)
{
	MESSAGE_CLASS_CREATED(StreamedSoundBuffer);
}

#pragma message(TODO_IMPLEMENTATION)
StreamedSoundBuffer::StreamedSoundBuffer(bool a2, unsigned int totalchunks, int channels, unsigned int isstereo, float bytespersec, unsigned int frequency, bool ismonosound, unsigned int a9, char a10, const char* filepath)
{
	MESSAGE_CLASS_CREATED(StreamedSoundBuffer);
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::stub2()
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::stub3()
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::SetSampledData(void*)
{
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSoundBuffer::Is3DSound(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::_4433A0(bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSoundBuffer::IsLooped(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
int StreamedSoundBuffer::Play(int, char, int)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSoundBuffer::IsPlaying(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSoundBuffer::AreAnyInstancesPlaying()
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::_440850(int)
{
}

void StreamedSoundBuffer::Stop(int)
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
void StreamedSoundBuffer::SetPause(int, bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::SetPause_A(int, bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSoundBuffer::IsPaused(int)
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
bool StreamedSoundBuffer::IsMonoStreamCreated()
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
int StreamedSoundBuffer::_443480()
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::SetVolume(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSoundBuffer::GetVolume(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::SetFrequencyMultiplier(int, float mul)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSoundBuffer::GetFrequencyMultiplier(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
int StreamedSoundBuffer::_4435C0(int, int)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSoundBuffer::_443650(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::SetSoundPosition(int, const Vector4f*)
{
}

void StreamedSoundBuffer::GetPosition(Vector4f& outPos, const int)
{
	if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
	{
		if (m_DieselPowerSoundBuffer)
		{
			float x, y, z;
			m_DieselPowerSoundBuffer->GetPosition(x, y, z);
			outPos = { x, y, z, 0 };

			return;
		}
		else
		{
			outPos = BuiltinType::ZeroVector;
			return;
		}
	}

	if (m_DirectSound3DBuffer)
	{
		m_DirectSound3DBuffer->GetPosition((LPD3DVECTOR)&m_Position);
		outPos = m_Position;
	}
	else
	{
		outPos = BuiltinType::ZeroVector;
		return;
	}
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

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::SetPan(int, float)
{
}

float StreamedSoundBuffer::GetPan(int)
{
	if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
		if (m_DieselPowerSoundBuffer)
			return m_DieselPowerSoundBuffer->GetPan();
		else
			return NULL;

	if (!m_DirectSound3DBuffer)
		return NULL;

	m_DirectSound3DBuffer->GetMaxDistance(&m_Pan);
	return m_Pan;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::SetFrequency(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSoundBuffer::GetFrequency(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::_443C20(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSoundBuffer::_443C90(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::_443CD0(int, float)
{
}

#pragma message(TODO_IMPLEMENTATION)
float StreamedSoundBuffer::_443D30(int)
{
	return 0.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void StreamedSoundBuffer::_443D40(int, float, float, float)
{
}

int StreamedSoundBuffer::GetChannelsNumber() const
{
	return m_Channels;
}

void StreamedSoundBuffer::DumpInfo()
{
	Vector4f sndpos;
	GetPosition(sndpos, NULL);
	
	const bool islooped = m_Flags.m_FlagBits.Looped;
	const bool isplaying = IsPlaying(NULL);
	const bool is3d = Is3DSound(NULL);
	const float vol = GetVolume(NULL);

	LogDump::LogA("   %s, vol=%.1f, playing=%s, looped=%s, pos=(%.1f,%.1f,%.1f)\n",
		is3d ? "3d" : "2d",
		vol,
		isplaying,
		islooped,
		sndpos.x, sndpos.y, sndpos.z);
}

void StreamedSoundBuffer::StopZerothSound()
{
	Stop(0);
}

void StreamedSoundBuffer::FillSoundData(const bool overwrite)
{
	char* soundbufferptr = nullptr;
	unsigned int soundbuffersize = NULL;

	if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
	{
		soundbufferptr = m_DieselPowerSoundBuffer->GetSoundBufferPtr();
		soundbuffersize = m_DieselPowerSoundBuffer->stub9();
	}
	else
	{
		unsigned int i = NULL;
		HRESULT hr = NULL;

		for (; i < 100; ++i)
		{
			hr = m_DirectSoundBuffer->Lock(NULL, m_SoundBufferBlockSize, (LPVOID*)&soundbufferptr, (LPDWORD)&soundbuffersize, nullptr, nullptr, DSBLOCK_ENTIREBUFFER);

			if (SUCCEEDED(hr))
				break;

			//	NOTE: this method is executed from another thread, so SoundSystem MAY change at any moment.
			if (g_StreamedSoundBuffers->m_SoundSystem == SOUND_SYSTEM_DIESELPOWER)
			{
				soundbufferptr = m_DieselPowerSoundBuffer->GetSoundBufferPtr();
				soundbuffersize = m_DieselPowerSoundBuffer->stub9();
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
		if (m_StreamedWAV->m_SoundBufferPtr)
		{
			memcpy(soundbufferptr, m_StreamedWAV->m_SoundBufferPtr, 4 * (m_SoundBufferBlockSize >> 2));
			memcpy(&soundbufferptr[4 * (m_SoundBufferBlockSize >> 2)], &m_StreamedWAV->m_SoundBufferPtr[4 * (m_SoundBufferBlockSize >> 2)], m_SoundBufferBlockSize & 3);
		}
		else
			memset(soundbufferptr, NULL, 4 * (m_SoundBufferBlockSize >> 2) + (m_SoundBufferBlockSize & 3));
	}
	else
	{
		if (m_StreamedWAV->m_SoundBufferPtr)
			memcpy(soundbufferptr + m_SoundBufferBlockSize, m_StreamedWAV->m_SoundBufferPtr, m_SoundBufferBlockSize);
		else
			memset(soundbufferptr + m_SoundBufferBlockSize, NULL, 4 * (m_SoundBufferBlockSize >> 2) + (m_SoundBufferBlockSize & 3));
	}

	if (g_StreamedSoundBuffers->m_SoundSystem != SOUND_SYSTEM_DIESELPOWER)
	{
		unsigned int i = NULL;
		HRESULT hr = NULL;

		for (; i < 100; ++i)
		{
			hr = m_DirectSoundBuffer->Unlock(soundbufferptr, soundbuffersize, nullptr, NULL);

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

	if (m_TerminateThreadEvent)
		SetEvent(m_TerminateThreadEvent);

	if (m_StreamThread)
	{
		LogDump::LogA("Shutting down thread - hTerminateThreadEvent = %i\n", this->m_TerminateThreadEvent);
		
		const unsigned int currtime = Performance::GetMilliseconds();
		constexpr unsigned int timeouttime = 5000;
		constexpr unsigned int sleeptime = 5;
		DWORD exitcode = NULL;
		GetExitCodeThread(m_StreamThread, &exitcode);

		while (exitcode == STILL_ACTIVE)
		{
			if (Performance::GetMilliseconds() - currtime > timeouttime)
				break;

			GetExitCodeThread(m_StreamThread, &exitcode);
			Sleep(sleeptime);
		}

		LogDump::LogA("Thread closed...\n");
	}

	if (m_StreamedWAV)
		m_StreamedWAV->field_34 = -1;

	RELEASE_SAFE(m_DirectSoundBuffer);
	RELEASE_SAFE(m_DirectSound3DBuffer);

	if (m_DieselPowerSoundBuffer)
	{
		m_DieselPowerSoundBuffer->stub24();
		m_DieselPowerSoundBuffer->stub27();
		m_DieselPowerSoundBuffer->stub28();

		m_DieselPowerSoundBuffer = nullptr;
		m_DieselPowerStream = nullptr;
	}

	field_58 = 0xDEA110CA;

	if (IsMonoStreamCreated())
		g_StreamedSoundBuffers->RemoveSoundBufferFromList(this);

	if (m_StreamThread)
		CloseHandle(m_StreamThread);

	if (m_EventNotify)
		CloseHandle(m_EventNotify);

	if (m_EvHandle_2)
		CloseHandle(m_EvHandle_2);

	if (m_TerminateThreadEvent)
		CloseHandle(m_TerminateThreadEvent);

	if (field_9C)
		CloseHandle(field_9C);

	if (field_A0)
		CloseHandle(field_A0);

	if (field_B0)
		CloseHandle(field_B0);

	if (field_61 && m_StreamedWAV)
		delete m_StreamedWAV;
	else
		if (m_StreamedWAV)
			m_StreamedWAV->RemoveSoundBuffer();
	
	m_StreamedWAV = nullptr;

	SoundBufferStatus& sbs = StreamedSoundBuffers::FindSoundBufferInBuffersList(field_8C);
	if (sbs.m_StreamBufferPtr)
		sbs.m_InUse;

	m_ThreadId = NULL;
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