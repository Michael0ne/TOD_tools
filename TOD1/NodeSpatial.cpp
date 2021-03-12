#include "NodeSpatial.h"

ScriptType_Entity*	tNodeSpatial;

NodeSpatial::NodeSpatial() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(NodeSpatial);

	m_BoundRadius = 1.f;
}

#pragma message(TODO_IMPLEMENTATION)
void NodeSpatial::Register()
{
	tNodeSpatial = new ScriptType_Entity("NodeSpatial");
	tNodeSpatial->InheritFrom(tNode);
	tNodeSpatial->SetCreator((ScriptType_Entity::CREATOR)Create);
}

NodeSpatial* NodeSpatial::Create(AllocatorIndex)
{
	return new NodeSpatial();
}