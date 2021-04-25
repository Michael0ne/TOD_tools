#include "Camera.h"
#include "Scene.h"
#include "BuiltinType.h"
#include "NumberType.h"

EntityType*	tCamera;
Vector4f Camera::ActiveCameraPosition;

void Camera::StoreActiveCameraPosition()
{
	D3DXMATRIX cameraMatrix;
	Scene::SceneInstance->m_ActiveCamera->GetMatrix(cameraMatrix);

	ActiveCameraPosition = *(Vector4f*)&cameraMatrix._41;
	ActiveCameraPosition.a = (float)tan((0.017453292 * Scene::SceneInstance->m_ActiveCamera->m_Fov) * 0.5f);
}

Camera::Camera() : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(Camera);

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

float Camera::GetOffset() const
{
	return m_Offset;
}

void Camera::SetOffset(const float offset)
{
	SetParam(10, &m_Offset, tNUMBER);
	m_Offset = offset;
}

float Camera::GetNearClip() const
{
	return m_NearClip;
}

void Camera::SetNearClip(const float nearclip)
{
	SetParam(12, &m_NearClip, tNUMBER);
	m_NearClip = nearclip;
}

float Camera::GetFarClip() const
{
	return m_FarClip;
}

void Camera::SetFarClip(const float farclip)
{
	SetParam(13, &m_FarClip, tNUMBER);
	m_FarClip = farclip;
}

float Camera::GetFov() const
{
	return m_Fov;
}

void Camera::SetFov(const float fov)
{
	SetParam(11, &m_Fov, tNUMBER);
	m_Fov = fov;
}

float Camera::GetDynlightCullRange() const
{
	return m_DynlightCullRange;
}

void Camera::SetDynlightCullRange(const float dynlightcullrange)
{
	m_DynlightCullRange = dynlightcullrange;
}

void Camera::GetCameraPos(Vector3f* pos)
{
	D3DXMATRIX mat;
	GetMatrix(mat);

	*pos = { mat._41, mat._42, mat._43 };
}

void Camera::Project(float* params)
{
	//	TODO: this probably should be a macro or something.
	struct ParamsStruct
	{
		Vector3f  m_ReturnVector;
		Vector3f  m_PosVector;
	} *args = (ParamsStruct*)params;

	Vector4f invec = { args->m_PosVector.x, args->m_PosVector.y, args->m_PosVector.z, 0 };
	Vector2f outvec;

	Project_Impl(outvec, invec);

	args->m_ReturnVector = { outvec.x, outvec.y, 0 };
}

void Camera::Register()
{
	tCamera = new EntityType("Camera");
	tCamera->InheritFrom(tNode);
	tCamera->SetCreator((EntityType::CREATOR)Create);

	tCamera->RegisterProperty(tNUMBER, "offset", &GetOffset, 0, 0, 0, &SetOffset, 0, 0, 0, nullptr, 0, 0, 10);
	tCamera->RegisterProperty(tNUMBER, "nearclip", &GetNearClip, 0, 0, 0, &SetNearClip, 0, 0, 0, "control=slider|min=0.1|max=1.0", 0, 0, 12);
	tCamera->RegisterProperty(tNUMBER, "farclip", &GetFarClip, 0, 0, 0, &SetFarClip, 0, 0, 0, "control=slider|min=100|max=1000", 0, 0, 13);
	tCamera->RegisterProperty(tNUMBER, "fov", &GetFov, 0, 0, 0, &SetFov, 0, 0, 0, "control=slider|min=20|max=170", 0, 0, 11);
	tCamera->RegisterProperty(tNUMBER, "dynlightcullrange", &GetDynlightCullRange, 0, 0, 0, &SetDynlightCullRange, 0, 0, 0, "control=slider|min=20|max=1000", 0, 0, -1);

	tCamera->RegisterScript("getcamerapos:vector", &GetCameraPos, 0, 0, 0, nullptr, nullptr);
	tCamera->RegisterScript("project(vector):vector", &Project, 0, 0, 0, nullptr, nullptr);

	tCamera->_86E9B0();
}

Camera* Camera::Create(AllocatorIndex)
{
	return new Camera();
}

Camera::CameraMatrix::CameraMatrix()
{
	MESSAGE_CLASS_CREATED(CameraMatrix);

	SetupMatrix(BuiltinType::ZeroVector, BuiltinType::Orient, 70.f, 50.f, 1000.f);
}

#pragma message(TODO_IMPLEMENTATION)
void Camera::CameraMatrix::SetupMatrix(const Vector4f& pos, const Orientation& orient, const float fov, const float nearclip, const float farclip)
{
	m_Pos = pos;

	float orientdist = sqrtf(orient.w * orient.w + orient.x * orient.x + orient.y * orient.y + orient.z * orient.z);
	float fovsinneg = -sinf(fov * 0.0087266462f);
	float fovcosneg = -cos(fov * 0.0087266462f);
	float fovcos = cos(fov * 0.0087266462f);
	Vector4f invorient = {
		1.f / orientdist * orient.w,
		1.f / orientdist * orient.x,
		1.f / orientdist * orient.y,
		1.f / orientdist * orient.z
	};
}