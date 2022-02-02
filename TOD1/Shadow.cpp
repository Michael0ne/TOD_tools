#include "Shadow.h"
#include "AssetManager.h"
#include "Scene.h"

int Shadow::TotalShadows;
Vector4f Shadow::_A12B70 = { 1000, 1000, 1000, 0 };
std::vector<Vector4f> Shadow::_A12B60;
std::vector<int> Shadow::_A3E09C;
int* Shadow::_A3E090;

EntityType* tShadow;

Shadow::Shadow() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Shadow);
}

Shadow::~Shadow()
{
    MESSAGE_CLASS_DESTROYED(Shadow);

    if (m_Texture)
        g_AssetManager->DecreaseResourceReferenceCount(m_Texture);

    delete field_5C;
}

void Shadow::Destroy()
{
    TotalShadows--;

    if (TotalShadows == 1)
        delete _A3E090;

    delete m_FrameBuffer_1;
    delete m_FrameBuffer_2;

    m_CollisionProbe->Destroy();

    field_80 = 0;

    Node::Destroy();
}

void Shadow::Instantiate()
{
    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;
    m_QuadTree->Refresh();

    if (m_BoneName.Empty() || m_Parent == Scene::SceneInstance)
        m_Bone = nullptr;
    else
        m_Bone = (Node*)m_Parent->FindNode(m_BoneName.m_Str);

    field_80 = 0;
    if (m_QuadTree)
        m_QuadTree->Refresh();

    m_Id._3 = true;
}

#pragma message(TODO_IMPLEMENTATION)
void Shadow::Render()
{
}

#pragma message(TODO_IMPLEMENTATION)
void Shadow::Register()
{
    tShadow = new EntityType("Shadow");
    tShadow->InheritFrom(tNode);
    tShadow->SetCreator((EntityType::CREATOR)Create);
}

Shadow* Shadow::Create(AllocatorIndex)
{
    return new Shadow;
}

void Shadow::ClearHints()
{
    m_HintNode = nullptr;
}