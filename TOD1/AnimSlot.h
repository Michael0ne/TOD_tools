#pragma once
#include "Node.h"
#include "AnimationAsset.h"

class AnimSlot : public Node
{
    friend class AnimLayer;

    enum LoopMode_t
    {
        LOOP = 0,
        ONESHOT,
        RANDOM
    };

    enum AnimFlags_t
    {
        FREEZE_ROOT_NODE = 0,
        OVERRIDE_AVERAGE_Z = 1,
        OVERRIDE_ESCAPE_X = 2,
        OVERRIDE_ESCAPE_Y = 3,
        OVERRIDE_ESCAPE_Z = 4,
        OVERRIDE_AVERAGE_X = 5,
        OVERRIDE_ALL = 6
    };

protected:
    AssetLoader         TargetAnimation;
    LoopMode_t          LoopMode;
    float               Speed;
    float               CrossBlendSpeed;
    Vector4f            OverrideAverage;
    Vector4f            OverrideEscape;
    AnimFlags_t         AnimFlags;
    union
    {
        struct
        {
            unsigned    StallFirstFrame : 1;
            unsigned    GamePivotIndex : 12;    //  NOTE: not sure how much bits is reserved for this.
        };
    }                   Flags;

    void                GetGamePivotStartOrient(Orientation& orient) const; //  @9051E0
    void                GetGamePivotEndOrient(Orientation& orient) const;   //  @905240

public:
    AnimSlot(); // @905D90
    virtual ~AnimSlot();

    const char* const   GetTarget1() const; //  @9058A0
    void                SetTarget1(const char* const target1);  //  @906380
    const float         GetSpeedReal() const;   //  @9058C0
    LoopMode_t          GetLoopmode1() const;   //  @900F90
    void                SetLoopmode1(const LoopMode_t loopmode); //  @9057D0
    const float         GetSpeed() const;   //  @905800
    void                SetSpeed(const float speed);    //  @8E3730
    const float         GetCrossBlendSpeed() const; //  @8A6D40
    void                SetCrossBlendSpeed(const float speed);  //  @8DDA50
    const float         GetCrossBlendFrames() const;    //  @905810
    void                SetCrossBlendFrames(const int frames);    //  @5FD750
    const bool          GetShouldStallFirstFrame() const;   //  @900FA0
    void                SetShouldStallFirstFrame(const bool stallff);   //  @9057E0
    const float         GetOverrideAverageX() const;    //  @905830
    void                SetOverrideAverageX(const float averagex); //  @8A6CD0
    const float         GetOverrideAverageY() const;    //  @905840
    void                SetOverrideAverageY(const float averagey);  //  @8DDA60
    const float         GetOverrideAverageZ() const;    //  @905850
    void                SetOverrideAverageZ(const float averagez);  //  @5EA6C0
    const float         GetOverrideEscapeX() const; //  @905860
    void                SetOverrideEscapeX(const float escapex);    //  @8E37A0
    const float         GetOverrideEscapeY() const; //  @905870
    void                SetOverrideEscapeY(const float escapey);    //  @916E20
    const float         GetOverrideEscapeZ() const; //  @905880
    void                SetOverrideEscapeZ(const float escapez);    //  @905890
    const AnimFlags_t   GetAnimFlags() const;   //  @4A66A0
    void                SetAnimFlags(const AnimFlags_t flags); //  @9057C0

    void                GetGamePivotStartPos(int *args) const;    //  @905990
    void                GetGamePivotEndPos(int *args) const;  //  @9059E0
    void                GetGamePivotPos(int *args) const; //  @905A40
    void                GetGamePivotOrient(Orientation& orient, const float_t playPos) const;   //  @9052B0

    static void         Register(); //  @905EA0
    static AnimSlot*    Create(AllocatorIndex); //  @905E60

private:
    void                GetGamePivotPos_Impl(Vector4f& outPos, const float playPos) const;    //  @905100
};

extern EntityType* tAnimSlot;   //  @A3E120

ASSERT_CLASS_SIZE(AnimSlot, 140);