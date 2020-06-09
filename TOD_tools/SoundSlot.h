#pragma once

#include "stdafx.h"
#include "Entity.h"

#define SOUNDSLOT_CLASS_SIZE 176

class SoundSlot : Entity
{
private:
	int m_nMode;
	int field_7C;
	int* field_80;
	__int16 m_fVolume;
	__int16 field_83;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	unsigned int m_nFlags;
	float m_fDopplerFactor;
};

static_assert(sizeof(SoundSlot) == SOUNDSLOT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SoundSlot));