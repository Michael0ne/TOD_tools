#pragma once

#include "stdafx.h"

struct Scene_Buffer92
{
	int field_0;
	int field_4;
	int field_8;
	Scene_Buffer92* m_pBuffer;
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
	int field_50;
	int field_54;
	int field_58;
};

class Renderer
{
private:
	char field_0[4];
	int field_4;
	int field_8;
	int field_c;
	__int16 field_10;
	__int16 field_12;
	int field_14;
	int* field_18;
	int m_nUnkStatesCount;
	int field_20;
	Scene_Buffer92* m_pUnkBuffersArray;
	float m_fTimeDelta;
	int m_nTimeMilliseconds;
	int field_30;
	int field_34;
	__int64 m_nUnkTime_1;
	__int64 m_nUnkTime_2;
};

static_assert(sizeof(Renderer) == 0x48, MESSAGE_WRONG_CLASS_SIZE("Renderer"));