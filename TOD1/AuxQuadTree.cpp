#include "AuxQuadTree.h"
#include "CollisionList.h"
#include "Camera.h"
#include "Node.h"

#pragma message(TODO_IMPLEMENTATION)
AuxQuadTree::AuxQuadTree(class Node* owner)
{
    MESSAGE_CLASS_CREATED(AuxQuadTree);
}

#pragma message(TODO_IMPLEMENTATION)
AuxQuadTree::~AuxQuadTree()
{
    MESSAGE_CLASS_DESTROYED(AuxQuadTree);
}

void AuxQuadTree::CalculateLodForAllChildren()
{
    UpdateLodDistance();

    for (auto child = m_FirstSibling; child; child = child->m_NextSibling)
        child->CalculateLodForAllChildren();
}

void AuxQuadTree::UpdateLodDistance()
{
    AuxQuadTree* n = this;

    while (!n->m_Owner->m_Collision)
    {
        n = n->field_4;
        if (!n)
            break;
    }

    if (n == this)
    {
        field_4D = field_4D & 0xF8 | 8;
        m_Lod = false;
        m_LodFade = -1;
        m_LodDistance = (int)((m_Bounds.z - Camera::ActiveCameraPosition.z) * (m_Bounds.z - Camera::ActiveCameraPosition.z) +
            (m_Bounds.y - Camera::ActiveCameraPosition.y) * (m_Bounds.y - Camera::ActiveCameraPosition.y) +
            (m_Bounds.x - Camera::ActiveCameraPosition.x) * (m_Bounds.x - Camera::ActiveCameraPosition.x));
        SetLodLevel(Camera::ActiveCameraPosition);
    }
    else
    {
        n->UpdateLodDistance();
        m_LodDistance = (int)((m_Bounds.z - Camera::ActiveCameraPosition.z) * (m_Bounds.z - Camera::ActiveCameraPosition.z) +
            (m_Bounds.y - Camera::ActiveCameraPosition.y) * (m_Bounds.y - Camera::ActiveCameraPosition.y) +
            (m_Bounds.x - Camera::ActiveCameraPosition.x) * (m_Bounds.x - Camera::ActiveCameraPosition.x));
        m_Lod = n->m_Lod;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void AuxQuadTree::_8A3810(void* ptr)
{
}

#pragma message(TODO_IMPLEMENTATION)
void AuxQuadTree::Refresh()
{
}

#pragma message(TODO_IMPLEMENTATION)
void AuxQuadTree::SetLodLevel(const Vector4f& pos)
{
    if (m_Bounds.a >= 10000)
    {
        m_LodFade = -1;
        m_Lod = false;
    }
    else
    {
        // TODO: ...
    }
}

void AuxQuadTree::CopyOwnerBounds()
{
    Vector4f bounds;
    Vector4f worldPos;
    m_Owner->GetBounds(bounds);
    m_Owner->ConvertToWorldSpace(worldPos, bounds);

    m_Bounds = worldPos;
}

#pragma message(TODO_IMPLEMENTATION)
void AuxQuadTree::_8A36A0(const bool use)
{
}

#pragma message(TODO_IMPLEMENTATION)
void AuxQuadTree::_8A3320()
{
}

#pragma message(TODO_IMPLEMENTATION)
const int AuxQuadTree::GetContactMaterialID(const int a1) const
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
const int AuxQuadTree::GetContactSurfacePropFields(const int a1) const
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
const int AuxQuadTree::GetContactFlags(const int index) const
{
    return 0;
}

#pragma message(TODO_IMPLEMENTATION)
Node* AuxQuadTree::GetRealNode(const int index) const
{
    return nullptr;
}

Node* AuxQuadTree::GetContactNode(const int index) const
{
    Node* node = GetRealNode(index);
    return AuxQuadTree::GetForNode(node)->m_Owner;
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* AuxQuadTree::GetPeerContactPoint(const int index) const
{
    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* AuxQuadTree::GetContactPoint(const int index) const
{
    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* AuxQuadTree::GetContactNormal(Vector4f& outNormal, const int index) const
{
    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
int AuxQuadTree::Contacts() const
{
    return 0;
}

AuxQuadTree* AuxQuadTree::GetForNode(Node* node)
{
    if (node->m_QuadTree)
    {
        AuxQuadTree* qdtr = node->m_QuadTree;
        while (!qdtr->m_Owner->m_Collision)
        {
            qdtr = qdtr->field_4;
            if (!qdtr)
                return node->m_QuadTree;
        }

        return qdtr;
    }
    else
    {
        Node* nd = node->m_Parent;
        if (nd)
        {
            while (!nd->m_QuadTree)
            {
                nd = nd->m_Parent;
                if (!nd)
                    return nullptr;
            }

            AuxQuadTree* qdtr = nd->m_QuadTree;
            if (qdtr)
            {
                while (!qdtr->m_Owner->m_Collision)
                {
                    qdtr = qdtr->field_4;
                    if (!qdtr)
                        return nd->m_QuadTree;
                }

                return nd->m_QuadTree;
            }
        }
        else
            return nullptr;
    }

    return nullptr;
}