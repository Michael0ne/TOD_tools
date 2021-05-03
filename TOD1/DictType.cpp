#include "DictType.h"

DictType::DictType(BaseType* elementsType) : BaseType(TYPE_DICT, "dict", TYPE_DICT_SIZE)
{
	MESSAGE_CLASS_CREATED(DictType);

	m_ElementsType = elementsType;

	char dtname[64] = {};
	sprintf(dtname, "dict(%s)", elementsType->m_TypeName.m_szString);
	m_TypeName = dtname;
}

DictType::~DictType()
{
	MESSAGE_CLASS_DESTROYED(DictType);
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::stub2(int*, int*)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
void* DictType::stub3(void*) const
{
	return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::stub4(char*)
{
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::stub5(int*, int*)
{
}

#pragma message(TODO_IMPLEMENTATION)
String& DictType::stub6(String& outstr, void*, int) const
{
	return outstr;
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::stub7(char*, void*) const
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::stub9(char*, char*)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int DictType::stub10(char*, char*)
{
	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
bool DictType::stub16(void*, void*) const
{
	return false;
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::stub17(const char* const operation, int* outopid, BaseType** outoprestype, char* a4) const
{
}

#pragma message(TODO_IMPLEMENTATION)
void DictType::stub18(int operationId, void* params) const
{
}

#pragma message(TODO_IMPLEMENTATION)
char DictType::stub19(int, int)
{
	return 0;
}
