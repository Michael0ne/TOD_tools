#include "Camera.h"

Camera::Camera() : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(Camera);

	(*(void(__thiscall*)(Vector4f*))0x4087C0)(&m_vUnknown_2);

	m_Offset = 0.0f;
	m_Fov = 70.0f;
	m_NearClip = 1.0f;
	m_FarClip = 1000.0f;
	m_DynlightCullRange = 100.0f;

	m_vUnknown_1 = Vector4<float>();
}