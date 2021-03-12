#include "NodeLogical.h"

ScriptType_Entity*	tNodeLogical;

NodeLogical::NodeLogical() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(NodeLogical);
}

#pragma message(TODO_IMPLEMENTATION)
void NodeLogical::Register()
{
	tNodeLogical = new ScriptType_Entity("NodeLogical");
	tNodeLogical->InheritFrom(tNode);
	tNodeLogical->SetCreator((ScriptType_Entity::CREATOR)Create);
}

NodeLogical* NodeLogical::Create(AllocatorIndex)
{
	return new NodeLogical();
}