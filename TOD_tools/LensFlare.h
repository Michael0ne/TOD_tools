#pragma once

#include "Node.h"

#define LENSFLARE_CLASS_SIZE 152

class LensFlare : public Node
{
protected:
	int* m_ResourceInfo;
	int field_54;
	float m_Offset;
	float m_Size;
	int m_BlendMode;
	float m_FadeCameraAngle1;
	float m_f68;
	float m_FadeCameraAngle2;
	float m_f70;
	float m_FadeCamBlindDiming;
	float m_Opacity;
	ColorRGB m_Color;
	float m_FadeFlareAngle;
	float m_f90;
	char m_InverseFlareAngle;

public:
	LensFlare();	//	@8E3F50
};

static_assert(sizeof(LensFlare) == LENSFLARE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(LensFlare));