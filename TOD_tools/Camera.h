#pragma once
#include "Node.h"

class Camera : public Node
{
protected:
	Vector4f m_vUnknown_2;
	int field_60;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	int field_A8;
	int field_AC;
	float m_Offset;
	float m_Fov;
	float m_NearClip;
	float m_FarClip;
	float m_DynlightCullRange;
	Vector4f m_vUnknown_1;
public:
	Camera();	//	@87D8F0
};

ASSERT_CLASS_SIZE(Camera, 212);