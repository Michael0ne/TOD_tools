#pragma once

#include "Types.h"
#include "StringsPool.h"
#include "MemoryAllocators.h"
#include "List.h"

namespace ScriptTypes
{

#define SCRIPTTYPE_NOTHING_ID 0
#define SCRIPTTYPE_NOTHING_NAME "nothing"
#define SCRIPTTYPE_NOTHING_SIZE 0

#define SCRIPTTYPE_NUMBER_ID 1
#define SCRIPTTYPE_NUMBER_NAME "number"
#define SCRIPTTYPE_NUMBER_SIZE 1

#define SCRIPTTYPE_INTEGER_ID 2
#define SCRIPTTYPE_INTEGER_NAME "integer"
#define SCRIPTTYPE_INTEGER_SIZE 1

#define SCRIPTTYPE_STRING_ID 3
#define SCRIPTTYPE_STRING_NAME "string"
#define SCRIPTTYPE_STRING_SIZE 1

#define SCRIPTTYPE_TRUTH_ID 4
#define SCRIPTTYPE_TRUTH_NAME "truth"
#define SCRIPTTYPE_TRUTH_SIZE 1

#define SCRIPTTYPE_VECTOR_ID 5
#define SCRIPTTYPE_VECTOR_NAME "vector"
#define SCRIPTTYPE_VECTOR_SIZE 3

#define SCRIPTTYPE_QUATERNION_ID 6
#define SCRIPTTYPE_QUATERNION_NAME "quaternion"
#define SCRIPTTYPE_QUATERNION_SIZE 4

#define SCRIPTTYPE_COLOR_ID 7
#define SCRIPTTYPE_COLOR_NAME "color"
#define SCRIPTTYPE_COLOR_SIZE 1

#define SCRIPTTYPE_LIST_ID 8
#define SCRIPTTYPE_LIST_NAME "list"
#define SCRIPTTYPE_LIST_SIZE 1

#define SCRIPTTYPE_DICT_ID 9
#define SCRIPTTYPE_DICT_NAME "dict"
#define SCRIPTTYPE_DICT_SIZE 1

//	Used for any entity being created.
#define SCRIPTTYPE_ENTITY_ID 10
#define SCRIPTTYPE_ENTITY_NAME NULL
#define SCRIPTTYPE_ENTITY_SIZE 1

	class ScriptType;

	struct ScriptType__vtable
	{
		void* (__stdcall* free)(bool);
		int* (__thiscall* stub1)(ScriptType* _this, int, List<ScriptType>*);
		void(__thiscall* RemoveVtable)(ScriptType* _this);
		void(__stdcall* nullstub1)();
		int* (__thiscall* stub2)(ScriptType* _this, int*, int*);
		void (__cdecl* makeEmptyString)(String* outString, int*, int);	//	SetFormattingPrecision
		signed int(__stdcall* return_negative_one)(const char*, int*);	//	OverrideOperator
		int(__thiscall* stub3)(ScriptType* _this, int);
		int(__thiscall* stub4)(ScriptType* _this, char*, char*);
		int(__thiscall* stub5)(ScriptType* _this, char*, char*);
		int(__thiscall* stub6)(ScriptType* _this, int, String* str, int);
		int(__thiscall* stub7)(ScriptType* _this, int, int, int*);
		int(__stdcall* nullstub2)(int, int, int (__thiscall*)(int), int, int, int, void* outParam);
		int(__stdcall* nullstub3)(int*, int, int (__thiscall*)(int, int), int, int, int);
		bool(__thiscall* stub8)(ScriptType* _this, int, int);
		bool(__stdcall* returnTrue)(void*, void*);	//	return *arg1 != *arg2
		void(__stdcall* _ParseOperator)(const char* operatorStr, int* outOperationId, ScriptType** outResultType, char* dummyParam);
		void(__thiscall* _DoOperation)(ScriptType* _this, const int operationId, void* inParams);
		bool(__stdcall* returnTrue1)(int, int);
		bool(__stdcall* returnTrue2)(int);
	};

