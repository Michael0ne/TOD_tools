#pragma once
#include "Node.h"

class Camera : public Node
{
protected:
	int				field_50[24];
	float			m_Offset;
	float			m_Fov;
	float			m_NearClip;
	float			m_FarClip;
	float			m_DynlightCullRange;
	Vector4f		field_4C;
public:
	Camera();	//	@87D8F0

	void			GetMatrix(D3DXMATRIX& outmat) const;	//	@87BC60

	static Vector4f	ActiveCameraPosition;	//	@A3D898

	static void		StoreActiveCameraPosition();	//	@87E160

	static void		Register();	//	@87E1E0
	static Camera*	Create(AllocatorIndex);	//	@87E560
};

extern EntityType*	tCamera;	//	@A3D80C

ASSERT_CLASS_SIZE(Camera, 212);