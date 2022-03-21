#pragma once
#include "DataType.h"
#include "ScriptDatabase.h"

typedef void (Entity::* EntityFunctionMember)(void*);
typedef void* (Entity::* EntityGetterFunction)(void);
typedef void (Entity::* EntitySetterFunction)(void*);
typedef void* (*CREATOR)(AllocatorIndex);

class EntityType : public DataType
{
    friend class Node;
    friend class Entity;

    struct ScriptInfo
    {
        EntityFunctionMember    m_ScriptPtr = nullptr;
        int                     field_4 = 0;
        int                     field_8 = 0;
        int                     field_C = 0;

        ScriptInfo()
        {};

        ScriptInfo(EntityFunctionMember scriptptr, int _f4, int _f8, int _fC)
        {
            m_ScriptPtr = scriptptr;
            field_4 = _f4;
            field_8 = _f8;
            field_C = _fC;
        }
    };

    struct PropertyInfo
    {
        DataType* m_ReturnType = nullptr;
        int* field_4 = nullptr;
        EntityGetterFunction    m_GetterPtr = nullptr;
        int                     field_C = 0;
        int                     field_10 = 0;
        int                     field_14 = 0;
        EntitySetterFunction    m_SetterPtr = nullptr;
        int                     field_1C = 0;
        int                     field_20 = 0;
        int                     field_24 = 0;
        unsigned short          m_GlobalIndex = 0;
        unsigned short          m_LocalIndex = 0;
        int                     field_2C = 0;

        PropertyInfo()
        {};

        inline PropertyInfo(EntityGetterFunction getter, int _fC, int _f10, int _f14, EntitySetterFunction setter, DataType* rettype, int _f1C, int _f20, int _f24)
        {
            m_GetterPtr = getter;
            field_C = _fC;
            field_10 = _f10;
            field_14 = _f14;
            m_SetterPtr = setter;
            m_ReturnType = rettype;
            field_1C = _f1C;
            field_20 = _f20;
            field_24 = _f24;
        }
    };

public:
    CREATOR                                     m_Creator;
    EntityType                                 *m_Parent;
    Scriptbaked                                *m_Script;

protected:
    std::map<unsigned short, ScriptInfo*>       m_ScriptsList; // NOTE: each 'derived' script derives it's parent scripts.
    int                                         field_38;
    std::map<unsigned short, unsigned short>    m_PropertiesMappings;
    int                                         field_48;
    std::vector<PropertyInfo>                   m_LocalPropertiesList;
    std::vector<PropertyInfo>                   m_GlobalPropertiesList;
    int                                         m_TotalLocalProperties;
    int                                         m_TotalGlobalProperties;
    bool                                        m_IsBaseEntity;

public:
    EntityType(const char* const entityname); // @86CC00
    virtual             ~EntityType(); // @4886C0
    virtual char        IsReferenced(int* a1, int a2) override;  //  @86CF60

    void*               CreateNode(); // @86C770
    void                InheritFrom(EntityType* from); // @86CB40
    inline void         SetCreator(CREATOR creator)
    {
        m_Creator = creator;
    };

    void RegisterScript(const char* const scriptname, EntityFunctionMember scriptptr, const int a3 = 0, const int a4 = 0, const int a5 = 0, const char* const editorcontrolstr = nullptr, const char* const a7 = nullptr); // @86EC70
    void RegisterProperty(DataType* returntype, const char* const propertyname, EntityGetterFunction getterptr, const int a4, const int a5, const int a6, EntitySetterFunction setterptr, const int a8, const int a9, const int a10, const char* const editorcontrolstring, const int a12, const int a13, const int propertyind); // @86D370
    void RegisterProperty(DataType* returntype, const char* const propertyname, EntityGetterFunction getterptr, EntitySetterFunction setterptr, const char* const editorcontrolstring, const int propertyind = -1)
    {
        RegisterProperty(returntype, propertyname, getterptr, NULL, NULL, NULL, setterptr, NULL, NULL, NULL, editorcontrolstring, NULL, NULL, propertyind);
    }

    void                PropagateProperties(); // @86E9B0
    bool                HasPropertyId(const unsigned int propertyId) const;  //  @86C9E0
    DataType*           PropertyByIndex(const int index) const; //  @86CCB0
    void                ExecuteScript(Node* node) const;  //  @86CFF0

    static Entity*      IsParentOf(EntityType* ett, Entity* ent);   //  @48C3B0
};

ASSERT_CLASS_SIZE(EntityType, 120);