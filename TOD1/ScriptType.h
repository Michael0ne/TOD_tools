#pragma once
#include "BaseType.h"

//	NOTE: this is essentially a struct.
class ScriptType : public BaseType
{
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
	};

protected:
	String				m_Name;
	ScriptFieldsList	m_Fields;

public:
	ScriptType(const char* const name, const ScriptFieldsList& fields);	//	@7A1E90
};

ASSERT_CLASS_SIZE(ScriptType, 72);