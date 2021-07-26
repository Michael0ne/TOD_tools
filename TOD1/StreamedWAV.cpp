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

size_t StreamedWAV::OggReadCallback(void* buff, size_t size, size_t nmemb, void* datasource)
{
	return ((File*)datasource)->Read(buff, size * nmemb);
}

int StreamedWAV::OggSeekCallback(void* datasource, INT64 pos, int seektype)
{
	switch (seektype)
	{
	case 0:
		((File*)datasource)->Seek((int)pos);
		return 0;
	case 1:
		((File*)datasource)->Seek(((File*)datasource)->GetPosition() + (int)pos);
		return 0;
	case 2:
		((File*)datasource)->Seek(((File*)datasource)->GetSize() - (int)pos);
		return 0;
	default:
		LogDump::LogA("OggSeekCallBack - unknown seek-type (%i)\n", seektype);
		return 0;
	}
}

int StreamedWAV::OggCloseCallback(void* datasource)
{
	delete datasource;
	return 0;
}

long StreamedWAV::OggTellCallback(void* datasource)
{
	return ((File*)datasource)->GetPosition();
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
	memset(m_ChunkId, 0, sizeof(m_ChunkId));
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
			memset(m_ChunkId, 0, sizeof(m_ChunkId));
			m_Flags = m_Flags & 0xFFFFFFF9;

			return;
		}
	}
	else
		if (m_FileHandle && m_WavFile && unk && m_WavFile->m_FileHandle)
		{
			m_WavFile->m_FileHandle->FlushAndClose(m_FileHandle);
			m_WavFile->m_FileHandle->ReleaseFileHandle();
			m_FileHandle = NULL;
		}

	memset(m_ChunkId, 0, sizeof(m_ChunkId));
	m_Flags = m_Flags & 0xFFFFFFF9;
}

bool StreamedWAV::OpenSoundFile(bool a1)
{
	if ((m_Flags & 16) == 0 ||
		(m_Flags & 4) != 0 &&
		(m_Flags & 1) != 0 &&
		(m_Flags & 2) != 0 &&
		(m_WavFile || m_OggInfo))
		return true;

	switch (m_SoundFormat)
	{
	case FORMAT_OGG:
		return OpenOGG(a1);
	case FORMAT_WAV:
	case FORMAT_XBOX:
		return OpenWAV(a1);
	default:
		return false;
	}
}

bool StreamedWAV::TryLocateCurrentStreamFile() const
{
	return (m_WavFile || m_OggInfo) && File::FindFileEverywhere(m_FileName.m_szString);
}

void StreamedWAV::RemoveSoundBuffer()
{
	if (m_SoundBufferPtr)
	{
		SoundBufferInfo* sbi = CheckIfSoundBufferIsUsed(m_SoundBufferPtr);
		if (sbi->m_BufferPtr)
			sbi->m_Used = false;

		m_SoundBufferPtr = nullptr;
	}

	m_Flags &= 0xFFFFFFFE;
}

bool StreamedWAV::OpenOGG(bool createnew)
{
	if (!createnew)
		return true;

	if (!m_OggInfo)
	{
		File* f = new File(m_FileName.m_szString, 0x21, true);

		if (!f->IsFileOpen())
		{
			m_Flags &= 0xFFFFFFF9;
			return false;
		}

		m_OggInfo = new OggVorbis_File;
		ov_callbacks callbacks;
		callbacks.close_func = OggCloseCallback;
		callbacks.seek_func = OggSeekCallback;
		callbacks.tell_func = OggTellCallback;
		callbacks.read_func = OggReadCallback;

		ov_open_callbacks(f, m_OggInfo, nullptr, NULL, callbacks);
	}

	vorbis_info* vi = ov_info(m_OggInfo, -1);
	ogg_int64_t pcmt = ov_pcm_total(m_OggInfo, -1);

	m_Channels = vi->channels;
	m_SamplesPerSec = vi->rate;
	m_Samples = (int)pcmt;
	m_BytesPerSample = 2.f;
	m_BlockAlign = 0;
	m_ChunkSize = m_Channels * 2 * (int)pcmt;
	field_C = m_ChunkSize;
	m_Flags = m_Flags & 0xFFFFFFF7 | 7;

	return true;
}

