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
            unsigned int i = 0;
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

void EntityType::RegisterScript(const char* const scriptname, EntityFunctionMember scriptptr, const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7)
{
    const unsigned short commandId = GetMessage(scriptname, true);

    m_ScriptsList.insert(std::make_pair(commandId, new ScriptInfo(scriptptr, a3, a4, a5)));
}

void EntityType::RegisterProperty(DataType* returntype, const char* const propertyname, EntityGetterFunction getterptr, const int a4, const int a5, const int a6, EntitySetterFunction setterptr, const int a8, const int a9, const int a10, const char* const editorcontrolstring, const int a12, const int a13, const int propertyind)
{
    char propstr[512] = {};
    sprintf(propstr, "%s:%s", propertyname, returntype->m_TypeName.m_Str);
    const unsigned short globalIndex = (unsigned short)GetProperty(propstr, true);

    PropertyInfo propinfo(getterptr, a4, a5, a6, setterptr, returntype, a8, a9, a10);
    if (propertyind < 0)
    {
        propinfo.m_GlobalIndex = globalIndex;
        propinfo.m_LocalIndex = (unsigned short)m_GlobalPropertiesList.size() + m_TotalGlobalProperties;

        m_GlobalPropertiesList.push_back(propinfo);
    }
    else
    {
        propinfo.m_GlobalIndex = globalIndex;
        propinfo.m_LocalIndex = propertyind;

        if (propertyind - m_TotalLocalProperties >= (int)m_LocalPropertiesList.size())
            m_LocalPropertiesList.resize(propertyind - m_TotalLocalProperties + 1);

        m_LocalPropertiesList[propertyind - m_TotalLocalProperties] = propinfo;
    }
}

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

        std::map<unsigned short, ScriptInfo*> parentScripts;
        if (m_Parent)
            parentScripts = m_Parent->m_ScriptsList;

        if (m_ScriptsList.size() > 0)
            for (auto it = m_ScriptsList.begin(); it != m_ScriptsList.end(); it++)
                parentScripts[it->first] = it->second;

        m_ScriptsList = parentScripts;

        std::map<unsigned short, unsigned short> parentProperties;
        if (m_Parent)
            parentProperties = m_Parent->m_PropertiesMappings;

        if (m_LocalPropertiesList.size() > 0)
        {
            for (unsigned int i = 0; i < m_LocalPropertiesList.size(); i++)
            {
                const short globalpropindex = m_LocalPropertiesList[i].m_GlobalIndex;
                if (globalpropindex < 0)
                    continue;

                parentProperties[globalpropindex] = m_LocalPropertiesList[i].m_LocalIndex;
            }
        }

        if (m_GlobalPropertiesList.size() > 0)
        {
            for (unsigned int i = 0; i < m_GlobalPropertiesList.size(); i++)
            {
                const short globalpropindex = m_GlobalPropertiesList[i].m_GlobalIndex;
                if (globalpropindex < 0)
                    continue;

                parentProperties[globalpropindex] = m_GlobalPropertiesList[i].m_LocalIndex;
            }
        }

        m_PropertiesMappings = parentProperties;
    }
    else
        m_IsBaseEntity = true;
}

bool EntityType::HasPropertyId(const unsigned int propertyId) const
{
    bool found = m_IsBaseEntity ? m_Parent->m_PropertiesMappings.find((const unsigned short)propertyId) != m_Parent->m_PropertiesMappings.end() : m_PropertiesMappings.find((const unsigned short)propertyId) != m_PropertiesMappings.end();
    if (found)
        return true;

    return m_Script && m_Script->HasPropertyId(propertyId);
}

DataType* EntityType::PropertyByIndex(const int index) const
{
    const int localPropertiesTotal = m_IsBaseEntity
        ? m_Parent->m_LocalPropertiesList.size() + m_Parent->m_TotalLocalProperties
        : m_LocalPropertiesList.size() + m_TotalLocalProperties;

    if (index >= localPropertiesTotal)
    {
        return m_Script->m_PropertiesList[index - localPropertiesTotal].m_Info->m_PropertyType;
    }
    else
    {
        EntityType* parentEntity = m_Parent;
        if (index >= 0)
        {
            while (index < parentEntity->m_TotalLocalProperties)
                parentEntity = parentEntity->m_Parent;

            return parentEntity->m_LocalPropertiesList[index - parentEntity->m_TotalLocalProperties].m_ReturnType;
        }
        else
        {
            int i = -index;
            while (i < parentEntity->m_TotalLocalProperties)
                parentEntity = parentEntity->m_Parent;

            return parentEntity->m_GlobalPropertiesList[i - parentEntity->m_TotalGlobalProperties].m_ReturnType;
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
void EntityType::ExecuteScript(Node* node) const
{
    unsigned int localPropertyTotal = m_IsBaseEntity
        ? (m_Parent->m_LocalPropertiesList.size() + m_Parent->m_TotalLocalProperties)
        : (m_LocalPropertiesList.size() + m_TotalLocalProperties);

    localPropertyTotal = localPropertyTotal > 40 ? 40 : localPropertyTotal;

    if (localPropertyTotal > 1)
    {
        for (unsigned int i = 0; i < localPropertyTotal; ++i)
        {

        }
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