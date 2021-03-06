#include "Placeholder.h"

PlaceHolder::PlaceHolder() : Model()
{
	MESSAGE_CLASS_CREATED(PlaceHolder);

	m_Flags = m_Flags & 0xFFFFFFFC | 0x3FFFC;
	m_BoundRadius = 1.0f;
	m_PlaceholderCollisionPivot = nullptr;
}