#include "Camera.h"
#include "Scene.h"
#include "BuiltinType.h"
#include "NumberType.h"

EntityType* tCamera;
Vector4f Camera::ActiveCameraPosition;

void Camera::StoreActiveCameraPosition()
{
    DirectX::XMMATRIX cameraMatrix;
    Scene::SceneInstance->m_ActiveCamera->GetMatrix(cameraMatrix);

    ActiveCameraPosition = *(Vector4f*)&cameraMatrix.r[3];
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

void Camera::Update()
{
    Orientation orient;
    if (m_Position)
        orient = m_Position->m_Orientation;
    else
        orient = { 0.f, 0.f, 0.f, BuiltinType::Orient.z };

    const float d = (1.f / sqrtf((orient.x * orient.x) + (orient.w * orient.w) + (orient.y * orient.y) + (orient.z * orient.z)));
    Orientation normalisedOrient =
    {
        orient.w * d,
        orient.x * d,
        orient.y * d,
        orient.z * d
    };

    SetOrient(normalisedOrient);
    StoreActiveCameraPosition();
}

void Camera::GetMatrix(DirectX::XMMATRIX& outmat) const
{
    DirectX::XMMATRIX nodeMatrix;
    GetWorldMatrix(nodeMatrix);

    outmat.r[0] =
    {
     (((nodeMatrix.r[3].m128_f32[0] * 0) + (nodeMatrix.r[2].m128_f32[0] * 0)) + (nodeMatrix.r[1].m128_f32[0] * 0)) + (nodeMatrix.r[0].m128_f32[0] * 0),
     (((nodeMatrix.r[3].m128_f32[1] * 0) + (nodeMatrix.r[2].m128_f32[1] * 0)) + (nodeMatrix.r[1].m128_f32[1] * 0)) + (nodeMatrix.r[0].m128_f32[1] * 0),
     (((nodeMatrix.r[1].m128_f32[2] * 0) + (nodeMatrix.r[3].m128_f32[2] * 0)) + (nodeMatrix.r[0].m128_f32[2] * 1)) + (nodeMatrix.r[2].m128_f32[2] * 0),
     (((nodeMatrix.r[1].m128_f32[3] * 0) + (nodeMatrix.r[0].m128_f32[3] * 1)) + (nodeMatrix.r[3].m128_f32[3] * 0)) + (nodeMatrix.r[2].m128_f32[3] * 0)
    };

    outmat.r[1] =
    {
     (((0 * nodeMatrix.r[0].m128_f32[0]) + (0 * nodeMatrix.r[3].m128_f32[0])) + (0 * nodeMatrix.r[2].m128_f32[0])) + (1 * nodeMatrix.r[1].m128_f32[0]),
     (((0 * nodeMatrix.r[2].m128_f32[1]) + (1 * nodeMatrix.r[1].m128_f32[1])) + (0 * nodeMatrix.r[3].m128_f32[1])) + (0 * nodeMatrix.r[0].m128_f32[1]),
     (((0 * nodeMatrix.r[2].m128_f32[2]) + (1 * nodeMatrix.r[1].m128_f32[2])) + (0 * nodeMatrix.r[3].m128_f32[2])) + (0 * nodeMatrix.r[0].m128_f32[2]),
     (((0 * nodeMatrix.r[2].m128_f32[3]) + (1 * nodeMatrix.r[1].m128_f32[3])) + (0 * nodeMatrix.r[3].m128_f32[3])) + (0 * nodeMatrix.r[0].m128_f32[3])
    };

    outmat.r[2] =
    {
     (((0 * nodeMatrix.r[0].m128_f32[0]) + (0 * nodeMatrix.r[3].m128_f32[0])) + (1 * nodeMatrix.r[2].m128_f32[0])) + (0 * nodeMatrix.r[1].m128_f32[0]),
     (((0 * nodeMatrix.r[1].m128_f32[1]) + (1 * nodeMatrix.r[2].m128_f32[1])) + (0 * nodeMatrix.r[3].m128_f32[1])) + (0 * nodeMatrix.r[0].m128_f32[1]),
     (((0 * nodeMatrix.r[1].m128_f32[2]) + (1 * nodeMatrix.r[2].m128_f32[2])) + (0 * nodeMatrix.r[3].m128_f32[2])) + (0 * nodeMatrix.r[0].m128_f32[2]),
     (((0 * nodeMatrix.r[1].m128_f32[3]) + (1 * nodeMatrix.r[2].m128_f32[3])) + (0 * nodeMatrix.r[3].m128_f32[3])) + (0 * nodeMatrix.r[0].m128_f32[3])
    };

    outmat.r[3] =
    {
     (((0 * nodeMatrix.r[0].m128_f32[0]) + (1 * nodeMatrix.r[3].m128_f32[0])) + ((0 - m_Offset) * nodeMatrix.r[2].m128_f32[0])) + (0 * nodeMatrix.r[1].m128_f32[0]),
     (((0 * nodeMatrix.r[1].m128_f32[1]) + ((0 - m_Offset) * nodeMatrix.r[2].m128_f32[1])) + (1 * nodeMatrix.r[3].m128_f32[1])) + (0 * nodeMatrix.r[0].m128_f32[1]),
     (((0 * nodeMatrix.r[1].m128_f32[2]) + ((0 - m_Offset) * nodeMatrix.r[2].m128_f32[2])) + (1 * nodeMatrix.r[3].m128_f32[2])) + (0 * nodeMatrix.r[0].m128_f32[2]),
     (((0 * nodeMatrix.r[1].m128_f32[3]) + ((0 - m_Offset) * nodeMatrix.r[2].m128_f32[3])) + (1 * nodeMatrix.r[3].m128_f32[3])) + (0 * nodeMatrix.r[0].m128_f32[3])
    };
}

float Camera::GetOffset() const
{
    return m_Offset;
}

void Camera::SetOffset(const float offset)
{
    StoreProperty(10, &m_Offset, tNUMBER);
    m_Offset = offset;
}

float Camera::GetNearClip() const
{
    return m_NearClip;
}

void Camera::SetNearClip(const float nearclip)
{
    StoreProperty(12, &m_NearClip, tNUMBER);
    m_NearClip = nearclip;
}

float Camera::GetFarClip() const
{
    return m_FarClip;
}

void Camera::SetFarClip(const float farclip)
{
    StoreProperty(13, &m_FarClip, tNUMBER);
    m_FarClip = farclip;
}

float Camera::GetFov() const
{
    return m_Fov;
}

void Camera::SetFov(const float fov)
{
    StoreProperty(11, &m_Fov, tNUMBER);
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
    DirectX::XMMATRIX mat;
    GetMatrix(mat);

    *pos = { mat.r[3].m128_f32[0], mat.r[3].m128_f32[1], mat.r[3].m128_f32[2] };
}

void Camera::Project(float* params)
{
    const Vector4f invec(params[3], params[4], params[5], 0.f);
    Vector2f outvec;

    Project_Impl(outvec, invec);

    *(Vector2f*)params = outvec;
}

#pragma message(TODO_IMPLEMENTATION)
void Camera::Project_Impl(Vector2f& projectedPos, const Vector4f& inpos) const
{
    if (inpos.z <= 0)
    {
        projectedPos = { -10000, -10000 };
        return;
    }
}

void Camera::Register()
{
    tCamera = new EntityType("Camera");
    tCamera->InheritFrom(tNode);
    tCamera->SetCreator((CREATOR)Create);

    tCamera->RegisterProperty(tNUMBER, "offset", (EntityGetterFunction)&GetOffset, 0, 0, 0, (EntitySetterFunction)&SetOffset, 0, 0, 0, nullptr, 0, 0, 10);
    tCamera->RegisterProperty(tNUMBER, "nearclip", (EntityGetterFunction)&GetNearClip, 0, 0, 0, (EntitySetterFunction)&SetNearClip, 0, 0, 0, "control=slider|min=0.1|max=1.0", 0, 0, 12);
    tCamera->RegisterProperty(tNUMBER, "farclip", (EntityGetterFunction)&GetFarClip, 0, 0, 0, (EntitySetterFunction)&SetFarClip, 0, 0, 0, "control=slider|min=100|max=1000", 0, 0, 13);
    tCamera->RegisterProperty(tNUMBER, "fov", (EntityGetterFunction)&GetFov, 0, 0, 0, (EntitySetterFunction)&SetFov, 0, 0, 0, "control=slider|min=20|max=170", 0, 0, 11);
    tCamera->RegisterProperty(tNUMBER, "dynlightcullrange", (EntityGetterFunction)&GetDynlightCullRange, 0, 0, 0, (EntitySetterFunction)&SetDynlightCullRange, 0, 0, 0, "control=slider|min=20|max=1000", 0, 0, -1);

    tCamera->RegisterScript("getcamerapos:vector", (EntityFunctionMember)&GetCameraPos, 0, 0, 0, nullptr, nullptr);
    tCamera->RegisterScript("project(vector):vector", (EntityFunctionMember)&Project, 0, 0, 0, nullptr, nullptr);

    tCamera->PropagateProperties();
}

Camera* Camera::Create(AllocatorIndex)
{
    return new Camera();
}

CameraMatrix::CameraMatrix()
{
    MESSAGE_CLASS_CREATED(CameraMatrix);

    SetupMatrix(BuiltinType::ZeroVector, BuiltinType::Orient, 70.f, 50.f, 1000.f);
}

#pragma message(TODO_IMPLEMENTATION)
void CameraMatrix::SetupMatrix(const Vector4f& pos, const Orientation& orient, const float fov, const float nearclip, const float farclip)
{
    m_Vec[5] = pos;

    float orientdist = sqrtf(orient.w * orient.w + orient.x * orient.x + orient.y * orient.y + orient.z * orient.z);
    float fovsinneg = -sinf(fov * 0.0087266462f);
    float fovcosneg = -cosf(fov * 0.0087266462f);
    float fovcos = cosf(fov * 0.0087266462f);
    Vector4f invorient = {
     1.f / orientdist * orient.w,
     1.f / orientdist * orient.x,
     1.f / orientdist * orient.y,
     1.f / orientdist * orient.z
    };
}