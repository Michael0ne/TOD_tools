#include "Group.h"

EntityType* tGroup;

Group::Group() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Group);

    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x80000000;
}

void Group::Register()
{
    tGroup = new EntityType("Group");
    tGroup->InheritFrom(tNode);
    tGroup->SetCreator((EntityType::CREATOR)Create);
    tGroup->PropagateProperties();
}

Group* Group::Create(AllocatorIndex)
{
    return new Group;
}