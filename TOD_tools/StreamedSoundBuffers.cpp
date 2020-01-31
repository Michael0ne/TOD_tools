#include "StreamedSoundBuffers.h"

void StreamedSoundBuffers::Dump() const
{
	SoundManager__DumpStreamedSoundBuffers(this);
}

//	Apply patches specific to this class.
inline void PATCH_SOUND_MANAGER()
{
}