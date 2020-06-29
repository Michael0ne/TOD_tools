#pragma once

#include "stdafx.h"
#include "Entity.h"

class Fragment
{
public:
	Entity *m_pOwner;
	void* m_pUnkStructPtr;
	int field_8;
	const char* m_szName;
	time_t m_nUniqueId0;
	int m_nUniqueId1;

public:
	Fragment(const Entity* owner);	//	@87F1E0

	const char*		_GetResourcePath();		//	@851720
};

