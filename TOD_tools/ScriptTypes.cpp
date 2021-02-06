#include "ScriptTypes.h"
#include "Blocks.h"
#include "Node.h"
#include "LogDump.h"
#include "ScriptDatabase.h"

int ScriptType::_489370(int* unk1, int* unk2)
{
	if (unk2)
		unk2[m_TypeId] += 4 * m_Size;

	return 4 * m_Size;
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
	(*(void (*)(int*, int*, int))0x4893C0)(unk2, unk1, m_Size);
}

String* ScriptType::PrintFormatted(String* outStr, void* value, int precision)
{
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

ScriptType::ScriptType(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType);

	m_TypeName = typeName;
	m_TypeId = typeId;
	m_Size = typeSize;

	TypesList.AddElement(this);
	m_GlobalId = TypesList.m_CurrIndex - 1;
	TypesListCRCCalculated = false;
}

ScriptType::ScriptType()
{
}

ScriptType::ScriptType(const ScriptType& _rhs)
{
	m_TypeName = _rhs.m_TypeName;
	m_TypeId = _rhs.m_TypeId;
	m_Size = _rhs.m_Size;
	m_GlobalId = _rhs.m_GlobalId;
}

unsigned int ScriptType::GetTypeSize_Impl(ScriptType* type)
{
	switch (type->m_TypeId)
	{
	case TYPE_NOTHING:
		return 0;
		break;
	case TYPE_NUMBER:
	case TYPE_INTEGER:
	case TYPE_STRING:
	case TYPE_TRUTH:
	case TYPE_LIST:
	case TYPE_DICT:
	case TYPE_ENTITY:
	case TYPE_STRUCT:
		return 1;
		break;
	case TYPE_VECTOR:
		return 3;
		break;
	case TYPE_QUATERNION:
		return 4;
		break;
	default:
		return -1;
		break;
	}
}

ScriptType::~ScriptType()
{
	MESSAGE_CLASS_DESTROYED(ScriptType);

	RemoveTypeFromList(m_TypeName.m_szString);
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

void ScriptType::RemoveTypeFromList(const char* name)
{
	if (TypesList.m_Elements == &TypesList.m_Elements[TypesList.m_CurrIndex])
	{
		TypesListCRCCalculated = false;
		return;
	}

	for (unsigned int i = NULL; i != TypesList.m_Capacity; i++)
		if (TypesList.m_Elements[i] && strncmp(name, TypesList.m_Elements[i]->m_TypeName.m_szString, strlen(name)) == NULL)
		{
			memcpy(TypesList.m_Elements, TypesList.m_Elements + 1, 4 * (((int)TypesList.m_Elements + TypesList.m_CurrIndex * 4 - (int)TypesList.m_Elements) >> 2) - 4);
			--TypesList.m_CurrIndex;

			TypesListCRCCalculated = false;
			return;
		}

	TypesListCRCCalculated = false;
}

unsigned int ScriptType::GetTypeSize()
{
	return GetTypeSize_Impl(this);
}

ScriptType* ScriptType::GetTypeByName(const char* name)
{
	if (TypesList.m_CurrIndex <= NULL)
		return nullptr;

	for (unsigned int i = NULL; i < TypesList.m_CurrIndex; i++)
		if (strcmp(TypesList.m_Elements[i]->m_TypeName.m_szString, name) == NULL)
			return TypesList.m_Elements[i];

	return nullptr;
}

ScriptType* ScriptType::LoadScript(const char* script)
{
	if (ScriptType* type_ = GetTypeByName(script))
		return type_;

	const char* parenthopenpos = strchr(script, '(');
	const char* parenthclospos = strrchr(script, ')');

	if (!parenthopenpos)
		return nullptr;

	if (strncmp(script, "list", parenthopenpos - script))
	{
		if (strncmp(script, "dict", parenthopenpos - script) == NULL)
		{
			char dict_element_type[50] = {};
			strncpy(dict_element_type, parenthopenpos + 1, parenthclospos - parenthopenpos - 1);

			new ScriptType_Dict(*LoadScript(dict_element_type));
		}

		String script_name, script_type;
		if (!ParseVariableString(script, script_name, script_type))
			return nullptr;

		String script_complete_name = script_name;
		script_complete_name.Append("(");
		script_complete_name.Append(script_type.m_szString);
		script_complete_name.Append(")");

		ScriptType* retn_type = GetTypeByName(script_complete_name.m_szString);
		if (!retn_type)
		{
			retn_type = ScriptType_Entity::GetScriptEntityByName(script_type.m_szString);
			if (!retn_type)
				return nullptr;

			GlobalScript* glob_script = GlobalScript::GetGlobalScriptByName(script_name.m_szString);
			if (!glob_script)
			{
				LogDump::LogA("Unable to load script '%s'\n", script_name.m_szString);
				return nullptr;
			}

			retn_type = glob_script->AssignScriptToEntity(*(ScriptType_Entity*)retn_type);
		}

		return retn_type;
	}

	char list_element_type[50] = {};
	strncpy(list_element_type, parenthopenpos + 1, parenthclospos - parenthopenpos - 1);

	return new ScriptType_List(*LoadScript(list_element_type));
}

bool ScriptType::ParseVariableString(const char* variable, String& variableName, String& variableType)
{
	const char* parenth_open_pos = strchr(variable, '(');
	const char* parenth_close_pos = strrchr(variable, ')');

	if (!parenth_open_pos || !parenth_close_pos)
		return false;

	char buf[64] = {};
	strncpy(buf, variable, parenth_open_pos - variable);
	variableName = buf;

	memset(buf, NULL, sizeof(buf));
	strncpy(buf, parenth_open_pos + 1, parenth_close_pos - parenth_open_pos - 1);
	variableType = buf;

	return true;
}

void InitScriptTypes()
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

	field_70 = 1;
	m_Creator = nullptr;
	m_Parent = nullptr;
}

