#pragma once

#include "stdafx.h"
#include "List.h"
#include "StringsPool.h"

#define DUMPTABLE_ELEMENT_LIST_SIZE 20

struct DumpTable_Item
{
	int		field_0;
	int		field_4;
	int		field_8;
	int		field_C;
	int		field_10;
};

//	Part of Builtin in code. For clarity it's here... for now.
class DumpTable
{
private:
	static		List<DumpTable_Item>&	DumpTableList;	//	@A0B3A4

public:
	static unsigned int					Create();		//	@486200	NOTE: returns new item index in dumptable list.
	static unsigned int					CreateFromFile(const String& sFileName);	//	@4862C0
	static void							AddIntegerColumn(unsigned int nColumnIndex, const String& sValue);	//	@4858C0
	static void							AddNumberColumn(unsigned int nColumnIndex, const String& sValue);	//	@4858F0
	static void							AddStringColumn(unsigned int nColumnIndex, const String& sValue);	//	@485920

};

static_assert(sizeof(DumpTable_Item) == DUMPTABLE_ELEMENT_LIST_SIZE, MESSAGE_WRONG_CLASS_SIZE("DumpTable_Item"));