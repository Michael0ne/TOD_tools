#include "EntityType.h"
#include "AssetManager.h"
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

inline void EntityType::SetCreator(CREATOR creator)
{
    m_Creator = creator;
}

template <typename T>
void EntityType::RegisterScript(const char* const scriptname, T scriptptr, const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7)
{
    //	NOTE: a workaround to insert a method pointer to a list.
    int sptr = 0;
    _asm push eax
    _asm mov eax, scriptptr
    _asm mov sptr, eax
    _asm pop eax

    m_ScriptsList[RegisterGlobalCommand(scriptname, true)] = { (void (__thiscall*)(void*, void*))sptr, a3, a4, a5 };
}

template <typename T, typename T1>
void EntityType::RegisterProperty(const BaseType* returntype, const char* const propertyname, T getterptr, const int a4, const int a5, const int a6, T1 setterptr, const int a8, const int a9, const int a10, const char* const a11, const int a12, const int a13, const int propertyind)
{
    char propstr[128] = {};
    sprintf(propstr, "%s:%s", propertyname, returntype->m_TypeName.m_szString);
    unsigned int ind = RegisterGlobalProperty(propstr, true);

    //	NOTE: a workaround to insert a method pointer to a list.
    int gptr = 0, sptr = 0;
    _asm push eax
    _asm mov eax, getterptr
    _asm mov gptr, eax
    _asm mov eax, setterptr
    _asm mov sptr, eax
    _asm pop eax

    if (propertyind < 0)
    {
        m_PropertiesList_1.push_back(
            {
                .m_Getter = (void* (__thiscall*)(void*))gptr,
                .field_C = a4,
                .field_10 = a5,
                .field_14 = a6,
                .m_Setter = (void(__thiscall*)(void*, void*))sptr,
                .m_ReturnType = returntype,
                .field_1C = a8,
                .field_20 = a9,
                .field_24 = a10,
                m_Flags.m_FlagsBits.GlobalIndex = m_PropertiesList_1.size() + field_70
            }
        );
    }
    else
    {
        m_PropertiesList_1[propertyind - field_6C] = 
            {
                .m_Getter = (void* (__thiscall*)(void*))gptr,
                .field_C = a4,
                .field_10 = a5,
                .field_14 = a6,
                .m_Setter = (void(__thiscall*)(void*, void*))sptr,
                .m_ReturnType = returntype,
                .field_1C = a8,
                .field_20 = a9,
                .field_24 = a10,
                m_Flags.m_FlagsBits.GlobalIndex = ind
            };
        }
}