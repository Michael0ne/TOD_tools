#pragma once

#include "Node.h"

#define WAYPOINT_CLASS_SIZE 80

class WayPoint : public Node
{
protected:

public:
 WayPoint() : Node(NODE_MASK_POSITION) // NOTE: no constructor.
 {
  MESSAGE_CLASS_CREATED(WayPoint);
 }
};

static_assert(sizeof(WayPoint) == WAYPOINT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(WayPoint));