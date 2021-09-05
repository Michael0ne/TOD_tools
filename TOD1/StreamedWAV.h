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
 bool   m_Used;
 char*   m_BufferPtr;

 SoundBufferInfo()
  : m_Used(false), m_BufferPtr(nullptr)
 {};

 SoundBufferInfo(bool _used, char* _buffer)
  : m_Used(_used), m_BufferPtr(_buffer)
 {};
};

class StreamedWAV
{
 friend class SoundFile;
 friend class StreamedSoundBuffer;
protected:
 AuxMonoStreamSoundFormat m_SoundFormat;
 int    m_WavChunkSize;
 int    field_C;
 int    m_Samples;
 int    m_Channels;
 float   m_BytesPerSample;
 int    m_AverageBytesPerSec;
 int    m_BlockAlign;
 int    m_SamplesPerSec;
 int    m_ChunkSize;
 char*   m_SoundBufferPtr;
 union
 {
  struct
  {
   unsigned char SoundBufferInUse : 1;
   unsigned char _1 : 1;
   unsigned char Initialised : 1;
   unsigned char _3 : 1;
   unsigned char HasSoundFile : 1;
   unsigned char _5 : 1;
   unsigned char _6 : 1;
   unsigned char _7 : 1;
  }   m_FlagsBits;
  unsigned int m_Flags;
 }    m_Flags;
 int    field_34;
 String   m_FileName;
 File*   m_WavFile;
 OggVorbis_File* m_OggInfo;
 HANDLE   m_FileHandle;
 char   m_ChunkId[4];
 int    m_SampleRate;

 static SoundBufferInfo* CheckIfSoundBufferIsUsed(char*); // @40F600
 static size_t OggReadCallback(void* buff, size_t size, size_t nmemb, void* datasource); // @40F000
 static int  OggSeekCallback(void* datasource, INT64 pos, int seektype); // @40F020
 static int  OggCloseCallback(void* datasource); // @40F0A0
 static long  OggTellCallback(void* datasource); // @40F0C0

 static SoundBufferInfo EmptySoundBufferInfo; // @A35B8C

public:
 virtual ~StreamedWAV(); // @40FD90

 StreamedWAV(unsigned int sampleRate); // @40FAB0
 StreamedWAV(unsigned int sampleRate, const char* soundFile); // @40FBA0

 bool   OpenOGG(bool createnew); // @40F0D0
 bool   OpenWAV(bool createnew); // @40F200
 bool   OpenSoundFile(bool a1); // @40F470
 void   DestroySoundBuffers(bool); // @40F4C0
 bool   TryLocateCurrentStreamFile() const; // @40F5A0
 void   RemoveSoundBuffer(); // @40F640
 int    ReadNextPiece(const bool flushbuffers); // @40F9F0

 void* operator new(size_t);
 void* operator new[](size_t size);
 void operator delete(void*);

 static std::vector<SoundBufferInfo> SoundBuffersList; // @A08468
};

ASSERT_CLASS_SIZE(StreamedWAV, 92);