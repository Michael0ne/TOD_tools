#include "SoundSlot.h"

SoundSlot::SoundSlot() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(SoundSlot);

	m_SoundResource = nullptr;
	field_64 = 1;
	m_Sound = String();
	m_StreamingSoundFullpathDirect = nullptr;
	field_7C = 1;

	m_List_1 = List<int>(0x1A300);

	m_Flags = m_Flags & 0xFF230111 | 0x1020111;
	m_Volume = 10000;
	m_Randompitch_Length = 0;
	m_MinRange = 1.0f;
	m_MaxRange = 100.0f;
	m_RollOff = 1.0f;
	m_Pitch = 10000;
	m_Randomvolume_Length = 0;
	m_StreamBuffer = nullptr;
	m_StreamingSoundName = nullptr;
	m_DopplerFactor = -1;
	field_94 = 0;
	m_LfeLevel = 0;
	field_8C = -1;
	
	//	TODO: initialize more lists here.
}