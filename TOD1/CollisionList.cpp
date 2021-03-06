#include "CollisionList.h"

#pragma message(TODO_IMPLEMENTATION)
CollisionList::CollisionList(const Entity* owner)
{
	MESSAGE_CLASS_CREATED(CollisionList);

	m_Owner = (Entity*)owner;
	m_List_1 = List<int>(0x1A300);
	m_CollisionProbesList = List<CollisionProbe>(0x1A300);
	field_78 = 0x80002710;

	//tScene->AddCollisionListToList(this);

	m_Position_2 = Vector3f();
	m_Position_3 = Vector3f();
	m_SafePos = Vector4f(0.f, 0.f, 0.f, 1.f);
	m_Unknown_1 = Vector4f(0.f, 0.f, 0.f, 1.0f);
}

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
