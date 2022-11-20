#pragma once
#include "Node.h"

class AnimSlot;
class MotionLayer;
class MotionAnimSlot;

class AnimLayer : public Node
{
protected:
    struct GamePivotInfo
    {
        Vector3f        m_Position;
        bool            m_Used;
    };

    struct TargetData
    {
        AssetLoader             TargetAsset;
        uint32_t                LoopMode;
        std::vector<uint32_t>   PhysAttachmentsList;    //  NOTE: index - phys. attachment index, value - bone index.
        float_t                 PlayPos;
        uint16_t                field_20;
        uint16_t                field_22;
        uint32_t                field_24;

        inline TargetData()
        {
            MESSAGE_CLASS_CREATED(TargetData);

            LoopMode = 0;
            PlayPos = 0.f;
            field_20 = 0;
            field_22 = 0;
            field_24 = 0;
        }
        ~TargetData();  //  @905610
        TargetData(AssetLoader& animation, const uint32_t loopMode, const std::vector<uint32_t>& physAttachmentsList);  //  @906C10

        inline TargetData(TargetData& rhs)
        {
            MESSAGE_CLASS_CREATED(TargetData);

            TargetAsset = rhs.TargetAsset;
            LoopMode = rhs.LoopMode;
            PhysAttachmentsList = rhs.PhysAttachmentsList;
            PlayPos = rhs.PlayPos;
            field_20 = rhs.field_20;
            field_22 = rhs.field_22;
            field_24 = rhs.field_24;
        }
    };

    Vector4f                    GamePivotPosition;    //  NOTE: this and below could be 'GamePivotAbsolutePosition/Orientation'.
    Orientation                 GamePivotOrientation;
    Vector4f                    Pos_1;
    AnimSlot                   *TargetAnimNode_1;
    AnimSlot                   *TargetAnimNode_2;
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
    }                           Flags;
    float                       SpeedMultiplier;
    float                       CrossBlendFactor;
    float                       CrossBlendSpeed;
    uint32_t                    TargetAnimationNodeIndex; // NOTE: which of 'TargetAnim_Entity' to use.
    float_t                     Weight;
    MotionLayer                *MotionLayerNode;
    uint32_t                    Event;
    uint32_t                    BlendFinishedEvent;
    union
    {
        struct
        {
            unsigned Target1 : 1;
            unsigned Target2 : 1;
        };
    }                           OverrideTarget;
    Vector4f                    GamePivotPos_1;
    Vector4f                    GamePivotPos_2;
    union
    {
        struct
        {
            unsigned Target1 : 1;
            unsigned Target2 : 1;
        };
    }                           ClearOverrideTargetOnBlendFinished;

    TargetData                  CrossTargets[2];

    std::vector<GamePivotInfo>  GamePivotList;
    float                       field_134;
    int                         field_138;
    float                       field_13C;
    float                       field_140;

