#pragma once

#include "Types.h"
#include "List.h"

#define COLLISIONLIST_CLASS_SIZE 124

class Entity;

class CollisionList
{
public:
	Entity*						m_Owner;
	Vector3f					m_Position_1;
	Orientation					m_Orientation;
	Vector3f					m_Position_2;
	Vector3f					m_Position_3;
	Vector4f					m_SafePos;
	Vector4f					m_Unknown_1;
	List<int>					m_List_1;
	List<class CollisionProbe>	m_CollisionProbesList;
	int							field_78;

public:
	CollisionList(const Entity* owner);	//	@8A2D00

	void						AddEntity(const Entity* pEnt);	//	@8A6410
	void						RemoveEntity(const Entity* pEnt);	//	@891DB0
	void						Init();	//	@891E10
};

static_assert(sizeof(CollisionList) == COLLISIONLIST_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(CollisionList));