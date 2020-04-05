#pragma once

#include "stdafx.h"
#include "Entity.h"

class AuxQuadTree
{
	Entity entity;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int m_nUserType;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int *m_pContactNode;
	int field_38;
	int m_nFlags;
	Vector4f m_vPosition;
	byte field_4D;
	byte m_bLod;
	byte m_bLodFade;
	byte m_pContactFilter;
	int m_nLodDistance;
public:
};