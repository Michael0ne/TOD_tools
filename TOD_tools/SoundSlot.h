#pragma once

#include "Node.h"

#define SOUNDSLOT_CLASS_SIZE 176

class SoundSlot : public Node
{
protected:
	float m_MinRange;
	float m_MaxRange;
	float m_RollOff;
	char* m_StreamingSoundName;
	int* m_SoundResource;
	int field_64;
	String m_Sound;
	int* m_StreamingSoundFullpathDirect;	//	NOTE: what is this type?
	int field_7C;
	class StreamBuffer* m_StreamBuffer;
	__int16 m_Volume;
	__int16 m_Pitch;
	int field_88;
	int field_8C;
	__int16 m_Randompitch_Length;
	__int16 m_Randomvolume_Length;
	Node* field_94;
	List<int> m_List_1;
	unsigned int m_Flags;
	char m_DopplerFactor;
	char m_LfeLevel;

public:
	SoundSlot();	//	@89D590
};

static_assert(sizeof(SoundSlot) == SOUNDSLOT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SoundSlot));