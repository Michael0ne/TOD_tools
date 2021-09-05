#pragma once

#include "Model.h"

#define STRETCHMODEL_CLASS_SIZE 272

class StretchModel : public Model
{
protected:
 Vector4f m_Scale;
public:
 StretchModel(); // @8F87A0
};

static_assert(sizeof(StretchModel) == STRETCHMODEL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(StretchModel));