ScriptType_Entity::~ScriptType_Entity()
{
	MESSAGE_CLASS_DESTROYED(ScriptType_Entity);
}

#pragma message(TODO_IMPLEMENTATION)
void* ScriptType_Entity::CreateNode()
{
	Node* node_ = (Node*)m_Creator(g_Blocks->GetAllocatorType());
	node_->SetScript(this);

	return node_;
}

void ScriptType_Entity::InheritFrom(ScriptType_Entity* from)
{
	//field_6C = from->m_HasParent ? from->m_Parent->m_PropertiesList_1.m_CurrIndex + from->m_Parent->field_6C : from->m_PropertiesList_1.m_CurrIndex + from->field_6C;
	//field_70 = from->m_HasParent ? from->m_Parent->m_PropertiesList_2.m_CurrIndex + from->m_Parent->field_70 : from->m_PropertiesList_2.m_CurrIndex + from->field_70;
	m_Creator = from->m_Creator;
	m_Parent = from;
}

ScriptType_Entity* ScriptType_Entity::GetScriptEntityByName(const char* name)
{
	if (TypesList.m_CurrIndex <= NULL)
		return nullptr;

	for (unsigned int i = NULL; i < TypesList.m_CurrIndex; i++)
		if (TypesList.m_Elements[i]->m_TypeId == TYPE_ENTITY &&
			strcmp(TypesList.m_Elements[i]->m_TypeName.m_szString, name) == 0)
			return (ScriptType_Entity*)TypesList.m_Elements[i];

	return nullptr;
}

ScriptType_List::ScriptType_List(const ScriptType& elementsType) : ScriptType(TYPE_LIST, szScriptTypeName[TYPE_LIST], TYPE_LIST_SIZE)
{
	MESSAGE_CLASS_CREATED(ScriptType_List);

	*m_ListElementsType = elementsType;
	m_ListElementSize = elementsType.m_Size;

	//	NOTE: is this correct?
	char buffer[64];
	sprintf(buffer, "list(%s)", elementsType.m_TypeName.m_szString);
	m_TypeName = buffer;

	if (m_TypeId == 3 || m_TypeId == 8 || m_TypeId == 9 || m_TypeId == 11)
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
	sprintf(buffer, "dict(%s)", elementsType.m_TypeName.m_szString);
	m_TypeName = buffer;
}

