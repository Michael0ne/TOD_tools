#include "Builtin.h"

Builtin* tBuiltin = nullptr;

Builtin::Builtin() : ScriptTypes::ScriptType_Entity("builtin")
{
	MESSAGE_CLASS_CREATED(Builtin);

	m_HandlersList = List<Builtin_Handler>(0x19300);

	field_88 = 0 | 0x80000000;
	field_90 = 0;
	field_94 = 0;
	field_8C = 50 & 0xFF800000 | 0x800000;
}

#pragma message(TODO_IMPLEMENTATION)
Builtin::~Builtin()
{
	MESSAGE_CLASS_DESTROYED(Builtin);
}

#pragma message(TODO_IMPLEMENTATION)
void Builtin::RegisterHandler(const char* handlerProto, void* handler, const char* handlerName)
{

}

#pragma message(TODO_IMPLEMENTATION)
void Builtin::RegisterMember(ScriptTypes::ScriptType* returnType, const char* memberName, void* memberProc, int unk1, const char* memberProto, const char* unk2)
{

}

void Builtin::GetMessageId(void* params)
{
	*((int*)params) = GetMessageId_Impl((const char*)((int*)params + 4));
}

#pragma message(TODO_IMPLEMENTATION)
int Builtin::GetMessageId_Impl(const char* msg)
{
	return NULL;
}

const Vector4f& Builtin::ZeroVector = *(Vector4f*)0x9B7084;
const Vector4f& Builtin::RightVector = *(Vector4f*)0x9B70A4;
const Vector4f& Builtin::UpVector = *(Vector4f*)0x9B70B4;
const Vector4f& Builtin::InVector = *(Vector4f*)0x9B70C4;
const Orientation& Builtin::Orientation = *(Quaternion<float>*)0x9B70D4;
const Vector4f& Builtin::LeftVector = *(Vector4f*)0x9B70E4;
const Vector4f& Builtin::DownVector = *(Vector4f*)0x9B70F4;
const Vector4f& Builtin::OutVector = *(Vector4f*)0x9B7104;
const Vector4f& Builtin::UnkColor = *(Vector4f*)0x9B7114;