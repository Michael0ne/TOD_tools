#include "ScriptTypes.h"

namespace ScriptTypes
{
	void ScriptType::Register(unsigned int typeId, const char* typeName, unsigned int typeSize)
	{
		SetVtablePtr((ScriptType__vtable*)0x9CB1A8);
		m_sTypeName.Set(typeName);
		m_nTypeId = typeId;
		m_nSizeInBytes = typeSize;

		ListTypes.AddElement(this);

		m_nId = ListTypes.m_nCurrIndex - 1;
		_A3CEC8 = 0;
	}

	void Init()
	{
		if (!Allocators::Released) {
			if (tyNothing = new ScriptType()) {
				tyNothing->Register(SCRIPTTYPE_NOTHING_ID, SCRIPTTYPE_NOTHING_NAME, SCRIPTTYPE_NOTHING_SIZE);
				tyNothing->SetVtablePtr((ScriptType__vtable*)tNOTHING_vtable);
			}

			if (tyNumber = new ScriptType()) {
				tyNumber->Register(SCRIPTTYPE_NUMBER_ID, SCRIPTTYPE_NUMBER_NAME, SCRIPTTYPE_NUMBER_SIZE);
				tyNumber->SetVtablePtr((ScriptType__vtable*)tNUMBER_vtable);
			}
			
			if (tyInteger = new ScriptType()) {
				tyInteger->Register(SCRIPTTYPE_INTEGER_ID, SCRIPTTYPE_INTEGER_NAME, SCRIPTTYPE_INTEGER_SIZE);
				tyInteger->SetVtablePtr((ScriptType__vtable*)tINTEGER_vtable);
			}

			if (tyBoolean = new ScriptType()) {
				tyBoolean->Register(SCRIPTTYPE_TRUTH_ID, SCRIPTTYPE_TRUTH_NAME, SCRIPTTYPE_TRUTH_SIZE);
				tyBoolean->SetVtablePtr((ScriptType__vtable*)tBOOLEAN_vtable);
			}

			if (tyVector = new ScriptType()) {
				tyVector->Register(SCRIPTTYPE_VECTOR_ID, SCRIPTTYPE_VECTOR_NAME, SCRIPTTYPE_VECTOR_SIZE);
				tyVector->SetVtablePtr((ScriptType__vtable*)tVECTOR_vtable);
			}

			if (tyQuaternion = new ScriptType()) {
				tyQuaternion->Register(SCRIPTTYPE_QUATERNION_ID, SCRIPTTYPE_QUATERNION_NAME, SCRIPTTYPE_QUATERNION_SIZE);
				tyQuaternion->SetVtablePtr((ScriptType__vtable*)tQUATERNION_vtable);
			}

			if (tyColor = new ScriptType()) {
				tyColor->Register(SCRIPTTYPE_COLOR_ID, SCRIPTTYPE_COLOR_NAME, SCRIPTTYPE_COLOR_SIZE);
				tyColor->SetVtablePtr((ScriptType__vtable*)tCOLOR_vtable);
			}

			if (tyString = new ScriptType()) {
				tyString->Register(SCRIPTTYPE_STRING_ID, SCRIPTTYPE_STRING_NAME, SCRIPTTYPE_STRING_SIZE);
				tyString->SetVtablePtr((ScriptType__vtable*)tSTRING_vtable);
			}
		}

		*tNOTHING = tyNothing;
		*tNUMBER = tyNumber;
		*tINTEGER = tyInteger;
		*tBOOLEAN = tyBoolean;
		*tVECTOR = tyVector;
		*tQUATERNION = tyQuaternion;
		*tCOLOR = tyColor;
		*tSTRING = tyString;
	}
}