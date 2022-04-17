#pragma once
#include "Node.h"

class TrackPoint : public Node
{
protected:
    float m_Speed;

public:
    inline TrackPoint() : Node(NODE_MASK_POSITION)
    {
        MESSAGE_CLASS_CREATED(TrackPoint);

        m_Speed = 1.0f;
    }

    const float         GetSpeed() const;   //  @89A880
    void                SetSpeed(const float speed);    //  @8A6C90

    static void         Register(); //  @924270
    static TrackPoint*  Create(AllocatorIndex); //  @924210
};

extern EntityType* tTrackPoint; //  @A3E150

ASSERT_CLASS_SIZE(TrackPoint, 84);