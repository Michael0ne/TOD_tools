#pragma once

#include "Node.h"

#define SHADOW_CLASS_SIZE 192

class Shadow : public Node
{
protected:
	int field_50;
	int field_54;
	int* m_ResourceInfo;
	int field_5C;
	class CollisionProbe* m_CollisionProbe;
	float m_ShadowRadiusX;
	float m_ShadowRadiusY;
	String m_BoneName;
	Node* m_Bone;
	int field_80;
	Vector4f m_Unknown;
	unsigned int m_Flags;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	int field_A8;
	int field_AC;
	int field_B0;
	int field_B4;
	int m_Hints;
	int field_BC;

public:
	Shadow();	//	@8EE3B0

	static int&		TotalShadows;	//	@A3E094
};

static_assert(sizeof(Shadow) == SHADOW_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Shadow));