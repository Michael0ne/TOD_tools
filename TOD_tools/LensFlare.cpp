#include "LensFlare.h"

LensFlare::LensFlare() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(LensFlare);

	m_ResourceInfo = nullptr;
	field_54 = 1;
	m_Offset = 0.0f;
	m_Size = 100.0f;
	m_BlendMode = 1;
	m_FadeCameraAngle1 = 20.0f;
	m_f68 = (*(float(*)(float))0x4659A0)(0.017453292f * 20.0f);
	m_FadeCameraAngle2 = 40.0f;
	m_f70 = (*(float(*)(float))0x4659A0)(0.017453292f * 40.0f);
	m_FadeCamBlindDiming = 0.8f;
	m_FadeFlareAngle = 180.0f;
	m_f90 = (*(float(*)(float))0x4659A0)(0.017453292f * 180.0f);
	m_Color = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
	m_InverseFlareAngle = false;
	m_Opacity = 1.0f;
}