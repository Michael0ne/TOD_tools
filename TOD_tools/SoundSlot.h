#pragma once

#include "stdafx.h"
#include "Entity.h"

#define SOUNDSLOT_CLASS_SIZE 176

class StreamBuffer;

class SoundSlot
{
private:
	void* lpVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	float m_f50;
	float m_f54;
	float m_f58;
	int field_5C;
	int m_pSound;
	int field_64;
	String m_sSoundName;
	int m_nMode;
	int field_7C;
	StreamBuffer* m_pStreamBuffer;
	__int16 m_fVolume;
	__int16 field_83;
	int field_88;
	int field_8C;
	__int16 field_90;
	__int16 field_92;
	Entity* m_pEntity;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	unsigned int m_nFlags;
	char field_AC;
	char field_AD;
	__int16 field_AE;

public:
	void		Play(const Vector4f& position);	//	@89D970
};

static_assert(sizeof(SoundSlot) == SOUNDSLOT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SoundSlot));