#pragma once

#include "Node.h"

#define NODE_SPATIAL_CLASS_SIZE 84

class NodeSpatial : public Node
{
private:
	float	m_BoundRadius;

public:
	NodeSpatial() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)	//	NOTE: no constructor routine present.
	{
		MESSAGE_CLASS_CREATED(NodeSpatial);

		m_BoundRadius = 1.0f;
	}
};

static_assert(sizeof(NodeSpatial) == NODE_SPATIAL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(NodeSpatial));