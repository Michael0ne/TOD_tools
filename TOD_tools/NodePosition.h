#pragma once

#include "Types.h"

class Entity;

//	NOTE: is this just D3DMATRIX?
class NodePosition
{
public:
	Quaternion<float> m_vOrientation;
	Vector4<float> m_vPosition2;
	Vector4<float> m_vPosition3;
	Vector4<float> m_vPosition4;
	Vector4<float> m_vPosition5;
	Vector4<float> m_vPosition6;
	Entity* m_pOwner;
};