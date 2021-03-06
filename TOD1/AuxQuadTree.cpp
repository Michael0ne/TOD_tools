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

	while (!n->m_Owner->m_CollisionIgnoreList)
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
		m_LodDistance = (int)((m_Position.z - Camera::ActiveCameraPosition.z) * (m_Position.z - Camera::ActiveCameraPosition.z) +
						(m_Position.y - Camera::ActiveCameraPosition.y) * (m_Position.y - Camera::ActiveCameraPosition.y) +
						(m_Position.x - Camera::ActiveCameraPosition.x) * (m_Position.x - Camera::ActiveCameraPosition.x));
		SetLodLevel(Camera::ActiveCameraPosition);
	}
	else
	{
		n->UpdateLodDistance();
		m_LodDistance = (int)((m_Position.z - Camera::ActiveCameraPosition.z) * (m_Position.z - Camera::ActiveCameraPosition.z) +
						(m_Position.y - Camera::ActiveCameraPosition.y) * (m_Position.y - Camera::ActiveCameraPosition.y) +
						(m_Position.x - Camera::ActiveCameraPosition.x) * (m_Position.x - Camera::ActiveCameraPosition.x));
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
	if (m_Position.a >= 10000)
	{
		m_LodFade = -1;
		m_Lod = false;
	}
	else
	{
		//	TODO: ...
	}
}