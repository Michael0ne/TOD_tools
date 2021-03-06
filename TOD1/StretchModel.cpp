#include "StretchModel.h"

StretchModel::StretchModel() : Model()
{
	MESSAGE_CLASS_CREATED(StretchModel);

	m_Scale = Vector4f();

	field_7C = 0;
}