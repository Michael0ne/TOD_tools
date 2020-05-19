#include "Builtin.h"

Builtin* g_Builtin = NULL;

int Builtin::GetMessageId_Impl(const char* szMessage)
{
	return (*(int (*)(const char*))0x872410)(szMessage);
}

const Vector4f& Builtin::m_ZeroVector = *(Vector4f*)0x9B7084;
const Vector4f& Builtin::m_RightVector = *(Vector4f*)0x9B70A4;
const Vector4f& Builtin::m_UpVector = *(Vector4f*)0x9B70B4;
const Vector4f& Builtin::m_InVector = *(Vector4f*)0x9B70C4;
const Quaternion<float>& Builtin::m_Orientation = *(Quaternion<float>*)0x9B70D4;
const Vector4f& Builtin::m_LeftVector = *(Vector4f*)0x9B70E4;
const Vector4f& Builtin::m_DownVector = *(Vector4f*)0x9B70F4;
const Vector4f& Builtin::m_OutVector = *(Vector4f*)0x9B7104;
const Vector4f& Builtin::m_vUnkColor = *(Vector4f*)0x9B7114;