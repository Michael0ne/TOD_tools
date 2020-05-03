#pragma once

#include "stdafx.h"

#include "Entity.h"

#define CAMERA_CLASS_SIZE 212

struct Camera : public Entity
{
private:
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
	float m_fOffset;
	float m_fFov;
	float m_fNearclip;
	float m_fFarclip;
	float m_fDynlightCullRange;
	Vector4f m_vUnkVec1;

public:
	static Vector4f& CameraPosition;
	static void StoreCameraMatrix();	//	@87E160

	const float GetFov() const
	{
		return m_fFov;
	}
};

static_assert(sizeof(Camera) == CAMERA_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Camera"));