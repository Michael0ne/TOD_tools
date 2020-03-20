#pragma once

#include "Entity.h"
#include "GfxInternal_Dx9_Texture.h"
#include "List.h"

class Progress;

struct Progress__vtable {
	void(__thiscall* Destroy)(Progress* _this);
	void(__thiscall* stub1)(Progress* _this);
	void(__thiscall* UpdateProgress)(Progress* _this, int time, signed int);
};

class Progress
{
private:
	Progress__vtable* lpVtbl;
	int field_4;
	List<int> m_unkList;				//	Don't really know what list it is.
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	__int64 m_nUnkTimeStart_1;
	int m_nUnkTimeStart;
	int m_nUnkTimeEnd;
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	Entity* m_pLoadScreenSprite;
	GfxInternal_Dx9_Texture* m_pTexture;
	int* m_p60;
	int* m_p64;
	byte m_bEnabled;
	byte field_69[3];
	int field_6C;
};

extern Progress* g_pProgress;

static_assert(sizeof(Progress) == 0x70, MESSAGE_WRONG_CLASS_SIZE("Progress"));