#pragma once
#include "Node.h"

class Group : public Node
{
protected:

public:
	Group();	//	NOTE: constructor inlined.
};

ASSERT_CLASS_SIZE(Group, 80);