#include "NodeLogical.h"

EntityType*	tNodeLogical;

NodeLogical::NodeLogical() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(NodeLogical);
}

#pragma message(TODO_IMPLEMENTATION)
void NodeLogical::Register()
{
	tNodeLogical = new EntityType("NodeLogical");
	tNodeLogical->InheritFrom(tNode);
	tNodeLogical->SetCreator((EntityType::CREATOR)Create);
}

NodeLogical* NodeLogical::Create(AllocatorIndex)
{
	return new NodeLogical();
}