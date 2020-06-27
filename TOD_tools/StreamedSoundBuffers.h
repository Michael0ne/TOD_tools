#pragma once

#include "stdafx.h"

#include "Types.h"
#include "Globals.h"

#include "MemoryAllocators.h"

namespace Audio {

#define STREAMEDSOUNDBUFFERS_CLASS_SIZE 300

	enum E_SOUND_SYSTEM_TYPE : unsigned int
	{
		UNKNOWN = 1,
		DIRECT_SOUND = 2,
		DIESELPOWER = 3
	};

	class DieselPower;

	//	TODO: better name.
	struct SoundBufferInstance
	{
		int* m_nUnkInt1;
		int m_nUnkInt2;
		int m_nUnkInt3;
		int m_nUnkInt4;
	};

	/*
	 *------------------------------------------------------------
	 *------------------------------------------------------------
	 *----------------- Handles streamed sounds-------------------
	 *------------------------------------------------------------
	 *--------- Game's alias is 'StreamedSoundBuffers' -----------
	 *------------------------------------------------------------
	 * -----------------------------------------------------------
	*/
	//	FIXME: alignment is probably fucked up here, because lack of 'lpVtbl' member.
	class StreamedSoundBuffers
	{
	private:
		int m_nMaxConcurrentSounds;
		int m_nCurrentlyPlaying;
		int	field_8;
		int	field_C;
		BYTE m_nUnkByte1;
		BYTE m_bGlobalPauseCalled;
		BYTE m_bGlobalPause;
		BYTE m_bSound;
		int	field_14;
		E_SOUND_SYSTEM_TYPE m_nSoundSystem;
		int m_nSpeakerConfig;
		int	field_20;
		int	field_24;
		int	field_28;
		int m_nDirectSoundChannels;
		int m_nDirectSoundSampleRate;
		int m_nDirectSoundBits;
		int	field_38;
		int m_nMonoStreamsTotal;
		int	field_40;
		LPDIRECTSOUND3DBUFFER m_pDirectSound3DBuffer;
		LPDIRECTSOUNDBUFFER m_pDirectSoundBuffer;
		DieselPower* m_pDieselPower;
		int	field_50;
		int	field_54;
		LPDIRECTSOUND m_pDirectSound;
		int* m_pSoundBuffers;
		int m_nUnkDieselPowerSoundsCount;
		int	field_64;
		int	field_68;
		int* m_pSoundBuffers_1;
		int m_nAllocatedTotal;
		int	field_74;
		int	field_78;
		int	field_7C;	//	NOTE: Below are 8 lists of 'StreamBuffers'.
		int	field_80;
		int	field_84;
		int	field_88;
		int	field_8C;
		int	field_90;
		int	field_94;
		int	field_98;
		int	field_9C;
		int	field_A0;
		int	field_A4;
		int	field_A8;
		int	field_AC;
		int	field_B0;
		int	field_B4;
		int	field_B8;
		int	field_BC;
		int	field_C0;
		int	field_C4;
		int	field_C8;
		int	field_CC;
		int	field_D0;
		int	field_D4;
		int	field_D8;
		int	field_DC;
		int	field_E0;
		int	field_E4;
		int	field_E8;
		int	field_EC;
		int	field_F0;
		int	field_F4;
		SoundBufferInstance* m_pInstances;
		int	field_FC;
		int	field_100;
		int	field_104;
		int	field_108;
		int	field_10C;
		int	field_110;
		int	field_114;
		int	field_118;
		int	field_11C;
		int	field_120;
		int	field_124;
		int	field_128;

	public:
		StreamedSoundBuffers()
		{
			MESSAGE_CLASS_CREATED(StreamedSoundBuffers);
		}

		~StreamedSoundBuffers()
		{
			MESSAGE_CLASS_DESTROYED(StreamedSoundBuffers);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}

		void					Dump() const;	//	@43EAD0

		static inline void		SetDefaultFxVolumeVar(float fVol)	//	@43CDA0
		{
			DefaultFxVolume = fVol;
		}
		static inline void		SetDefaultAmbienceVolumeVar(float fVol)	//	@43CDB0
		{
			DefaultAmbienceVolume = fVol;
		}
		static inline void		SetDefaultMusicVolumeVar(float fVol)	//	@43CDC0
		{
			DefaultMusicVolume = fVol;
		}
		static inline void		SetDefaultSpeaksVolumeVar(float fVol)	//	@43CDD0
		{
			DefaultSpeaksVolume = fVol;
		}
		//static float			GetDefaultFxVolumeVar() { return g_fDefaultFxVolume; }	//	@43CDE0
		//static float			GetDefaultAmbienceVolumeVar() { return g_fDefaultAmbienceVolume; }	//	@43CDF0
		//	>> 43CE00
		//static float			GetDefaultMusicVolumeVar() { return g_fDefaultMusicVolume; }
		//	>> 43CE10
		//static float			GetDefaultSpeaksVolumeVar() { return g_fDefaultSpeaksVolume; }
		void					_unkVolumeSet();	//	@43CE20
		void					InitDieselPower();	//	@43CE90
		void					_43CFD0(int* pOut);	//	@43CFD0
		int						_43D060(int nUnk);	//	@43D060
		void					SetDirectSoundBufferFormat(int nChannels, int nSampleRate, int nBits);	//	@43D0D0
		float					GetDefaultVolumeForType(int nWhat);	//	@43D180
		void					SetGlobalPause(bool bPause);	//	@43D1D0
		void					nullsub_35();	//	@43D200
		static int				GetSoundRenderer();	//	@43D280
		void					InitiDirectSound(char nChannels, int nSampleRate);	//	@43D310
		void					SetListener3DPos(const Vector3<float>& vPos);	//	@43D560
		void					SetListener3DOrient(const Quaternion<float>& orient);	//	@43D6F0
		void					GetListener3DOrientation(Quaternion<float>& pOutOrient);	//	@43DFD0
		void					SelectAudioRenderer(int nChannels, int nSampleRate);	//	@43E080
		void					_unkDestroyBuffers();	//	@43E450
		void					WaitForSoftPause();	//	@43E640
		int						_43E7B0();	//	@43E7B0
		void					MeasureWaitForSoftPause();	//	@43E800

		void					SetMaxConcurrentSounds(int maxsounds)
		{
			m_nMaxConcurrentSounds = maxsounds;
		}

		void					SetSoundEnabled(bool enabled)
		{
			m_bSound = enabled;
		}

		static	int& SoundRendererId;
		static	HANDLE& SemaphoreObject;
		static	float& DefaultFxVolume;
		static	float& DefaultAmbienceVolume;
		static	float& DefaultMusicVolume;
		static	float& DefaultSpeaksVolume;

		static void		RememberSoundRenderer(signed int id);	//	@43D210
	};

	extern StreamedSoundBuffers* g_StreamedSoundBuffers;
}

static_assert(sizeof(Audio::StreamedSoundBuffers) == STREAMEDSOUNDBUFFERS_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(StreamedSoundBuffers));