#include "DictType.h"

DictType::DictType(DataType* elementsType) : DataType(TYPE_DICT, "dict", TYPE_DICT_SIZE)
{
    MESSAGE_CLASS_CREATED(DictType);

    m_ElementsType = elementsType;

    char dtname[64] = {};
    sprintf(dtname, "dict(%s)", elementsType->TypeName.m_Str);
    TypeName = dtname;
}

DictType::~DictType()
{
    MESSAGE_CLASS_DESTROYED(DictType);
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::GetSize(int*, int*)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
void* DictType::ReturnNew(void*) const
{
    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::Delete(char*)
{
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::Clone(int*, int*)
{
}

#pragma message(TODO_IMPLEMENTATION)
String& DictType::PrintFormattedValue(String& outstr, void*, int) const
{
    return outstr;
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::MakeFromString(char*, void*) const
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::CopyNoAllocate(char*, char*)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::CopyAndAllocate(char*, char*)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
bool DictType::NotEqualTo(void*, void*) const
{
    return false;
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
{
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::PerformOperation(int operationId, void* params) const
{
}

#pragma message(TODO_IMPLEMENTATION)
char DictType::IsReferenced(int, int)
{
    return 0;
}