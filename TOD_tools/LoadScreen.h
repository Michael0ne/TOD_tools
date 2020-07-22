#pragma once

#include "Node.h"

#define LOAD_SCREEN_NODE_CLASS_SIZE 104

class LoadScreenNode : public Node
{
protected:
	String m_ImageFile;
	Node* m_RenderOnTopNode;
	Node* m_LoadBarSprite;

public:
	LoadScreenNode() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor
	{
		MESSAGE_CLASS_CREATED(LoadScreenNode);

		m_ImageFile = String();

		m_RenderOnTopNode = nullptr;
		m_LoadBarSprite = nullptr;
	}
};

static_assert(sizeof(LoadScreenNode) == LOAD_SCREEN_NODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(LoadScreenNode));