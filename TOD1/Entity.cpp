#include "Entity.h"
#include "Blocks.h"
#include "Globals.h"
#include "MemoryCards.h"

ScriptType_Entity* tEntity = nullptr;

void Entity::Destroy()
{
	if (this)
		delete this;
}

#pragma message(TODO_IMPLEMENTATION)
Entity::~Entity()
{
	MESSAGE_CLASS_DESTROYED(Entity);
}

Entity::Entity()
{
	MESSAGE_CLASS_CREATED(Entity);

	field_1C = NULL;
	field_20 = nullptr;
	m_Id = m_Id | 255;
	m_ScriptEntity = nullptr;
	m_Parameters = nullptr;
	field_20 = nullptr;

	memset(field_8, NULL, sizeof(field_8));

	m_Id = (g_Blocks->AddEntity(this) << 8) | m_Id;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned char Entity::SaveScriptDataToFile_Impl(ScriptType_Entity*, int, int, const char*)
{
	return NULL;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned char Entity::LoadScriptDataFromFile_Impl(ScriptType_Entity*, int, int)
{
	return NULL;
}

int Entity::GetId() const
{
	return m_Id >> 8;
}

#pragma message(TODO_IMPLEMENTATION)
int Entity::GetScriptPriority() const
{
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

	ScriptType_Entity* memcard = (ScriptType_Entity*)(((MemoryCards*)params[1])->m_ScriptEntity);

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

	ScriptType_Entity* memcard = (ScriptType_Entity*)(((MemoryCards*)params[1])->m_ScriptEntity);

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
void Entity::SetScript(ScriptType_Entity* script)
{
	//if (field_18)
		//if (m_ScriptEntity)
			//m_ScriptEntity->m_ParentNode->_489C90(this);

	//m_ScriptEntity = script;

	//if (script->m_ParentNode)
		//script->m_ParentNode->_489BE0(this);
}

#pragma message(TODO_IMPLEMENTATION)
void Entity::Register()
{
	tEntity = new ScriptType_Entity("Entity");
	tEntity->SetCreator((ScriptType_Entity::CREATOR)Create);

	//tEntity->RegisterScript("getpropertyid(string):integer", GetPropertyId, NULL, NULL, NULL, NULL, "GetPropertyIDMSG");
	//tEntity->RegisterScript("hasproperty(string):truth", HasProperty, NULL, NULL, NULL, NULL, "HasPropertyMSG");
	//tEntity->RegisterScript("savescriptdatatofile(entity,integer,integer,string):truth", SaveScriptDataToFile, NULL, NULL, NULL, NULL, NULL);
	//tEntity->RegisterScript("loadscriptdatafromfile(entity,integer,integer):integer", LoadScriptDataFromFile, NULL, NULL, NULL, NULL, NULL);
	
	//tEntity->RegisterProperty("id", tINTEGER, INT_GETTER(Entity::GetId), NULL, NULL, NULL, INT_SETTER(nullptr), NULL, NULL, -1, "control=string");
	//tEntity->RegisterProperty("script_priority", tINTEGER, INT_GETTER(Entity::GetScriptPriority), NULL, NULL, NULL, INT_SETTER(Entity::SetScriptPriority), NULL, NULL, NULL, "control=string");

	//tEntity->_86E9B0();
}

Entity* Entity::Create(AllocatorIndex)
{
	return new Entity();
}