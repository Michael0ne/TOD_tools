#pragma once

#include "Node.h"

#define CONTROL_CLASS_SIZE 92

enum E_CONTROL_TYPE
{
	CONTROL_TYPE_NONE = 0,
	CONTROL_TYPE_KEYBOARD = 1,
	CONTROL_TYPE_MOUSE = 2,
	CONTROL_TYPE_MOUSE_BUTTON = 3,
	CONTROL_TYPE_GAMEPAD = 4,
	CONTROL_TYPE_DPAD = 5	//	NOTE: "A_LEFT" "A_RIGHT" etc...
};

class Control : public Node
{
protected:
	int m_Key;
	char field_54;
	int m_ControlType;

public:
	Control() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(Control);

		m_Key = -1;
		field_54 = 0;
		m_ControlType = CONTROL_TYPE_KEYBOARD;
	}
};

static_assert(sizeof(Control) == CONTROL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Control));