#include "ScriptType.h"

ScriptType::ScriptType(const String& name, const ScriptFieldsList& fields) : DataType(TYPE_SCRIPT, name.m_Str, TYPE_SCRIPT_SIZE)
{
 MESSAGE_CLASS_CREATED(ScriptType);

 m_Name = name;
 m_Fields = fields;
 m_Fields.m_TotalSizeBytes = fields.m_TotalSizeBytes;
 m_Fields.m_TotalSize = fields.m_TotalSize;
}

ScriptType::~ScriptType()
{
 MESSAGE_CLASS_DESTROYED(ScriptType);
}

void ScriptType::ScriptFieldsList::Add(const char* const fieldname, DataType* fieldtype, unsigned int)
{
 m_List.push_back({ fieldname, fieldtype, m_TotalSizeBytes });
 m_TotalSizeBytes += fieldtype->m_Size;
 m_TotalSize += fieldtype->GetTypeSize();
}

void ScriptType::ScriptFieldsList::Clear()
{
 m_List.clear();
 m_TotalSizeBytes = 0;
 m_TotalSize = 0;
}