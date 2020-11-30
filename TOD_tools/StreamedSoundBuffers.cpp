#include "StreamedSoundBuffers.h"
#include "DieselPowerSound.h"
#include "Window.h"
#include "Performance.h"

namespace Audio
{
	StreamedSoundBuffers* g_StreamedSoundBuffers = nullptr;

	void SetDefaultFxVolume(float vol)
	{
		DefaultFxVolume = vol;
	}

	void SetDefaultAmbienceVolume(float vol)
	{
		DefaultAmbienceVolume = vol;
	}

	void SetDefaultMusicVolume(float vol)
	{
		DefaultMusicVolume = vol;
	}

	void SetDefaultSpeaksVolume(float vol)
	{
		DefaultSpeaksVolume = vol;
	}

	float GetDefaultFxVolume()
	{
		return DefaultFxVolume;
	}

	float GetDefaultAmbienceVolume()
	{
		return DefaultAmbienceVolume;
	}

	float GetDefaultMusicVolume()
	{
		return DefaultMusicVolume;
	}

	float GetDefaultSpeaksVolume()
	{
		return DefaultSpeaksVolume;
	}

	void RememberSoundRenderer(SoundSystemType soundRendererId)
	{
		SoundRendererId = soundRendererId;

		if (RegCreateKeyEx(HKEY_CURRENT_USER, RegistryKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, (PHKEY)&soundRendererId, NULL) == ERROR_SUCCESS)
		{
			RegSetValueEx((HKEY)soundRendererId, "SoundRenderer", NULL, REG_DWORD, (const BYTE*)&SoundRendererId, sizeof(SoundRendererId));
			RegCloseKey((HKEY)soundRendererId);
		}
	}

