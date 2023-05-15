#pragma once
#include "DataType.h"
#include "ScriptDatabase.h"

typedef void* (*CREATOR)(AllocatorIndex);

class EntityType : public DataType
{
    friend class Node;
    friend class Entity;

    struct ScriptInfo
    {
        EntityFunctionMember    ScriptPtr = nullptr;
        uint32_t                _f4 = 0;    //  NOTE: this one and below are related to virtual function offset.
        uint32_t                _f8 = 0;
        uint32_t                VirtualFunctionOffset = 0;

        ScriptInfo()
        {};

        ScriptInfo(const EntityFunctionMember scriptptr, const uint32_t a2, const uint32_t a3, const uint32_t vmtoffset)
        {
            ScriptPtr = scriptptr;
            _f4 = a2;
            _f8 = a3;
            VirtualFunctionOffset = vmtoffset;
        }
    };

    struct PropertyInfo
    {
        DataType*               ReturnType = nullptr;
        int*                    _f4 = nullptr;
        EntityGetterFunction    Getter = nullptr;
        int                     _fC = 0;
        int                     _f10 = 0;
        int                     _f14 = 0;
        EntitySetterFunction    Setter = nullptr;
        int                     _f1C = 0;
        int                     _f20 = 0;
        int                     _f24 = 0;
        uint16_t                GlobalIndex = 0;
        uint16_t                LocalIndex = 0;
        int                     field_2C = 0;

        PropertyInfo()
        {};

        inline PropertyInfo(EntityGetterFunction getter, int _fC, int _f10, int _f14, EntitySetterFunction setter, DataType* rettype, int _f1C, int _f20, int _f24)
        {
            Getter = getter;
            _fC = _fC;
            _f10 = _f10;
            _f14 = _f14;
            Setter = setter;
            ReturnType = rettype;
            _f1C = _f1C;
            _f20 = _f20;
            _f24 = _f24;
        }
    };

public:
    CREATOR                                     Creator;
    EntityType                                 *Parent;
    Scriptbaked                                *Script;

protected:
    std::map<uint16_t, ScriptInfo*> ScriptsList; // NOTE: each 'derived' script derives it's parent scripts.
    uint32_t                        _f38;
    std::map<uint16_t, uint16_t>    PropertiesMappings;
    uint32_t                        _f48;
    std::vector<PropertyInfo>       LocalPropertiesList;
    std::vector<PropertyInfo>       GlobalPropertiesList;
    int32_t                         TotalLocalProperties;
    uint32_t                        TotalGlobalProperties;
    bool                            IsBaseEntity;

public:
    EntityType(const char* const entityname); // @86CC00
    virtual             ~EntityType(); // @4886C0
    virtual char        IsReferenced(int* a1, int a2) override;  //  @86CF60

    void*               CreateNode(); // @86C770
    void                InheritFrom(EntityType* from); // @86CB40
    inline void         SetCreator(CREATOR creator)
    {
        Creator = creator;
    };

    void RegisterScript(const char* const scriptname, EntityFunctionMember scriptptr, const int a3 = 0, const int a4 = 0, const int a5 = 0, const char* const editorcontrolstr = nullptr, const char* const a7 = nullptr); // @86EC70
    void RegisterProperty(DataType* returntype, const char* const propertyname, EntityGetterFunction getterptr, const int a4, const int a5, const int a6, EntitySetterFunction setterptr, const int a8, const int a9, const int a10, const char* const editorcontrolstring, const int a12, const int a13, const int propertyind); // @86D370
    void RegisterProperty(DataType* returntype, const char* const propertyname, EntityGetterFunction getterptr, EntitySetterFunction setterptr, const char* const editorcontrolstring, const int propertyind = -1)
    {
        RegisterProperty(returntype, propertyname, getterptr, NULL, NULL, NULL, setterptr, NULL, NULL, NULL, editorcontrolstring, NULL, NULL, propertyind);
    }

    void                GetPropertyValue(const Node* callerNode, int* nodeParameters, const int propertyId, int* outPropertyValue) const; //  @86C020
    void                PropagateProperties(); // @86E9B0
    bool                HasPropertyId(const unsigned int propertyId) const;  //  @86C9E0
    DataType*           PropertyByIndex(const int index) const; //  @86CCB0
    void                ExecuteScript(Node* node) const;  //  @86CFF0

    static Entity*      IsParentOf(EntityType* ett, Entity* ent);   //  @48C3B0
};

ASSERT_CLASS_SIZE(EntityType, 120);