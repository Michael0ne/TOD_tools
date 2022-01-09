#pragma once
#include "Node.h"

class NodeLogical : public Node
{
public:
    inline NodeLogical() : Node(NODE_MASK_EMPTY)  //  NOTE: constructor inlined.
    {
        MESSAGE_CLASS_CREATED(NodeLogical);
    }

    static void             Register(); // @88E840
    static NodeLogical*     Create(AllocatorIndex); // @891CD0
};

extern EntityType* tNodeLogical; // @A3D88C

ASSERT_CLASS_SIZE(NodeLogical, 80);