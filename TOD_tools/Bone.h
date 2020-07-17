#pragma once

#include "Node.h"

#define BONE_CLASS_SIZE 80

class Bone : public Node
{
protected:

public:
	Bone() : Node(NODE_MASK_POSITION)	//	NOTE: has no constructor.
	{
		MESSAGE_CLASS_CREATED(Bone);
	};
};

static_assert(sizeof(Bone) == BONE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Bone));