#pragma once

#include "Node.h"

#define LENSFLAREMANAGER_CLASS_SIZE 176

class LensFlareManager : public Node
{
protected:
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	Vector4f m_Unknown_1;
	int* field_78;
	class CollisionProbe* m_CollisionProbe[4];
	int field_8C;
	int field_90;
	int field_94;
	float m_SizeReduction;
	float m_FadeDistance;
	float m_OcclusionFactor;
	int field_A4;
	float m_EmitterSize;
	float m_LineCheckLength;

public:
	LensFlareManager();	//	@8E3960
};

static_assert(sizeof(LensFlareManager) == LENSFLAREMANAGER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(LensFlareManager));