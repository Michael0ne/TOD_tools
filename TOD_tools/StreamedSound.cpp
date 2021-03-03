#include "StreamedSound.h"
#include "LogDump.h"

void StreamedSound::Stop(int)
{
	field_54 = field_54 & 0xFF00FFFF | (((unsigned short)field_54 + 1) << 16) & 0xFF0000;
	m_Flags.m_FlagBits.StopRequest = m_Flags.m_FlagBits._4 = true;

	if (m_StreamedWAV)
	{
		m_StoppingFinished = !m_DirectSoundBuffer && !m_DieselPowerSoundBuffer;

		if (field_A0)
			ResetEvent(field_A0);
		if (field_B0)
			SetEvent(field_B0);
		if (m_EventNotify)
			ResetEvent(m_EventNotify);
		if (m_EvHandle_2)
			ResetEvent(m_EvHandle_2);

		m_Flags.m_FlagBits.LastChunkPlaying = false;
		field_5C = NULL;
	}
}

void StreamedSound::_443E00()
{
	Stop(0);
}