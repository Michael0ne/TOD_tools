#pragma once

#include "Node.h"

#define COLLISIONBOX_CLASS_SIZE 100

class CollisionBox : public Node
{
protected:
 void*   m_Unknown; // NOTE: sizeof(20)
 float   m_Width;
 float   m_Height;
 float   m_Length;
 unsigned char m_CollisionFlags;
public:
 CollisionBox(); // @8AEC30
};

static_assert(sizeof(CollisionBox) == COLLISIONBOX_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(CollisionBox));