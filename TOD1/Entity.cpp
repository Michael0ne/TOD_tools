#include "Entity.h"
#include "AssetManager.h"
#include "Globals.h"
#include "MemoryCards.h"
#include "ScriptThread.h"
#include "MemoryCards.h"
#include "SavePoint.h"
#include "MemoryCard.h"
#include "IntegerType.h"

EntityType* tEntity;

void Entity::Destroy()
{
    if (this)
        delete this;
}

#pragma message(TODO_IMPLEMENTATION)
Entity::~Entity()
{
    MESSAGE_CLASS_DESTROYED(Entity);

    if (field_20 && field_20->m_ScriptThread)
    {
        //m_Defragmentator->stub4((field_20 - m_Defragmentator->m_AllocatedSpace) / 12);
        field_20 = nullptr;
    }

    if (m_Parameters)
        m_ScriptEntity->m_Script->ClearEntityProperties(this);

    g_AssetManager->m_NodesList[m_Id.BlockId - 1][m_Id.Id] = nullptr;

    if (g_AssetManager->m_NodesInNodeList[m_Id.BlockId - 1] > m_Id.Id)
        g_AssetManager->m_NodesInNodeList[m_Id.BlockId - 1] = m_Id.Id;
}

Entity::Entity()
{
    MESSAGE_CLASS_CREATED(Entity);

    m_Defragmentator = NULL;
    field_20 = nullptr;
    m_Id.HasPosition = true;
    m_Id.HasFragment = true;
    m_Id.HasQuadTree = true;
    m_Id._3 = 0x1F; //  NOTE: what is this?
    m_ScriptEntity = nullptr;
    m_Parameters = nullptr;
    field_20 = nullptr;

    memset(field_8, NULL, sizeof(field_8));

    m_Id.Id = g_AssetManager->AddEntity(this);
}

