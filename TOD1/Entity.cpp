#include "Entity.h"
#include "AssetManager.h"
#include "Globals.h"
#include "MemoryCards.h"
#include "ScriptThread.h"
#include "MemoryCards.h"
#include "SavePoint.h"
#include "MemoryCard.h"

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

    if (field_20 && field_20[1])
    {
        //m_Defragmentator->stub4((field_20 - m_Defragmentator->m_AllocatedSpace) / 12);
        field_20 = nullptr;
    }

    if (m_Parameters)
        m_ScriptEntity->m_Script->ClearEntityProperties(this);

    const unsigned int blockid = ((m_Id >> 28) & 7) - 1;
    const int entindd = (m_Id >> 8) & 0xFF8FFFFF;

    g_AssetManager->m_NodesList[blockid][entindd] = nullptr;

    if (g_AssetManager->m_NodesInNodeList[blockid] > entindd)
        g_AssetManager->m_NodesInNodeList[blockid] = entindd;
}

Entity::Entity()
{
    MESSAGE_CLASS_CREATED(Entity);

    m_Defragmentator = NULL;
    field_20 = nullptr;
    m_Id = m_Id | 255;
    m_ScriptEntity = nullptr;
    m_Parameters = nullptr;
    field_20 = nullptr;

    memset(field_8, NULL, sizeof(field_8));

    m_Id = (g_AssetManager->AddEntity(this) << 8) | m_Id;
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
    return m_Id >> 8;
}

#pragma message(TODO_IMPLEMENTATION)
int Entity::GetScriptPriority() const
{
    return NULL;
}

void Entity::SetScriptPriority(const unsigned char priority)
{
    if (!field_20 || !field_20[1])
        return;

    ScriptThread* st = (ScriptThread*)field_20[1];
    st->m_ThreadFlags.m_FlagBits.Priority = priority;
    
    if (st->m_ScriptNode->m_GlobalIdInBlockigList >= 0)
        //  TODO: some flag value here...
        Node::NodesWithUpdateOrBlockingScripts[st->m_ScriptNode->m_GlobalIdInBlockigList].m_Enabled = false;
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

#pragma message(TODO_IMPLEMENTATION)
void Entity::SetScript(const EntityType* script)
{
    //if (field_18)
     //if (m_ScriptEntity)
      //m_ScriptEntity->m_ParentNode->_489C90(this);

    //m_ScriptEntity = script;

    //if (script->m_ParentNode)
     //script->m_ParentNode->_489BE0(this);
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

#pragma message(TODO_IMPLEMENTATION)
void Entity::Register()
{
    tEntity = new EntityType("Entity");
    tEntity->SetCreator((EntityType::CREATOR)Create);
}

Entity* Entity::Create(AllocatorIndex)
{
    return new Entity();
}