#pragma once
#include "Model.h"

class SkyBox : public Model
{
protected:
	int		field_104;

public:
	SkyBox() : Model()	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(SkyBox);

		m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;

		if (!ms_A3E0B0[0])
			(*(void(__thiscall*)(SkyBox*))0x8F20B0)(this);	//	TODO: implementation!
	}

	static	int**	ms_A3E0B0;	//	@A3E0B0	//	NOTE: this could be vertex buffers array.
};

ASSERT_CLASS_SIZE(SkyBox, 260);