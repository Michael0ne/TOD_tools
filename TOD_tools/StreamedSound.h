#pragma once
#include "List.h"
#include "Types.h"
#include <map>

class IStreamBuffer
{
public:
	virtual ~IStreamBuffer();	//	@43F3B0
};

class StreamBuffer : public IStreamBuffer
{
protected:
	unsigned int			m_SamplesPerSec;
	char					m_Channels;
	char					field_9;
	char					field_A;
	char					field_B;
	unsigned int			m_TotalChunks;
	unsigned int			m_SampledDataSize;
	float					field_14;
	float					m_BytesPerSec;
	int						field_1C;
	float					field_20;
	void*					m_SampledData;
	std::map<int, int>		field_28;
	int						field_34;
	class StreamedWAV*		m_AuxMonoStream_1;
	List<int>				m_List;

public:
	virtual ~StreamBuffer();	//	@441AC0
	StreamBuffer();	//	@446690
};

class StreamedSound : public StreamBuffer
{
	friend class StreamedSoundBuffers;
protected:
	int*					field_4C;

	union
	{
		struct
		{
			unsigned _0  : 1;
			unsigned Started  : 1;
			unsigned PlayRequest	 : 1;
			unsigned StopRequest  : 1;
			unsigned _4  : 1;
			unsigned PreLoaded  : 1;
			unsigned ThreadExited  : 1;
			unsigned _7  : 1;
			unsigned LastChunkPlaying  : 1;
			unsigned _9  : 1;
			unsigned _10 : 1;
			unsigned _11 : 1;
			unsigned _12 : 1;
			unsigned _13 : 1;
			unsigned WaitMult : 1;
			unsigned PauseSound : 1;
			unsigned StopEvent : 1;
			unsigned PreEvent : 1;
			unsigned StartSent : 1;
			unsigned _19 : 1;
			unsigned _20 : 1;
			unsigned _21 : 1;
			unsigned _22 : 1;
			unsigned _23 : 1;
			unsigned _24 : 1;
			unsigned _25 : 1;
			unsigned _26 : 1;
			unsigned _27 : 1;
			unsigned _28 : 1;
			unsigned _29 : 1;
			unsigned StartEvent : 1;
		}					m_FlagBits;
		unsigned int		m_Flags;
	}						m_Flags;

	int						field_54;
	int						field_58;
	int						field_5C;
	char					m_StoppingFinished;
	void*					m_SoundBufferBlockStartPtr;
	int						m_SoundBufferBlockSize;
	int						field_6C;
	int						field_70;
	class StreamedWAV*		m_StreamedWAV;
	int						field_78;
	int						field_7C;
	LPDWORD					m_ThreadId;
	int						field_84;
	LPDIRECTSOUNDBUFFER		m_DirectSoundBuffer;
	int*					field_8C;
	LPDIRECTSOUND3DBUFFER	m_DirectSound3DBuffer;
	int*					field_94;
	int*					m_DieselPowerSoundBuffer;
	HANDLE					field_9C;
	HANDLE					field_A0;
	HANDLE					m_EventNotify;
	HANDLE					m_EvHandle_2;
	HANDLE					m_TerminateThreadEvent;
	HANDLE					field_B0;
	HANDLE					m_StreamThread;
	int						m_CurrentAudioPosition;
	float					field_BC;
	int						m_FrequencyMultiplied;
	int						field_C4;
	Vector4f				m_Position;
	float					m_Pan;
	float					m_Frequency;
	float					field_E0;
	float					m_Volume;
	int						field_E8;
	int						field_EC;

public:
	virtual ~StreamedSound();	//	@446640
	StreamedSound(class SoundFile* sndfile, int, int, int, char, char);	//	@445770

	virtual void			stub2();
	virtual void			stub3();
	virtual void			SetSampledData(void*);
	virtual bool			_443350(int);
	virtual void			_4433A0(bool);
	virtual bool			IsLooped(int);
	virtual int				Play(int, char, int);
	virtual bool			IsPlaying(int);
	virtual bool			AreAnyInstancesPlaying();
	virtual void			_440850(int);
	virtual void			Stop(int);
	virtual void			SetPause(int, bool);
	virtual void			SetPause_A(int, bool);
	virtual bool			IsPaused(int);
	virtual bool			IsMonoStreamCreated();
	virtual int				_443480();
	virtual void			SetVolume(int, float);
	virtual float			GetVolume(int);
	virtual void			SetFrequencyMultiplier(int, float mul);
	virtual float			GetFrequencyMultiplier(int);
	virtual int				_4435C0(int, int);
	virtual float			_443650(int);
	virtual void			SetSoundPosition(int, const Vector4f*);
	virtual void			_4438B0(Vector4f*, int);
	virtual void			_443990(int, int*);
	virtual Vector4f*		_4439E0(Vector4f*, int);
	virtual void			SetPan(int, float);
	virtual float			GetPan(int);
	virtual void			SetFrequency(int, float);
	virtual float			GetFrequency(int);
	virtual void			_443C20(int, float);
	virtual float			_443C90(int);
	virtual void			_443CD0(int, float);
	virtual float			_443D30(int);
	virtual void			_443D40(int, float, float, float);
	virtual int				_442810();
	virtual void			DumpInfo();
	virtual void			_443E00();
};

ASSERT_CLASS_SIZE(StreamBuffer, 76);
ASSERT_CLASS_SIZE(StreamedSound, 240);