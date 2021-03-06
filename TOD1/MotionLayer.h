#pragma once

#include "AnimLayer.h"

#define MOTIONLAYER_CLASS_SIZE 392

class MotionLayer : public AnimLayer
{
protected:
	Vector4f m_WorldStartPos;
	Orientation m_WorldStartOrient;
	Vector4f m_WorldEndPos;
	Orientation m_WorldEndOrient;
	char m_UpdateAbsolute;
public:
	MotionLayer() : AnimLayer()	//	NOTE: no constructor.
	{
		m_WorldStartOrient = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
		m_WorldEndOrient = Orientation(0.0f, 0.0f, 0.0f, 1.0f);

		m_UpdateAbsolute = false;
	}
};

static_assert(sizeof(MotionLayer) == MOTIONLAYER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(MotionLayer));