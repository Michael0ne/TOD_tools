#pragma once
#include "MemoryManager.h"
#include "Types.h"
#include <vector>

#define COLLISIONLIST_CLASS_SIZE 124

class CollisionList
{
public:
 class Entity*    m_Owner;
 Vector3f     m_Position_1;
 Orientation     m_Orientation;
 Vector3f     m_Position_2;
 Vector3f     m_Position_3;
 Vector4f     m_SafePos;
 Vector4f     m_Unknown_1;
 std::vector<int>   m_List_1;
 std::vector<class CollisionProbe*> m_CollisionProbesList;
 int       field_78;

public:
 CollisionList(class Entity* owner); // @8A2D00

 void      AddEntity(class Entity* pEnt); // @8A6410
 void      RemoveEntity(class Entity* pEnt); // @891DB0
 void      Init(); // @891E10
};

ASSERT_CLASS_SIZE(CollisionList, 124);