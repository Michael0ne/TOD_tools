#include "CollisionList.h"

#pragma message(TODO_IMPLEMENTATION)
void CollisionList::AddEntity(const Entity* pEnt)
{
	(*(void(__thiscall*)(CollisionList*, const Entity*))0x8A6410)(this, pEnt);
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionList::RemoveEntity(const Entity* pEnt)
{
	(*(void(__thiscall*)(CollisionList*, const Entity*))0x891DB0)(this, pEnt);
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionList::Init()
{
	(*(void(__thiscall*)(CollisionList*))0x891E10)(this);
}
