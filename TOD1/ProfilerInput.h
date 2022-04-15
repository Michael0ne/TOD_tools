#pragma once
#include "Node.h"

enum class eProfileType
{
    CODE_PROFILE        = 0,
    TRAVERSE_PROFILE    = 1,
    SCRIPTS_PROFILE     = 2,
    CUSTOM_PROFILE      = 3
};

enum class eColumnSortType
{
    NO_SORTING          = -1,
    COLUMN_1            = 0,
    COLUMN_2            = 1,
    COLUMN_3            = 2,
    COLUMN_4            = 3,
    COLUMN_5            = 4,
    COLUMN_6            = 5,
    COLUMN_7            = 6
};

enum class eCodeProfileMode
{
    MODE_ABSOLUTE       = 0,
    MODE_PERCENTAGE     = 1,
    MODE_CLOCK_CYCLES   = 2,
    MODE_FAST           = 3
};

enum class eTraverseProfileMode
{
    TRAVERSE_TIME       = 0,
    SCRIPT_TIME         = 1,
    RENDER_TIME         = 2,
    CALCLOD_TIME        = 3
};

enum class eScriptProfileMode
{
    ALL                 = 0,
    SPECIFIC_SCRIPT     = 1
};

class ProfilerInput : public Node
{
protected:
    eProfileType            m_ProfileType;
    int                     m_CurrentPage;
    eCodeProfileMode        m_CodeProfileMode;
    eTraverseProfileMode    m_TraverseProfileMode;
    eScriptProfileMode      m_ScriptProfileMode;
    String                  m_ProfilerScript;
    String                  m_Command;
    eColumnSortType         m_SortByColumn;
    bool                    m_SortAscending;

public:
    ProfilerInput(); // @929450

    const bool              GetPsep1() const;   //  @9293E0
    void                    SetPsep1(const int);    //  @883EC0
    const eProfileType      GetProfileType() const; //  @8F92C0
    void                    SetProfileType(const eProfileType ptype);   //  @8CC950
    const int               GetCurrentPage() const; //  @8A9B10
    void                    SetCurrentPage(const int page); //  @929AF0
    const int               GetLinesPerPage() const;    //  @4783C0
    void                    SetLinesPerPage(const int lpp); //  @883EC0
    const eColumnSortType   GetSortByColumn() const;    //  @4A66A0
    void                    SetSortByColumn(const eColumnSortType sorttype);    //  @883EC0
    const bool              SortAscending() const;  //  @929400
    void                    SetSortAscending(const bool sortascending); //  @929410
    const bool              GetPsep6() const;   //  @9293E0
    void                    SetPsep6(const bool);    //  @883EC0
    const bool              EnableOutput() const;   //  @929420
    void                    SetEnableOutput(const bool enabled);    //  @929430
    const float             GetProfOpacity() const; //  @9293F0
    void                    SetProfOpacity(const float opacity);    //  @883EC0
    const bool              GetPsep2() const;   //  @9293E0
    void                    SetPsep2(const int);    //  @883EC0
    const eCodeProfileMode  GetCodeProfileMode() const; //  @900F90
    void                    SetCodeProfileMode(const eCodeProfileMode cprofmode);   //  @9057D0
    const bool              GetPsep3() const;   //  @9293E0
    void                    SetPsep3(const int);    //  @883EC0
    const eTraverseProfileMode  GetTraverseProfileMode() const; //  @4B3FB0
    void                    SetTraverseProfileMode(const eTraverseProfileMode tprofmode);   //  @929B00
    const bool              GetPsep4() const;   //  @9293E0
    void                    SetPsep4(const int);    //  @883EC0
    const eScriptProfileMode    GetScriptProfileMode() const;   //  @8E3700
    void                    SetScriptProfileMode(const eScriptProfileMode sprofmode);   //  @8E3710
    const char* const       GetProfileScript() const;   //  @5A1B00
    void                    SetProfileScript(const char* const profscript); //  @929BE0
    const bool              GetPsep5() const;   //  @9293E0
    void                    SetPsep5(const int);    //  @883EC0
    const char* const       GetCommand() const; //  @8A6D20
    void                    SetCommand(const char* const cmd);  //  @929B90

    void                    Go(int* args);  //  @883EC0
    void                    ResetHistory(int* args);    //  @883EC0

    static void             Register(); //  @9294C0

private:
    static ProfilerInput*   Create(AllocatorIndex); //  @929B10
};

extern EntityType* tProfilerInput;  //  @A3E17C

ASSERT_CLASS_SIZE(ProfilerInput, 140);