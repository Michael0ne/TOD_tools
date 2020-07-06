#include "Position.h"

Position::Position(class Node* owner)
{
	MESSAGE_CLASS_CREATED(Position);

	m_vOrientation = Quaternion<float>();
	m_vPosition2 = Vector4<float>();
	m_Owner = owner;
}