#include "ListType.h"

ListType::ListType(BaseType* elementsType) : BaseType(TYPE_LIST, "list", TYPE_LIST_SIZE)
{
	MESSAGE_CLASS_CREATED(ListType);

	m_ListElementsType = elementsType;
	m_ListElementSize = elementsType->m_Size;

	char str[64] = {};
	sprintf(str, "list(%s)", elementsType->m_TypeName.m_szString);
	
	m_ComplexType = (elementsType->m_TypeId == TYPE_STRING || elementsType->m_TypeId == TYPE_LIST || elementsType->m_TypeId == TYPE_DICT || elementsType->m_TypeId == TYPE_SCRIPT);
}