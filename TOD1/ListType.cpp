#include "ListType.h"

ListType::ListType(DataType* elementsType) : DataType(TYPE_LIST, "list", TYPE_LIST_SIZE)
{
    MESSAGE_CLASS_CREATED(ListType);

    m_ListElementsType = elementsType;
    m_ListElementSize = elementsType->m_Size;

    char str[64] = {};
    sprintf(str, "list(%s)", elementsType->m_TypeName.m_Str);

    m_ComplexType = (elementsType->m_TypeId == TYPE_STRING || elementsType->m_TypeId == TYPE_LIST || elementsType->m_TypeId == TYPE_DICT || elementsType->m_TypeId == TYPE_SCRIPT);
}

ListType::~ListType()
{
    MESSAGE_CLASS_DESTROYED(ListType);
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::stub2(int*, int*)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
void* ListType::ReturnNew(void*) const
{
    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::Delete(char*)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::stub5(int*, int*)
{
}

#pragma message(TODO_IMPLEMENTATION)
String& ListType::PrintFormattedValue(String& outstr, void*, int) const
{
    return outstr;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::StrToType(char*, void*) const
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::stub9(char*, char*)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::Copy(char*, char*)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::stub11(char*, String&, int)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::stub12(char*, char*, int*)
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
bool ListType::NotEqualTo(void*, void*) const
{
    return false;
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
{
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::PerformOperation(int operationId, void* params) const
{
}

#pragma message(TODO_IMPLEMENTATION)
char ListType::IsReferenced(int, int)
{
    return 0;
}
