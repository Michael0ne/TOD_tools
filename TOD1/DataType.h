#pragma once
#include "StringsPool.h"
#include "Types.h"
#include <vector>
#include <map>

class Entity;
class Node;

typedef void (Entity::* EntityFunctionMember)(void*);
typedef void* (Entity::* EntityGetterFunction)(void);
typedef void (Entity::* EntitySetterFunction)(const void*);

//  NOTE: original class path is '/kernel/database/datatype.h".
class DataType
{
public:
    enum ScriptTypeId
    {
        TYPE_NOTHING = 0,
        TYPE_NUMBER = 1,  // NOTE: float.
        TYPE_INTEGER = 2,   //  NOTE: int.
        TYPE_STRING = 3,  // NOTE: char*.
        TYPE_TRUTH = 4,   // NOTE: bool.
        TYPE_VECTOR = 5,  // NOTE: vector3<float>.
        TYPE_QUATERNION = 6, // NOTE: vector4<float>.
        TYPE_COLOR = 7,   // NOTE: vector4<float>.
        TYPE_LIST = 8,   // NOTE: std::vector.
        TYPE_DICT = 9,   // NOTE: std::map.
        TYPE_ENTITY = 10,  // NOTE: EntityType.
        TYPE_STRUCT = 11,  // NOTE: StructType.
        TYPE_LAST_DONTUSE
    };
    enum ScriptTypeSize
    {
        TYPE_NOTHING_SIZE = 0,
        TYPE_NUMBER_SIZE = 1,
        TYPE_INTEGER_SIZE = 1,
        TYPE_STRING_SIZE = 1,
        TYPE_TRUTH_SIZE = 1,
        TYPE_VECTOR_SIZE = 3,
        TYPE_QUATERNION_SIZE = 4,
        TYPE_COLOR_SIZE = 1,
        TYPE_LIST_SIZE = 1,
        TYPE_DICT_SIZE = 1,
        TYPE_ENTITY_SIZE = 1,
        TYPE_STRUCT_SIZE = 1
    };

public:
    String          m_TypeName;
    ScriptTypeId    m_TypeId;
    unsigned int    m_Size;
    unsigned int    m_GlobalId;

    virtual         ~DataType(); // @867A70
    virtual int     GetSize(int* dummy, int* list) const; // @489370  //  NOTE: increments the second argument's array value with index being this type's ID by type size.
    virtual void*   ReturnNew(void* outdataptr) const; // NOTE: probably 'operator new'. Returns new instance of the type into the first argument. Placement new.
    virtual void    Delete(char* dataptr);  //  NOTE: destroys an object at first argument.
    virtual void    Clone(const int* from, int* to); // @4893A0  //  NOTE: essentialy, an '=' operator overload, copies object into the second argument. Both arguments should be of the same type.
    virtual String& PrintFormattedValue(String&, void*, int) const; // @862F50 // NOTE: lowest word is number of digits after dot, highest - total number of digits.
    virtual int     MakeFromString(const char* const input, char* const outdata) const; // @862AA0  //  NOTE: parse input string and pass it to second argument as it's the underlying type. Returns the number of digits in the input string.
    virtual int     MakeCopy(char* to); // @489410    //  NOTE: returns size of the copied object. This is a 'generic' version of the copy.
    virtual int     CopyNoAllocate(const char* const from, char* to); // @863760    //  NOTE: copy first argument into second, store additional information in second argument. Space in second argument should be allocated.
    virtual int     CopyAndAllocate(const char* const from, char* to); // @863760  //  NOTE: another version of the 'copy', but space is allocated for the second argument first.
    virtual int     AsString(const char* const from, String& outString, int format); // @8637F0 //  NOTE: print formatted data-specific value, this one creates a copy of an object.
    virtual int     MakeFromString_A(const char* const inputstr, char* outtype, int* const outsize); // @862A50 //  NOTE: same as 'strtotype' but makes a copy of already existing object found using input string.
    virtual void    CallGetterFunction(Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, int* const outResult) const;   //  NOTE: execute procptr and return result into result variable.
    virtual void    CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const;
    virtual bool    AreEqual(const void* const arg1, const void* const arg2) const; // @7A1F00
    virtual bool    NotEqualTo(const void* const arg1, const void* const arg2) const; // @862AB0
    virtual void    ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const; // @8637D0
    virtual void    PerformOperation(int operationId, void* params) const; // @8C4D60
    virtual char    IsReferenced(int* a1, int); // @8637E0
    virtual bool    IsValidValueForType(const void* value) const; // @489440

private:
    static void     RemoveTypeFromList(const char* const name); // @862B50
    static unsigned int GetTypeSize_Impl(const DataType* type); // @862AC0

protected:
    static int  ParseFloatNumberString(const char* const, float* const); // @8628C0
    static int  ParseNumberString(const char* const, int* const); // @862850

public:
    DataType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize); // @862E90

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete (void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    unsigned int        GetTypeSize() const; // @862B20

    static const bool   IsSimpleType(const DataType* t); //  @8627C0
    static void         CopyValue(int* to, const int* from, const size_t size); // @4893C0
    static DataType*    GetTypeByName(const char* name); // @862C00
    static DataType*    LoadScript(const char* script); // @863070
    static bool         ParseVariableString(const char* variable, String& variableName, String& variableType); // @862F70
    static void         InitScriptTypes(); // @8634E0
    static void         ClearScriptLists(); // @863380
private:
    static void         ClearGlobalScriptList(); // @48CAA0
public:
    static unsigned int GetTypesListChecksum(); // @862CF0
    static unsigned int GetTypesListSize(); // @862B30
    static class EntityType* GetScriptEntityByName(const char* name); // @862C70
};

extern class NothingType* tNOTHING; // @A3CE94
extern class NumberType* tNUMBER; // @A3CEC0
extern class IntegerType* tINTEGER; // @A3CEB8
extern class TruthType* tTRUTH; // @A3CEC4
extern class VectorType* tVECTOR; // @A3CEB4
extern class QuaternionType* tQUATERNION; // @A3CE98
extern class ColorType* tCOLOR; // @A3CEA4
extern class StringType* tSTRING; // @A3CEB0

extern bool      TypesListCRCCalculated;  // @A3CEC8
extern std::vector<DataType*> TypesList;  // @A3CECC
extern unsigned int    TypesListCRC;    // @A3CEDC
extern float     _A3A064; // @A3A064

ASSERT_CLASS_SIZE(DataType, 32);