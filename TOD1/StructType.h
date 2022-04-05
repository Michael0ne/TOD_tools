#pragma once
#include "DataType.h"

class StructType : public DataType
{
public:
    struct StructField
    {
        String   m_Name;
        DataType* m_Type = nullptr;
        unsigned int m_FieldOffset = NULL;

        StructField(const char* name, DataType* stype, unsigned int fldoffset)
            : m_Name(name), m_Type(stype), m_FieldOffset(fldoffset)
        {};
        StructField(const StructField& rhs) // @7A1D40
            : m_Name(rhs.m_Name), m_Type(rhs.m_Type), m_FieldOffset(rhs.m_FieldOffset)
        {};
    };

    struct ScriptFieldsList
    {
        std::vector<StructField> m_List;

        unsigned int m_TotalSizeBytes = NULL;
        unsigned int m_TotalSize = NULL;

        void            Add(const char* const fieldname, DataType* fieldtype, unsigned int); // @862550
        void            Clear(); // @862540
    };

protected:
    String    m_Name;
    ScriptFieldsList m_Fields;

public:
    StructType(const String& name, const ScriptFieldsList& fields); // @7A1E90

    virtual ~StructType() override; // @7A1F20
    virtual int GetSize(int* dummy, int* list) const override;   //  @87AAD0
};

ASSERT_CLASS_SIZE(StructType, 72);