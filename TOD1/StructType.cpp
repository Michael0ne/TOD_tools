#include "StructType.h"

StructType::StructType(const String& name, const ScriptFieldsList& fields) : DataType(TYPE_STRUCT, name.m_Str, TYPE_STRUCT_SIZE)
{
    MESSAGE_CLASS_CREATED(StructType);

    m_Name = name;
    m_Fields = fields;
    m_Fields.m_TotalSizeBytes = fields.m_TotalSizeBytes;
    m_Fields.m_TotalSize = fields.m_TotalSize;
}

StructType::~StructType()
{
    MESSAGE_CLASS_DESTROYED(StructType);
}

int StructType::GetSize(int* dummy, int* list) const
{
    if (!m_Fields.m_List.size())
        return 0;

    int counter = 0;
    for (unsigned int i = 0; i < m_Fields.m_List.size(); ++i)
        counter += m_Fields.m_List[i].m_Type->GetSize((int*)(*dummy + m_Fields.m_List[i].m_FieldOffset * 4), list);

    return counter;
}

void StructType::ScriptFieldsList::Add(const char* const fieldname, DataType* fieldtype, unsigned int)
{
    m_List.push_back({ fieldname, fieldtype, m_TotalSizeBytes });
    m_TotalSizeBytes += fieldtype->Size;
    m_TotalSize += fieldtype->GetTypeSize();
}

void StructType::ScriptFieldsList::Clear()
{
    m_List.clear();
    m_TotalSizeBytes = 0;
    m_TotalSize = 0;
}