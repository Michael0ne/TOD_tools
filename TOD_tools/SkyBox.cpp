#include "SkyBox.h"

SkyBox::SkyBoxStructUnknown* SkyBox::_A3E0B0[5];

#pragma message(TODO_IMPLEMENTATION)
SkyBox::SkyBox() : Model()
{
	MESSAGE_CLASS_CREATED(SkyBox);

	m_QuadTree->field_1C = m_QuadTree->field_1C & 0xFFFFFF | m_QuadTree->field_1C & 0xFF000000 | 0x80000000;

	//if (!_A3E0B0[0])
}