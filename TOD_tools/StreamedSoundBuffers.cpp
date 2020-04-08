#include "StreamedSoundBuffers.h"

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
		void(__thiscall * SoundManager__SetGlobalPause)(StreamedSoundBuffers * _this, bool _pause) = (void(__thiscall*)(StreamedSoundBuffers*, bool))0x43D1D0;

		SoundManager__SetGlobalPause(this, bPause);
	}

	void StreamedSoundBuffers::MeasureWaitForSoftPause()
	{
		void(__thiscall * SoundManager__MeasureWaitForSoftPause)(StreamedSoundBuffers * _this) = (void(__thiscall*)(StreamedSoundBuffers*))0x43E800;

		SoundManager__MeasureWaitForSoftPause(this);
	}

	void StreamedSoundBuffers::SelectAudioRenderer(int nChannels, int nSampleRate)
	{
		void(__thiscall * _SelectAudioRend)(StreamedSoundBuffers * _this, int _chann, int _samplrat) = (void(__thiscall*)(StreamedSoundBuffers*, int, int))0x43E080;

		_SelectAudioRend(this, nChannels, nSampleRate);
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