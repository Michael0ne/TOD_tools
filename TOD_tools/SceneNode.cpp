#include "SceneNode.h"
#include "RewindBuffer.h"

SceneNode* tSceneNode = nullptr;

#pragma message(TODO_IMPLEMENTATION)
SceneNode::SceneNode()
{
	MESSAGE_CLASS_CREATED(SceneNode);
	patch(0xA3D7B8, this, 4);

	field_30 = field_40 = field_3C = 0;
	field_38 = field_48 = nullptr;

	m_RewindBuffer_1 = new RewindBuffer(614400);	//	NOTE: move constant somewhere
}

#pragma message(TODO_IMPLEMENTATION)
void SceneNode::_874940()
{
	//	NOTE: possible name is 'CurrentUndo'.
}
