#pragma once

#include "Node.h"

#define CONTROLSETUP_CLASS_SIZE 96

class ControlSetup : public Node
{
protected:
	List<int>				m_ControlsList;
public:
	ControlSetup() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(ControlSetup);

		m_ControlsList = List<int>(0x24B00);
	}

	static ControlSetup*	CurrentController;	//	@A3E16C
	static bool&			WaitForController;	//	@A3E168
	static short*			WaitForControllerText;	//	@A3E164

};

static_assert(sizeof(ControlSetup) == CONTROLSETUP_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ControlSetup));