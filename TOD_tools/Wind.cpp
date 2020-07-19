#include "Wind.h"

Wind::Wind() : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(Wind);

	for (int ind = 0; ind < 20; ind++)
		m_Pos[ind] = Vector4f();

	m_f194 = m_f190 = 0.0f;
	m_f19C = m_f198 = 1.0f;

	m_WindSpeed = 1.0f;
	m_WindFlux = m_WindFrenzy = m_VariationSpeed = 0.5f;
	m_FrenzyAngle = 180.0f;
}