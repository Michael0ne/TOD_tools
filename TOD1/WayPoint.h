#pragma once
#include "Node.h"

class WayPoint : public Node
{
public:
    inline WayPoint() : Node(NODE_MASK_POSITION)
    {
        MESSAGE_CLASS_CREATED(WayPoint);
    }

    virtual ~WayPoint();

    static void         Register(); //  @924530
    static WayPoint*    Create(AllocatorIndex);    //  @9244E0
};

extern EntityType* tWayPoint;   //  @A3E154

ASSERT_CLASS_SIZE(WayPoint, 80);