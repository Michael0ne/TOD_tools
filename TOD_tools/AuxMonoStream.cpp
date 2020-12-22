#include "AuxMonoStream.h"
#include "Globals.h"
#include "StreamedSoundBuffers.h"
#include "LogDump.h"

namespace Audio
{
	Audio::SoundBufferInfo AuxMonoStream::EmptySoundBufferInfo = { false, nullptr };

	Audio::SoundBufferInfo* AuxMonoStream::CheckIfSoundBufferIsUsed(char* ptr)
	{
		if (SoundBuffersList.m_CurrIndex <= NULL)
			return &EmptySoundBufferInfo;

		unsigned int ind = 0;
		while (SoundBuffersList.m_Elements[ind]->m_BufferPtr != ptr ||
			!SoundBuffersList.m_Elements[ind]->m_Used)
			if (++ind >= SoundBuffersList.m_CurrIndex)
				return &EmptySoundBufferInfo;
		return SoundBuffersList.m_Elements[ind];
	}

	AuxMonoStream::AuxMonoStream(unsigned int sampleRate)
	{
		MESSAGE_CLASS_CREATED(AuxMonoStream);

		m_FileName;
		m_Channels = -1;
		m_BlockAlign = -1;
		m_SamplesPerSec = -1;
		m_AverageBytesPerSec = -1;
		m_ChunkSize = -1;
		field_10 = -1;
		field_34 = -1;
		m_BytesPerSample = 0.f;
		m_ChunkId = NULL;
		m_SoundFormat = FORMAT_UNSET;
		m_Flags = 0 | 0xFFFFFFE8;
		m_FileHandle = NULL;
		m_WavFile = nullptr;
		m_OggInfo = nullptr;
		m_SampleRate = sampleRate;
		m_SoundBufferPtr = nullptr;

		if (sampleRate <= NULL)
			return;

		m_SoundBufferPtr = new char[sampleRate];

		SoundBufferInfo _sndBufInf = { true, m_SoundBufferPtr };
		SoundBuffersList.AddElement(&_sndBufInf);
	}

	AuxMonoStream::AuxMonoStream(unsigned int sampleRate, const char* soundFile)
	{
		m_Channels = -1;
		m_BytesPerSample = 0.f;
		m_BlockAlign = -1;
		m_SamplesPerSec = -1;
		m_ChunkSize = -1;
		field_10 = -1;
		field_34 = -1;
		m_FileHandle = NULL;
		m_WavFile = nullptr;
		m_OggInfo = nullptr;
		m_Flags = 0 | AUDIO_AUXMONOSTREAM_FLAG_FILE_SET;
		m_SampleRate = sampleRate;

		SoundBuffersList.SetCapacity(12);

		Utils::ToLowercase((char*)soundFile);
		m_FileName = soundFile;

		if (m_FileName.Equal("ogg"))
			m_SoundFormat = FORMAT_OGG;
		else if (m_FileName.Equal("wav"))
			m_SoundFormat = FORMAT_WAV;
		else if (m_FileName.Equal("xbwav"))
			m_SoundFormat = FORMAT_XBOX;
		else
			m_SoundFormat = FORMAT_UNSET;
	}

	AuxMonoStream::~AuxMonoStream()
	{
		DestroySoundBuffers(true);

		if (m_SoundBufferPtr)
		{
			SoundBufferInfo* _usedbuf = CheckIfSoundBufferIsUsed(m_SoundBufferPtr);
			if (_usedbuf)
				_usedbuf->m_Used = false;
			m_SoundBufferPtr = nullptr;
		}

		m_Flags = m_Flags & 0xFFFFFFFE;

		if (m_WavFile)
		{
			WaitForSingleObject(SemaphoreObject, INFINITE);
			delete m_WavFile;
			m_WavFile = nullptr;
			ReleaseSemaphore(SemaphoreObject, 1, 0);
		}

		MESSAGE_CLASS_DESTROYED(AuxMonoStream);
	}

	void AuxMonoStream::DestroySoundBuffers(bool unk)
	{
		if (m_SoundFormat == FORMAT_OGG)
		{
			if (m_OggInfo && ov_pcm_seek(m_OggInfo, NULL))
				LogDump::LogA("DeInit: %s\n", m_FileName.m_szString);
		}else
			if (m_WavFile && m_WavFile->IsFileOpen())
				m_WavFile->_WriteBufferAndSetToStart();

		if (m_SoundFormat == FORMAT_OGG && unk)
		{
			if (m_OggInfo)
			{
				ov_clear(m_OggInfo);
				m_OggInfo = nullptr;
				field_34 = -1;
				m_ChunkId = NULL;
				m_Flags = m_Flags & 0xFFFFFFF9;

				return;
			}
		}else
			if (m_FileHandle && m_WavFile && unk && m_WavFile->m_FileHandle)
			{
				m_WavFile->m_FileHandle->_436FF0(m_FileHandle);
				m_WavFile->m_FileHandle->ReleaseFileHandle();
				m_FileHandle = NULL;
			}

		m_ChunkId = NULL;
		m_Flags = m_Flags & 0xFFFFFFF9;
	}

	void* AuxMonoStream::operator new(size_t size)
	{
		if (!Allocators::Released)
			return Allocators::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	}

	void* AuxMonoStream::operator new[](size_t size)
	{
		if (!Allocators::Released)
			return Allocators::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	}

	void AuxMonoStream::operator delete(void* ptr)
	{
		if (ptr)
			Allocators::AllocatorsList[DEFAULT]->Free(ptr);
	}
}