bool StreamedWAV::OpenWAV(bool createnew)
{
	if (!m_WavFile)
		m_WavFile = new File(m_FileName.m_szString, 0x61, false);

	if (!createnew)
		return true;

	if (!m_WavFile->IsFileOpen() && m_WavFile->m_FileHandle)
	{
		m_FileHandle = m_WavFile->m_FileHandle->CreateFile_();
		m_WavFile->m_FileHandle->SetFileHandle(m_FileHandle);

		if (m_FileHandle)
			m_WavFile->_WriteBufferAndSetToStart();
	}

	if (!m_WavFile->IsFileOpen())
	{
		ReleaseSemaphore(StreamedSoundBuffers::SemaphoreObject, 1, 0);
		return false;
	}

	m_Flags &= 0xFFFFFFF7;

	m_WavFile->Read(&m_ChunkId, sizeof(m_ChunkId));

#ifdef INCLUDE_FIXES
	if (strncmp(m_ChunkId, "RIFF", sizeof(m_ChunkId)))
		return false;
#else
	strncmp(m_ChunkId, "RIFF", 4);
#endif

	m_WavChunkSize = m_WavFile->ReadIntLittleToBigEndian();

	m_WavFile->Read(&m_ChunkId, sizeof(m_ChunkId));

#ifdef INCLUDE_FIXES
	if (strncmp(m_ChunkId, "WAVE", sizeof(m_ChunkId)))
		return false;
#else
	strncmp(m_ChunkId, "WAVE", sizeof(m_ChunkId));
#endif

	m_Flags &= 0xFFFFFFFD;
	m_BlockAlign = 0;

	if (m_WavFile->Read(&m_ChunkId, sizeof(m_ChunkId)) != sizeof(m_ChunkId))
	{
		m_Samples = m_ChunkSize / (int)(m_Channels * m_BytesPerSample);
		m_Flags |= 4;

		return true;
	}

	while (strncmp(m_ChunkId, "fmt", sizeof(m_ChunkId)) == NULL)
	{
		unsigned int pcm_chunksize = m_WavFile->ReadIntLittleToBigEndian();
		unsigned short pcm_format_tag = m_WavFile->ReadShortLittleToBigEndian();
		unsigned short pcm_channels = m_WavFile->ReadShortLittleToBigEndian();
		unsigned int pcm_samplespersec = m_WavFile->ReadIntLittleToBigEndian();
		unsigned int pcm_avbytespersec = m_WavFile->ReadIntLittleToBigEndian();
		unsigned short pcm_align = m_WavFile->ReadShortLittleToBigEndian();
		unsigned short pcm_bitspersample = m_WavFile->ReadShortLittleToBigEndian();

		m_Channels = pcm_channels;
		m_SamplesPerSec = pcm_samplespersec;
		m_AverageBytesPerSec = pcm_avbytespersec;
		m_BlockAlign = pcm_align;

		if (pcm_format_tag == 105)
		{
			//	TODO: what is this tag?
			m_BlockAlign = m_Channels * 36;
			m_AverageBytesPerSec = (m_Channels * 36) * m_SamplesPerSec / 64;
		}

		m_BytesPerSample = pcm_format_tag == WAVE_FORMAT_PCM ? pcm_bitspersample / 8 : pcm_bitspersample * 0.125f;

		for (unsigned int i = pcm_chunksize - 16; i > 0; --i)
			m_WavFile->ReadBlock();

		m_Flags |= 2;

		if (m_WavFile->Read(&m_ChunkId, sizeof(m_ChunkId)) != sizeof(m_ChunkId))
		{
			m_Samples = m_ChunkSize / (int)(m_Channels * m_BytesPerSample);
			m_Flags |= 4;

			return true;
		}

		if (strncmp(m_ChunkId, "data", sizeof(m_ChunkId)))
		{
			for (unsigned int i = m_WavFile->ReadIntLittleToBigEndian(); i > 0; --i)
				m_WavFile->ReadBlock();

			if (m_WavFile->Read(&m_ChunkId, sizeof(m_ChunkId)) != sizeof(m_ChunkId))
			{
				m_Samples = m_ChunkSize / (int)(m_Channels * m_BytesPerSample);
				m_Flags |= 4;

				return true;
			}
		}
	}

	m_ChunkSize = m_WavFile->ReadIntLittleToBigEndian();
	field_C = m_ChunkSize;
	m_Samples = m_ChunkSize / (int)(m_Channels * m_BytesPerSample);
	m_Flags |= 4;

	return true;
}

void* StreamedWAV::operator new(size_t size)
{
	if (!MemoryManager::Released)
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	else
		return nullptr;
}

void* StreamedWAV::operator new[](size_t size)
{
	if (!MemoryManager::Released)
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
	else
		return nullptr;
}

void StreamedWAV::operator delete(void* ptr)
{
	if (ptr)
		MemoryManager::AllocatorsList[DEFAULT]->Free(ptr);
	ptr = nullptr;
}