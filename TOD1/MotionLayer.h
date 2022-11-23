#pragma once
#include "AnimLayer.h"

class MotionLayer : public AnimLayer
{
protected:
    Vector4f        WorldStartPos;
    Orientation     WorldStartOrient;
    Vector4f        WorldEndPos;
    Orientation     WorldEndOrient;
    bool            UpdateAbsolute;
public:
    inline MotionLayer() : AnimLayer()
    {
        WorldStartOrient = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
        WorldEndOrient = Orientation(0.0f, 0.0f, 0.0f, 1.0f);

        UpdateAbsolute = false;
    }

    void                Motion(int* args);  //  @91CC40
    void                GetWorldStartPos(Vector4f& outStartPos) const;  //  @522FD0
    void                SetWorldStartPos(const Vector4f& startPos); //  @523000
    void                GetWorldStartOrient(Orientation& outStartOrient) const; //  @523040
    void                SetWorldStartOrient(const Orientation& startOrient);    //  @523070
    void                GetWorldEndPos(Vector4f& outEndPos) const;  //  @4CF6E0
    void                SetWorldEndPos(const Vector4f& endPos); //  @543B40
    void                GetWorldEndOrient(Orientation& outEndOrient) const; //  @4CF710
    void                SetWorldEndOrient(const Orientation& endOrient);    //  @543B80
    const bool          GetUpdateAbsolute() const; //  @919CE0
    void                SetUpdateAbsolute(const bool update);   //  @5230B0

    void                UseAbsoluteEndValues(int* args);    //  @91CC30
    void                UseAbsoluteStartValues(int* args);  //  @91CC20

private:
    void                Motion_Impl(AnimSlot* animation, const int val); //  @91C870
    void                UpdatePivotValues();    //  @919CF0
    void                UseAbsoluteEndValues_Impl();    //  @91B100
    void                UseAbsoluteStartValues_Impl();  //  @91A770

public:
    static void         Register(); //  @91C9C0
    static MotionLayer* Create(AllocatorIndex); //  @91C980
};

extern EntityType* tMotionLayer;    //  @A3E138

ASSERT_CLASS_SIZE(MotionLayer, 392);