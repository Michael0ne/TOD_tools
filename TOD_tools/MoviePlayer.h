#pragma once

#include "stdafx.h"
#include "Types.h"

#include "Entity.h"

struct BINK;

#define MOVIEPLAYER_CLASS_SIZE 192

class MoviePlayer
{
private:
	Entity entity;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	float m_f9C;
	float m_fA0;
	int field_A4;
	float m_fA8;
	int field_AC;
	Entity* m_pSubtitleNode;
	int field_B4;
	int field_B8;
	int field_BC;
public:
	static bool& MoviePlayerAllocated;	//	@A35ED4
	static BINK& BinkHandle;	//	@A35ED8
};

extern MoviePlayer* g_MoviePlayer;

static_assert(sizeof(MoviePlayer) == MOVIEPLAYER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("MoviePlayer"));