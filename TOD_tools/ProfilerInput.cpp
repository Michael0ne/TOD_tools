#include "ProfilerInput.h"

ProfilerInput::ProfilerInput()
{
	m_sProfilerScript = String();
	m_sCommand = String();

	m_nCurrentPage = 0;
	m_nCodeProfileMode = ABSOLUTE_;
	m_nTraverseProfileMode = TRAVERSE_TIME;
	m_nScriptProfileMode = ALL;
	m_nSortAscending = 0;
	m_nProfileType = TRAVERSE_PROFILE;
	m_nSortByColumn = NO_SORTING;
}