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
void* ListType::stub3(void*) const
{
	return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::stub4(char*)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::stub5(int*, int*)
{
}

#pragma message(TODO_IMPLEMENTATION)
String& ListType::stub6(String& outstr, void*, int) const
{
	return outstr;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::stub7(char*, void*) const
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::stub9(char*, char*)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int ListType::stub10(char*, char*)
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
bool ListType::stub16(void*, void*) const
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::stub17(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
{
}

#pragma message(TODO_IMPLEMENTATION)
void ListType::stub18(int operationId, void* params) const
{
}

#pragma message(TODO_IMPLEMENTATION)
char ListType::stub19(int, int)
{
	return 0;
}
