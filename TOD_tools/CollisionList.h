#pragma once

#include "Types.h"

class Entity;
class CollisionProbe;

class CollisionList
{
public:
	Entity *m_pOwner;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	Vector4f m_vPosition;
	int field_30;
	int field_34;
	Vector4f m_vSafePos;
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	CollisionProbe **m_pCollisionProbe;
	int m_nSize;
	int field_70;
	int field_74;

public:
	void	AddEntity(const Entity* pEnt);	//	@8A6410
	void	RemoveEntity(const Entity* pEnt);	//	@891DB0
	void	Init();	//	@891E10
};

