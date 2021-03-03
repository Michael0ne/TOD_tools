#pragma once
#include "Node.h"

class Fog : public Node
{
protected:
	int m_FogType;
	float m_FogStart;
	float m_FogEnd;
	float m_Density;
	ColorRGB m_FogColor;

public:
	Fog();	//	NOTE: constructor inlined.
};

ASSERT_CLASS_SIZE(Fog, 112);