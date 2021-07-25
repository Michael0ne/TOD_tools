#include "SoundAsset.h"
#include "File.h"
#include "LogDump.h"
#include "StreamedSoundBuffer.h"

AssetInstance* SoundAsset::Instance;

SoundAsset::SoundFileInfo::SoundFileInfo(const char* const filepath)
{
	MESSAGE_CLASS_CREATED(SoundFileInfo);

	char buf[4] = {};
	File sndfile(filepath, 0x21, true);

	field_8 = 12;
	sndfile.Read(buf, sizeof(buf));

#ifdef INCLUDE_FIXES
	if (strncmp(buf, "RIFF", sizeof(buf)))
		return;
#else
	strncmp(buf, "RIFF", sizeof(buf));
#endif

	sndfile.ReadIntLittleToBigEndian();	//	NOTE: skip chunksize property.
	sndfile.Read(buf, sizeof(buf));

#ifdef INCLUDE_FIXES
	if (strncmp(buf, "WAVE", sizeof(buf)))
		return;
#else
	strncmp(buf, "WAVE", sizeof(buf));
#endif

	int blocksize = 0;
	while (sndfile.Read(buf, sizeof(buf)) == sizeof(buf))
	{
		field_8 += 4;
		
		if (strncmp(buf, "fmt ", sizeof(buf)) == NULL)
		{
			int chunksize = sndfile.ReadIntLittleToBigEndian();
			field_8 += chunksize + 4;
			sndfile.ReadShortLittleToBigEndian();	//	NOTE: skip format code property.
			m_InterleavedChannels = sndfile.ReadShortLittleToBigEndian();
			m_SamplesPerSec = sndfile.ReadIntLittleToBigEndian();
			sndfile.ReadIntLittleToBigEndian();	//	NOTE: skip data rate property.
			blocksize = sndfile.ReadShortLittleToBigEndian();
			m_BytesPerSample = sndfile.ReadShortLittleToBigEndian() / 8;

			if (chunksize - 16 > 0)
				for (chunksize = chunksize - 16; chunksize; chunksize--)
					sndfile.ReadBlock();
		}
		else
			if (strncmp(buf, "data", sizeof(buf)) == NULL)
			{
				m_ChunkSize = sndfile.ReadIntLittleToBigEndian();
				field_8 += 4 + m_ChunkSize;
				m_SampledData = new char[m_ChunkSize];
				sndfile.Read(m_SampledData, m_ChunkSize);
			}
			else
			{
				int chnk = sndfile.ReadIntLittleToBigEndian();
				field_8 += chnk + 4;

				if (chnk > 0)
					for (; chnk; chnk--)
						sndfile.ReadBlock();
			}
	}

	m_TotalChunks = m_ChunkSize / blocksize;
}

SoundAsset::SoundFileInfo::~SoundFileInfo()
{
	MESSAGE_CLASS_DESTROYED(SoundFileInfo);

	delete m_SampledData;
}

const int SoundAsset::SoundFileInfo::GetTotalChunks() const
{
	return m_TotalChunks;
}

const short SoundAsset::SoundFileInfo::GetNumberOfChannels() const
{
	return m_InterleavedChannels;
}

const short SoundAsset::SoundFileInfo::GetBytesPerSample() const
{
	return m_BytesPerSample;
}

const int SoundAsset::SoundFileInfo::GetSamplesPerSec() const
{
	return m_SamplesPerSec;
}

const void* SoundAsset::SoundFileInfo::GetSampledData() const
{
	return m_SampledData;
}

SoundAsset::SoundAsset() : Asset(false)
{
	MESSAGE_CLASS_CREATED(SoundAsset);

	m_StreamBuffer = nullptr;
}

SoundAsset::~SoundAsset()
{
	MESSAGE_CLASS_DESTROYED(SoundAsset);
}

AssetInstance* SoundAsset::GetInstancePtr() const
{
	return Instance;
}

void SoundAsset::LoadResource(const char* const resPath)
{
	SoundFileInfo sndfile(resPath);
	LogDump::LogA("loading sound: %s\n", resPath);

	if (sndfile.GetTotalChunks())
	{
		m_StreamBuffer = new StreamedSoundBuffer(
			false,
			sndfile.GetTotalChunks(),
			sndfile.GetNumberOfChannels(),
			sndfile.GetNumberOfChannels() == 2,
			sndfile.GetBytesPerSample(),
			sndfile.GetSamplesPerSec(),
			sndfile.GetNumberOfChannels() != 2,
			1,
			1,
			resPath);
		m_StreamBuffer->SetSampledData((void*)sndfile.GetSampledData());
		m_StreamBuffer->stub3();
	}
	else
		delete m_StreamBuffer;
}

void SoundAsset::CreateInstance()
{
	Instance = new AssetInstance("sound", (CREATOR)Create);
	Instance->m_FileExtensions.push_back("wav");
	Instance->SetAlignment(16, 1);
	Instance->SetAlignment(16, 2);
	Instance->SetAlignment(16, 0);
	Instance->field_2C = 1;
}

SoundAsset* SoundAsset::Create()
{
	return new SoundAsset;
}
