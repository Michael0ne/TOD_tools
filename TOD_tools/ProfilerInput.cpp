#include "ProfilerInput.h"

ProfilerInput::ProfilerInput() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(ProfilerInput);

	m_ProfilerScript = String();
	m_Command = String();

	m_CurrentPage = 0;
	m_CodeProfileMode = ABSOLUTE_;
	m_TraverseProfileMode = TRAVERSE_TIME;
	m_ScriptProfileMode = ALL;
	m_SortAscending = false;
	m_ProfileType = TRAVERSE_PROFILE;
	m_SortByColumn = NO_SORTING;
}