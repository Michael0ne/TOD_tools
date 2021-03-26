#pragma once
#include "Node.h"

class NodeSpatial : public Node
{
private:
	float	m_BoundRadius;

public:
	NodeSpatial();

	static void				Register();	//	@8921C0
	static NodeSpatial*		Create(AllocatorIndex);	//	@892160
};

extern EntityType*	tNodeSpatial;	//	@A3D888

ASSERT_CLASS_SIZE(NodeSpatial, 84);