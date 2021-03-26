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