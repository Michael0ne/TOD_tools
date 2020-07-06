#include "Entity.h"
#include "Blocks.h"

Entity::Entity()
{
	MESSAGE_CLASS_CREATED(Entity);

	m_ScriptEntity = nullptr;
	field_1C = field_20 = field_20 = field_18 = field_14 = 0;
	field_14 = field_14 | 255;
	m_FragmentPath = nullptr;

	field_14 = (g_Blocks->AddEntity(this) << 8) | field_14 & 255;
}