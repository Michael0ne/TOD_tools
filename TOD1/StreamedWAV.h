#pragma once
#include "File.h"
#include <vector>
#include "libvorbis-1.1.0/include/vorbisfile.h"

#define AUDIO_AUXMONOSTREAM_CLASS_SIZE 92
#define AUDIO_AUXMONOSTREAM_FLAG_FILE_SET 0x10

enum AuxMonoStreamSoundFormat
{
	FORMAT_UNSET = 0,
	FORMAT_OGG = 1,
	FORMAT_WAV = 2,
	FORMAT_XBOX = 3
};

struct SoundBufferInfo
{
	bool			m_Used;
	char* m_BufferPtr;

	SoundBufferInfo()
		: m_Used(false), m_BufferPtr(nullptr)
	{};

	SoundBufferInfo(bool _used, char* _buffer)
		: m_Used(_used), m_BufferPtr(_buffer)
	{};
};

class StreamedWAV
{
protected:
	AuxMonoStreamSoundFormat m_SoundFormat;
	int				m_WavChunkSize;
	int				field_C;
	int				m_Samples;
	int				m_Channels;
	float			m_BytesPerSample;
	int				m_AverageBytesPerSec;
	int				m_BlockAlign;
	int				m_SamplesPerSec;
	int				m_ChunkSize;
	char*			m_SoundBufferPtr;
	int				m_Flags;
	int				field_34;
	String			m_FileName;
	File*			m_WavFile;
	OggVorbis_File* m_OggInfo;
	HANDLE			m_FileHandle;
	char			m_ChunkId;
	int				m_SampleRate;

	static SoundBufferInfo* CheckIfSoundBufferIsUsed(char*);	//	@40F600

	static SoundBufferInfo EmptySoundBufferInfo;	//	@A35B8C

public:
	virtual ~StreamedWAV();	//	@40FD90

	StreamedWAV(unsigned int sampleRate);	//	@40FAB0
	StreamedWAV(unsigned int sampleRate, const char* soundFile);	//	@40FBA0

	void			DestroySoundBuffers(bool);	//	@40F4C0

	void* operator new(size_t);
	void* operator new[](size_t size);
	void operator delete(void*);

	static std::vector<SoundBufferInfo>	SoundBuffersList;	//	@A08468
};

ASSERT_CLASS_SIZE(StreamedWAV, 92);