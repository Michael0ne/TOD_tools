#include "Camera.h"

ScriptType_Entity*	tCamera;

#pragma message(TODO_IMPLEMENTATION)
Camera::Camera() : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(Camera);

	(*(void(__thiscall*)(int*))0x4087C0)(field_50);	//	TODO: some method that does set up matrix projection with default parameters?

	m_Offset = 0.f;
	m_Fov = 70.f;
	m_NearClip = 1.f;
	m_FarClip = 1000.f;
	m_DynlightCullRange = 100.f;
	field_4C = ScriptType_Builtin::ZeroVector;
}

#pragma message(TODO_IMPLEMENTATION)
void Camera::Register()
{
	tCamera = new ScriptType_Entity("Camera");
	tCamera->InheritFrom(tNode);
	tCamera->SetCreator((ScriptType_Entity::CREATOR)Create);
}

Camera* Camera::Create(AllocatorIndex)
{
	return new Camera();
}