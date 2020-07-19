#pragma once

#include "stdafx.h"
#include "Types.h"

#define AUXQUADTREE_CLASS_SIZE 88

class AuxQuadTree
{
public:
	class Entity* m_pOwner;
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
	int* field_38;
	char field_3C;
	char field_3D;
	short field_3E;
	Vector4f m_vPosition;
	char field_4D;
	char m_bLod;
	char m_bLodFade;
	char m_pContactFilter;
	int m_nLodDistance;

public:
	AuxQuadTree(const class Entity* owner);	//	@89F430
	~AuxQuadTree()
	{
		MESSAGE_CLASS_DESTROYED(AuxQuadTree);
	}

	void	CalculateLodForAllChildren()	//	@8A3820
	{
		(*(void(__thiscall*)(AuxQuadTree*))0x8A3820)(this);
	}
	void	CalculateLod()		//	@8A24F0
	{
		(*(void(__thiscall*)(AuxQuadTree*))0x8A24F0)(this);
	}
	void	_8A3810(void* ptr);
};

static_assert(sizeof(AuxQuadTree) == AUXQUADTREE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(AuxQuadTree));