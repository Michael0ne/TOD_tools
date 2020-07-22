#pragma once

#include "Node.h"

#define GEKEYFRAME_CLASS_SIZE 140

class GEKeyFrame : public Node
{
protected:
	float m_KeyTranslationX;
	float m_KeyTranslationY;
	float m_KeyTranslationZ;
	float m_f5C;
	float m_KeyScaleX;
	float m_KeyScaleY;
	float m_KeyScaleZ;
	float m_f6C;
	float m_KeyRotationX;
	float m_KeyRotationY;
	float m_KeyRotationZ;
	float m_f7C;
	float m_KeyTime;
	float m_KeyOpacity;
	float m_KeyTextureScroll;

public:
	GEKeyFrame() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(GEKeyFrame);

		m_KeyTime = 0.0f;
		m_KeyTranslationX = m_KeyTranslationY = m_KeyTranslationZ = 0.0f;
		m_f5C = 0.0f;
		m_KeyScaleX = m_KeyScaleY = m_KeyScaleZ = 0.0f;
		m_f6C = 0.0f;
		m_KeyRotationX = m_KeyRotationY = m_KeyRotationZ = 0.0f;
		m_f7C = 0.0f;
		m_KeyOpacity = 1.0f;
		m_KeyTextureScroll = 0.0f;
	}
};

static_assert(sizeof(GEKeyFrame) == GEKEYFRAME_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GEKeyFrame));