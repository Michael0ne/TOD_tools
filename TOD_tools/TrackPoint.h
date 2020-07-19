#pragma once

#include "Node.h"

#define TRACKPOINT_CLASS_SIZE 84

class TrackPoint : public Node
{
protected:
	float	m_Speed;

public:
	TrackPoint() : Node(NODE_MASK_POSITION)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(TrackPoint);

		m_Speed = 1.0f;
	}
};

static_assert(sizeof(TrackPoint) == TRACKPOINT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TrackPoint));