#pragma once

#include "Node.h"

#define FOG_CLASS_SIZE 112

class Fog : public Node
{
protected:
	int m_FogType;
	float m_FogStart;
	float m_FogEnd;
	float m_Density;
	ColorRGB m_FogColor;

public:
	Fog() : Node(NODE_MASK_QUADTREE)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(Fog);

		m_FogStart = 100.0f;
		m_FogEnd = 1000.0f;
		m_FogType = 1;
		m_Density = 0.02f;
		m_FogColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);

		m_QuadTree->m_nUserType = m_QuadTree->m_nUserType & 0xFFFFFF | m_QuadTree->m_nUserType & 0xFF000000 | 0x8000000;
	}
};

static_assert(sizeof(Fog) == FOG_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Fog));