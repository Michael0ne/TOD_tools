#pragma once
#include "StringsPool.h"
#include <vector>

struct DumpTableDescription
{
 enum ColumnType
 {
  TYPE_INTEGER = 0,
  TYPE_NUMBER = 1,
  TYPE_STRING = 2
 };

 class Column
 {
  friend struct DumpTableDescription;
 protected:
  unsigned int    m_Type;
  bool            m_IsVisible; // NOTE: if false, it won't be in the dumped file.
  String          m_ColumnName;
  String          field_1C;
  int             field_2C;

 public:
  inline Column()
  {
   MESSAGE_CLASS_CREATED(Column);

   m_IsVisible = true;
  }
  virtual ~Column();
 };

 class IntegerColumn : public Column
 {
  friend struct DumpTableDescription;
 protected:
  int             field_30;
  int             field_34;
  std::vector<int> m_Values;

 public:
  IntegerColumn(); // @4059C0
  virtual ~IntegerColumn();
 };

 class StringColumn : public Column
 {
  friend struct DumpTableDescription;
 protected:
  int             field_30;
  std::vector<String> m_Values;

 public:
  StringColumn(); // @405A20
  virtual ~StringColumn();
 };

protected:
 std::vector<Column*> m_Columns;
 int                 field_10;
 unsigned int        m_NumRows;

public:
 DumpTableDescription(); // @4014E0
 DumpTableDescription(const char* const s); // @401E40

 int                 AddIntegerColumn(const char* const columnname, const char* const a2, const int a3, const char a4, const int a5); // @401870
 int                 AddNumberColumn(const char* const columnname, const char* const a2, const int a3, const int a4, const int a5); // @401980
 int                 AddStringColumn(const char* const columnname, const char* const a2, const int a3, const int a4); // @401A90
 void                SetNumRows(const int rows); // @401BA0
 void                SetIntegerValue(const int rowindex, const int columnindex, const int val); // @401570
 void                SetNumberValue(const int rowindex, const int columnindex, const float val); // @4015B0
 void                SetStringValue(const int rowindex, const int columnindex, const char* str); // @401630
 void    DumpContents(String& outstr, const int a2, const int a3, const int a4, const int a5); // @485A00
};

extern std::vector<DumpTableDescription*>  DumpTable; // @A0B3A4

ASSERT_CLASS_SIZE(DumpTableDescription, 24);
ASSERT_CLASS_SIZE(DumpTableDescription::IntegerColumn, 72);
ASSERT_CLASS_SIZE(DumpTableDescription::StringColumn, 68);