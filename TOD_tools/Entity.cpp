#include "Entity.h"
#include "Blocks.h"

void Entity::scalar_destructor(bool freeMemory)
{
	if (freeMemory)
		Allocators::ReleaseMemory(this, false);
}

Entity::Entity()
{
	MESSAGE_CLASS_CREATED(Entity);

	m_ScriptEntity = nullptr;
	field_1C = NULL;
	field_18 = nullptr;
	field_20 = nullptr;
	m_Id = 0;
	m_Id = m_Id | 255;
	m_FragmentPath = nullptr;

	m_Id = (g_Blocks->AddEntity(this) << 8) | m_Id & 255;
}