#include "Camera.h"
#include "Scene.h"

Vector4f& Camera::CameraPosition = *(Vector4f*)0xA3D898;

void Camera::StoreCameraMatrix()
{
	D3DMATRIX cameraPositionMatrix;

	g_Scene->GetCameraEntity()->GetMatrix(&cameraPositionMatrix);

	CameraPosition.x = cameraPositionMatrix._41;
	CameraPosition.y = cameraPositionMatrix._42;
	CameraPosition.z = cameraPositionMatrix._43;
	CameraPosition.a = (float)tan(g_Scene->GetCameraEntity()->GetFov() * 0.017453292f * 0.5f);
}