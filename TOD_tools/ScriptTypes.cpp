#include "ScriptTypes.h"

#include "MemoryAllocators.h"

namespace ScriptTypes
{
	void ScriptType::Register(unsigned int typeId, const char* typeName, unsigned int typeSize)
	{
		void(__thiscall * _Register)(ScriptType * _this, unsigned int _tyid, const char* _tyname, unsigned int _tysize) = (void(__thiscall*)(ScriptType*, unsigned int, const char*, unsigned int))0x862E90;

		_Register(this, typeId, typeName, typeSize);
	}

	void Init()
	{
		if (!Allocators::Released) {
			if (tyNothing = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyNothing->Register(SCRIPTTYPE_NOTHING_ID, SCRIPTTYPE_NOTHING_NAME, SCRIPTTYPE_NOTHING_SIZE);
				tyNothing->SetVtablePtr((ScriptType__vtable*)tNOTHING_vtable);
			}

			if (tyNumber = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyNumber->Register(SCRIPTTYPE_NUMBER_ID, SCRIPTTYPE_NUMBER_NAME, SCRIPTTYPE_NUMBER_SIZE);
				tyNumber->SetVtablePtr((ScriptType__vtable*)tNUMBER_vtable);
			}
			
			if (tyInteger = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyInteger->Register(SCRIPTTYPE_INTEGER_ID, SCRIPTTYPE_INTEGER_NAME, SCRIPTTYPE_INTEGER_SIZE);
				tyInteger->SetVtablePtr((ScriptType__vtable*)tINTEGER_vtable);
			}

			if (tyBoolean = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyBoolean->Register(SCRIPTTYPE_TRUTH_ID, SCRIPTTYPE_TRUTH_NAME, SCRIPTTYPE_TRUTH_SIZE);
				tyBoolean->SetVtablePtr((ScriptType__vtable*)tBOOLEAN_vtable);
			}

			if (tyVector = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyVector->Register(SCRIPTTYPE_VECTOR_ID, SCRIPTTYPE_VECTOR_NAME, SCRIPTTYPE_VECTOR_SIZE);
				tyVector->SetVtablePtr((ScriptType__vtable*)tVECTOR_vtable);
			}

			if (tyQuaternion = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyQuaternion->Register(SCRIPTTYPE_QUATERNION_ID, SCRIPTTYPE_QUATERNION_NAME, SCRIPTTYPE_QUATERNION_SIZE);
				tyQuaternion->SetVtablePtr((ScriptType__vtable*)tQUATERNION_vtable);
			}

			if (tyColor = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyColor->Register(SCRIPTTYPE_COLOR_ID, SCRIPTTYPE_COLOR_NAME, SCRIPTTYPE_COLOR_SIZE);
				tyColor->SetVtablePtr((ScriptType__vtable*)tCOLOR_vtable);
			}

			if (tyString = (ScriptType*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(ScriptType))) {
				tyString->Register(SCRIPTTYPE_STRING_ID, SCRIPTTYPE_STRING_NAME, SCRIPTTYPE_STRING_SIZE);
				tyString->SetVtablePtr((ScriptType__vtable*)tSTRING_vtable);
			}
		}

		tNOTHING = tyNothing;
		tNUMBER = tyNumber;
		tINTEGER = tyInteger;
		tBOOLEAN = tyBoolean;
		tVECTOR = tyVector;
		tQUATERNION = tyQuaternion;
		tCOLOR = tyColor;
		tSTRING = tyString;
	}
}