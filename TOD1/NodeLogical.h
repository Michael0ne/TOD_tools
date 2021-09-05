#pragma once

#include "Node.h"

#define NODE_LOGICAL_CLASS_SIZE 80

class NodeLogical : public Node
{
private:

public:
 NodeLogical();

 static void   Register(); // @88E840
 static NodeLogical* Create(AllocatorIndex); // @891CD0
};

extern EntityType* tNodeLogical; // @A3D88C

static_assert(sizeof(NodeLogical) == NODE_LOGICAL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(NodeLogical));