#include "EntityType.h"
#include "AssetManager.h"
#include "Node.h"

EntityType::EntityType(const char* const entityname) : DataType(TYPE_ENTITY, entityname, TYPE_ENTITY_SIZE)
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
    Node* newnode = (Node*)m_Creator(g_AssetManager->GetAllocatorType());
    newnode->SetScript(this);

    return (void*)newnode;
}

void EntityType::InheritFrom(const EntityType* from)
{
    field_6C = from->m_HasParent ? from->m_Parent->m_PropertiesList.size() + from->m_Parent->field_6C : from->m_PropertiesList.size() + from->field_6C;
    field_70 = from->m_HasParent ? from->m_Parent->m_PropertiesList_1.size() + from->m_Parent->field_70 : from->m_PropertiesList_1.size() + from->field_70;
    m_Creator = from->m_Creator;
    m_Parent = (EntityType*)from;
}

#pragma message(TODO_IMPLEMENTATION)
void EntityType::_86E9B0()
{
    if (!m_Parent ||
        m_PropertiesList.size() ||
        m_PropertiesList_1.size() ||
        m_ScriptsList.size() ||
        m_Parent->m_HasParent)
    {
        //  TODO: copy properties list from parent?
    }
    else
    {
        m_HasParent = true;
    }
}

Entity* EntityType::IsParentOf(EntityType* ett, Entity* ent)
{
    if (!ent)
        return nullptr;

    if (!ent->m_ScriptEntity)
        return nullptr;

    EntityType* entscript = ent->m_ScriptEntity;
    while (ett != entscript)
    {
        entscript = entscript->m_Parent;
        if (!entscript)
            return nullptr;
    }

    return ent;
}