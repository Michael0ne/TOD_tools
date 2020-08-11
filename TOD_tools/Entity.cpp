#include "Entity.h"
#include "Blocks.h"

Entity::Entity()
{
	MESSAGE_CLASS_CREATED(Entity);

	m_ScriptEntity = nullptr;
	field_1C = field_18 = 0;
	field_20 = nullptr;
	m_Id = 0;
	m_Id = m_Id | 255;
	m_FragmentPath = nullptr;

	m_Id = (g_Blocks->AddEntity(this) << 8) | m_Id & 255;
}