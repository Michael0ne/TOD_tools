#include "EntityType.h"
#include "AssetManager.h"
#include "Node.h"
#include "Scene.h"

EntityType::EntityType(const char* const entityname) : DataType(TYPE_ENTITY, entityname, TYPE_ENTITY_SIZE)
{
    MESSAGE_CLASS_CREATED(EntityType);

    Creator = nullptr;
    Script = nullptr;
    Parent = nullptr;
    TotalLocalProperties = NULL;
    IsBaseEntity = false;
    TotalGlobalProperties = 1;
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
    Node* newnode = (Node*)Creator(g_AssetManager->GetAllocatorType());
    newnode->SetScript(this);

    return (void*)newnode;
}

void EntityType::InheritFrom(EntityType* from)
{
    TotalLocalProperties = from->IsBaseEntity ? from->Parent->LocalPropertiesList.size() + from->Parent->TotalLocalProperties : from->LocalPropertiesList.size() + from->TotalLocalProperties;
    TotalGlobalProperties = from->IsBaseEntity ? from->Parent->GlobalPropertiesList.size() + from->Parent->TotalGlobalProperties : from->GlobalPropertiesList.size() + from->TotalGlobalProperties;
    Creator = from->Creator;
    Parent = from;
}

void EntityType::RegisterScript(const char* const scriptname, EntityFunctionMember scriptptr, const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7)
{
    const uint16_t commandId = GetCommand(scriptname, true);

    ScriptsList.insert(std::make_pair(commandId, new ScriptInfo(scriptptr, a3, a4, a5)));
}

void EntityType::RegisterProperty(DataType* returntype, const char* const propertyname, EntityGetterFunction getterptr, const int a4, const int a5, const int a6, EntitySetterFunction setterptr, const int a8, const int a9, const int a10, const char* const editorcontrolstring, const int a12, const int a13, const int propertyind)
{
    char propstr[512] = {};
    sprintf(propstr, "%s:%s", propertyname, returntype->TypeName.m_Str);
    const unsigned short globalIndex = (unsigned short)GetProperty(propstr, true);

    PropertyInfo propinfo(getterptr, a4, a5, a6, setterptr, returntype, a8, a9, a10);
    if (propertyind < 0)
    {
        propinfo.GlobalIndex = globalIndex;
        propinfo.LocalIndex = (unsigned short)GlobalPropertiesList.size() + TotalGlobalProperties;

        GlobalPropertiesList.push_back(propinfo);
    }
    else
    {
        propinfo.GlobalIndex = globalIndex;
        propinfo.LocalIndex = propertyind;

        if (propertyind - TotalLocalProperties >= (int)LocalPropertiesList.size())
            LocalPropertiesList.resize(propertyind - TotalLocalProperties + 1);

        LocalPropertiesList[propertyind - TotalLocalProperties] = propinfo;
    }
}

void EntityType::GetPropertyValue(const Node* callerNode, uint32_t* nodeParameters, const uint32_t propertyIndex, uint8_t* outPropertyValue) const
{
    *outPropertyValue = 0;

    if (!Script || !Script->GetMappedPropertyValue(nodeParameters, propertyIndex, outPropertyValue))
    {
        const auto it = IsBaseEntity ? Parent->PropertiesMappings.find(propertyIndex) : PropertiesMappings.find(propertyIndex);

        EntityType* ent = (EntityType*)this;
        unsigned short mappedPropertyId = it->first;

        if (mappedPropertyId)
        {
            int propertiesSize;
            std::vector<PropertyInfo>* propertiesList;

            if (mappedPropertyId >= 0)
            {
                if (mappedPropertyId < TotalLocalProperties)
                {
                    do
                    {
                        ent = ent->Parent;
                    } while (mappedPropertyId < ent->TotalLocalProperties);
                }

                propertiesSize = ent->TotalLocalProperties;
                propertiesList = &ent->LocalPropertiesList;
            }
            else
            {
                mappedPropertyId = -mappedPropertyId;
                if (mappedPropertyId < TotalGlobalProperties)
                {
                    do
                    {
                        ent = ent->Parent;
                    } while (mappedPropertyId < ent->TotalGlobalProperties);
                }

                propertiesSize = ent->TotalGlobalProperties;
                propertiesList = &ent->GlobalPropertiesList;
            }

            PropertyInfo& propertyElement = (*propertiesList)[mappedPropertyId - propertiesSize];
            propertyElement.ReturnType->CallGetterFunction(callerNode, propertyElement.Getter, propertyElement._fC, propertyElement._f10, propertyElement._f14, outPropertyValue);
        }
    }
}

