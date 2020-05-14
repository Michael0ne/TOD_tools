#include "StreamedSoundBuffers.h"
#include "Performance.h"

namespace Audio
{

	StreamedSoundBuffers* g_StreamedSoundBuffers = NULL;
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

	void StreamedSoundBuffers::SetGlobalPause(bool bPause)
	{
		if (!bPause)
			debug("global pause off\n");
		else
			debug("global pause on\n");

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
			LPBYTE regData;

			if (!RegQueryValueExA(phkResult, "SoundRenderer", 0, &cbType, regData, &cbData) && cbType == 4)
				g_StreamedSoundBuffers->SoundRendererId = *(int*)regData;
			RegCloseKey(phkResult);
		}

		return g_StreamedSoundBuffers->SoundRendererId;
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