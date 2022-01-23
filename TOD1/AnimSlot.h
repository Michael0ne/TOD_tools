#pragma once
#include "Node.h"
#include "AnimationAsset.h"

class AnimSlot : public Node
{
    friend class AnimLayer;

    enum LoopMode
    {
        LOOP = 0,
        ONESHOT,
        RANDOM
    };

    enum AnimFlags
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
    AnimationAsset *m_TargetAnimation;
    int             field_54;
    LoopMode        m_LoopMode;
    float           m_Speed;
    float           m_CrossBlendSpeed;
    Vector4f        m_OverrideAverage;
    Vector4f        m_OverrideEscape;
    AnimFlags       m_AnimFlags;
    union
    {
        struct
        {
            unsigned    StallFirstFrame : 1;
            unsigned    _1 : 1;
        };
    }               m_Flags;

public:
    AnimSlot(); // @905D90
    virtual ~AnimSlot();

    const char* const   GetTarget1() const; //  @9058A0
    void                SetTarget1(const char* const target1);  //  @906380
    const float         GetSpeed() const;   //  @9058C0
    LoopMode            GetLoopmode1() const;   //  @900F90
    void                SetLoopmode1(const LoopMode loopmode); //  @9057D0
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
    const AnimFlags     GetAnimFlags() const;   //  @4A66A0
    void                SetAnimFlags(const AnimFlags flags); //  @9057C0

    void                GetGamePivotStartPos(int *args);    //  @905990
    void                GetGamePivotEndPos(int *args);  //  @9059E0
    void                GetGamePivotPos(int *args); //  @905A40

    static void         Register(); //  @905EA0
    static AnimSlot*    Create(AllocatorIndex); //  @905E60

private:
    Vector4f            GetGamePivotPos_Impl(Vector4f& outPos, const float pivotindex) const;    //  @905100
};

extern EntityType* tAnimSlot;   //  @A3E120

ASSERT_CLASS_SIZE(AnimSlot, 140);