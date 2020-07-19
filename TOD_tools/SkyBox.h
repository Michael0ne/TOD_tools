#pragma once

#include "Model.h"

#define SKYBOX_CLASS_SIZE 260

class SkyBox : public Model
{
protected:
	int		field_104;

public:
	SkyBox() : Model()	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(SkyBox);

		m_QuadTree->m_nUserType = m_QuadTree->m_nUserType & 0xFFFFFF | m_QuadTree->m_nUserType & 0xFF000000 | 0x8000000;

		if (!ms_A3E0B0[0])
			(*(void(__thiscall*)(SkyBox*))0x8F20B0)(this);	//	TODO: implementation!
	}

	static	int**	ms_A3E0B0;	//	@A3E0B0	//	NOTE: this could be vertex buffers array.
};

static_assert(sizeof(SkyBox) == SKYBOX_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SkyBox));