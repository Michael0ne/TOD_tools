#pragma once

#include "stdafx.h"
#include "StringsPool.h"
#include "Entity.h"

class MemoryCards
{
private:
	Entity entity;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	int field_8C;
	String m_sGameName;
	int m_nSaveFileSize;
};

static_assert(sizeof(MemoryCards) == 0xA4, MESSAGE_WRONG_CLASS_SIZE("MemoryCards"));