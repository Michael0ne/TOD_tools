#include "EntityType.h"
#include "AssetManager.h"
#include "Node.h"
#include "Scene.h"

EntityType::EntityType(const char* const entityname) : DataType(TYPE_ENTITY, entityname, TYPE_ENTITY_SIZE)
{
    MESSAGE_CLASS_CREATED(EntityType);

    m_Creator = nullptr;
    m_Script = nullptr;
    m_Parent = nullptr;
    m_TotalLocalProperties = NULL;
    m_IsBaseEntity = false;
    m_TotalGlobalProperties = 1;
}

EntityType::~EntityType()
{
    MESSAGE_CLASS_DESTROYED(EntityType);
}

char EntityType::IsReferenced(int* a1, int fixdangling)
{
    if (!a1)
        return true;

    if (Scene::DanglingEntityReferences)
    {
        if (Scene::DanglingEntityReferences->size())
        {
            int i = 0;
            for (Scene::EntityReference& entref = (*Scene::DanglingEntityReferences)[i]; (int*)*a1 < entref.m_AddressRegionBeginPtr || (int*)*a1 >= entref.m_AddressRegionEndPtr; i++)
                if (i + 1 >= Scene::DanglingEntityReferences->size())
                    return true;

            if (fixdangling)
            {
                LogDump::LogA("FIXED ");
                *a1 = NULL;
            }

            LogDump::LogA("DANGLING ENTITY REF (0x%x) ", *a1);
            return false;
        }
        return true;
    }

    if (Scene::DanglingEntityReferencesMap->find((int*)a1) != Scene::DanglingEntityReferencesMap->end())
        return true;

    if (fixdangling)
    {
        LogDump::LogA("FIXED ");
        *a1 = NULL;
    }

    LogDump::LogA("DANGLING ENTITY REF (0x%x) ", *a1);
    return false;
}

void* EntityType::CreateNode()
{
    Node* newnode = (Node*)m_Creator(g_AssetManager->GetAllocatorType());
    newnode->SetScript(this);

    return (void*)newnode;
}

void EntityType::InheritFrom(EntityType* from)
{
    m_TotalLocalProperties = from->m_IsBaseEntity ? from->m_Parent->m_LocalPropertiesList.size() + from->m_Parent->m_TotalLocalProperties : from->m_LocalPropertiesList.size() + from->m_TotalLocalProperties;
    m_TotalGlobalProperties = from->m_IsBaseEntity ? from->m_Parent->m_GlobalPropertiesList.size() + from->m_Parent->m_TotalGlobalProperties : from->m_GlobalPropertiesList.size() + from->m_TotalGlobalProperties;
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