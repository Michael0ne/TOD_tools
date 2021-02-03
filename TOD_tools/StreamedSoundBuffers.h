#pragma once

#include "Types.h"
#include "Globals.h"
#include <vector>

namespace Audio {

#define STREAMEDSOUNDBUFFERS_CLASS_SIZE 300
#define STREAMEDSOUNDBUFFERS_MAX_CONCURRENT_SOUNDS 100

	enum SoundSystemType
	{
		SOUND_SYSTEM_AUTOSELECT = 1,	//	NOTE: when this is selected, it gets re-selected with directsound or dieselpower based on speaker configuration.
		SOUND_SYSTEM_DSOUND = 2,
		SOUND_SYSTEM_DIESELPOWER = 3,
		SOUND_SYSTEM_UNDEFINED = -1
	};

	const int SoundPriorityLevels[] = {
		0x3F000000,
		DSSCL_NORMAL,
		DSSCL_PRIORITY,
		DSSCL_EXCLUSIVE,
		DSSCL_WRITEPRIMARY
	};

	struct SoundBufferStatus
	{
		class StreamBuffer* m_StreamBufferPtr;
		bool	m_InUse;
	};

	//-------------- Handles streamed game sounds ----------------
	//	TODO: redo this class.
	class StreamedSoundBuffers
	{
	public:
		int m_nMaxConcurrentSounds;
		int m_nCurrentlyPlaying;
		float m_f8;
		int field_C;
		bool m_Muted;
		bool m_GlobalPauseCalled;
		bool m_GlobalPause;
		bool m_Sound;
		char field_14;
		SoundSystemType m_nSoundSystem;
		enum {
			SPEAKER_CONFIG_DIRECT_OUT = 0,
			SPEAKER_CONFIG_HEADPHONES,
			SPEAKER_CONFIG_MONO,
			SPEAKER_CONFIG_QUAD,
			SPEAKER_CONFIG_SURROUND,
			SPEAKER_CONFIG_51,
			SPEAKER_CONFIG_71,
			SPEAKER_CONFIG_STEREO = -1
		} m_nSpeakerConfig;
		LPDIRECTSOUNDBUFFER m_pDirectSoundBuffer;
		int* field_24;	//	NOTE: this and one below are something related to Diesel Power sound buffers. The 3D and 2D ones.
		int* field_28;
		int m_nDirectSoundChannels;
		int m_nDirectSoundSampleRate;
		int m_nDirectSoundBits;
		int field_38;
		int m_nMonoStreamsTotal;
		int field_40;
		LPDIRECTSOUND3DBUFFER m_pDirectSound3DBuffer;
		LPDIRECTSOUNDBUFFER m_pDirectSoundBuffer_1;
		class DieselPower* m_pDieselPower;
		int* field_50;	//	NOTE: pointer to some DieselPower structure, size unknown.
		int* field_54;
		IDirectSound* m_pDirectSound;
		List<int> m_DieselPowerSoundBuffers;
		List<int> m_DieselPowerSoundBuffers_1;
		List<int> m_SoundList_1;
		List<int> m_SoundList_2;
		List<int> m_SoundList_3;
		List<int> m_SoundList_4;
		List<int> m_SoundList_5;
		List<int> m_SoundList_6;
		List<int> m_SoundList_7;
		List<int> m_SoundList_8;
		List<int>* m_SoundList_1_Ptr;
		List<int>* m_SoundList_2_Ptr;
		List<int>* m_SoundList_3_Ptr;
		List<int>* m_SoundList_4_Ptr;
		List<int>* m_SoundList_5_Ptr;
		List<int>* m_SoundList_6_Ptr;
		List<int>* m_SoundList_7_Ptr;
		List<int>* m_SoundList_8_Ptr;
		String m_Str_1;
	public:
		StreamedSoundBuffers(char channels, int sampleRate);	//	@43E080
		~StreamedSoundBuffers();	//	@43E450

		inline void	SetDefaultVolumeLevels();	//	@43CE20
		void		InitDieselPower();	//	@43CE90
		void		GetMaxDistance(Vector4f& outVec) const;	//	@43CFD0
		HRESULT		CreateSoundBuffer(DSBUFFERDESC* bufferDesc);	//	@43D060
		void		SetPrimarySoundFormat(int channels, int sampleRate, int bits);	//	@43D0D0
		float		GetDefaultVolumeForType(int type);	//	@43D180
		void		SetGlobalPause(bool pause);	//	@43D1D0
		void		_43D200(int unk1, int unk2, int unk3, int unk4, int unk5);	//	@43D200
		void		InitDirectSound(char channels, int sampleRate);	//	@43D310
		void		SetListener3DPos(const Vector4f& pos);	//	@43D560
		void		SetListener3DOrientation(const Orientation& orient);	//	@43D6F0
		void		GetListener3DOrientation(Orientation& outOrient);	//	@43DFD0
		void		WaitForSoftPause();	//	@43E640
		int			_43E7B0();	//	@43E7B0	//	NOTE: unused
		void		MeasureWaitForSoftPause();	//	@43E800
		void		_43E850();	//	@43E850	//	NOTE: unused
		void		_43E880();	//	@43E880	//	NOTE: unused
		void		PreallocateStreamBuffersPool();	//	@
		void		CreateStaticStreamBuffer();	//	@

		static std::vector<SoundBufferStatus>	vSoundBuffers;	//	@A09314
	};

	static float	DefaultFxVolume;	//	@A09230
	static float	DefaultAmbienceVolume;	//	@A09234
	static float	DefaultMusicVolume;	//	@A09238
	static float	DefaultSpeaksVolume;	//	@A0923C

	static SoundSystemType		SoundRendererId;	//	@A35ECC

	static HANDLE	SemaphoreObject;	//	@A35EC8

	void		SetDefaultFxVolume(float vol);	//	@43CDA0
	void		SetDefaultAmbienceVolume(float vol);	//	@43CDB0
	void		SetDefaultMusicVolume(float vol);	//	@43CDC0
	void		SetDefaultSpeaksVolume(float vol);	//	@43CDD0

	float	GetDefaultFxVolume();	//	@43CDE0
	float	GetDefaultAmbienceVolume();	//	@43CDF0
	float	GetDefaultMusicVolume();	//	@43CE00
	float	GetDefaultSpeaksVolume();	//	@43CE10

	void		RememberSoundRenderer(SoundSystemType soundRendererId);	//	@43D210
	SoundSystemType		GetSoundRenderer();	//	@43D280
}

extern Audio::StreamedSoundBuffers* g_StreamedSoundBuffers;

static_assert(sizeof(Audio::StreamedSoundBuffers) == STREAMEDSOUNDBUFFERS_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(StreamedSoundBuffers));