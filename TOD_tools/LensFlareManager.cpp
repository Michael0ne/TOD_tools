#include "LensFlareManager.h"

LensFlareManager::LensFlareManager() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(LensFlareManager);

	field_64 = 0;
	field_78 = nullptr;
	m_FadeDistance = 100.0f;
	m_LineCheckLength = 100.0f;
	m_Unknown_1 = Vector4f(10000.0f, 10000.0f, 10000.0f, 0.0f);
	m_SizeReduction = 0.0f;
	m_EmitterSize = 0.0f;

	//	TODO: create array of collisionprobes.
}