#pragma once

#include "Node.h"

#define TEXTSLOT_CLASS_SIZE 96

class TextSlot : public Node
{
protected:
	char* m_TextStringId;
	int m_CurrentIndex;
	int* m_TextRes;
	int field_5C;

public:
	TextSlot() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(TextSlot);

		m_TextRes = nullptr;
		field_5C = 1;
		m_TextStringId = 0;
		m_CurrentIndex = 0;
	}
};

static_assert(sizeof(TextSlot) == TEXTSLOT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TextSlot));