void EntityType::PropagateProperties()
{
    if (!Parent ||
        !Script ||
        LocalPropertiesList.size() ||
        GlobalPropertiesList.size() ||
        ScriptsList.size() ||
        Parent->IsBaseEntity)
    {

        IsBaseEntity = false;

        std::map<unsigned short, ScriptInfo*> parentScripts;
        if (Parent)
            parentScripts = Parent->ScriptsList;

        if (ScriptsList.size() > 0)
            for (auto it = ScriptsList.begin(); it != ScriptsList.end(); it++)
                parentScripts[it->first] = it->second;

        ScriptsList = parentScripts;

        std::map<unsigned short, unsigned short> parentProperties;
        if (Parent)
            parentProperties = Parent->PropertiesMappings;

        if (LocalPropertiesList.size() > 0)
        {
            for (unsigned int i = 0; i < LocalPropertiesList.size(); i++)
            {
                const short globalpropindex = LocalPropertiesList[i].GlobalIndex;
                if (globalpropindex < 0)
                    continue;

                parentProperties[globalpropindex] = LocalPropertiesList[i].LocalIndex;
            }
        }

        if (GlobalPropertiesList.size() > 0)
        {
            for (unsigned int i = 0; i < GlobalPropertiesList.size(); i++)
            {
                const short globalpropindex = GlobalPropertiesList[i].GlobalIndex;
                if (globalpropindex < 0)
                    continue;

                parentProperties[globalpropindex] = GlobalPropertiesList[i].LocalIndex;
            }
        }

        PropertiesMappings = parentProperties;
    }
    else
        IsBaseEntity = true;
}

bool EntityType::HasPropertyId(const unsigned int propertyId) const
{
    bool found = IsBaseEntity ? Parent->PropertiesMappings.find((const unsigned short)propertyId) != Parent->PropertiesMappings.end() : PropertiesMappings.find((const unsigned short)propertyId) != PropertiesMappings.end();
    if (found)
        return true;

    return Script && Script->HasPropertyId(propertyId);
}

DataType* EntityType::PropertyByIndex(const int index) const
{
    const int localPropertiesTotal = IsBaseEntity
        ? Parent->LocalPropertiesList.size() + Parent->TotalLocalProperties
        : LocalPropertiesList.size() + TotalLocalProperties;

    if (index >= localPropertiesTotal)
    {
        return Script->m_PropertiesList[index - localPropertiesTotal].m_Info->m_PropertyType;
    }
    else
    {
        EntityType* parentEntity = Parent;
        if (index >= 0)
        {
            while (index < parentEntity->TotalLocalProperties)
                parentEntity = parentEntity->Parent;

            return parentEntity->LocalPropertiesList[index - parentEntity->TotalLocalProperties].ReturnType;
        }
        else
        {
            int i = -index;
            while (i < parentEntity->TotalLocalProperties)
                parentEntity = parentEntity->Parent;

            return parentEntity->GlobalPropertiesList[i - parentEntity->TotalGlobalProperties].ReturnType;
        }
    }
}

void EntityType::SaveData(Node* node) const
{
    uint32_t localPropertiesTotal = IsBaseEntity ? Parent->LocalPropertiesList.size() + Parent->TotalLocalProperties : LocalPropertiesList.size() + TotalLocalProperties;
    localPropertiesTotal = clamp(localPropertiesTotal, (uint32_t)0, (uint32_t)40);

    if (localPropertiesTotal > 1)
    {
        int32_t propertyIndex = -1;
        for (uint32_t i = 1; i < localPropertiesTotal; i++, propertyIndex--)
        {
            const bool propertyUsed = (node->m_PropertiesSlots[i / 8] & (1 << (i & 7))) != 0;
            if (!propertyUsed)
                continue;

            EntityType* parent = const_cast<EntityType*>(this);
            EntityType::PropertyInfo* propertyInfo = nullptr;
            if (propertyIndex <= 0)
            {
                if (i < TotalLocalProperties)
                {
                    do
                    {
                        parent = parent->Parent;
                    } while (i < parent->TotalLocalProperties);
                }

                propertyInfo = &(parent->LocalPropertiesList[i - parent->TotalLocalProperties]);
            }
            else
            {
                if (propertyIndex < TotalGlobalProperties)
                {
                    do
                    {
                        parent = parent->Parent;
                    } while (i < parent->TotalGlobalProperties);
                }

                propertyInfo = &(parent->GlobalPropertiesList[propertyIndex - parent->TotalGlobalProperties]);
            }

            uint8_t getterResult[4] = {};
            propertyInfo->ReturnType->CallGetterFunction(node, propertyInfo->Getter, propertyInfo->_fC, propertyInfo->_f10, propertyInfo->_f14, getterResult);
            node->StorePropertyData(i, (uint32_t*)getterResult, propertyInfo->ReturnType);
        }
    }

    if (Script)
        Script->SaveNodeProperties(node);
}

const uint32_t EntityType::_86CAC0(const uint16_t propertyIndex) const
{
    const auto propertiesMappings = IsBaseEntity ? Parent->PropertiesMappings : PropertiesMappings;
    const auto mappedPropertyIndex = propertiesMappings.find(propertyIndex);
    if (mappedPropertyIndex != propertiesMappings.cend())
        return mappedPropertyIndex->first;

    const uint32_t localPropertiesTotal = IsBaseEntity ? Parent->LocalPropertiesList.size() + Parent->TotalLocalProperties : LocalPropertiesList.size() + TotalLocalProperties;
    return localPropertiesTotal + Script->GetPropertyValueByIndex(propertyIndex);
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
        entscript = entscript->Parent;
        if (!entscript)
            return nullptr;
    }

    return ent;
}