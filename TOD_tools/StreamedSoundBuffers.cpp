#include "StreamedSoundBuffers.h"

StreamedSoundBuffers* g_StreamedSoundBuffers = NULL;

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

//	Apply patches specific to this class.
inline void PATCH_SOUND_MANAGER()
{
}