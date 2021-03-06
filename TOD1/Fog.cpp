#include "Fog.h"

Fog::Fog() : Node(NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(Fog);

	m_FogStart = 100.0f;
	m_FogEnd = 1000.0f;
	m_FogType = 1;
	m_Density = 0.02f;
	m_FogColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);

	m_QuadTree->field_1C = m_QuadTree->field_1C & 0xFFFFFF | m_QuadTree->field_1C & 0xFF000000 | 0x8000000;
}