	//	@9CB1F8
	struct ScriptType_Nothing__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	//	@9CB250
	struct ScriptType_Number__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	//	@9CB340
	struct ScriptType_Integer__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	//	@9CB428
	struct ScriptType_Boolean__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	//	@9CB4B0
	struct ScriptType_Vector__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	//	@9CB5B8
	struct ScriptType_Quaternion__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	//	@9CB778
	struct ScriptType_Color__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	//	@9CC358
	struct ScriptType_String__vtable {
		void(__thiscall* stub0)(ScriptType* _this);
		void(__thiscall* stub1)(ScriptType* _this);
		void(__thiscall* stub2)(ScriptType* _this);
		void(__thiscall* stub3)(ScriptType* _this);
		void(__thiscall* stub4)(ScriptType* _this);
		void(__thiscall* stub5)(ScriptType* _this);
		void(__thiscall* stub6)(ScriptType* _this);
		void(__thiscall* stub7)(ScriptType* _this);
		void(__thiscall* stub8)(ScriptType* _this);
		void(__thiscall* stub9)(ScriptType* _this);
		void(__thiscall* stub10)(ScriptType* _this);
		void(__thiscall* stub11)(ScriptType* _this);
		void(__thiscall* stub12)(ScriptType* _this);
		void(__thiscall* stub13)(ScriptType* _this);
		void(__thiscall* stub14)(ScriptType* _this);
		void(__thiscall* stub15)(ScriptType* _this);
		void(__thiscall* stub16)(ScriptType* _this);
		void(__thiscall* stub17)(ScriptType* _this);
		void(__thiscall* stub18)(ScriptType* _this);
		void(__thiscall* stub19)(ScriptType* _this);
	};

	class ScriptType
	{
	private:
		ScriptType__vtable* lpVtbl;
		String m_sTypeName;
		int m_nTypeId;
		int m_nSizeInBytes;
		int m_nId;

	public:
		ScriptType()
		{
			lpVtbl = nullptr;
			m_sTypeName = String();
			m_nTypeId = 0;
			m_nSizeInBytes = 0;
			m_nId = 0;

			debug("ScriptTypes::ScriptType created at %X\n", this);
		}

		~ScriptType()
		{
			debug("ScriptTypes::ScriptType destroyed!\n");
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

		void	Register(unsigned int typeId, const char* typeName, unsigned int typeSize);	//	@862E90
		//	NOTE: this is temporary, until proper classes for each type has been made.
		void	SetVtablePtr(ScriptType__vtable* ptr) { lpVtbl = ptr; }
	};

	static ScriptType* tyNothing = (ScriptType*)0xA3CE88;
	static ScriptType* tyNumber = (ScriptType*)0xA3CEA8;
	static ScriptType* tyInteger = (ScriptType*)0xA3CE8C;
	static ScriptType* tyBoolean = (ScriptType*)0xA3CEBC;
	static ScriptType* tyVector = (ScriptType*)0xA3CEAC;
	static ScriptType* tyQuaternion = (ScriptType*)0xA3CE90;
	static ScriptType* tyColor = (ScriptType*)0xA3CE9C;
	static ScriptType* tyString = nullptr;

	//	In scripts, use these.
	static ScriptType** tNOTHING = (ScriptType**)0xA3CE94;
	static ScriptType** tNUMBER = (ScriptType**)0xA3CEC0;
	static ScriptType** tINTEGER = (ScriptType**)0xA3CEB8;
	static ScriptType** tBOOLEAN = (ScriptType**)0xA3CEC4;
	static ScriptType** tVECTOR = (ScriptType**)0xA3CEB4;
	static ScriptType** tQUATERNION = (ScriptType**)0xA3CE98;
	static ScriptType** tCOLOR = (ScriptType**)0xA3CEA4;
	static ScriptType** tSTRING = (ScriptType**)0xA3CEB0;

	static const ScriptType_Nothing__vtable* tNOTHING_vtable = (ScriptType_Nothing__vtable*)0x9CB1F8;
	static const ScriptType_Number__vtable* tNUMBER_vtable = (ScriptType_Number__vtable*)0x9CB250;
	static const ScriptType_Integer__vtable* tINTEGER_vtable = (ScriptType_Integer__vtable*)0x9CB340;
	static const ScriptType_Boolean__vtable* tBOOLEAN_vtable = (ScriptType_Boolean__vtable*)0x9CB428;
	static const ScriptType_Vector__vtable* tVECTOR_vtable = (ScriptType_Vector__vtable*)0x9CB4B0;
	static const ScriptType_Quaternion__vtable* tQUATERNION_vtable = (ScriptType_Quaternion__vtable*)0x9CB5B8;
	static const ScriptType_Color__vtable* tCOLOR_vtable = (ScriptType_Color__vtable*)0x9CB778;
	static const ScriptType_String__vtable* tSTRING_vtable = (ScriptType_String__vtable*)0x9CC358;

	void Init();	//	@8634E0

	static List<ScriptType>& ListTypes = *(List<ScriptType>*)0xA3CECC;	//	@A3CECC
	static int& _A3CEC8 = *(int*)0xA3CEC8;	//	@A3CEC8
}