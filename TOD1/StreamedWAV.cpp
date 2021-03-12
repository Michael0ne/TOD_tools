#include "StreamedWAV.h"
#include "StreamedSoundBuffers.h"
#include "LogDump.h"

SoundBufferInfo StreamedWAV::EmptySoundBufferInfo;
std::vector<SoundBufferInfo> StreamedWAV::SoundBuffersList;

SoundBufferInfo* StreamedWAV::CheckIfSoundBufferIsUsed(char* ptr)
{
	if (SoundBuffersList.size() <= NULL)
		return &EmptySoundBufferInfo;

	unsigned int ind = 0;
	while (SoundBuffersList[ind].m_BufferPtr != ptr ||
		!SoundBuffersList[ind].m_Used)
		if (++ind >= SoundBuffersList.size())
			return &EmptySoundBufferInfo;
	return &SoundBuffersList[ind];
}

StreamedWAV::StreamedWAV(unsigned int sampleRate)
{
	MESSAGE_CLASS_CREATED(StreamedWAV);

	m_FileName;
	m_Channels = -1;
	m_BlockAlign = -1;
	m_SamplesPerSec = -1;
	m_AverageBytesPerSec = -1;
	m_ChunkSize = -1;
	m_Samples = -1;
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

	SoundBuffersList.push_back({ true, m_SoundBufferPtr });
}

StreamedWAV::StreamedWAV(unsigned int sampleRate, const char* soundFile)
{
	m_Channels = -1;
	m_BytesPerSample = 0.f;
	m_BlockAlign = -1;
	m_SamplesPerSec = -1;
	m_ChunkSize = -1;
	m_Samples = -1;
	field_34 = -1;
	m_FileHandle = NULL;
	m_WavFile = nullptr;
	m_OggInfo = nullptr;
	m_Flags = 0 | AUDIO_AUXMONOSTREAM_FLAG_FILE_SET;
	m_SampleRate = sampleRate;

	SoundBuffersList.reserve(12);

	String::ToLowerCase((char*)soundFile);
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

StreamedWAV::~StreamedWAV()
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
		WaitForSingleObject(StreamedSoundBuffers::SemaphoreObject, INFINITE);
		delete m_WavFile;
		ReleaseSemaphore(StreamedSoundBuffers::SemaphoreObject, 1, 0);
	}

	MESSAGE_CLASS_DESTROYED(StreamedWAV);
}

void StreamedWAV::DestroySoundBuffers(bool unk)
{
	if (m_SoundFormat == FORMAT_OGG)
	{
		if (m_OggInfo && ov_pcm_seek(m_OggInfo, NULL))
			LogDump::LogA("DeInit: %s\n", m_FileName.m_szString);
	}
	else
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
	}
	else
		if (m_FileHandle && m_WavFile && unk && m_WavFile->m_FileHandle)
		{
			m_WavFile->m_FileHandle->_436FF0(m_FileHandle);
			m_WavFile->m_FileHandle->ReleaseFileHandle();
			m_FileHandle = NULL;
		}

	m_ChunkId = NULL;
	m_Flags = m_Flags & 0xFFFFFFF9;
}

void* StreamedWAV::operator new(size_t size)
{
	if (!Allocators::Released)
		return Allocators::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	else
		return nullptr;
}

void* StreamedWAV::operator new[](size_t size)
{
	if (!Allocators::Released)
		return Allocators::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	else
		return nullptr;
}

void StreamedWAV::operator delete(void* ptr)
{
	if (ptr)
		Allocators::AllocatorsList[DEFAULT]->Free(ptr);
	ptr = nullptr;
}