int Entity::SaveScriptDataToFile_Impl(MemoryCards * memcard, int memcardindex, int savegameslot, const char* a4)
{
    if (!m_ScriptEntity->m_Script)
        return -1;

    String saveslotstring;
    memcard->MakeSaveSlotString(saveslotstring, savegameslot);
    SavePoint sp(MemoryCardInfo[memcardindex], memcard->GetSaveDirectory(savegameslot), saveslotstring.m_Str, SAVEPOINT_SAVE_SIZE);

    if (!sp.Open(STATUS_1))
        return -1;

    const int writtendatasize = SaveScriptData(&sp);
    if (writtendatasize >= NULL && !SavePoint::GenerateAndWriteCRC(&sp, sp.GetPosition()))
        return -1;

    return writtendatasize;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned char Entity::LoadScriptDataFromFile_Impl(EntityType*, int, int)
{
    return NULL;
}

int Entity::GetId() const
{
    return m_Id.Id;
}

int Entity::GetScriptPriority() const
{
    if (!field_20 || !field_20->m_ScriptThread)
        return NULL;

    return field_20->m_ScriptThread->m_ThreadFlags.Priority;
}

void Entity::SetScriptPriority(const unsigned char priority)
{
    if (!field_20 || !field_20->m_ScriptThread)
        return;

    field_20->m_ScriptThread->m_ThreadFlags.Priority = priority;
    
    const short globid = field_20->m_ScriptThread->m_ScriptNode->m_GlobalIdInBlockigList;
    if (globid >= 0)
        Node::NodesWithUpdateOrBlockingScripts[globid].m_Flags.m_GlobalId = field_20->m_ScriptThread->m_ThreadFlags.Priority;
}

void Entity::GetPropertyId(int* args) const
{
    *args = GetPropertyIdByName((const char* const)args[1]);
}

void Entity::HasProperty(int* args) const
{
    *args = m_ScriptEntity->HasPropertyId(GetPropertyIdByName((const char* const)args[1]));
}

void Entity::HasPropertyId(int* args) const
{
    *args = m_ScriptEntity->HasPropertyId(args[1]);
}

void Entity::SaveScriptDataToFile(int* params)
{
    if (!params[1] || ((MemoryCards*)params[1])->m_ScriptEntity == nullptr)
    {
        *params = NULL;
        return;
    }

    EntityType* memcard = (EntityType*)(((MemoryCards*)params[1])->m_ScriptEntity);

    while (tMemoryCards != memcard)
    {
        if (!(memcard = memcard->m_Parent))
        {
            *params = NULL;
            return;
        }
    }

    *params = SaveScriptDataToFile_Impl((MemoryCards*)memcard, params[2], params[3], (const char*)params[4]) >= NULL;
}

void Entity::LoadScriptDataFromFile(int* params)
{
    if (!params[1] || ((MemoryCards*)params[1])->m_ScriptEntity == nullptr)
    {
        *params = NULL;
        return;
    }

    EntityType* memcard = (EntityType*)(((MemoryCards*)params[1])->m_ScriptEntity);

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

void Entity::SetScript(EntityType* script)
{
    if (m_Parameters && m_ScriptEntity)
        m_ScriptEntity->m_Script->ClearEntityProperties(this);

    m_ScriptEntity = script;

    if (script->m_Script)
        script->m_Script->CopyScriptParameters(this);
}

const int Entity::SaveScriptData(SavePoint * savefilehelper)
{
    if (!m_ScriptEntity->m_Script)
        return -1;

    const unsigned int propertiessize = m_ScriptEntity->m_Script->GetPropertiesListSize();
    unsigned int writtendatasize = savefilehelper->WriteBufferWithSize((const char*)&propertiessize, sizeof(propertiessize));

    if (propertiessize > 0)
    {
        int propval[4] = {};
#ifdef INCLUDE_FIXES
        // NOTE: 8 Kib of space should be sufficient, I guess.
        char buf[8192] = {};
#else
        char buf[32768] = {};
#endif

        for (unsigned int propindex = 0; propindex < propertiessize; ++propindex)
        {
            m_ScriptEntity->m_Script->GetEntityPropertyValue(this, propindex, propval);
            const unsigned int entvalsize = m_ScriptEntity->m_Script->m_PropertiesList[propindex].m_Info->m_PropertyType->stub9((char*)propval, buf);

            writtendatasize += savefilehelper->WriteBufferWithSize((const char*)&entvalsize, sizeof(entvalsize));
            writtendatasize += savefilehelper->WriteBufferWithSize(buf, entvalsize * sizeof(entvalsize));
        }
    }

    return writtendatasize;
}

void Entity::Register()
{
    tEntity = new EntityType("Entity");
    tEntity->RegisterScript("getpropertyid(string):integer", (EntityFunctionMember)&GetPropertyId, NULL, NULL, NULL, nullptr, "GetPropertyIDMSG");
    tEntity->RegisterScript("hasproperty(string):truth", (EntityFunctionMember)&HasProperty, NULL, NULL, NULL, nullptr, "HasPropertyMSG");
    tEntity->RegisterScript("haspropertyid(integer):truth", (EntityFunctionMember)&HasPropertyId, NULL, NULL, NULL, nullptr, "HasPropertyIDMSG");
    tEntity->RegisterScript("savescriptdatatofile(entity,integer,integer,string):truth", (EntityFunctionMember)&SaveScriptDataToFile, NULL, NULL, NULL, nullptr, nullptr);
    tEntity->RegisterScript("loadscriptdatafromfile(entity,integer,integer):integer", (EntityFunctionMember)&LoadScriptDataFromFile, NULL, NULL, NULL, nullptr, nullptr);

    tEntity->RegisterProperty(tINTEGER, "id", (EntityGetterFunction)&GetId, NULL, NULL, NULL, nullptr, NULL, NULL, -1, "control=string", NULL, NULL, -1);
    tEntity->RegisterProperty(tINTEGER, "script_priority", (EntityGetterFunction)&GetScriptPriority, NULL, NULL, NULL, (EntitySetterFunction)&SetScriptPriority, NULL, NULL, NULL, "control=string", NULL, NULL, -1);

    tEntity->SetCreator((CREATOR)Create);
    tEntity->PropagateProperties();
}

Entity* Entity::Create(AllocatorIndex)
{
    return new Entity;
}