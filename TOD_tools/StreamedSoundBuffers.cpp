#include "StreamedSoundBuffers.h"
#include "Performance.h"
#include "Window.h"

namespace Audio
{
	StreamedSoundBuffers* g_StreamedSoundBuffers = nullptr;
	int& StreamedSoundBuffers::SoundRendererId = *(int*)0xA35ECC;
	HANDLE& StreamedSoundBuffers::SemaphoreObject = *(HANDLE*)0xA35EC8;

	float& StreamedSoundBuffers::DefaultFxVolume = *(float*)0xA09230;
	float& StreamedSoundBuffers::DefaultAmbienceVolume = *(float*)0xA09234;
	float& StreamedSoundBuffers::DefaultMusicVolume = *(float*)0xA09238;
	float& StreamedSoundBuffers::DefaultSpeaksVolume = *(float*)0xA0923C;

	void StreamedSoundBuffers::Dump() const
	{
		void(__thiscall * SoundManager__DumpStreamedSoundBuffers)(const StreamedSoundBuffers * _this) = (void(__thiscall*)(const StreamedSoundBuffers*))0x43EAD0;

		SoundManager__DumpStreamedSoundBuffers(this);
	}

	void StreamedSoundBuffers::SetDirectSoundBufferFormat(int nChannels, int nSampleRate, int nBits)
	{
		WAVEFORMATEX	waveFormat;

		waveFormat.nSamplesPerSec = 0;
		waveFormat.cbSize = 0;
		waveFormat.wBitsPerSample = 8 * nBits;
		waveFormat.wFormatTag = 1;
		waveFormat.nChannels = nChannels;
		waveFormat.nAvgBytesPerSec = nSampleRate * (nChannels * (nBits & 0x1FFF));
		waveFormat.nBlockAlign = nChannels * (nBits & 0x1FFF);
		waveFormat.nSamplesPerSec = nSampleRate;

		m_pDirectSoundBuffer->SetFormat(&waveFormat);

		debug("Primary sound format set to %dHz, %dbit, %s \n", waveFormat.nSamplesPerSec, waveFormat.wBitsPerSample, nChannels == 2 ? "Stereo" : "Mono");

		m_nDirectSoundChannels = nChannels;
		m_nDirectSoundSampleRate = nSampleRate;
		m_nDirectSoundBits = nBits;
	}

	void StreamedSoundBuffers::SetGlobalPause(bool bPause)
	{
		debug("global pause %s", bPause ? "on" : "off");

		m_bGlobalPause = bPause;
		m_bGlobalPauseCalled = true;
	}

	int StreamedSoundBuffers::GetSoundRenderer()
	{
		if (g_StreamedSoundBuffers->SoundRendererId)
			return g_StreamedSoundBuffers->SoundRendererId;

		g_StreamedSoundBuffers->SoundRendererId = 1;

		HKEY phkResult;

		if (!RegOpenKeyExA(HKEY_CURRENT_USER, RegistryKey, 0, 1, &phkResult)) {
			DWORD cbData = 4;
			DWORD cbType;
			LPBYTE regData = 0;

			if (!RegQueryValueExA(phkResult, "SoundRenderer", 0, &cbType, regData, &cbData) && cbType == 4)
				g_StreamedSoundBuffers->SoundRendererId = *(int*)regData;
			RegCloseKey(phkResult);
		}

		return g_StreamedSoundBuffers->SoundRendererId;
	}

	void StreamedSoundBuffers::InitiDirectSound(char nChannels, int nSampleRate)
	{
		debug("Initializing Sound System.....\n");

		if (m_nSoundSystem == DIESELPOWER) {
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

		if (DirectSoundCreate(NULL, &m_pDirectSound, NULL) < 0)
			IncompatibleMachineParameterError(2, 0);

		debug("DirectSound created successfully\n");

		DSCAPS	dsoundCaps;
		memset(&dsoundCaps, 0, sizeof(DSCAPS));

		dsoundCaps.dwSize = 96;
		if (m_pDirectSound->GetCaps(&dsoundCaps) < 0)
			IncompatibleMachineParameterError(2, 0);

		debug("%d HW mixing buffers (%d free)\n", dsoundCaps.dwMaxHwMixingStaticBuffers, dsoundCaps.dwFreeHwMixingStaticBuffers);

		if (m_pDirectSound->SetCooperativeLevel(g_Window->m_hWindow, 2) < 0) {
			//	TODO: implementation!
		}
	}

	void StreamedSoundBuffers::MeasureWaitForSoftPause()
	{
		long startTime = Performance::GetMilliseconds();

		while (m_bGlobalPauseCalled) {
			WaitForSoftPause();
			Sleep(10);
		}

		long endTime = Performance::GetMilliseconds();

		if (endTime > 10)
			debug("WaitForSoftPause slept %dms\n", endTime);
	}

	void StreamedSoundBuffers::SelectAudioRenderer(int nChannels, int nSampleRate)
	{
		void(__thiscall * _SelectAudioRend)(StreamedSoundBuffers * _this, int _chann, int _samplrat) = (void(__thiscall*)(StreamedSoundBuffers*, int, int))0x43E080;

		_SelectAudioRend(this, nChannels, nSampleRate);
	}

	void StreamedSoundBuffers::WaitForSoftPause()
	{
		(*(void(__thiscall*)(StreamedSoundBuffers*))0x43E640)(this);
	}

	void StreamedSoundBuffers::RememberSoundRenderer(signed int id)
	{
		SoundRendererId = id;

		LPDWORD dwDisposition = 0;

		if (!RegCreateKeyExA(HKEY_CURRENT_USER, RegistryKey, 0, 0, 0, 0xF003F, 0, (PHKEY)&id, dwDisposition)) {
			RegSetValueExA((HKEY)id, "SoundRenderer", 0, 4, (const unsigned char*)&id, 4);
			RegCloseKey((HKEY)id);
		}

		debug("StreamedSoundBuffers RememberSoundRenderer returned %i\n", *dwDisposition);
	}
}

//	Apply patches specific to this class.
inline void PATCH_SOUND_MANAGER()
{
}