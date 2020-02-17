#pragma once

#include "stdafx.h"

class Renderer
{
private:
		int field_0;
		int field_4;
		int field_8;
		int field_c;
		__int16 field_10;
		__int16 field_12;
		int field_14;
		int field_18;
		int m_nUnkStatesCount;
		int field_20;
		int* field_24;
		float m_fTimeDelta;
		int m_nTimeMilliseconds;
		int field_30;
		int field_34;
		int field_38;
		int field_3c;
		__int64 m_nUnkTime;
};

static_assert(sizeof(Renderer) == 0x48, MESSAGE_WRONG_CLASS_SIZE("Renderer"));