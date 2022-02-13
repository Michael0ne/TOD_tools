#pragma once
#include "StringsPool.h"
#include "Types.h"
#include <vector>
#include <map>

class DataType
{
public:
    enum ScriptTypeId
    {
        TYPE_NOTHING = 0,
        TYPE_NUMBER = 1,  // NOTE: float.
        TYPE_INTEGER = 2,
        TYPE_STRING = 3,  // NOTE: char*.
        TYPE_TRUTH = 4,   // NOTE: bool.
        TYPE_VECTOR = 5,  // NOTE: vector3<float>.
        TYPE_QUATERNION = 6, // NOTE: vector4<float>.
        TYPE_COLOR = 7,   // NOTE: vector4<float>.
        TYPE_LIST = 8,   // NOTE: std::vector.
        TYPE_DICT = 9,   // NOTE: std::map.
        TYPE_ENTITY = 10,  // NOTE: EntityType.
        TYPE_SCRIPT = 11,  // NOTE: ScriptType.
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
        TYPE_SCRIPT_SIZE = 1
    };

public:
    String          m_TypeName;
    ScriptTypeId    m_TypeId;
    unsigned int    m_Size;
    unsigned int    m_GlobalId;

    virtual         ~DataType(); // @867A70
    virtual int     stub2(int*, int*); // @489370  //  NOTE: increments the second argument's array value with index being this type's ID by type size.
    virtual void*   ReturnNew(void*) const; // NOTE: probably 'operator new'. Returns new instance of the type into the first argument. Placement new.
    virtual void    Delete(char*);  //  NOTE: destroys an object at first argument.
    virtual void    stub5(int*, int*); // @4893A0  //  NOTE: essentialy, an '=' operator overload, copies object into the second argument. Both arguments should be of the same type.
    virtual String& PrintFormattedValue(String&, void*, int) const; // @862F50 // NOTE: lowest word is number of digits after dot, highest - total number of digits.
    virtual int     StrToType(char*, void*) const; // @862AA0  //  NOTE: parse string and pass it to second argument as it's the underlying type.
    virtual int     stub8(char*); // @489410
    virtual int     stub9(char*, char*); // @863760    //  NOTE: copy first argument into second, store additional information in second argument.
    virtual int     Copy(char*, char*); // @863760
    virtual int     stub11(char*, String&, int); // @8637F0
    virtual int     stub12(char*, char*, int*); // @862A50
    virtual void    stub13(int, int (__thiscall*procptr)(void*, void*), int, int, int, void* const outResult) const;   //  NOTE: execute procptr and return result into result variable.
    virtual void    stub14(int*, int, void*, int, int, int) const;
    virtual bool    AreEqual(void*, void*) const; // @7A1F00
    virtual bool    NotEqualTo(void*, void*) const; // @862AB0
    virtual void    ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const; // @8637D0
    virtual void    PerformOperation(int operationId, void* params) const; // @8C4D60
    virtual char    stub19(int, int); // @8637E0
    virtual bool    IsValidValueForType(void*) const; // @489440

private:
    void   _4893C0(int*, int*, int); // @4893C0

    static void  RemoveTypeFromList(const char* const name); // @862B50
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

    unsigned int GetTypeSize() const; // @862B20

    static DataType* GetTypeByName(const char* name); // @862C00
    static DataType* LoadScript(const char* script); // @863070
    static bool  ParseVariableString(const char* variable, String& variableName, String& variableType); // @862F70
    static void  InitScriptTypes(); // @8634E0
    static void  ClearScriptLists(); // @863380
private:
    static void  ClearGlobalScriptList(); // @48CAA0
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