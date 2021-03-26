#pragma once
#include "BaseType.h"

class ListType : public BaseType
{
protected:
	BaseType*		m_ListElementsType;
	unsigned int	m_ListElementSize;
	bool			m_ComplexType;

public:
	ListType(BaseType* elementsType);	//	@870630
	virtual ~ListType();
};