public:
    AnimLayer();    //  @902F90
    virtual ~AnimLayer();   //  @905670

    const bool                  IsPlaying() const;   //  @900FA0
    void                        SetIsPlaying(const bool playing);   //  @901050
    const bool                  IsLooping() const;  //  @900FC0
    void                        SetIsLooping(const bool loop);  //  @900FD0
    const bool                  IsFrozen() const;   //  @901020
    void                        SetIsFrozen(const bool frozen); //  @5010E0
    const uint32_t              GetTargetAnim() const;  //  @9010A0
    void                        SetTargetAnim(const uint32_t targetAnim);   //  @9010B0
    const float_t               GetWeight() const;  //  @916DF0
    void                        SetWeight(const float_t weight);    //  @5AB060
    Node*                       GetCrossTarget1() const;  //    @4A0C70
    void                        SetCrossTarget1(AnimSlot* target); //  @903560
    Node*                       GetCrossTarget2() const;  //  @4A66A0
    void                        SetCrossTarget2(AnimSlot* target);  //  @9037F0
    const float_t               GetPlayPos1() const;    //  @900FB0
    void                        SetPlayPos1(const float_t playpos); //  @9017A0
    const float_t               GetPlayPos2() const;    //  @68B210
    void                        SetPlayPos2(const float_t playpos); //  @9018F0
    const float_t               GetCrossBlendFactor() const;    //  @5A1D40
    void                        SetCrossBlendFactor(const float_t crossblendfactor);    //  @901A30
    const float_t               GetCrossBlendSpeed() const; //  @8D50E0
    void                        SetCrossBlendSpeed(const float_t crossblendspeed);  //  @501140
    const float_t               GetTargetCrossBlendFactor() const;  //  @901030
    void                        SetTargetCrossBlendFactor(const float_t crossblendfactor);  //  @50B460
    const uint32_t              GetEvent() const;   //  @917860
    void                        SetEvent(const uint32_t event); //  @501180
    const uint32_t              GetBlendFinishedEvent() const;  //  @4A66B0
    void                        SetBlendFinishedEvent(const uint32_t event);    //  @5011A0
    const bool                  ShouldStallFirstFrame() const;  //  @9010E0
    void                        SetShouldStallFirstFrame(const bool enabled);   //  @9010F0
    Node*                       GetTargetAnimNode() const;    //  @50B490
    Node*                       GetBlendInAnimNode() const; //  @50B490
    Node*                       GetBlendOutAnimNode() const;    //  @50B4B0
    MotionLayer*                GetMotionLayer() const; //  @9262A0
    void                        SetMotionLayer(MotionLayer* node); //  @5011C0
    const bool                  OverrideTarget1() const;  //  @901110
    void                        SetOverrideTarget1(const bool shouldoverride);    //  @901120
    const bool                  OverrideTarget2() const;    //  @901170
    void                        SetOverrideTarget2(const bool shouldoverride);  //  @901180
    void                        GetOverrideGamePivotPos1(Vector4f& pos) const;  //  @9012B0
    void                        SetOverrideGamePivotPos1(const Vector4f& pos);  //  @901310
    void                        GetOverrideGamePivotPos2(Vector4f& pos) const;  //  @9012E0
    void                        SetOverrideGamePivotPos2(const Vector4f& pos);  //  @901380
    const bool                  GetClearOverrideTarget1OnBlendFinished() const; //  @9015B0
    void                        SetClearOverrideTarget1OnBlendFinished(const bool clearoverride); //  @9015C0
    const bool                  GetClearOverrideTarget2OnBlendFinished() const; //  @901610
    void                        SetClearOverrideTarget2OnBlendFinished(const bool clearoverride); //  @901620
    const float_t               GetSpeedMultiplier() const; //  @501090
    void                        SetSpeedMultiplier(const float_t mult); //  @5010A0

    void                        Play(uint32_t* args);   //  @905780
    void                        BlendTo(uint32_t* args);    //  @9057A0
    void                        SetBlendFinishedEventScripted(uint32_t* args);  //  @901700
    void                        Stop(uint32_t* args);   //  @904A90
    void                        SyncToPrevious(uint32_t* args); //  @904AD0
    void                        SetPlayPos(uint32_t* args); //  @904B10
    void                        GetPlayPos(float_t* args);  //  @9016D0
    void                        GetVelocity(uint32_t* args);    //  @50D240
    void                        GetGamePivotStartPos(float_t* args) const;    //  @9054B0
    void                        GetGamePivotEndPos(float_t* args) const;    //  @905520
    void                        GetGamePivotStartOrient(float_t* args) const;   //  @905550
    void                        GetGamePivotEndOrient(float_t* args) const; //  @9055B0
    void                        GetOverrideBlendOut(uint32_t* args) const;  //  @9011D0
    void                        GetOverrideBlendIn(uint32_t* args) const;   //  @901210
    void                        SetOverrideBlendOut(uint32_t* args);    //  @901250
    void                        SetOverrideBlendIn(uint32_t* args); //  @901280
    void                        ClearBlendOutOverrideOnBlendFinished(uint32_t* args);   //  @901670
    void                        ClearBlendInOverrideOnBlendFinished(uint32_t* args);    //  @9016A0
    void                        GetOverrideBlendOutPos(float_t* args) const;    //  @9013F0
    void                        GetOverrideBlendInPos(float_t* args) const; //  @901470
    void                        SetOverrideBlendOutPos(float_t* args);  //  @9014F0
    void                        SetOverrideBlendInPos(float_t* args);   //  @901550

    void                        GetGamePivotPos(Vector4f& outPos) const;    //  @901730 //  TODO: the argument could be a vec3f since disassembly is explicit about it.
    void                        GetGamePivot(Vector4f& outPos) const;   //  @901EC0
    void                        GetGamePivotOrientA(Orientation& outOrient) const;  //  @901EF0
    void                        GetGamePivotOrient(Orientation& outOrient) const;   //  @901750
    void                        GetLoopMode(int& outLoopMode) const;    //  @901770

    void                        CalcAndSetGamePivot(int* args); //  @905660

private:
    void                        CalcAndSetGamePivot_Impl(); //  @902A00
    void                        SetTargetCrossBlendFactor_Impl(const float_t factor);   //  @901A30
    void                        GetVelocity_Impl(Vector4f& velocity) const; //  @901B30
    void                        GetGamePivotEndPos_Impl(Vector4f& pivotPos) const;    //  @9053D0
    void                        Play_Impl(AnimSlot* animSlot, const uint32_t event);    //  @903A80
    void                        BlendTo_Impl(AnimSlot* animSlot, const uint32_t event); //  @903C20

public:
    static void                 Register(); //  @903E60

private:
    static AnimLayer*           Create(AllocatorIndex); //  @905740
};

extern EntityType* tAnimLayer;  //  @A3E10C

ASSERT_CLASS_SIZE(AnimLayer, 324);