ScriptType_Script::ScriptType_Script(const char* name, const ScriptFieldsList& fields) : ScriptType(TYPE_STRUCT, name, TYPE_STRUCT_SIZE)
{
	MESSAGE_CLASS_CREATED(ScriptType_Script);

	m_Name = name;
	m_Fields = ScriptFieldsList(0x19300);
	m_Fields = fields;
	m_Fields.m_TotalSizeBytes = fields.m_TotalSizeBytes;
	m_Fields.m_TotalSize = fields.m_TotalSize;
}

ScriptFieldsList::ScriptFieldsList(unsigned int flags)
{
	m_Flags = NULL;
	m_Elements = nullptr;
	m_CurrentIndex = NULL;
	m_Capacity = NULL;
	m_Flags = m_Flags & 0xFFFFFFFF | flags;
	m_TotalSize = NULL;
	m_TotalSizeBytes = NULL;
}

ScriptFieldsList::ScriptFieldsList(const ScriptFieldsList& rhs)
{
	if (&rhs == this)
		return;

	if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS && m_Elements)
		for (unsigned int i = NULL; i < m_CurrentIndex; i++)
			delete& (m_Elements[i].m_Name);

	if (m_Capacity < m_CurrentIndex)
	{
		m_Capacity = m_CurrentIndex;

		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL && m_Elements)
			delete m_Elements;
		else
			m_Flags |= LIST_FLAGS_CLEAR_ELEMENTS_ALL;

		m_Elements = (ScriptField*)Allocators::AllocateByType((unsigned char)m_Flags, sizeof(ScriptField) * m_CurrentIndex);
		m_Capacity = m_CurrentIndex;

		if (!m_Elements)
		{
			m_Flags |= LIST_FLAGS_NOT_ALLOCATED;
			m_Capacity = NULL;
			m_CurrentIndex = NULL;

			return;
		}

		if (m_CurrentIndex <= NULL)
			return;

		for (unsigned int i = NULL; i < m_CurrentIndex; i++)
			if (&m_Elements[i])
				m_Elements[i] = rhs.m_Elements[i];
	}
}

void ScriptFieldsList::Add(const ScriptField& _sf)
{
	if (m_CurrentIndex >= m_Capacity)
	{
		if (m_CurrentIndex + 1 > m_Capacity)
		{
			m_Capacity = m_CurrentIndex + 1 + ((m_CurrentIndex + 1) >> 1);

			ScriptField* newElements = new ScriptField[m_Capacity];

			if (newElements && m_Elements)
			{
				memcpy(newElements, m_Elements, sizeof(ScriptField) * m_CurrentIndex);
				delete m_Elements;

				m_Elements = newElements;
			}

			m_CurrentIndex++;
		}
	}

	m_Elements[m_CurrentIndex] = _sf;

	m_Elements[m_CurrentIndex].m_FieldOffset = m_TotalSizeBytes;
	m_TotalSizeBytes += _sf.m_Type->m_Size;
	m_TotalSize += _sf.m_Type->GetTypeSize();
}

void ScriptFieldsList::Clear()
{
	if (!m_Elements)
		return;

	if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS)
		for (unsigned int i = NULL; i < m_CurrentIndex; i++)
			if (&m_Elements[i])
				delete& m_Elements[i];

	if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL)
		delete m_Elements;
}

ScriptField::ScriptField(const ScriptField& rhs)
{
	m_Name = rhs.m_Name;
	m_Type = rhs.m_Type;
	m_FieldOffset = rhs.m_FieldOffset;
}

ScriptField::ScriptField(const char* name, ScriptType* stype, unsigned int)
{
	m_Name = name;
	m_Type = stype;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::RegisterMember(ScriptType* _rettype, const char* _membname, void* (*_getproc)(), void (*_setproc)(int), const char* _membproto, const char* _unk)
{
}

void ScriptType_Builtin::RegisterHandler(const char* _hsignature, void* (*_hndlr)(void*), const char* _hmsg)
{
	BuiltinHandler _hndlrtmp(_hsignature, _hndlr, _hmsg);
	m_HandlersList.AddElement(&_hndlrtmp);
}

void ScriptType_Builtin::Print(void* args)
{
	LogDump::Print_Impl("%s", (const char*)*((int*)args));
}

BuiltinHandler::BuiltinHandler(const char* _prot, void* (*_hndlr)(void*), const char* _name)
{
	m_Prototype = _prot;
	m_Handler = _hndlr;
	m_Name = _name;
}