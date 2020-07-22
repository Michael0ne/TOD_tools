#pragma once

#include "stdafx.h"
#include "Types.h"
#include "StringsPool.h"
#include "MemoryAllocators.h"
#include "List.h"

#define SCRIPT_TYPE_CLASS_SIZE 32
#define SCRIPT_TYPE_ENTITY_CLASS_SIZE 120

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
		TYPE_SCRIPT = 11,		//	SCRIPT WITH PARAMETERS
		TYPE_LAST_DONTUSE
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

	static const char* szScriptTypeName[TYPE_LAST_DONTUSE] = {
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
	public:
		String m_sTypeName;
		int m_nTypeId;
		int m_nSizeInBytes;
		int m_nId;

		virtual ScriptType* scalar_destructor(bool freeMemory);	//	@867A70
		virtual int _489370(int* unk1, int* unk2);	//	@489370
		virtual void GetDefaultValue(void* unk1);	//	NOTE: this pointer type is actually class-dependent. Returns zero (default value?).
		virtual void nullsub_2(String* unk2);
		virtual void _4893A0(int* unk1, int* unk2);	//	@4893A0
		virtual String* PrintFormatted(String* outStr, void* value, int precision);	//	@862F50	//	NOTE: lowest word is number of digits after dot, highest - total number of digits.
		virtual int DoOperation(char* unk1, void* unk2);	//	@862AA0
		virtual int _489410(int* unk1);	//	@489410
		virtual int _863760_1(int* unk1, int* unk2);	//	@863760
		virtual int _863760_2(int* unk1, int* unk2);	//	@863760
		virtual int _8637F0(int* unk1, String* unk2, int unk3);	//	@8637F0
		virtual int _862A50(int unk1, int* unk2, int* unk3);	//	@862A50
		virtual void nullsub_3(int unk1, void* unk2, int unk3, int unk4, int unk5, void* outop);
		virtual void nullsub_4(int* unk1, int unk2, int unk3, int unk4, int unk5, int unk6);
		virtual bool AreParamsEqual(int unk1, int unk2);	//	@7A1F00
		virtual bool AreParamsNotEqual(int unk1, int unk2);	//	@862AB0
		virtual void GetOperationType(const char* operationStr, int* outOperationId, ScriptType** outResultType, char* unk4);	//	@8637D0
		virtual void PerformOperation(int operationId, void* params);	//	@8C4D60	//	NOTE: get operation id by calling 'GetOperationType'. Params is of type of the class, zero element stores result of operation.
		virtual char _8637E0(int unk1, int unk2);	//	@8637E0
		virtual bool IsInfinite(void* unk1);	//	@489440

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

	static int GetTypeSize(ScriptType* type);	//	@862AC0
	static class Entity* GetScriptEntityByName(const char* name);	//	@862C70

	class ScriptType_Nothing : public ScriptType
	{
	private:

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

	struct EntityProperties
	{
	private:
		ScriptTypes::ScriptType* m_ReturnType;
		int field_4;
		void* m_Getter;
		int field_C;
		int field_10;
		int field_14;
		void* m_Setter;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
	};

	class ScriptType_Entity : public ScriptType
	{
	protected:
		void* (*m_Creator)(unsigned int allocatorIndex);
		ScriptType_Entity* m_Parent;
		class GlobalNode* m_ParentNode;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		List<EntityProperties> m_PropertiesList_1;
		List<EntityProperties> m_PropertiesList_2;
		int field_6C;
		int field_70;
		char field_74;
	public:
		ScriptType_Entity(const char* szEntityName);	//	@86CC00
		~ScriptType_Entity()
		{
			MESSAGE_CLASS_DESTROYED(ScriptType_Entity);
		}

		void*	CreateNode();	//	@86C770
	};

	ScriptType_Entity*			tENTITY = (ScriptType_Entity*)0xA3CEE0;		//	@A3CEE0

	ScriptType_Nothing*			tNOTHING = (ScriptType_Nothing*)0xA3CE94;	//	@A3CE94
	ScriptType_Number*			tNUMBER = (ScriptType_Number*)0xA3CEC0;		//	@A3CEC0
	ScriptType_Integer*			tINTEGER = (ScriptType_Integer*)0xA3CEB8;	//	@A3CEB8
	ScriptType_Boolean*			tBOOLEAN = (ScriptType_Boolean*)0xA3CEC4;	//	@A3CEC4
	ScriptType_Vector*			tVECTOR = (ScriptType_Vector*)0xA3CEB4;		//	@A3CEB4
	ScriptType_Quaternion*		tQUATERNION = (ScriptType_Quaternion*)0xA3CE98;	//	@A3CE98
	ScriptType_Color*			tCOLOR = (ScriptType_Color*)0xA3CEA4;		//	@A3CEA4
	ScriptType_String*			tSTRING = (ScriptType_String*)0xA3CEB0;		//	@A3CEB0

	void						Init();	//	@8634E0

	static bool&				TypesListCRCCalculated = *(bool*)0xA3CEC8;	//	@A3CEC8
	static List<ScriptType>&	TypesList = *(List<ScriptType>*)0xA3CECC;	//	@A3CECC
	static int&					TypesListCRC = *(int*)0xA3CEDC;				//	@A3CEDC
}

static_assert(sizeof(ScriptTypes::ScriptType) == SCRIPT_TYPE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ScriptTypes::ScriptType));
static_assert(sizeof(ScriptTypes::ScriptType_Entity) == SCRIPT_TYPE_ENTITY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ScriptTypes::ScriptType_Entity));