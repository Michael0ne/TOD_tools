#pragma once
#include "Node.h"

class ControlSetup : public Node
{
protected:
	int				m_ControlsList[4];
public:
	ControlSetup();

	static ControlSetup*	CurrentController;	//	@A3E16C
	static bool&			WaitForController;	//	@A3E168
	static short*			WaitForControllerText;	//	@A3E164

};

ASSERT_CLASS_SIZE(ControlSetup, 96);