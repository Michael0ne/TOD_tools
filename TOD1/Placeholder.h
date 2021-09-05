#pragma once

#include "Model.h"

#define PLACEHOLDER_CLASS_SIZE 268

class PlaceHolder : public Model
{
protected:
 unsigned int m_Flags;
 float m_BoundRadius;
 Node* m_PlaceholderCollisionPivot;

public:
 PlaceHolder(); // @8D17A0
};

static_assert(sizeof(PlaceHolder) == PLACEHOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(PlaceHolder));