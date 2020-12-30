#include "Entity.h"
#include "Blocks.h"
#include "Globals.h"
#include "MemoryCards.h"

void Entity::Destroy()
{
	if (this)
		delete this;
}

#pragma message(TODO_IMPLEMENTATION)
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

#pragma message(TODO_IMPLEMENTATION)
unsigned char Entity::SaveScriptDataToFile_Impl(ScriptTypes::ScriptType_Entity*, int, int, const char*)
{
	return NULL;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned char Entity::LoadScriptDataFromFile_Impl(ScriptTypes::ScriptType_Entity*, int, int)
{
	return NULL;
}

#pragma message(TODO_IMPLEMENTATION)
Entity::~Entity()
{
	MESSAGE_CLASS_DESTROYED(Entity);

	if ((int*)field_20 && ((int*)field_20)[1])
	{
		(*(void(__stdcall**)(int))(*(int*)field_1C + 16))((int)field_20 - *(int*)(field_1C + 16) / 12);
		field_20 = NULL;
	}

	//if (field_18)
		//m_ScriptEntity->m_ParentNode->_489C90();

	//*(int*)(*((int*)&g_Blocks->m_UnkList_3.m_Elements + 4 * ((m_Id >> 28) & 7) - 1) + 4 * ((m_Id >> 8) & 0xFF8FFFFF)) = NULL;

	//if (g_Blocks->field_1B0[((m_Id >> 28) & 7) - 1] > ((m_Id >> 8) & 0xFF8FFFFF))
		//g_Blocks->field_1B0[((m_Id >> 28) & 7) - 1] = ((m_Id >> 8) & 0xFF8FFFFF);
}

int Entity::GetId()
{
	return m_Id >> 8;
}

int Entity::GetScriptPriority()
{
	if (field_20 &&
		field_20->m_ScriptEntity)
		return NULL;
	else
		return NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void Entity::SetScriptPriority()
{
}

void Entity::SaveScriptDataToFile(int* params)
{
	if (!params[1] || ((MemoryCards*)params[1])->m_ScriptEntity == nullptr)
	{
		*params = NULL;
		return;
	}

	ScriptTypes::ScriptType_Entity* memcard = (ScriptTypes::ScriptType_Entity*)(((MemoryCards*)params[1])->m_ScriptEntity);

	while (tMemoryCards != memcard)
	{
		if (!(memcard = memcard->m_Parent))
		{
			*params = NULL;
			return;
		}
	}

	*params = SaveScriptDataToFile_Impl(memcard, params[2], params[3], (const char*)params[4]) >= NULL;
}

void Entity::LoadScriptDataFromFile(int* params)
{
	if (!params[1] || ((MemoryCards*)params[1])->m_ScriptEntity == nullptr)
	{
		*params = NULL;
		return;
	}

	ScriptTypes::ScriptType_Entity* memcard = (ScriptTypes::ScriptType_Entity*)(((MemoryCards*)params[1])->m_ScriptEntity);

	while (tMemoryCards != memcard)
	{
		if (!(memcard = memcard->m_Parent))
		{
			*params = NULL;
			return;
		}
	}

	*params = LoadScriptDataFromFile_Impl(memcard, params[2], params[3]);
}

#pragma message(TODO_IMPLEMENTATION)
void Entity::SetScript(ScriptTypes::ScriptType_Entity* script)
{
	if (field_18)
		if (m_ScriptEntity)
			//m_ScriptEntity->m_ParentNode->_489C90(this);
			;

	m_ScriptEntity = script;

	if (script->m_ParentNode)
		//script->m_ParentNode->_489BE0(this);
		;
}

#pragma message(TODO_IMPLEMENTATION)
int Entity::GetPropertyId(const char* prop)
{
	char _prop[64];
	memset(&_prop, NULL, sizeof(_prop));

	//	NOTE: copy 'prop' to local buffer and cut out everything after ':', after that turn to lower case.
	if (strstr(prop, ":") != nullptr)
		strncpy_s(_prop, prop, strstr(prop, ":") - prop);
	else
		strncpy_s(_prop, prop, sizeof(_prop));

	String::ToLowerCase(_prop);

	int* msg = nullptr;	//	GetMessageId_A(PropertiesList, PropertiesList.field_8, _prop);

	if (msg && (msg + 4) != NULL)
		return *(msg + 4);
	else
		return -1;
}