#pragma once
#include "StringsPool.h"
#include <vector>

struct DumpTableDescription
{
	class Column
	{
	public:
		Column();
		virtual ~Column();
	};

	class IntegerColumn : public Column
	{
	protected:
		unsigned int	field_4;
		char			field_8;
		String			m_ColumnName;
	};

protected:
	std::vector<Column*>	m_Columns;
	int				field_10;
	unsigned int	m_NumRows;

public:
	DumpTableDescription();	//	@4014E0
	DumpTableDescription(const char* const s);	//	@401E40
};

extern std::vector<DumpTableDescription>	DumpTable;	//	@A0B3A4

ASSERT_CLASS_SIZE(DumpTableDescription, 24);