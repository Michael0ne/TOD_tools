#pragma once

#include "stdafx.h"

class Node;

class Fragment
{
public:
	Node *m_pOwner;
	void* field_4;
	int field_8;
	const char* m_szName;
	time_t m_nUniqueId0;
	int m_nUniqueId1;

public:
	const char*		_GetResourcePath();		//	@851720
};

