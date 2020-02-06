#pragma once

#include "stdafx.h"
#include "KapowStringsPool.h"

class ResourceTypeInfo
{
	int *(__cdecl *m_pCreator)();
	String m_sTypename;
	void *lpTypeVtbl;
	int m_nUnkTypeId;
	String m_sExtension;
	byte field_2C[4];
	int m_UnkBufferArray[3];
public:
};

