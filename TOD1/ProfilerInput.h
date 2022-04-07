#pragma once

#include "Node.h"

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

class ProfilerInput : public Node
{
protected:
    E_PROFILER_PROFILE_TYPE m_ProfileType;
    int m_CurrentPage;
    E_PROFILER_CODE_PROFILE_MODE m_CodeProfileMode;
    E_PROFILER_TRAVERSE_PROFILE_MODE m_TraverseProfileMode;
    E_PROFILER_SCRIPT_PROFILE_MODE m_ScriptProfileMode;
    String m_ProfilerScript;
    String m_Command;
    E_PROFILER_SORT_BY_COLUMN m_SortByColumn;
    bool m_SortAscending;

public:
    ProfilerInput(); // @929450

    static void             Register(); //  @9294C0

private:
    static ProfilerInput*   Create(AllocatorIndex); //  @929B10
};

extern EntityType* tProfilerInput;  //  @A3E17C

static_assert(sizeof(ProfilerInput) == PROFILER_INPUT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ProfilerInput));