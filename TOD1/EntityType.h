#pragma once
#include "DataType.h"
#include "ScriptDatabase.h"

class EntityType : public DataType
{
    friend class Node;

    struct ScriptInfo
    {
        void            (__thiscall* m_ScriptPtr)(void*, void*) = nullptr;
        int             field_4 = NULL;
        int             field_8 = NULL;
        int             field_C = NULL;
    };

    struct PropertyInfo
    {
        DataType       *m_ReturnType = nullptr;
        int            *field_4 = nullptr;
        void*           (__thiscall *m_GetterPtr)(void*) = nullptr;
        unsigned int    field_C = NULL;
        unsigned int    field_10 = NULL;
        unsigned int    field_14 = NULL;
        void            (__thiscall *m_SetterPtr)(void*, void*) = nullptr;
        unsigned int    field_1C = NULL;
        unsigned int    field_20 = NULL;
        int             field_24 = NULL;
        short           m_GlobalPropertyIndex = -1;
        short           m_LocalPropertyIndex = -1;
        unsigned int    field_2C = NULL;
    };

public:
    typedef void* (*CREATOR)(AllocatorIndex);

    CREATOR             m_Creator;
    EntityType         *m_Parent;
    GlobalScript       *m_Script;
protected:
    std::map<unsigned short, ScriptInfo> m_ScriptsList; // NOTE: each 'derived' script derives it's parent scripts.
    int                 field_38;
    std::map<unsigned short, unsigned short> field_3C; // NOTE: could be list with properties id's and their values.
    int                 field_48;
    std::vector<PropertyInfo> m_LocalPropertiesList;
    std::vector<PropertyInfo> m_GlobalPropertiesList;
    int                 field_6C;   //  NOTE: 'TotalProperties' including parent's properties?
    int                 field_70;   //  NOTE: same as above, but for second list.
    bool                m_IsBaseEntity;

public:
    EntityType(const char* const entityname); // @86CC00
    virtual ~EntityType(); // @4886C0

    void*               CreateNode(); // @86C770
    void                InheritFrom(EntityType* from); // @86CB40
    inline void         SetCreator(CREATOR creator)
    {
        m_Creator = creator;
    };

    template <typename T>
    void RegisterScript(const char* const scriptname, T scriptptr, const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7) // @86EC70
    {
        // NOTE: a workaround to insert a method pointer to a list.
        int sptr = 0;
        _asm push eax
        _asm mov eax, scriptptr
        _asm mov sptr, eax
        _asm pop eax

        const int commandId = RegisterGlobalCommand(scriptname, true);

        m_ScriptsList[commandId] = { (void(__thiscall*)(void*, void*))sptr, a3, a4, a5 };
    }

    template <typename T, typename T1>
    void RegisterProperty(DataType* returntype, const char* const propertyname, T getterptr, const int a4, const int a5, const int a6, T1 setterptr, const int a8, const int a9, const int a10, const char* const editorcontrolstring, const int a12, const int a13, const int propertyind) // @86D370
    {
        char propstr[128] = {};
        sprintf(propstr, "%s:%s", propertyname, returntype->m_TypeName.m_Str);
        int ind = RegisterGlobalProperty(propstr, true);

        // NOTE: a workaround to insert a method pointer to a list.
        int gptr = 0, sptr = 0;
        _asm push eax
        _asm mov eax, getterptr
        _asm mov gptr, eax
        _asm mov eax, setterptr
        _asm mov sptr, eax
        _asm pop eax

        PropertyInfo propinfo;
        propinfo.m_GetterPtr = (void* (__thiscall*)(void*))gptr;
        propinfo.m_SetterPtr = (void (__thiscall*)(void*, void*))sptr;
        propinfo.field_C = a4;
        propinfo.field_C = a4;
        propinfo.field_10 = a5;
        propinfo.field_14 = a6;
        propinfo.m_ReturnType = returntype;
        propinfo.field_1C = a8;
        propinfo.field_20 = a9;
        propinfo.field_24 = a10;
        propinfo.m_LocalPropertyIndex = propertyind;

        if (propertyind < 0)
        {
            propinfo.m_GlobalPropertyIndex = (short)(m_GlobalPropertiesList.size() + field_70);

            m_GlobalPropertiesList.push_back(propinfo);
        }
        else
        {
            propinfo.m_GlobalPropertyIndex = (short)ind;

            if (propertyind - field_6C >= (int)m_LocalPropertiesList.size())
                m_LocalPropertiesList.resize(propertyind - field_6C + 1);
            m_LocalPropertiesList.insert(m_LocalPropertiesList.begin() + (propertyind - field_6C), propinfo);
        }
    }
    void    PropagateProperties(); // @86E9B0
    bool    HasPropertyId(const unsigned int propertyId) const;  //  @86C9E0

    static Entity*      IsParentOf(EntityType* ett, Entity* ent);   //  @48C3B0
};

ASSERT_CLASS_SIZE(EntityType, 120);