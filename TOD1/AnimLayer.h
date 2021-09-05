#pragma once
#include "Node.h"

class AnimSlot;
class MotionLayer;

class AnimLayer : public Node
{
protected:
    Vector3f                    m_GamePivotPosition;
    int                         field_5C;
    Orientation                 m_GamePivotOrient;
    Vector4f                    m_Pos_1;
    AnimSlot                   *m_TargetAnim_Entity;
    AnimSlot                   *m_TargetAnim_Entity_2;
    union
    {
        struct
        {
            unsigned Playing : 1;
            unsigned Looping : 1;
            unsigned Freeze : 1;
            unsigned StallFirstFrame : 1;
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
            unsigned _25 : 1;
            unsigned _26 : 1;
            unsigned _27 : 1;
            unsigned _28 : 1;
            unsigned _29 : 1;
            unsigned _30 : 1;
        };
        unsigned int            m_Flags;
    }                           m_Flags;
    float                       m_SpeedMultiplier;
    float                       m_CrossBlendFactor;
    float                       m_CrossBlendSpeed;
    int                         m_TargetAnim; // NOTE: which of 'TargetAnim_Entity' to use.
    float                       m_Weight;
    MotionLayer                *m_MotionLayer;
    int                         m_Event;
    int                         m_BlendFinishedEvent;
    union
    {
        struct
        {
            unsigned Target1 : 1;
            unsigned Target2 : 1;
        };
    }                           m_OverrideTarget;
    Vector4f                    m_GamePivotPos;
    Vector4f                    m_GamePivotPos_2;
    union
    {
        struct
        {
            unsigned Target1 : 1;
            unsigned Target2 : 1;
        };
    }                           m_ClearOverrideTargetOnBlendFinished;
    Asset                      *field_D4;   //  NOTE: probably it's 'AssetHolder'.
    int                        *field_D8;
    int                         field_DC;
    std::vector<int>            m_List_1;
    float                       m_PlayPos_1;
    int                         field_F4;
    int                         field_F8;
    Asset                      *field_FC;   //  NOTE: probably it's 'AssetHolder'.
    int                        *field_100;
    int                         field_104;
    std::vector<int>            m_List_2;
    float                       m_PlayPos_2;
    int                         field_11C;
    int                         field_120;
    std::vector<int>            m_List_3;
    float                       field_134;
    int                         field_138;
    float                       field_13C;
    float                       field_140;

public:
    AnimLayer();    //  @902F90
    virtual ~AnimLayer();   //  @905670

    const bool                  GetIsPlaying() const;   //  @900FA0
    void                        SetIsPlaying(const bool playing);   //  @901050
    void                        GetGamePivotPos(Vector4f& outPos) const;    //  @901730
    void                        GetGamePivotOrient(Orientation& outOrient) const;   //  @901750
    void                        GetLoopMode(int& outLoopMode) const;    //  @901770

    static void                 Register(); //  @903E60

private:
    static AnimLayer*           Create(AllocatorIndex); //  @905740
};

extern EntityType* tAnimLayer;  //  @A3E10C

ASSERT_CLASS_SIZE(AnimLayer, 324);