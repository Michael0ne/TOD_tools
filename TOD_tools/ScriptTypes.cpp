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

	int GetTypeSize(ScriptType* type)
	{
		//	NOTE: compiler should optimize this automatically.
		switch (type->m_nTypeId)
		{
		case TYPE_NOTHING:
			return TYPE_NOTHING_SIZE;
		case TYPE_NUMBER:
			return TYPE_NUMBER_SIZE;
		case TYPE_INTEGER:
			return TYPE_INTEGER_SIZE;
		case TYPE_STRING:
			return TYPE_STRING_SIZE;
		case TYPE_TRUTH:
			return TYPE_TRUTH_SIZE;
		case TYPE_VECTOR:
			return TYPE_VECTOR_SIZE;
		case TYPE_QUATERNION:
			return TYPE_QUATERNION_SIZE;
		case TYPE_COLOR:
			return TYPE_COLOR_SIZE;
		case TYPE_LIST:
			return TYPE_LIST_SIZE;
		case TYPE_DICT:
			return TYPE_DICT_SIZE;
		case TYPE_ENTITY:
			return TYPE_ENTITY_SIZE;
		case TYPE_SCRIPT:
			return TYPE_SCRIPT_SIZE;
		default:
			return -1;
		}
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

	ScriptType_Entity::ScriptType_Entity(const char* szEntityName) : ScriptType(TYPE_ENTITY, szEntityName, TYPE_ENTITY_SIZE)
	{
		MESSAGE_CLASS_CREATED(ScriptType_Entity);

		field_2C = field_34 = field_38 = field_3C = field_44 = field_48 = field_6C = field_74 = 0;
		field_30 = 0x00000032 | 0x10000;
		field_40 = 0x00000032 | 0x10000;
		field_70 = 1;
		m_Creator = nullptr;
		m_Parent = nullptr;
		m_ParentNode = nullptr;

		m_PropertiesList_1 = List<EntityProperties>(0x19300);
		m_PropertiesList_2 = List<EntityProperties>(0x19300);
	}
}