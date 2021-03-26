#include "Camera.h"
#include "Scene.h"

EntityType*	tCamera;
Vector4f Camera::ActiveCameraPosition;

void Camera::StoreActiveCameraPosition()
{
	D3DXMATRIX cameraMatrix;
	Scene::SceneInstance->m_ActiveCamera->GetMatrix(cameraMatrix);

	ActiveCameraPosition = *(Vector4f*)&cameraMatrix._41;
	ActiveCameraPosition.a = (float)tan((0.017453292 * Scene::SceneInstance->m_ActiveCamera->m_Fov) * 0.5f);
}

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
	field_4C = BuiltinType::ZeroVector;
}

void Camera::GetMatrix(D3DXMATRIX& outmat) const
{
	D3DXMATRIX nodeMatrix;
	GetWorldMatrix(nodeMatrix);

	outmat._11 = (((nodeMatrix._41 * 0.f) + (nodeMatrix._31 * 0.f)) + (nodeMatrix._21 * 0.f)) + (nodeMatrix._11 * 0.f);
	outmat._12 = (((nodeMatrix._42 * 0.f) + (nodeMatrix._32 * 0.f)) + (nodeMatrix._22 * 0.f)) + (nodeMatrix._12 * 0.f);
	outmat._13 = (((nodeMatrix._23 * 0.f) + (nodeMatrix._43 * 0.f)) + (nodeMatrix._13 * 1.f)) + (nodeMatrix._33 * 0.f);
	outmat._14 = (((nodeMatrix._24 * 0.f) + (nodeMatrix._14 * 1.f)) + (nodeMatrix._44 * 0.f)) + (nodeMatrix._34 * 0.f);
	outmat._21 = (((0.f * nodeMatrix._11) + (0.f * nodeMatrix._41)) + (0.f * nodeMatrix._31)) + (1.f * nodeMatrix._21);
	outmat._22 = (((0.f * nodeMatrix._32) + (1.f * nodeMatrix._22)) + (0.f * nodeMatrix._42)) + (0.f * nodeMatrix._12);
	outmat._23 = (((0.f * nodeMatrix._33) + (1.f * nodeMatrix._23)) + (0.f * nodeMatrix._43)) + (0.f * nodeMatrix._13);
	outmat._24 = (((0.f * nodeMatrix._34) + (1.f * nodeMatrix._24)) + (0.f * nodeMatrix._44)) + (0.f * nodeMatrix._14);
	outmat._31 = (((0.f * nodeMatrix._11) + (0.f * nodeMatrix._41)) + (1.f * nodeMatrix._31)) + (0.f * nodeMatrix._21);
	outmat._32 = (((0.f * nodeMatrix._22) + (1.f * nodeMatrix._32)) + (0.f * nodeMatrix._42)) + (0.f * nodeMatrix._12);
	outmat._33 = (((0.f * nodeMatrix._23) + (1.f * nodeMatrix._33)) + (0.f * nodeMatrix._43)) + (0.f * nodeMatrix._13);
	outmat._34 = (((0.f * nodeMatrix._24) + (1.f * nodeMatrix._34)) + (0.f * nodeMatrix._44)) + (0.f * nodeMatrix._14);
	outmat._41 = (((0.f * nodeMatrix._11) + (1.f * nodeMatrix._41)) + ((0.f - m_Offset) * nodeMatrix._31)) + (0.f * nodeMatrix._21);
	outmat._42 = (((0.f * nodeMatrix._22) + ((0.f - m_Offset) * nodeMatrix._32)) + (1.f * nodeMatrix._42)) + (0.f * nodeMatrix._12);
	outmat._43 = (((0.f * nodeMatrix._23) + ((0.f - m_Offset) * nodeMatrix._33)) + (1.f * nodeMatrix._43)) + (0.f * nodeMatrix._13);
	outmat._44 = (((0.f * nodeMatrix._24) + ((0.f - m_Offset) * nodeMatrix._34)) + (1.f * nodeMatrix._44)) + (0.f * nodeMatrix._14);
}

#pragma message(TODO_IMPLEMENTATION)
void Camera::Register()
{
	tCamera = new EntityType("Camera");
	tCamera->InheritFrom(tNode);
	tCamera->SetCreator((EntityType::CREATOR)Create);
}

Camera* Camera::Create(AllocatorIndex)
{
	return new Camera();
}