#pragma once

#include "Node.h"

struct BINK;

#define MOVIE_PLAYER_CLASS_SIZE 192

struct MoviePlayer_Struct72
{
	float m_f0;
	float m_f4;
	int field_8;
	int field_C;
	float m_f10;
	String m_MovieName;
	char field_24;
	char field_25;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	IDirect3DDevice9* m_Direct3DDevice9;
	int* field_44;

	MoviePlayer_Struct72();	//	@442F00
};

class MoviePlayer : public Node
{
protected:
	MoviePlayer_Struct72 field_50;
	float m_Volume;
	float m_ScaleX;
	float m_ScaleY;
	__int16 m_DisplacementX;
	__int16 m_DisplacementY;
	float m_Opacity;
	int field_AC;
	int field_B0;
	int field_B4;
	int field_B8;
	int field_BC;

public:
	MoviePlayer() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(MoviePlayer);

		field_B8 = -1;
		field_B4 = -1;
		field_BC = field_BC & 0xFFFFFFFD;
		m_DisplacementX = 0;
		m_ScaleX = m_ScaleY = 1.0f;
		m_DisplacementY = 0;
		m_Opacity = 1.0f;
		field_AC = field_B0 = 0;
		m_Volume = 0.0f;
	}

	static String&	MovieName;	//	@A3D864
};

static_assert(sizeof(MoviePlayer) == MOVIE_PLAYER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(MoviePlayer));