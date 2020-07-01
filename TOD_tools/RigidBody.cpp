#include "RigidBody.h"

void RigidBody::SetBoxInertialMatrix_Impl(const Vector4f& box)
{
	(*(void(__thiscall*)(RigidBody*, const Vector4f*))0x9332C0)(this, &box);
}

RigidBody::RigidBody()
{

}