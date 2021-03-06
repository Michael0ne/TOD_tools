#pragma once

#include "Node.h"

#define WIND_CLASS_SIZE 436

class Wind : public Node
{
protected:
	Vector4f m_Pos[20];
	float m_f190;
	float m_f194;
	float m_f198;
	float m_f19C;
	float m_WindSpeed;
	float m_WindFlux;
	float m_WindFrenzy;
	float m_FrenzyAngle;
	float m_VariationSpeed;
public:
	Wind();	//	@8D2200
};

static_assert(sizeof(Wind) == WIND_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Wind));