#include "DumpTable.h"

std::vector<DumpTableDescription*>  DumpTable;

DumpTableDescription::Column::~Column()
{
	MESSAGE_CLASS_DESTROYED(Column);
}

DumpTableDescription::IntegerColumn::IntegerColumn()
{
	MESSAGE_CLASS_CREATED(IntegerColumn);
}

DumpTableDescription::IntegerColumn::~IntegerColumn()
{
	MESSAGE_CLASS_DESTROYED(IntegerColumn);
}

DumpTableDescription::StringColumn::StringColumn()
{
	MESSAGE_CLASS_CREATED(StringColumn);
}

DumpTableDescription::StringColumn::~StringColumn()
{
	MESSAGE_CLASS_DESTROYED(StringColumn);
}

DumpTableDescription::DumpTableDescription()
{
	MESSAGE_CLASS_CREATED(DumpTableDescription);

	m_NumRows = 0;
}

int DumpTableDescription::AddIntegerColumn(const char* const columnname, const char* const a2, const int a3, const char a4, const int a5)
{
	IntegerColumn* intclmn = new IntegerColumn;
	intclmn->field_2C = a3;
	intclmn->field_34 = a4;
	intclmn->m_ColumnName = columnname;
	intclmn->m_Type = TYPE_INTEGER;
	intclmn->field_30 = a5;
	intclmn->field_1C = a2;

	m_Columns.push_back(intclmn);

	return m_Columns.size() - 1;
}

int DumpTableDescription::AddNumberColumn(const char* const columnname, const char* const a2, const int a3, const int a4, const int a5)
{
	IntegerColumn* intclmn = new IntegerColumn;
	intclmn->field_2C = a3;
	intclmn->field_34 = a4;
	intclmn->m_ColumnName = columnname;
	intclmn->m_Type = TYPE_NUMBER;
	intclmn->field_30 = a5;
	intclmn->field_1C = a2;

	m_Columns.push_back(intclmn);

	return m_Columns.size() - 1;
}

int DumpTableDescription::AddStringColumn(const char* const columnname, const char* const a2, const int a3, const int a4)
{
	StringColumn* strclmn = new StringColumn;
	strclmn->field_2C = a3;
	strclmn->m_ColumnName = columnname;
	strclmn->m_Type = TYPE_STRING;
	strclmn->field_30 = a4;
	strclmn->field_1C = a2;

	m_Columns.push_back(strclmn);

	return m_Columns.size() - 1;
}

void DumpTableDescription::SetNumRows(const int rows)
{
	m_NumRows = rows;

	if (!m_Columns.size())
		return;

	for (std::vector<Column*>::iterator it = m_Columns.begin(); it != m_Columns.end(); ++it)
	{
		switch ((*it)->m_Type)
		{
		case TYPE_INTEGER:
			((IntegerColumn*)(*it))->m_Values.push_back(-1);
			break;
		case TYPE_NUMBER:
			((IntegerColumn*)(*it))->m_Values.push_back((int)-1.f);
			break;
		case TYPE_STRING:
			((StringColumn*)(*it))->m_Values.push_back("n/a");
			break;
		}
	}
}

void DumpTableDescription::SetIntegerValue(const int rowindex, const int columnindex, const int val)
{
	((IntegerColumn*)m_Columns[columnindex])->m_Values[rowindex] = val;
}

void DumpTableDescription::SetNumberValue(const int rowindex, const int columnindex, const float val)
{
	((IntegerColumn*)m_Columns[columnindex])->m_Values[rowindex] = (int)val;
}

void DumpTableDescription::SetStringValue(const int rowindex, const int columnindex, const char* str)
{
	((StringColumn*)m_Columns[columnindex])->m_Values[rowindex] = str;
}

#pragma message(TODO_IMPLEMENTATION)
void DumpTableDescription::DumpContents(String& outstr, const int a2, const int a3, const int a4, const int a5)
{
	for (std::vector<Column*>::const_iterator it = m_Columns.cbegin(); it != m_Columns.cend(); ++it)
	{
		char buf[128] = {};
		sprintf(buf, "%s-%ds%s", (*it)->m_ColumnName.m_szString, (*it)->field_2C, it + 1 != m_Columns.cend() ? ";" : "");
		outstr.Append(buf);
	}

	//	TODO: output actual rows content.
}