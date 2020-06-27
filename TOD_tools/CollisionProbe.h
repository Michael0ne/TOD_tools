#pragma once

#include "stdafx.h"

#include "Entity.h"

#define COLLISIONPROBE_CLASS_SIZE 292

class CollisionProbe
{
public:
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
	int field_9C;
	int field_A0;
	int field_A4;
	int field_A8;
	int field_AC;
	float m_fB0;
	int field_B4;
	int field_B8;
	int field_BC;
	int field_C0;
	int field_C4;
	int field_C8;
	int field_CC;
	int* field_D0;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	char field_FC[4];
	Vector4f m_vUnkVec1;
	int field_110;
	int field_114;
	int field_118;
	int field_11C;
	int field_120;

public:
	static List<CollisionProbe>&		ms_CollisionProbesList;	//	@A3DD4C
};

static_assert(sizeof(CollisionProbe) == COLLISIONPROBE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(CollisionProbe));