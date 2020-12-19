#include "Entity.h"
#include "Blocks.h"
#include "Globals.h"

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

Entity::~Entity()
{
	MESSAGE_CLASS_DESTROYED(Entity);

	if (this)
		scalar_destructor(true);
}

int Entity::GetPropertyId(const char* prop)
{
	char _prop[64];
	memset(&_prop, NULL, sizeof(_prop));

	//	NOTE: copy 'prop' to local buffer and cut out everything after ':', after that turn to lower case.
	if (strstr(prop, ":") != nullptr)
		strncpy_s(_prop, prop, strstr(prop, ":") - prop);
	else
		strncpy_s(_prop, prop, sizeof(_prop));

	Utils::ToLowercase(_prop);

	int* msg = GetMessageId_A(PropertiesList, PropertiesList.field_8, _prop);

	if (msg && (msg + 4) != NULL)
		return *(msg + 4);
	else
		return -1;
}