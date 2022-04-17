#pragma once
#include "Node.h"

class CutscenePlayer : public Node
{
public:
    struct CutsceneRootObject
    {
        int field_0;
        int field_4;
        int field_8;
        int field_C;
        int field_10;
        int field_14;
        int field_18;
        int field_1C;
        int field_20;
        int field_24;
        int field_28;
        int field_2C;
        int field_30;
        int field_34;
        int field_38;
        int field_3C;
        int field_40;
        int field_44;
        int field_48;
    };

    struct CutsceneObject
    {
        int field_0;
        int field_4;
        int field_8;
        int field_C;
        int field_10;
        int field_14;
        int field_18;
        int field_1C;
        int field_20;
        int field_24;
        int field_28;
        int field_2C;
        Node* m_NodePtr;
        int m_Inherit;
    };

    int                 field_50;
    int                 field_54;
    std::vector<CutsceneRootObject>    m_RegisteredRootObjects;
    std::vector<CutsceneObject>    m_CutsceneObjects;
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

    const char* const       GetCutscene() const;    //  @916F20
    void                    SetCutscene(const char* const cutscene);    //  @916F10
    const bool              IsPlaying() const;  //  @916E40
    const bool              IsLooped() const;   //  @916E50
    void                    SetIsLooped(const bool looped); //  @916E60
    const float             GetTimePos() const; //  @916DF0
    void                    SetTimePos(const float timepos);    //  @88AAE0
    const int               GetFramePos() const;    //  @916E00
    void                    SetFramePos(const int framepos);    //  @917830
    const float             GetSpeed() const;   //  @905870
    void                    SetSpeed(const float speed);    //  @916E20
    const float             GetCamNearClip() const; //  @916E30
    void                    SetCamNearClip(const float nearclip);   //  @8D5060
    const float             GetCamDynLightsCullRange() const;   //  @916ED0
    void                    SetCamDynLightsCullRange(const float cullrange);    //  @916EB0
    const bool              OverrideCurrentCamera() const;  //  @916EA0
    void                    SetOverrideCurrentCamera(const bool overridecamera);  //  @916E80
    const char* const       GetActiveCameraAnim() const;    //  @917860
    void                    SetActiveCameraAnim(const char* const anim);    //  @9179A0
    const char* const       GetBeforeStartEvent() const;    //  @917870
    void                    SetBeforeStartEvent(const char* const eventname);   //  @9179F0
    const char* const       GetAfterStopEvent() const;  //  @917880
    void                    SetAfterStopEvent(const char* const eventname); //  @917A40
    const bool              IsDefaultInherit() const;   //  @916F00
    void                    SetDefaultInherit(const bool definherit);   //  @916EE0

    void                    Play(int* args);    //  @883EC0
    void                    PlayWithStopEvent(int* args);   //  @917BC0
    void                    Pause(const int);   //  @917890
    void                    Stop(int* args);    //  @883EC0
    void                    OrganizeAnimationData(int* args);   //  @883EC0
    void                    PlayModeStop(int* args);    //  @883EC0
    void                    GetNumRegisteredRootObjs(int* args) const;    //  @9178C0
    void                    GetRegisteredRootObj(int* args) const;  //  @917A90
    void                    SetObjectInherit(int* args);    //  @917AC0
    void                    SetObjectInherit_Impl(Node* node, const bool inherit);  //  @916FF0
    void                    GetObjectInherit(int* args);    //  @917AE0

    static void             Register(); //  @9171F0
    static CutscenePlayer*  Create(AllocatorIndex);  //  @917C70

    static int          InterpolationOnCommand; //  @A132C8
    static int          InterpolationOffCommand;    //  @A132CC
    static int          InterpolationOffFrameCommand;   //  @A132D0
};

extern EntityType* tCutscenePlayer;

ASSERT_CLASS_SIZE(CutscenePlayer, 244);