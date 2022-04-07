#include "NodeSpatial.h"
#include "NumberType.h"

EntityType* tNodeSpatial;

NodeSpatial::NodeSpatial() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(NodeSpatial);

    m_BoundRadius = 1.f;
}

Vector4f* NodeSpatial::GetBounds(Vector4f& outBounds)
{
    outBounds = { 0, 0, 0, m_BoundRadius };
    return &outBounds;
}

void NodeSpatial::Register()
{
    tNodeSpatial = new EntityType("NodeSpatial");
    tNodeSpatial->InheritFrom(tNode);
    tNodeSpatial->SetCreator((CREATOR)Create);

    tNodeSpatial->RegisterProperty(tNUMBER, "boundradius", (EntityGetterFunction)&GetBoundRadius, NULL, NULL, NULL, (EntitySetterFunction)&SetBoundRadius, NULL, NULL, NULL, "control=slider|min=0|max=50|step=0.5", NULL, NULL, 10);

    tNodeSpatial->PropagateProperties();
}

const float NodeSpatial::GetBoundRadius() const
{
    return m_BoundRadius;
}

void NodeSpatial::SetBoundRadius(const float radius)
{
    StoreProperty(10, &m_BoundRadius, tNUMBER);
    m_BoundRadius = radius;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

NodeSpatial* NodeSpatial::Create(AllocatorIndex)
{
    return new NodeSpatial();
}