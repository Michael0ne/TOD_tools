#pragma once

#include "Placeholder.h"

#define GOODIE_PLACEHOLDER_CLASS_SIZE 284

class GoodiePlaceHolder : public PlaceHolder
{
protected:
	unsigned int m_Flags_13;
	float m_VIPTimer;
	unsigned int m_Flags_14;
	float m_RespawnTime;

public:
	GoodiePlaceHolder() : PlaceHolder()	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(GoodiePlaceHolder);

		m_Flags_13 = m_Flags_13 & 0xFFFFFC05 | 5;
		m_Flags_14 = m_Flags_14 & 0xFFFFFE00;

		m_VIPTimer = 0.0f;
		m_RespawnTime = 0.0f;
	}
};

static_assert(sizeof(GoodiePlaceHolder) == GOODIE_PLACEHOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GoodiePlaceHolder));