#pragma once

#include "Camera.h"

#define EDITORCAMERA_CLASS_SIZE 312

class EditorCamera : public Camera
{
protected:
	char field_D4;
	char field_D5;
	char field_D6;
	char field_D7;
	float m_fD8;
	float m_fDC;
	char field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;
	int field_100;
	int field_104;
	char field_108;
	Orientation m_Orient_1;
	Orientation m_Orient_2;
	int field_12C;
	int field_130;
	int field_134;
public:
	EditorCamera();	//	@484B20
};

static_assert(sizeof(EditorCamera) == EDITORCAMERA_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(EditorCamera));