#include "SceneNode.h"
#include "RewindBuffer.h"

SceneNode* g_SceneNode = nullptr;

SceneNode::SceneNode()
{
	patch(0xA3D7B8, this, 4);

	field_30 = field_40 = field_3C = 0;
	field_38 = field_48 = nullptr;

	m_RewindBuffer_1 = new RewindBuffer(614400);	//	NOTE: move constant somewhere
}