#include "SoundManager.h"

void SoundManager::Dump()
{
	SoundManager__DumpStreamedSoundBuffers((DWORD*)this);
}

//	Apply patches specific to this class.
inline void PATCH_SOUND_MANAGER()
{
}