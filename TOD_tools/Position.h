#pragma once

#include "Types.h"
#include "MemoryAllocators.h"

#define POSITION_CLASS_SIZE 100

class Position
{
public:
	Quaternion<float>	m_vOrientation;
	Vector4<float>		m_vPosition2;
	Vector4<float>		m_vPosition3;
	Vector4<float>		m_vPosition4;
	Vector4<float>		m_vPosition5;
	Vector4<float>		m_vPosition6;
	class Node*			m_Owner;

public:
	Position(class Node* owner);	//	@892300
	~Position()
	{
		MESSAGE_CLASS_DESTROYED(Position);
	}
};

static_assert(sizeof(Position) == POSITION_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Position));