	SoundSystemType GetSoundRenderer()
	{
		if (SoundRendererId)
			return SoundRendererId;

		SoundRendererId = SOUND_SYSTEM_AUTOSELECT;

		HKEY phkResult;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, RegistryKey, NULL, KEY_QUERY_VALUE, &phkResult) == ERROR_SUCCESS)
		{
			DWORD lpType;
			BYTE Data[4];
			DWORD cbData = NULL;
			if (RegQueryValueEx(phkResult, "SoundRenderer", NULL, &lpType, Data, &cbData) == ERROR_SUCCESS &&
				lpType == REG_DWORD)
				SoundRendererId = *(SoundSystemType*)Data;
			RegCloseKey(phkResult);
		}

		return SoundRendererId;
	}

	StreamedSoundBuffers::StreamedSoundBuffers(char channels, int sampleRate)
	{
		MESSAGE_CLASS_CREATED(StreamedSoundBuffers);

		LPDIRECTSOUND directSound;

		m_DieselPowerSoundBuffers;
		m_DieselPowerSoundBuffers_1;
		m_SoundList_1;
		m_SoundList_2;
		m_SoundList_3;
		m_SoundList_4;
		m_SoundList_5;
		m_SoundList_6;
		m_SoundList_7;
		m_SoundList_8;
		m_Str_1;
		m_SoundList_1_Ptr = &m_SoundList_1;
		m_SoundList_4_Ptr = &m_SoundList_4;
		m_SoundList_5_Ptr = &m_SoundList_5;
		m_SoundList_6_Ptr = &m_SoundList_6;
		m_f8 = 0.001f;
		field_C = 1;
		field_38 = NULL;
		field_40 = NULL;
		m_nMonoStreamsTotal = 0;
		m_SoundList_2_Ptr = &m_SoundList_2;
		m_SoundList_3_Ptr = &m_SoundList_3;
		m_SoundList_7_Ptr = &m_SoundList_7;
		m_SoundList_8_Ptr = &m_SoundList_8;
		m_Sound = true;
		field_14 = NULL;
		SemaphoreObject = CreateSemaphore(NULL, 1, 1, NULL);
		m_nMaxConcurrentSounds = STREAMEDSOUNDBUFFERS_MAX_CONCURRENT_SOUNDS;
		m_nCurrentlyPlaying = NULL;
		m_pDirectSoundBuffer = nullptr;
		field_24 = nullptr;
		field_28 = nullptr;
		m_pDieselPower = nullptr;

		if (FAILED(DirectSoundCreate(NULL, &directSound, NULL)))
			IncompatibleMachineParameterError(2, false);

		if (FAILED(directSound->GetSpeakerConfig((LPDWORD)&m_nSpeakerConfig)))
			IncompatibleMachineParameterError(2, false);

		if (directSound)
		{
			directSound->Release();
			directSound = nullptr;
		}

		switch (m_nSpeakerConfig)
		{
		case SPEAKER_CONFIG_DIRECT_OUT:
			debug("direct out speaker configuration\n");
			break;
		case SPEAKER_CONFIG_HEADPHONES:
			debug("headphone speaker configuration\n");
			break;
		case SPEAKER_CONFIG_MONO:
			debug("mono speaker configuration\n");
			break;
		case SPEAKER_CONFIG_QUAD:
			debug("quad speaker configuration\n");
			break;
		case SPEAKER_CONFIG_SURROUND:
			debug("surround speaker configuration\n");
			break;
		case SPEAKER_CONFIG_51:
			debug("5.1 speaker configuration\n");
			break;
		case SPEAKER_CONFIG_71:
			debug("7.1 speaker configuration\n");
			break;
		default:
			debug("stereo speaker configuration\n");
			break;
		}

		m_nSoundSystem = GetSoundRenderer();
		switch (m_nSoundSystem)
		{
		case SOUND_SYSTEM_AUTOSELECT:
			if (m_nSpeakerConfig >= SPEAKER_CONFIG_71)
			{
				m_nSoundSystem = SOUND_SYSTEM_DSOUND;
				debug("auto selecting DirectSound audio renderer\n");
			}else{
				m_nSoundSystem = SOUND_SYSTEM_DIESELPOWER;
				debug("auto selecting Diesel Power audio renderer\n");
			}
			break;
		case SOUND_SYSTEM_DSOUND:
			debug("using DirectSound audio renderer\n");
			break;
		case SOUND_SYSTEM_DIESELPOWER:
			debug("using Diesel Power audio renderer\n");
			break;
		}

		if (m_nSoundSystem == SOUND_SYSTEM_DIESELPOWER)
			InitDieselPower();
		else
			InitDirectSound(channels, sampleRate);

		PreallocateStreamBuffersPool();
		CreateStaticStreamBuffer();
		SetListener3DPos(Vector4f());

		m_GlobalPauseCalled = false;
		m_GlobalPause = false;
	}

	StreamedSoundBuffers::~StreamedSoundBuffers()
	{
		MESSAGE_CLASS_DESTROYED(StreamedSoundBuffers);

		CloseHandle(SemaphoreObject);

		if (m_nSoundSystem == SOUND_SYSTEM_DSOUND)
		{
			if (m_pDirectSoundBuffer_1)
			{
				m_pDirectSoundBuffer_1->Release();
				m_pDirectSoundBuffer_1 = nullptr;
			}

			if (m_pDirectSound)
			{
				m_pDirectSound->Release();
				m_pDirectSound = nullptr;
			}
		}else
			if (m_nSoundSystem == SOUND_SYSTEM_DIESELPOWER)
			{
				for (int ind = 0; ind < m_DieselPowerSoundBuffers.m_nCurrIndex; ++ind)
					if (m_DieselPowerSoundBuffers.m_pElements[ind])
						(*(void (__stdcall *)(signed int))m_DieselPowerSoundBuffers.m_pElements[ind])(1);
				for (int ind = 0; ind < m_DieselPowerSoundBuffers_1.m_nCurrIndex; ++ind)
					if (m_DieselPowerSoundBuffers_1.m_pElements[ind])
						(*(void (__stdcall*)(signed int))m_DieselPowerSoundBuffers_1.m_pElements[ind])(1);
				m_pDieselPower->stub33();
				Sleep(2000);

				delete m_pDieselPower;
				m_pDieselPower = nullptr;
			}

		g_StreamedSoundBuffers = nullptr;

		//	NOTE: destructors for String and List objects are called here generated by the compiler.
	}

	inline void StreamedSoundBuffers::SetDefaultVolumeLevels()
	{
		if (m_nSoundSystem == SOUND_SYSTEM_DIESELPOWER)
		{
			//	TODO: are we sure we want numbers to be here?
			DefaultFxVolume = 1.0f;
			DefaultAmbienceVolume = 1.0f;
			DefaultMusicVolume = 0.75f;
			DefaultSpeaksVolume = 0.75f;
		}else{
			DefaultFxVolume = 0.89999998f;
			DefaultAmbienceVolume = 0.89999998f;
			DefaultMusicVolume = 1.0f;
			DefaultSpeaksVolume = 1.0f;
		}
	}

	void StreamedSoundBuffers::InitDieselPower()
	{
		debug("Initializing Sound System.....\n");

		SetDefaultVolumeLevels();

		m_pDieselPower = DieselPower::CallFactory(1, 2, 1, 3.0f, g_Window->m_hWindow, 1, 0, 0);

		if (!m_pDieselPower)
			return;

		m_pDieselPower->stub25(1);
		m_pDieselPower->stub26(1);

		field_50 = m_pDieselPower->stub24();

		switch (m_nSpeakerConfig - 1)
		{
		case SPEAKER_CONFIG_DIRECT_OUT:
			(*(void(__stdcall*)(int))(field_50 + 8))(0);	//	FIXME: what is stub24 exactly?
			break;
		case SPEAKER_CONFIG_MONO:
		case SPEAKER_CONFIG_SURROUND:
		case SPEAKER_CONFIG_51:
		case SPEAKER_CONFIG_71:
			(*(void(__stdcall*)(int))(field_50 + 8))(2);
			break;
		default:
			(*(void(__stdcall*)(int))(field_50 + 8))(1);
			break;
		}

		(*(void(__stdcall*)())(field_50 + 80))();
		field_54 = m_pDieselPower->stub23();
		m_pDirectSound = m_pDieselPower->GetDirectSound();
		m_Muted = false;

		debug("Sound Rendering System is '%s'\n", m_pDieselPower->GetSystemName());

		g_StreamedSoundBuffers = this;
	}

	void StreamedSoundBuffers::GetMaxDistance(Vector4f& outVec) const
	{
		if (m_nSoundSystem == SOUND_SYSTEM_DIESELPOWER)
			(*(void(__stdcall*)(Vector4f&))(field_54 + 16))(outVec);
		else
			m_pDirectSound3DBuffer->GetMaxDistance((D3DVALUE*)&outVec);
	}

	HRESULT StreamedSoundBuffers::CreateSoundBuffer(DSBUFFERDESC* bufferDesc)
	{
		m_pDirectSoundBuffer = nullptr;
		field_24 = NULL;
		field_28 = NULL;

		if (m_nSoundSystem != SOUND_SYSTEM_DIESELPOWER)
			return m_pDirectSound->CreateSoundBuffer(bufferDesc, &m_pDirectSoundBuffer, NULL);

		if (bufferDesc->dwFlags & DSBCAPS_CTRL3D)
			field_24 = field_28 = m_pDieselPower->stub21(bufferDesc->dwBufferBytes, bufferDesc->lpwfxFormat);
		else
			field_24 = m_pDieselPower->stub16(bufferDesc->dwBufferBytes, 11, &bufferDesc->lpwfxFormat);

		return S_OK;
	}

	void StreamedSoundBuffers::SetPrimarySoundFormat(int channels, int sampleRate, int bits)
	{
		WAVEFORMATEX waveFormat;

		waveFormat.nSamplesPerSec = 0;
		waveFormat.cbSize = NULL;
		waveFormat.wBitsPerSample = 8 * bits;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nChannels = channels;
		waveFormat.nAvgBytesPerSec = sampleRate * (channels * (bits & 0x1FFF));
		waveFormat.nBlockAlign = channels * (bits & 0x1FFF);
		waveFormat.nSamplesPerSec = sampleRate;

		m_pDirectSoundBuffer_1->SetFormat(&waveFormat);

		debug("Primary sound format set to %dHz, %dbit, %s \n", waveFormat.nSamplesPerSec, waveFormat.wBitsPerSample, channels == 2 ? "Stereo" : "Mono");

		m_nDirectSoundChannels = channels;
		m_nDirectSoundSampleRate = sampleRate;
		m_nDirectSoundBits = bits;
	}

	float StreamedSoundBuffers::GetDefaultVolumeForType(int type)
	{
		switch (type)
		{
		case 0:
			return DefaultFxVolume;
			break;
		case 1:
			return DefaultAmbienceVolume;
			break;
		case 2:
			return DefaultMusicVolume;
			break;
		case 3:
			return DefaultSpeaksVolume;
			break;
		default:
			return 1.0f;
			break;
		}
	}

	void StreamedSoundBuffers::SetGlobalPause(bool pause)
	{
		debug("global pause %s\n", pause ? "on" : "off");

		m_GlobalPause = pause;
		m_GlobalPauseCalled = true;
	}

	void StreamedSoundBuffers::_43D200(int unk1, int unk2, int unk3, int unk4, int unk5)
	{
		return;
	}

	void StreamedSoundBuffers::InitDirectSound(char channels, int sampleRate)
	{
		debug("Initializing Sound System.....\n");

		SetDefaultVolumeLevels();

		if (FAILED(DirectSoundCreate(NULL, &m_pDirectSound, NULL)))
			IncompatibleMachineParameterError(2, false);

		debug("DirectSound created successfully\n");

		DSCAPS dsCaps;
		memset(&dsCaps, NULL, sizeof(DSCAPS));
		dsCaps.dwSize = sizeof(DSCAPS);

		if (FAILED(m_pDirectSound->GetCaps(&dsCaps)))
			IncompatibleMachineParameterError(2, false);

		debug("%d HW mixing buffers (%d free)\n", dsCaps.dwMaxHwMixingStaticBuffers, dsCaps.dwFreeHwMixingStaticBuffers);

		int soundPriorityLevel = 1;
		if (FAILED(m_pDirectSound->SetCooperativeLevel(g_Window->m_hWindow, SoundPriorityLevels[2])))
		{
			for (; soundPriorityLevel < 5; soundPriorityLevel++)
				if (SUCCEEDED(m_pDirectSound->SetCooperativeLevel(g_Window->m_hWindow, SoundPriorityLevels[soundPriorityLevel])))
					break;

			if (soundPriorityLevel == 5)
			{
				IncompatibleMachineParameterError(2, false);
				m_pDirectSound->Release();
				m_pDirectSound = nullptr;

				return;
			}
		}

		debug("Sound priority level set to %d\n", soundPriorityLevel);

		DSBUFFERDESC dsBuffer;

		dsBuffer.dwSize = NULL;
		dsBuffer.dwReserved = NULL;
		dsBuffer.lpwfxFormat = NULL;
		dsBuffer.guid3DAlgorithm = { NULL, NULL, NULL, NULL };

		dsBuffer.dwSize = sizeof(DSBUFFERDESC);
		dsBuffer.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

		if (SUCCEEDED(m_pDirectSound->CreateSoundBuffer(&dsBuffer, &m_pDirectSoundBuffer_1, NULL)))
			SetPrimarySoundFormat((channels != NULL) + 1, sampleRate, 2);

		g_StreamedSoundBuffers = this;

		m_Muted = false;

		if (FAILED(m_pDirectSoundBuffer_1->QueryInterface(IID_IDirectSound, (LPVOID*)&m_pDirectSound3DBuffer)))
			IncompatibleMachineParameterError(2, false);

		m_pDirectSound3DBuffer->SetConeOutsideVolume(0, 0);
		m_pDirectSound3DBuffer->SetAllParameters(0, 0);

		debug("Sound Rendering System is 'DirectSound'\n");
	}

	void StreamedSoundBuffers::SetListener3DPos(const Vector4f& pos)
	{
		float dist = *(float*)0xA3DCC4 >= 0.0099999998f ? *(float*)0xA3DCC4 : 0.0099999998f;
		Vector4f maxDistanceVec;
		GetMaxDistance(maxDistanceVec);

		maxDistanceVec.x -= pos.x;
		maxDistanceVec.y -= pos.y;
		maxDistanceVec.z -= pos.z;
		maxDistanceVec.a -= pos.a;

		if (m_nSoundSystem == SOUND_SYSTEM_DIESELPOWER)
		{
			(*(void(__stdcall*)(float, float, float))field_54)(pos.x, pos.y, pos.z);
			(*(void(__stdcall*)(float, float, float))(field_54 + 8))(maxDistanceVec.x * (1.0f / dist), maxDistanceVec.y * (1.0f / dist), maxDistanceVec.z * (1.0f / dist));
			(*(void(__stdcall*)())(field_54 + 40))();
		}else{
			m_pDirectSound3DBuffer->SetConeOrientation(pos.x, pos.y, pos.z, 1);
			HRESULT sposres = m_pDirectSound3DBuffer->SetPosition(maxDistanceVec.x * (1.0f / dist), maxDistanceVec.y * (1.0f / dist), maxDistanceVec.z * (1.0f / dist), 1);
			if (FAILED(sposres))
				debug("Failed to set 3D listener position (%d)\n", sposres);
		}
	}

	void StreamedSoundBuffers::SetListener3DOrientation(const Orientation& orient)
	{
		//	TODO: hell no, too many calculations here. Will do later...
	}

	void StreamedSoundBuffers::GetListener3DOrientation(Orientation& outOrient)
	{
		if (m_nSoundSystem == SOUND_SYSTEM_DIESELPOWER)
			(*(void(__stdcall*)(Orientation&))(field_54 + 20))(outOrient);
		else
			if (FAILED(m_pDirectSound3DBuffer->GetConeOrientation((D3DVECTOR*)&outOrient)))
				debug("Failed to get 3D listener orientation\n");
	}

	void StreamedSoundBuffers::WaitForSoftPause()
	{
		//	TODO:	SoundBuffer class is not fully researched yet, so no implementation for now...
	}

	int StreamedSoundBuffers::_43E7B0()
	{
		int res = 0;
		for (int ind = 0; ind < m_DieselPowerSoundBuffers_1.m_nCurrIndex; ++ind)
			if ((*(bool(__stdcall*)(int))(m_DieselPowerSoundBuffers_1.m_pElements[ind] + 32))(0) &&
				!(*(bool(__stdcall*)(int))(m_DieselPowerSoundBuffers_1.m_pElements[ind] + 56))(0))
				++res;
		return res;
	}

	void StreamedSoundBuffers::MeasureWaitForSoftPause()
	{
		int startTime = Performance::GetMilliseconds();

		while (m_GlobalPauseCalled)
		{
			WaitForSoftPause();
			Sleep(10);
		}

		int interval = Performance::GetMilliseconds() - startTime;

		if (interval > 10)
			debug("WaitForSoftPause slept %dms\n", interval);
	}

	void StreamedSoundBuffers::_43E850()
	{
		for (int ind = 0; ind < m_DieselPowerSoundBuffers.m_nCurrIndex; ++ind)
			(*(void(__stdcall*)(int))(m_DieselPowerSoundBuffers.m_pElements[ind] + 40))(0);
	}

	void StreamedSoundBuffers::_43E880()
	{
		for (int ind = 0; ind < m_DieselPowerSoundBuffers_1.m_nCurrIndex; ++ind)
			(*(void(__stdcall*)())(m_DieselPowerSoundBuffers_1.m_pElements[ind] + 152))();
	}

	//	TODO: implementation!
	void StreamedSoundBuffers::PreallocateStreamBuffersPool()
	{

	}

	//	TODO: implementation!
	void StreamedSoundBuffers::CreateStaticStreamBuffer()
	{

	}
}