#pragma once
#include "BaseType.h"

//	NOTE: this is essentially a struct.
class ScriptType : public BaseType
{
public:
	struct ScriptField
	{
		String			m_Name;
		BaseType*		m_Type = nullptr;
		unsigned int	m_FieldOffset = NULL;

		ScriptField(const char* name, BaseType* stype, unsigned int fldoffset);
		ScriptField(const ScriptField& rhs);	//	@7A1D40
	};

	struct ScriptFieldsList
	{
		std::vector<ScriptField>	m_List;

		unsigned int	m_TotalSizeBytes = NULL;
		unsigned int	m_TotalSize = NULL;

		void            Add(const char* const fieldname, BaseType* fieldtype, unsigned int);	//	@862550
		void            Clear();	//	@862540
	};

protected:
	String				m_Name;
	ScriptFieldsList	m_Fields;

public:
	ScriptType(const String& name, const ScriptFieldsList& fields);	//	@7A1E90

	virtual ~ScriptType() override;	//	@7A1F20
};

ASSERT_CLASS_SIZE(ScriptType, 72);