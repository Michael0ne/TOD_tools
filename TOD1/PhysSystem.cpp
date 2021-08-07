#include "PhysSystem.h"

PhysSystem::~PhysSystem()
{
	MESSAGE_CLASS_DESTROYED(PhysSystem);
}

void PhysSystem::SetVertexPos(const unsigned int vertexindex, const Vector4f& pos)
{
	m_List_1[vertexindex] = pos;
	m_VerticesList[vertexindex].m_Position = pos;
	m_VerticesList[vertexindex].m_Normal = {};
	m_VerticesList[vertexindex].m_Orientation = { 1, 0, 0, 0 };

	m_VerticesList[vertexindex].field_30 = -1;
	m_VerticesList[vertexindex].field_32 = -1;
}

void PhysSystem::SetNormalPos(const unsigned int vertexindex, const Vector4f& norm)
{
	m_VerticesList[vertexindex].m_Normal = norm;
}