#include "SoundFile.h"
#include "StreamedSoundBuffers.h"
#include "LogDump.h"
#include "ScriptDatabase.h"

SoundFile SoundFile::GlobalMonoStream;

SoundFile::SoundFile()
{
 MESSAGE_CLASS_CREATED(SoundFile);

 m_StreamedWAV = nullptr;
 field_24 &= 0x7FFFFFFF;
 field_24 &= 0xFFFFFFFE;
}

SoundFile::~SoundFile()
{
 MESSAGE_CLASS_DESTROYED(SoundFile);

 delete m_StreamedWAV;
}

void SoundFile::_444F90(const String& fn)
{
 if (fn == m_FileName)
 {
  // NOTE: why?
  m_FileName = fn;
  field_24 &= 0xFFFFFFFE;
 }
}

void SoundFile::CopySoundPropertiesFromStreamedWav()
{
 if (!m_StreamedWAV)
  return;

 m_SoundFormat = m_StreamedWAV->m_SoundFormat;
 m_WavChunkSize = m_StreamedWAV->m_WavChunkSize;
 field_4 = m_StreamedWAV->field_C;
 m_Samples = m_StreamedWAV->m_Samples;
 field_24 = (field_24 ^ (32 * m_StreamedWAV->m_Channels)) & 0x7FFFFFE0 ^ field_24;
 m_BytesPerSample = m_StreamedWAV->m_BytesPerSample;
 m_AverageBytesPerSec = m_StreamedWAV->m_AverageBytesPerSec;
 m_BlockAlign = m_StreamedWAV->m_BlockAlign;
 m_Frequency = m_StreamedWAV->m_SamplesPerSec;
 m_ChunkSize = m_StreamedWAV->m_ChunkSize;
 field_24 = ((unsigned char)field_24 ^ (unsigned char)(2 * m_StreamedWAV->m_Flags.m_Flags)) & 2 ^ field_24;
 field_24 = field_24 ^ ((unsigned char)field_24 ^ (unsigned char)(2 * m_StreamedWAV->m_Flags.m_Flags)) & 4;
 field_24 = field_24 ^ ((unsigned char)field_24 ^ (unsigned char)(2 * m_StreamedWAV->m_Flags.m_Flags)) & 8;
 field_24 = field_24 ^ ((unsigned char)field_24 ^ ((unsigned char)m_StreamedWAV->m_Flags.m_Flags)) & 0x10 | 1;
}

#pragma message(TODO_IMPLEMENTATION)
void SoundFile::CopySoundPropertiesFromStreamedWav_1()
{
 if (!m_StreamedWAV)
  return;
}

void SoundFile::ReadStreamedSoundFile(bool a1)
{
 if (!a1 && (field_24 & 1) == 0)
  a1 = true;

 delete m_StreamedWAV;
 m_StreamedWAV = new StreamedWAV(72000, m_FileName.m_Str);
 
 WaitForSingleObject(StreamedSoundBuffers::SemaphoreObject, INFINITE);

 if (m_StreamedWAV->OpenSoundFile(a1))
 {
  ReleaseSemaphore(StreamedSoundBuffers::SemaphoreObject, 1, 0);
  m_StreamedWAV->DestroySoundBuffers(true);

  if (a1)
   CopySoundPropertiesFromStreamedWav();
  else
   CopySoundPropertiesFromStreamedWav_1();

  field_24 |= 0x80000000;
 }
 else
 {
  LogDump::LogA("Could not open/find streamed sound called %s\n", m_FileName.m_Str);

  delete m_StreamedWAV;
  field_24 &= 0x7FFFFFFF;
  ReleaseSemaphore(StreamedSoundBuffers::SemaphoreObject, 1, 0);
 }
}

void SoundFile::Open(const char* const filename)
{
 delete m_StreamedWAV;
 m_FileName = filename;

 if (!filename || !*filename)
  return;

 char dir[1024] = {};
 char fn[128] = {};
 char finalname[1152] = {};
 File::ExtractFilePath(m_FileName.m_Str, dir, fn, nullptr);

 strcpy(finalname, dir);
 strcat(finalname, fn);
 strcat(finalname, ".");
 strcat(finalname, Script::StreamedSoundExt.m_Str);

 m_FileName = finalname;
 ReadStreamedSoundFile(true);
}

void SoundFile::ChangeFileNameIfDifferent(const String& fname)
{
 if (m_FileName.Equal(fname.m_Str))
 {
  m_FileName = fname;
  field_24 &= 0xFFFFFFFE;
 }
}