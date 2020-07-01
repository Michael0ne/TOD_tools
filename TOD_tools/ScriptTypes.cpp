#include "ScriptTypes.h"

namespace ScriptTypes
{
	ScriptType::ScriptType(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize)
	{
		MESSAGE_CLASS_CREATED(ScriptType);

		m_sTypeName = typeName;
		m_nTypeId = typeId;
		m_nSizeInBytes = typeSize;

		TypesList.AddElement(this);
		m_nId = TypesList.m_nCurrIndex - 1;
		TypesListCRCCalculated = false;
	}

	void Init()
	{
		tyNothing = new ScriptType_Nothing(TYPE_NOTHING, szScriptTypeName[TYPE_NOTHING], TYPE_NOTHING_SIZE);
		tyNumber = new ScriptType_Number(TYPE_NUMBER, szScriptTypeName[TYPE_NUMBER], TYPE_NUMBER_SIZE);
		tyInteger = new ScriptType_Integer(TYPE_INTEGER, szScriptTypeName[TYPE_INTEGER], TYPE_INTEGER_SIZE);
		tyBoolean = new ScriptType_Boolean(TYPE_TRUTH, szScriptTypeName[TYPE_TRUTH], TYPE_TRUTH_SIZE);
		tyVector = new ScriptType_Vector(TYPE_VECTOR, szScriptTypeName[TYPE_VECTOR], TYPE_VECTOR_SIZE);
		tyQuaternion = new ScriptType_Quaternion(TYPE_QUATERNION, szScriptTypeName[TYPE_QUATERNION], TYPE_QUATERNION_SIZE);
		tyColor = new ScriptType_Color(TYPE_COLOR, szScriptTypeName[TYPE_COLOR], TYPE_COLOR_SIZE);
		tyString = new ScriptType_String(TYPE_STRING, szScriptTypeName[TYPE_STRING], TYPE_STRING_SIZE);

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