#pragma once

#include "ScriptTypes.h"

struct GlobalProperty
{
	int							m_PropertyId;
	const char*					m_PropertyName;
	ScriptTypes::ScriptType*	m_PropertyType;
};

static List<GlobalProperty>&	GlobalPropertiesList = *(List<GlobalProperty>*)0xA3CF20;	//	@A3CF20

struct ScriptNodeProperties
{
	String						m_PropertyName;
	ScriptTypes::ScriptType*	m_ScriptType;
	int							m_TotalSizeBytes;
};

struct GlobalCommand
{
	struct
	{
		List<ScriptNodeProperties>	m_PropertiesList;
		int						m_TotalSizeBytes;
		int						m_TotalSize;
	}							m_ArgumentsList;	//	NOTE: element at index 0 is always return scripttype.
	int							m_GlobalIndex;
	const char*					m_ArgumentsString;
	const char*					m_CommandName;
};

static List<GlobalCommand>&		GlobalCommandsList = *(List<GlobalCommand>*)0xA11470;	//	@A11470