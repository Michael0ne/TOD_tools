#pragma once
#include "Node.h"

class CutscenePlayer : public Node
{
protected:
    int                 field_50;
    int                 field_54;
    std::vector<int>    m_RegisteredRootObjects;
    std::vector<int>    m_List_2;
    float               m_Speed;
    int                 field_7C;
    float               field_80;
    float               m_CamNearclip;
    union
    {
        struct
        {
            unsigned Playing : 1;
            unsigned _1 : 1;
            unsigned _2 : 1;
            unsigned _3 : 1;
            unsigned _4 : 1;
            unsigned _5 : 1;
            unsigned _6 : 1;
            unsigned _7 : 1;
            unsigned _8 : 1;
            unsigned _9 : 1;
            unsigned _10 : 1;
            unsigned _11 : 1;
            unsigned _12 : 1;
            unsigned _13 : 1;
            unsigned _14 : 1;
            unsigned _15 : 1;
            unsigned _16 : 1;
            unsigned _17 : 1;
            unsigned _18 : 1;
            unsigned _19 : 1;
            unsigned _20 : 1;
            unsigned _21 : 1;
            unsigned _22 : 1;
            unsigned _23 : 1;
            unsigned _24 : 1;
            unsigned Paused : 1;
            unsigned Looped : 1;
            unsigned OverrideCurrentCamera : 1;
            unsigned DefaultInherit : 1;
            unsigned _29 : 1;
            unsigned _30 : 1;
        };
    }       m_Flags;
    String              m_StopEvent;
    float               m_Timepos;
    String              m_ActiveCameraAnim;
    String              m_BeforeStartEvent;
    String              m_AfterStopEvent;
    int                 field_D0;
    int                 field_D4;
    float               m_CamDynlightsCullrange;
    String              m_String_5;
    int                 field_EC;
    int                 field_F0;
public:
    CutscenePlayer(); // @9170D0
    virtual ~CutscenePlayer();

private:
    void                Pause(const int);   //  @917890
};

extern EntityType* tCutscenePlayer;

ASSERT_CLASS_SIZE(CutscenePlayer, 244);