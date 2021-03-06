#pragma once

#include "Node.h"

#define NODE_LOGICAL_CLASS_SIZE 80

class NodeLogical : public Node
{
private:

public:
	NodeLogical() : Node(NODE_MASK_EMPTY)
	{
		MESSAGE_CLASS_CREATED(NodeLogical);
	}
};

static_assert(sizeof(NodeLogical) == NODE_LOGICAL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(NodeLogical));