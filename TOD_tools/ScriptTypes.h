#pragma once

#include "stdafx.h"
#include "Types.h"
#include "StringsPool.h"
#include "MemoryAllocators.h"
#include "List.h"

#define SCRIPT_TYPE_CLASS_SIZE 32

namespace ScriptTypes
{
	enum E_SCRIPT_TYPE_ID {
		TYPE_NOTHING = 0,		//	NOTHING! Really!
		TYPE_NUMBER = 1,		//	FLOAT
		TYPE_INTEGER = 2,		//	INTEGER! Really!
		TYPE_STRING = 3,		//	LIBC STRING
		TYPE_TRUTH = 4,			//	BOOLEAN
		TYPE_VECTOR = 5,		//	VECTOR4
		TYPE_QUATERNION = 6,	//	QUATERNION
		TYPE_COLOR = 7,			//	COLOR RGBA
		TYPE_LIST = 8,			//	SIMPLE LIST
		TYPE_DICT = 9,			//	DICTIONARY. (HASHMAP)
		TYPE_ENTITY = 10,		//	SCRIPT ENTITY
		TYPE_SCRIPT = 11		//	SCRIPT WITH PARAMETERS
	};

	enum E_SCRIPT_TYPE_SIZE {
		TYPE_NOTHING_SIZE = 0,
		TYPE_NUMBER_SIZE = 1,
		TYPE_INTEGER_SIZE = 1,
		TYPE_STRING_SIZE = 1,
		TYPE_TRUTH_SIZE = 1,
		TYPE_VECTOR_SIZE = 3,
		TYPE_QUATERNION_SIZE = 4,
		TYPE_COLOR_SIZE = 1,
		TYPE_LIST_SIZE = 1,
		TYPE_DICT_SIZE = 1,
		TYPE_ENTITY_SIZE = 1,
		TYPE_SCRIPT_SIZE = 1
	};

	static const char* szScriptTypeName[TYPE_ENTITY + 1] = {
		"nothing",
		"number",
		"integer",
		"string",
		"truth",
		"vector",
		"quaternion",
		"color",
		"list",
		"dict",
		NULL,
		NULL
	};

	//	NOTE: THIS is base class for all script entities.
	class ScriptType
	{
	private:
		String m_sTypeName;
		int m_nTypeId;
		int m_nSizeInBytes;
		int m_nId;

		virtual void stub0() = 0;
		virtual void stub1() = 0;
		virtual void stub2() = 0;
		virtual void stub3() = 0;
		virtual void stub4() = 0;
		virtual void stub5() = 0;
		virtual void stub6() = 0;
		virtual void stub7() = 0;
		virtual void stub8() = 0;
		virtual void stub9() = 0;
		virtual void stub10() = 0;
		virtual void stub11() = 0;
		virtual void stub12() = 0;
		virtual void stub13() = 0;
		virtual void stub14() = 0;
		virtual void stub15() = 0;
		virtual void stub16() = 0;
		virtual void stub17() = 0;
		virtual void stub18() = 0;
		virtual void stub19() = 0;

	public:
		ScriptType(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize);	//	@862E90

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	class ScriptType_Nothing : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;

	public:
		ScriptType_Nothing(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Nothing);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_Nothing*			tyNothing = (ScriptType_Nothing*)0xA3CE88;	//	@A3CE88

	class ScriptType_Number : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;
	public:
		ScriptType_Number(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Number);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_Number*			tyNumber = (ScriptType_Number*)0xA3CEA8;	//	@A3CEA8

	class ScriptType_Integer : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;
	public:
		ScriptType_Integer(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Integer);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_Integer*			tyInteger = (ScriptType_Integer*)0xA3CE8C;	//	@A3CE8C

	class ScriptType_Boolean : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;
	public:
		ScriptType_Boolean(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Boolean);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_Boolean*			tyBoolean = (ScriptType_Boolean*)0xA3CEBC;	//	@A3CEBC

	class ScriptType_Vector : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;
	public:
		ScriptType_Vector(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Vector);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_Vector*			tyVector = (ScriptType_Vector*)0xA3CEAC;	//	@A3CEAC

	class ScriptType_Quaternion : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;
	public:
		ScriptType_Quaternion(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Quaternion);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_Quaternion*		tyQuaternion = (ScriptType_Quaternion*)0xA3CE90;	//	@A3CE90

	class ScriptType_Color : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;
	public:
		ScriptType_Color(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Color);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_Color*			tyColor = (ScriptType_Color*)0xA3CE9C;		//	@A3CE9C

	class ScriptType_String : public ScriptType
	{
	private:
		virtual void stub0() override;
		virtual void stub1() override;
		virtual void stub2() override;
		virtual void stub3() override;
		virtual void stub4() override;
		virtual void stub5() override;
		virtual void stub6() override;
		virtual void stub7() override;
		virtual void stub8() override;
		virtual void stub9() override;
		virtual void stub10() override;
		virtual void stub11() override;
		virtual void stub12() override;
		virtual void stub13() override;
		virtual void stub14() override;
		virtual void stub15() override;
		virtual void stub16() override;
		virtual void stub17() override;
		virtual void stub18() override;
		virtual void stub19() override;
	public:
		ScriptType_String(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_String);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}
	};

	ScriptType_String*			tyString = (ScriptType_String*)0xA3CEA0;	//	@A3CEA0

	ScriptType_Nothing*			tNOTHING = (ScriptType_Nothing*)0xA3CE94;	//	@A3CE94
	ScriptType_Number*			tNUMBER = (ScriptType_Number*)0xA3CEC0;		//	@A3CEC0
	ScriptType_Integer*			tINTEGER = (ScriptType_Integer*)0xA3CEB8;	//	@A3CEB8
	ScriptType_Boolean*			tBOOLEAN = (ScriptType_Boolean*)0xA3CEC4;	//	@A3CEC4
	ScriptType_Vector*			tVECTOR = (ScriptType_Vector*)0xA3CEB4;		//	@A3CEB4
	ScriptType_Quaternion*		tQUATERNION = (ScriptType_Quaternion*)0xA3CE98;	//	@A3CE98
	ScriptType_Color*			tCOLOR = (ScriptType_Color*)0xA3CEA4;		//	@A3CEA4
	ScriptType_String*			tSTRING = (ScriptType_String*)0xA3CEB0;		//	@A3CEB0

	static void					Init();	//	@8634E0

	static bool&				TypesListCRCCalculated = *(bool*)0xA3CEC8;	//	@A3CEC8
	static List<ScriptType>&	TypesList = *(List<ScriptType>*)0xA3CECC;	//	@A3CECC
	static int&					TypesListCRC = *(int*)0xA3CEDC;				//	@A3CEDC
}

static_assert(sizeof(ScriptTypes::ScriptType) == SCRIPT_TYPE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ScriptTypes::ScriptType));