#include "ScriptTypes.h"
#include "Blocks.h"

namespace ScriptTypes
{
	void ScriptType::scalar_destructor(bool freeMemory)
	{
		if (freeMemory)
			Allocators::ReleaseMemory(this, freeMemory);
	}

	int ScriptType::_489370(int* unk1, int* unk2)
	{
		if (unk2)
			unk2[m_nTypeId] += 4 * m_nSizeInBytes;

		return 4 * m_nSizeInBytes;
	}

#pragma message(TODO_IMPLEMENTATION)
	void ScriptType::GetDefaultValue(void* unk1)
	{
		*(int*)unk1 = NULL;
	}

#pragma message(TODO_IMPLEMENTATION)
	void ScriptType::nullsub_2(String* unk2)
	{
		*(int*)unk2 = NULL;
	}

#pragma message(TODO_IMPLEMENTATION)
	void ScriptType::_4893A0(int* unk1, int* unk2)
	{
		(*(void (*)(int*, int*, int))0x4893C0)(unk2, unk1, m_nSizeInBytes);
	}

	String* ScriptType::PrintFormatted(String* outStr, void* value, int precision)
	{
		outStr->Set("");
		return outStr;
	}

	int ScriptType::DoOperation(char* unk1, void* unk2)
	{
		return -1;
	}

#pragma message(TODO_IMPLEMENTATION)
	int ScriptType::_489410(int* unk1)
	{
		return (*(int(__thiscall*)(ScriptType*, int*))0x489410)(this, unk1);
	}

#pragma message(TODO_IMPLEMENTATION)
	int ScriptType::_863760_1(int* unk1, int* unk2)
	{
		return (*(int(__thiscall*)(ScriptType*, int*, int*))0x863760)(this, unk1, unk2);
	}

#pragma message(TODO_IMPLEMENTATION)
	int ScriptType::_863760_2(int* unk1, int* unk2)
	{
		return (*(int(__thiscall*)(ScriptType*, int*, int*))0x863760)(this, unk1, unk2);
	}

#pragma message(TODO_IMPLEMENTATION)
	int ScriptType::_8637F0(int* unk1, String* unk2, int unk3)
	{
		return (*(int(__thiscall*)(ScriptType*, int*, String*, int))0x8637F0)(this, unk1, unk2, unk3);
	}

	ScriptType::ScriptType(E_SCRIPT_TYPE_ID typeId, const char* typeName, E_SCRIPT_TYPE_SIZE typeSize)
	{
		MESSAGE_CLASS_CREATED(ScriptType);

		m_sTypeName = typeName;
		m_nTypeId = typeId;
		m_nSizeInBytes = typeSize;

		TypesList.AddElement(this);
		m_nId = TypesList.m_CurrIndex - 1;
		TypesListCRCCalculated = false;
	}

#pragma message(TODO_IMPLEMENTATION)
	int ScriptType::_862A50(int unk1, int* unk2, int* unk3)
	{
		return (*(int(__thiscall*)(ScriptType*, int, int*, int*))0x862A50)(this, unk1, unk2, unk3);
	}

#pragma message(TODO_IMPLEMENTATION)
	void ScriptType::nullsub_3(int unk1, void* unk2, int unk3, int unk4, int unk5, void* outop)
	{
		return;
	}

#pragma message(TODO_IMPLEMENTATION)
	void ScriptType::nullsub_4(int* unk1, int unk2, int unk3, int unk4, int unk5, int unk6)
	{
		return;
	}

	bool ScriptType::AreParamsEqual(int unk1, int unk2)
	{
		return AreParamsNotEqual(unk1, unk2) == false;
	}

	bool ScriptType::AreParamsNotEqual(int unk1, int unk2)
	{
		return true;
	}

	void ScriptType::GetOperationType(const char* operationStr, int* outOperationId, ScriptType** outResultType, char* unk4)
	{
		*outOperationId = -1;
	}

	void ScriptType::PerformOperation(int operationId, void* params)
	{
		return;
	}

	char ScriptType::_8637E0(int unk1, int unk2)
	{
		return 1;
	}

	bool ScriptType::IsInfinite(void* unk1)
	{
		return true;
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

		field_2C = field_3C = field_44 = field_48 = field_6C = field_74 = 0;
		field_30 = 0x00000032 | 0x10000;
		field_40 = 0x00000032 | 0x10000;
		field_70 = 1;
		m_Creator = nullptr;
		m_Parent = nullptr;
		m_ParentNode = nullptr;
		field_38 = nullptr;

		m_PropertiesList_1 = List<EntityProperties>(0x19300);
		m_PropertiesList_2 = List<EntityProperties>(0x19300);
	}

#pragma message(TODO_IMPLEMENTATION)
	void* ScriptType_Entity::CreateNode()
	{
		void* ent = m_Creator(g_Blocks->GetAllocatorType());
		(*(void(__thiscall*)(void*, ScriptType_Entity*))0x869E20)(ent, this);

		return ent;
	}

	void ScriptType_Entity::InheritFrom(ScriptType_Entity* from)
	{
		field_6C = from->field_74 ? from->m_Parent->m_PropertiesList_1.m_CurrIndex + from->m_Parent->field_6C : from->m_PropertiesList_1.m_CurrIndex + from->field_6C;
		field_70 = from->field_74 ? from->m_Parent->m_PropertiesList_2.m_CurrIndex + from->m_Parent->field_70 : from->m_PropertiesList_2.m_CurrIndex + from->field_70;
		m_Creator = from->m_Creator;
		m_Parent = from;
	}

	ScriptType_List::ScriptType_List(const ScriptType& elementsType) : ScriptType(TYPE_LIST, szScriptTypeName[TYPE_LIST], TYPE_LIST_SIZE)
	{
		MESSAGE_CLASS_CREATED(ScriptType_List);

		*m_ListElementsType = elementsType;
		m_ListElementSize = elementsType.m_nSizeInBytes;

		//	NOTE: is this correct?
		char buffer[64];
		sprintf(buffer, "list(%s)", elementsType.m_sTypeName.m_szString);
		m_sTypeName.Set(buffer);

		if (m_nTypeId == 3 || m_nTypeId == 8 || m_nTypeId == 9 || m_nTypeId == 11)
			m_IsTypeId3_8_9_11 = true;
		else
			m_IsTypeId3_8_9_11 = false;
	}

	ScriptType_Dict::ScriptType_Dict(const ScriptType& elementsType) : ScriptType(TYPE_DICT, szScriptTypeName[TYPE_DICT], TYPE_DICT_SIZE)
	{
		MESSAGE_CLASS_CREATED(ScriptType_Dict);

		*m_ElementsType = elementsType;

		//	NOTE: is this correct?
		char buffer[64];
		sprintf(buffer, "dict(%s)", elementsType.m_sTypeName.m_szString);
		m_sTypeName.Set(buffer);
	}
}