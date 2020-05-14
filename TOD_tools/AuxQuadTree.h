#pragma once

#include "stdafx.h"
#include "Entity.h"

#define AUXQUADTREE_CLASS_SIZE 88

class AuxQuadTree
{
public:
	Entity* m_pOwner;
	int field_4;
	AuxQuadTree* m_pChildLast;
	AuxQuadTree* m_pChildFirst;
	int field_10;
	int field_14;
	int field_18;
	int m_nUserType;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int* m_pContactNode;
	int field_38;
	union {
		char flag0 : 1;
		char flag1 : 1;
		char flag2 : 1;
		char flag3 : 1;
	} m_nFlags;
	//int m_nFlags;
	Vector4f m_vPosition;
	byte field_4D;
	byte m_bLod;
	byte m_bLodFade;
	byte m_pContactFilter;
	int m_nLodDistance;

public:
	void	CalculateLodForAllChildren()	//	@8A3820
	{
		(*(void(__thiscall*)(AuxQuadTree*))0x8A3820)(this);
	}

	void	CalculateLod()		//	@8A24F0
	{
		(*(void(__thiscall*)(AuxQuadTree*))0x8A24F0)(this);
	}

	AuxQuadTree(const Entity* owner);	//	@89F430
};

static_assert(sizeof(AuxQuadTree) == AUXQUADTREE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("AuxQuadTree"));