#pragma once

#include "Node.h"

#define COLLISIONSPHERE_CLASS_SIZE 108

class CollisionSphere : public Node
{
protected:
	Vector4f m_Extrusion;
	void* m_Unknown;
	float m_Radius;
	char m_CollisionFlags;
public:
	CollisionSphere();	//	@8C47D0
};

static_assert(sizeof(CollisionSphere) == COLLISIONSPHERE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(CollisionSphere));