#include "CollisionList.h"
#include "Entity.h"

#pragma message(TODO_IMPLEMENTATION)
CollisionList::CollisionList(Entity* owner)
{
 MESSAGE_CLASS_CREATED(CollisionList);

 m_Owner = owner;
 field_78 = 0x80002710;

 //tScene->AddCollisionListToList(this);

 m_Position_2 = {};
 m_Position_3 = {};
 m_SafePos = Vector4f(0.f, 0.f, 0.f, 1.f);
 m_Unknown_1 = Vector4f(0.f, 0.f, 0.f, 1.0f);
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionList::AddEntity(Entity* pEnt)
{
 (*(void(__thiscall*)(CollisionList*, Entity*))0x8A6410)(this, pEnt);
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionList::RemoveEntity(Entity* pEnt)
{
 (*(void(__thiscall*)(CollisionList*, Entity*))0x891DB0)(this, pEnt);
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionList::Init()
{
 (*(void(__thiscall*)(CollisionList*))0x891E10)(this);
}
