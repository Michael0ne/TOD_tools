#include "NodeLogical.h"

EntityType* tNodeLogical;

void NodeLogical::Register()
{
    tNodeLogical = new EntityType("NodeLogical");
    tNodeLogical->InheritFrom(tNode);
    tNodeLogical->SetCreator((CREATOR)Create);
    tNodeLogical->PropagateProperties();
}

NodeLogical* NodeLogical::Create(AllocatorIndex)
{
    return new NodeLogical();
}