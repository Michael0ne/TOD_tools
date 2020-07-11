#pragma once

#include "stdafx.h"
#include "Types.h"

class Entity;
class CollisionProbe;

#define COLLISIONLIST_CLASS_SIZE 124

class CollisionList
{
public:
	Entity *m_pOwner;
	Vector3f m_vPosition_1;
	Orientation m_vOrientation;
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
	int field_78;

public:
	void	AddEntity(const Entity* pEnt);	//	@8A6410
	void	RemoveEntity(const Entity* pEnt);	//	@891DB0
	void	Init();	//	@891E10
};

static_assert(sizeof(CollisionList) == COLLISIONLIST_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(CollisionList));