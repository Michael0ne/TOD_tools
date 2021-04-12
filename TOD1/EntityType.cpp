#include "EntityType.h"
#include "Blocks.h"
#include "Node.h"

EntityType::EntityType(const char* const entityname) : BaseType(TYPE_ENTITY, entityname, TYPE_ENTITY_SIZE)
{
	MESSAGE_CLASS_CREATED(EntityType);

	m_Creator = nullptr;
	m_Script = nullptr;
	m_Parent = nullptr;
	field_6C = NULL;
	m_HasParent = false;
	field_70 = 1;
}

EntityType::~EntityType()
{
	MESSAGE_CLASS_DESTROYED(EntityType);
}

void* EntityType::CreateNode() const
{
	Node* newnode = (Node*)m_Creator(g_Blocks->GetAllocatorType());
	newnode->SetScript(this);

	return (void*)newnode;
}

void EntityType::InheritFrom(EntityType* from)
{
	field_6C = from->m_HasParent ? from->m_Parent->m_PropertiesList.size() + from->m_Parent->field_6C : from->m_PropertiesList.size() + from->field_6C;
	field_70 = from->m_HasParent ? from->m_Parent->m_PropertiesList_1.size() + from->m_Parent->field_70 : from->m_PropertiesList_1.size() + from->field_70;
	m_Creator = from->m_Creator;
	m_Parent = from;
}