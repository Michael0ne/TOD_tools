#pragma once

#include "ScriptTypes.h"

#define BUILTIN_CLASS_SIZE 152

struct Builtin_Member
{
	ScriptTypes::ScriptType* m_ReturnType;
	void* m_GetProcPtr;
	void* m_SetProcPtr;
	String m_MemberProto;
	String m_Unknown_2;
};

struct Builtin_Handler
{
	String m_sProto;
	void* m_pFunction;
	String m_sName;
};

class Builtin : public ScriptTypes::ScriptType_Entity
{
protected:
	List<Builtin_Handler> m_HandlersList;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;

public:
	Builtin();	//	@486D00
	~Builtin()
	{
		MESSAGE_CLASS_DESTROYED(Builtin);
	}

	void	RegisterHandler(const char* handlerProto, void* handler, const char* handlerName);	//	@486430
	void	RegisterMember(ScriptTypes::ScriptType* returnType, const char* memberName, void* memberProc, int unk1, const char* memberProto, const char* unk2);	//	@486D90

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	static const Vector4f& ZeroVector;
	static const Vector4f& RightVector;
	static const Vector4f& UpVector;
	static const Vector4f& InVector;
	static const Orientation& Orientation;
	static const Vector4f& LeftVector;
	static const Vector4f& DownVector;
	static const Vector4f& OutVector;
	static const Vector4f& UnkColor;
};

static_assert(sizeof(Builtin) == BUILTIN_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Builtin));