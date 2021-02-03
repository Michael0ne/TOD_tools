#pragma once

#include "stdafx.h"

#define FRAGMENT_CLASS_SIZE 24

class Fragment
{
public:
	class Entity	*m_Owner;
	void*			m_ResourceInfo;
	int				field_8;
	const char*		m_szName;
	__int64			m_nUniqueId;

public:
	Fragment(const class Entity* owner);	//	@87F1E0

	const char*		_GetResourcePath();		//	@851720
};

static_assert(sizeof(Fragment) == FRAGMENT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Fragment));