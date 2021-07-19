#pragma once
#include "Node.h"

class Group : public Node
{
protected:

public:
	Group();	//	NOTE: constructor inlined.

	static void		Register();	//	@87F520

private:
	static Group*	Create(AllocatorIndex);	//	@87F4B0
};

extern EntityType* tGroup;	//	@A3D814

ASSERT_CLASS_SIZE(Group, 80);