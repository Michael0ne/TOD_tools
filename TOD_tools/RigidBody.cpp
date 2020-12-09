#include "RigidBody.h"

RigidBody::RigidBody() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(RigidBody);

	m_Orientation_t = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
	m_UnkVec_6 = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
	
	m_Unknown_1 = List<int>(0x25300);
	
	m_CollMass = 10.0f;

	m_Position_t = Vector4f();
	m_LinearSpeed = Vector4f();
	m_AngularMomentum = Vector4f();
	m_LinearVelocity = Vector4f();
	m_CollAngularSpeed = Vector4f();

	m_UnkVec_1 = Vector4f(1.0f, 1.0f, 1.0f, 0.0f);

	SetBoxInertialMatrix(&m_UnkVec_1);

	field_22D = 0;
	m_Grounded = false;
}

#pragma message(TODO_IMPLEMENTATION)
void RigidBody::SetBoxInertialMatrix(const Vector4f* vec)
{
	(*(void(__thiscall*)(RigidBody*, const Vector4f*))0x9332C0)(this, vec);
}