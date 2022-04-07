#include "ProfilerInput.h"

EntityType* tProfilerInput;

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

void ProfilerInput::Register()
{
    tProfilerInput = new EntityType("ProfilerInput");
    tProfilerInput->InheritFrom(tNode);
    tProfilerInput->SetCreator((CREATOR)Create);

    tProfilerInput->RegisterProperty(tTRUTH, "psep1", (EntityGetterFunction)&GetPsep1, (EntitySetterFunction)&SetPsep1, "control=drawline|name=Main Profiler Input Settings");
    tProfilerInput->RegisterProperty(tINTEGER, "profiletype", (EntityGetterFunction)&GetProfileType, (EntitySetterFunction)&SetProfileType, "control=dropdown|Code_Profile=0|Traverse_Profile=1|Scripts_Profile=2|Custom_Profile=3");
    tProfilerInput->RegisterProperty(tINTEGER, "currentpage", (EntityGetterFunction)&GetCurrentPage, (EntitySetterFunction)&SetCurrentPage, "control=slider|min=1|max=50");
    tProfilerInput->RegisterProperty(tINTEGER, "linesperpage", (EntityGetterFunction)&GetLinesPerPage, (EntitySetterFunction)&SetLinesPerPage, "control=slider|min=5|max=50");
    tProfilerInput->RegisterProperty(tINTEGER, "sortbycolumn", (EntityGetterFunction)&GetSortByColumn, (EntitySetterFunction)&SetSortByColumn, "control=dropdown|No_sorting=-1|Column_1=0|Column_2=1|Column_3=2|Column_4=3|Column_5=4|Column_6=5|Column_7=6");
    tProfilerInput->RegisterProperty(tTRUTH, "sortascending", (EntityGetterFunction)&SortAscending, (EntitySetterFunction)&SetSortAscending, "control=truth|name=");

    tProfilerInput->RegisterProperty(tTRUTH, "psep6", (EntityGetterFunction)&GetPsep6, (EntitySetterFunction)&SetPsep6, "control=drawline|name=Output Window Settings");
    tProfilerInput->RegisterProperty(tTRUTH, "enableoutput", (EntityGetterFunction)&OutputEnabled, (EntitySetterFunction)&EnableOutput, "control=truth|name=");
    tProfilerInput->RegisterProperty(tNUMBER, "profopacity", (EntityGetterFunction)&GetProfOpacity, (EntitySetterFunction)&SetProfOpacity, "control=slider|min=0|max=1");

    tProfilerInput->RegisterProperty(tTRUTH, "psep2", (EntityGetterFunction)&GetPsep2, (EntitySetterFunction)&SetPsep2, "control=drawline|name=Code Profiler Settings");
    tProfilerInput->RegisterProperty(tINTEGER, "codeprofilemode", (EntityGetterFunction)&GetCodeProfileMode, (EntitySetterFunction)&SetCodeProfileMode, "control=dropdown|Absolute=0|Percentage=1|Clock_Cycles=2|Fast=3");

    tProfilerInput->RegisterProperty(tTRUTH, "psep3", (EntityGetterFunction)&GetPsep3, (EntitySetterFunction)&SetPsep3, "control=drawline|name=Traverse Profiler Settings");
    tProfilerInput->RegisterProperty(tINTEGER, "traverseprofilemode", (EntityGetterFunction)&GetTraverseProfileMode, (EntitySetterFunction)&SetTraverseProfileMode, "control=dropdown|Traverse_Time=0|Script_Time=1|Render_Time=2|CALCLod_Time=3");

    tProfilerInput->RegisterProperty(tTRUTH, "psep4", (EntityGetterFunction)&GetPsep4, (EntitySetterFunction)&SetPsep4, "control=drawline|name=Scripts Profiler Settings");
    tProfilerInput->RegisterProperty(tINTEGER, "scriptprofilemode", (EntityGetterFunction)&GetScriptProfileMode, (EntitySetterFunction)&SetScriptProfileMode, "control=dropdown|All=0|Specific_script=1");
    tProfilerInput->RegisterProperty(tSTRING, "profilescript", (EntityGetterFunction)&GetProfileScript, (EntitySetterFunction)&SetProfileScript, "control=string");

    tProfilerInput->RegisterProperty(tTRUTH, "psep5", (EntityGetterFunction)&GetPsep5, (EntitySetterFunction)&SetPsep5, "control=drawline|name=Custom Profiler Settings");
    tProfilerInput->RegisterProperty(tSTRING, "command", (EntityGetterFunction)&GetCommand, (EntitySetterFunction)&SetCommand, "control=string");

    tProfilerInput->RegisterScript("go", (EntityFunctionMember)&Go, 0, 0, 0, "control=button|text=go");
    tProfilerInput->RegisterScript("resethistory", (EntityFunctionMember)&ResetHistory, 0, 0, 0, "control=button|text=ResetHistory");

    tProfilerInput->PropagateProperties();
}

ProfilerInput* ProfilerInput::Create(AllocatorIndex)
{
    return new ProfilerInput;
}