#pragma once

#include "Node.h"

#define GUIDEBOX_CLASS_SIZE 100

class GuideBox : public Node
{
protected:
	float m_Left;
	float m_Right;
	float m_Front;
	float m_Back;
	float m_Height;

public:
	GuideBox() : Node(NODE_MASK_POSITION)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(GuideBox);

		m_Left = m_Front = -1.5f;
		m_Right = m_Back = 1.5;
		m_Height = 3.0f;
	}
};

static_assert(sizeof(GuideBox) == GUIDEBOX_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GuideBox));