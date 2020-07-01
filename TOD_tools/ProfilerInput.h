#pragma once

#include "stdafx.h"
#include "StringsPool.h"

#define PROFILER_INPUT_CLASS_SIZE 140

enum E_PROFILER_PROFILE_TYPE {
	CODE_PROFILE = 0,
	TRAVERSE_PROFILE = 1,
	SCRIPTS_PROFILE = 2,
	CUSTOM_PROFILE = 3
};

enum E_PROFILER_SORT_BY_COLUMN {
	NO_SORTING = -1,
	COLUMN_1 = 0,
	COLUMN_2 = 1,
	COLUMN_3 = 2,
	COLUMN_4 = 3,
	COLUMN_5 = 4,
	COLUMN_6 = 5,
	COLUMN_7 = 6
};

enum E_PROFILER_CODE_PROFILE_MODE {
	ABSOLUTE_ = 0,
	PERCENTAGE = 1,
	CLOCK_CYCLES = 2,
	FAST = 3
};

enum E_PROFILER_TRAVERSE_PROFILE_MODE {
	TRAVERSE_TIME = 0,
	SCRIPT_TIME = 1,
	RENDER_TIME = 2,
	CALCLOD_TIME = 3
};

enum E_PROFILER_SCRIPT_PROFILE_MODE {
	ALL = 0,
	SPECIFIC_SCRIPT = 1
};

class ProfilerInput
{
private:
	void* lpVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	void* lpPositionVtbl;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	E_PROFILER_PROFILE_TYPE m_nProfileType;
	int m_nCurrentPage;
	E_PROFILER_CODE_PROFILE_MODE m_nCodeProfileMode;
	E_PROFILER_TRAVERSE_PROFILE_MODE m_nTraverseProfileMode;
	E_PROFILER_SCRIPT_PROFILE_MODE  m_nScriptProfileMode;
	String m_sProfilerScript;
	String m_sCommand;
	E_PROFILER_SORT_BY_COLUMN m_nSortByColumn;
	int m_nSortAscending;
public:
	ProfilerInput();	//	@929450
};

static_assert(sizeof(ProfilerInput) == PROFILER_INPUT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ProfilerInput));