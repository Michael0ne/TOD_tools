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
        union
        {
            struct
            {
                unsigned GlobalIndex : 15;
                unsigned _16 : 1;
                unsigned _17 : 1;
                unsigned _18 : 1;
                unsigned _19 : 1;
                unsigned _20 : 1;
                unsigned _21 : 1;
                unsigned _22 : 1;
                unsigned _23 : 1;
                unsigned _24 : 1;
                unsigned _25 : 1;
                unsigned _26 : 1;
                unsigned _27 : 1;
                unsigned _28 : 1;
                unsigned _29 : 1;
                unsigned _30 : 1;
            }           m_FlagsBits;
            unsigned int m_Flags;
        }               m_Flags;
        unsigned int    field_2C = NULL;
    };

public:
    typedef void* (*CREATOR)(AllocatorIndex);

    CREATOR             m_Creator;
    EntityType         *m_Parent;
    GlobalScript       *m_Script;
protected:
    std::map<unsigned short, ScriptInfo> m_ScriptsList;	//	NOTE: each 'derived' script derives it's parent scripts.
    int                 field_38;
    std::map<unsigned short, unsigned short> field_3C;	//	NOTE: could be list with properties id's and their values.
    int                 field_48;
    std::vector<PropertyInfo> m_PropertiesList;	//	NOTE: could be list with methods for THIS exact class only.
    std::vector<PropertyInfo> m_PropertiesList_1;	//	NOTE: another list same as above, but there are more methods in this list.
    int                 field_6C;
    int                 field_70;
    bool                m_HasParent;

public:
    EntityType(const char* const entityname);	//	@86CC00
    virtual ~EntityType();	//	@4886C0

    void*               CreateNode() const;	//	@86C770
    void                InheritFrom(const EntityType* from);	//	@86CB40
    inline void         SetCreator(CREATOR creator)
    {
        m_Creator = creator;
    };

    template <typename T>
    void RegisterScript(const char* const scriptname, T scriptptr, const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7)	//	@86EC70
    {
        //	NOTE: a workaround to insert a method pointer to a list.
        int sptr = 0;
        _asm push eax
        _asm mov eax, scriptptr
        _asm mov sptr, eax
        _asm pop eax

        m_ScriptsList[RegisterGlobalCommand(scriptname, true)] = { (void(__thiscall*)(void*, void*))sptr, a3, a4, a5 };
    }

    template <typename T, typename T1>
    void RegisterProperty(DataType* returntype, const char* const propertyname, T getterptr, const int a4, const int a5, const int a6, T1 setterptr, const int a8, const int a9, const int a10, const char* const editorcontrolstring, const int a12, const int a13, const int propertyind)	//	@86D370
    {
        char propstr[128] = {};
        sprintf(propstr, "%s:%s", propertyname, returntype->m_TypeName.m_Str);
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
            PropertyInfo tmp;
            tmp.m_GetterPtr = (void* (__thiscall*)(void*))gptr;
            tmp.m_SetterPtr = (void(__thiscall*)(void*, void*))sptr;
            tmp.field_C = a4;
            tmp.field_10 = a5;
            tmp.field_14 = a6;
            tmp.m_ReturnType = returntype;
            tmp.field_1C = a8;
            tmp.field_20 = a9;
            tmp.field_24 = a10;
            tmp.m_Flags.m_FlagsBits.GlobalIndex = m_PropertiesList_1.size() + field_70;

            m_PropertiesList_1.push_back(tmp);
        }
        else
        {
            PropertyInfo tmp;
            tmp.m_GetterPtr = (void* (__thiscall*)(void*))gptr;
            tmp.m_SetterPtr = (void(__thiscall*)(void*, void*))sptr;
            tmp.m_ReturnType = returntype;
            tmp.field_C = a4;
            tmp.field_10 = a5;
            tmp.field_14 = a6;
            tmp.field_1C = a8;
            tmp.field_20 = a9;
            tmp.field_24 = a10;
            tmp.m_Flags.m_FlagsBits.GlobalIndex = ind;

            if (propertyind - field_6C >= (int)m_PropertiesList.size())
                m_PropertiesList.resize(propertyind - field_6C + 1);
            m_PropertiesList.insert(m_PropertiesList.begin() + (propertyind - field_6C), tmp);
        }
    }
    void				_86E9B0();	//	@86E9B0	//	NOTE: probably, this propagates all methods and properties from parent entity.

    static Entity*      IsParentOf(EntityType* ett, Entity* ent);   //  @48C3B0
};

ASSERT_CLASS_SIZE(EntityType, 120);