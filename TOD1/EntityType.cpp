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
    m_IsBaseEntity = false;
    field_70 = 1;
}

EntityType::~EntityType()
{
    MESSAGE_CLASS_DESTROYED(EntityType);
}

void* EntityType::CreateNode()
{
    Node* newnode = (Node*)m_Creator(g_AssetManager->GetAllocatorType());
    newnode->SetScript(this);

    return (void*)newnode;
}

void EntityType::InheritFrom(EntityType* from)
{
    field_6C = from->m_IsBaseEntity ? from->m_Parent->m_LocalPropertiesList.size() + from->m_Parent->field_6C : from->m_LocalPropertiesList.size() + from->field_6C;
    field_70 = from->m_IsBaseEntity ? from->m_Parent->m_GlobalPropertiesList.size() + from->m_Parent->field_70 : from->m_GlobalPropertiesList.size() + from->field_70;
    m_Creator = from->m_Creator;
    m_Parent = from;
}

#pragma message(TODO_IMPLEMENTATION)
void EntityType::PropagateProperties()
{
    if (!m_Parent ||
        !m_Script ||
        m_LocalPropertiesList.size() ||
        m_GlobalPropertiesList.size() ||
        m_ScriptsList.size() ||
        m_Parent->m_IsBaseEntity)
    {

        m_IsBaseEntity = false;

        std::map<unsigned short, EntityType::ScriptInfo> parentScripts;
        if (m_Parent)
            parentScripts = m_Parent->m_ScriptsList;

        if (m_ScriptsList.size() > 0)
            for (auto it = m_ScriptsList.begin(); it != m_ScriptsList.end(); it++)
                parentScripts[it->first] = it->second;

        m_ScriptsList = parentScripts;

        std::map<unsigned short, unsigned short> parentProperties;
        std::map<unsigned short, unsigned short>::iterator* propertyIterator = nullptr;
        if (m_Parent)
        {
            parentProperties = m_Parent->field_3C;
            propertyIterator = &parentProperties.begin();
        }

        //  TODO: local and global properties lists are cursed right now. Fix them! Otherwise code is legit.
        if (m_LocalPropertiesList.size() > 0)
        {
            for (unsigned int i = 0; i < m_LocalPropertiesList.size(); ++i)
            {
                const short globalpropindex = m_LocalPropertiesList[i].m_GlobalPropertyIndex;
                if (globalpropindex < 0)
                    continue;

                auto it = propertyIterator;
                while (it)
                {
                    if (globalpropindex <= (*it)->first)
                    {
                        if (globalpropindex >= (*it)->first)
                            break;

                        it--;
                    }
                    else
                        it++;
                }

                parentProperties[globalpropindex] = m_LocalPropertiesList[i].m_GlobalPropertyIndex >> 16;
                propertyIterator = &parentProperties.begin();
            }
        }

        if (m_GlobalPropertiesList.size() > 0)
        {
            for (unsigned int i = 0; i < m_GlobalPropertiesList.size(); ++i)
            {
                const short globalpropindex = m_GlobalPropertiesList[i].m_GlobalPropertyIndex;
                if (globalpropindex < 0)
                    continue;

                auto it = propertyIterator;
                while (it)
                {
                    if (globalpropindex <= (*it)->first)
                    {
                        if (globalpropindex >= (*it)->first)
                            break;

                        it--;
                    }
                    else
                        it++;

                    parentProperties[globalpropindex] = m_GlobalPropertiesList[i].m_GlobalPropertyIndex >> 16;
                    propertyIterator = &parentProperties.begin();
                }
            }
        }

        field_3C = parentProperties;
    }
    else
        m_IsBaseEntity = true;
}

bool EntityType::HasPropertyId(const unsigned int propertyId) const
{
    bool found = m_IsBaseEntity ? m_Parent->field_3C.find((const unsigned short)propertyId) != m_Parent->field_3C.end() : field_3C.find((const unsigned short)propertyId) != field_3C.end();
    if (found)
        return true;

    return m_Script && m_Script->HasPropertyId(propertyId);
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