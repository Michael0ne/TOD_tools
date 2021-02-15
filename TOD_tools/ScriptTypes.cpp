#include "ScriptTypes.h"
#include "Blocks.h"
#include "Node.h"
#include "LogDump.h"
#include "ScriptDatabase.h"
#include "Random.h"
#include "Performance.h"
#include "StreamedSoundBuffers.h"
#include "Window.h"
#include "GfxInternal.h"
#include "LoadScreenInfo.h"
#include "InputKeyboard.h"
#include "InputMouse.h"

ScriptType_Builtin* tBuiltin;
List<DumpTable_Element> DumpTable;

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

ScriptType_Builtin::ScriptType_Builtin() : ScriptType_Entity("builtin")
{
	MESSAGE_CLASS_CREATED(ScriptType_Builtin);

	m_HandlersList;
	m_MembersMap;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::RegisterMemberFunction(ScriptType* _rettype, const char* _membname, void* (*_getproc)(), void (*_setproc)(int), const char* _membproto, const char* _unk)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::RegisterHandler(const char* _hsignature, void*, const char* _hmsg)
{
}

void ScriptType_Builtin::Sin(float* arg)
{
	*arg = sin(arg[1]);
}

void ScriptType_Builtin::Cos(float* arg)
{
	*arg = cos(arg[1]);
}

void ScriptType_Builtin::Tan(float* arg)
{
	*arg = tan(arg[1]);
}

void ScriptType_Builtin::Asin(float* arg)
{
	*arg = asin(arg[1]);
}

void ScriptType_Builtin::Acos(float* arg)
{
	*arg = acos(arg[1]);
}

void ScriptType_Builtin::Atan(float* arg)
{
	*arg = atan(arg[1]);
}

void ScriptType_Builtin::Abs(int* arg)
{
	*arg = abs(arg[1]);
}

void ScriptType_Builtin::Fabs(float* arg)
{
	*arg = fabs(arg[1]);
}

void ScriptType_Builtin::Sqrt(float* arg)
{
	*arg = sqrt(arg[1]);
}

void ScriptType_Builtin::Floor(float* arg)
{
	*arg = floor(arg[1]);
}

void ScriptType_Builtin::Ceil(float* arg)
{
	*arg = ceil(arg[1]);
}

void ScriptType_Builtin::Clamp(float* arg)
{
	if (arg[2] <= arg[1])
		*arg = arg[1] <= arg[3] ? arg[1] : arg[3];
	else
		*arg = arg[2];
}

void ScriptType_Builtin::Testbits(int* arg)
{
	*arg = (arg[1] & arg[2]) != NULL;
}

void ScriptType_Builtin::Setbit(int* arg)
{
	*arg = arg[1] | (1 << arg[2]);
}

void ScriptType_Builtin::Getbit(int* arg)
{
	*arg = (arg[1] & (1 << arg[2])) != NULL;
}

void ScriptType_Builtin::Rand_seed(int* arg)
{
	Random::Init(*arg);
}

void ScriptType_Builtin::Rand_integer(int* arg)
{
	*arg = Random::Integer(arg[1]);
}

void ScriptType_Builtin::Rand_number(float* arg)
{
	*arg = Random::Float();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::Get_facecoll_MaterialID(int* arg)
{
	//*arg = GetFacecollMaterialId((const char*)arg[1]);	//	FIXME: this is obnoxious.
}

void ScriptType_Builtin::GetTime(float* arg)
{
	*arg = Performance::GetMilliseconds() * 0.001f;
}

void ScriptType_Builtin::Print(int* arg)
{
	LogDump::Print_Impl("%s", (const char*)*arg);
}

void ScriptType_Builtin::IsKeyDown(int* arg)
{
	*arg = g_InputKeyboard->m_bAcquired ? g_InputKeyboard->m_nButtonStates1[arg[1]] >> 7 : NULL;
}

void ScriptType_Builtin::IsKeyPressed(int* arg)
{
	*arg = g_InputKeyboard->m_bAcquired ? g_InputKeyboard->m_nButtonStates1[arg[1]] & 1 : NULL;
}

void ScriptType_Builtin::IsKeyReleased(int* arg)
{
	*arg = g_InputKeyboard->m_bAcquired ? (g_InputKeyboard->m_nButtonStates1[arg[1]] & 2) != NULL : NULL;
}

void ScriptType_Builtin::DrawPoint(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->DrawPoint: (%f, %f, %f) %i\n", (float)arg[1], (float)arg[2], (float)arg[3], arg[4]);
#endif
}

void ScriptType_Builtin::DrawLine(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->DrawLine: (%f, %f, %f) (%f, %f, %f) %i\n", (float)arg[1], (float)arg[2], (float)arg[3], (float)arg[4], (float)arg[5], (float)arg[6], arg[7]);
#endif
}

void ScriptType_Builtin::DrawLine2D(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->DrawLine2D: (%f, %f, %f) (%f, %f, %f) %i\n", (float)arg[1], (float)arg[2], (float)arg[3], (float)arg[4], (float)arg[5], (float)arg[6], arg[7]);
#endif
}

void ScriptType_Builtin::DrawSphere(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->DrawSphere: (%f, %f, %f) %f %i\n", (float)arg[1], (float)arg[2], (float)arg[3], (float)arg[4], arg[5]);
#endif
}

void ScriptType_Builtin::ProfileBegin(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->ProfileBegin: \"%s\" %i\n", (const char*)(arg[1]), arg[2]);
#endif
}

void ScriptType_Builtin::ProfileEnd(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->ProfileEnd: \"%s\" %i\n", (const char*)(arg[1]), arg[2]);
#endif
}

void ScriptType_Builtin::NumberToInteger(float* arg)
{
	*arg = (int)arg[1];
}

void ScriptType_Builtin::IntegerToNumber(int* arg)
{
	*arg = (float)arg[1];
}

void ScriptType_Builtin::PrintStack(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->PrintStack: %X\n", arg[1]);	//	NOTE: arg[1] is of type Entity.
#endif
}

void ScriptType_Builtin::GenericCall(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->GenericCall: %X\n", arg[1]);	//	NOTE: arg[1] is of type Entity.
#endif
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::QuadTreeQuery(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::AuxQuadTreeQuery(int* arg)
{
}

void ScriptType_Builtin::SetSelectedSoundrenderer(int* arg)
{
	LogDump::LogA("@@@@@@@@@ selected_sound: %d\n", *arg);

	switch (*arg)
	{
	case 1:
		Audio::RememberSoundRenderer(Audio::SOUND_SYSTEM_DIESELPOWER);
		break;
	case 2:
		Audio::RememberSoundRenderer(Audio::SOUND_SYSTEM_DSOUND);
		break;
	case 3:
		Audio::RememberSoundRenderer(Audio::SOUND_SYSTEM_UNDEFINED);	//	FIXME: this should be 0, but no enum for it right now.
		break;
	default:
		Audio::RememberSoundRenderer(Audio::SOUND_SYSTEM_AUTOSELECT);
		break;
	}
}

void ScriptType_Builtin::SfxMuteAll(int* arg)
{
	g_StreamedSoundBuffers->m_Muted = *arg == 1;
}

void ScriptType_Builtin::SfxIsMuteAll(int* arg)
{
	*arg = (int)g_StreamedSoundBuffers->m_Muted;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::AllocateGlobalStreamedSound(int* arg)
{
	//*arg = Audio::AllocateGlobalStreamedSound((const char*)(arg[1]), arg[2] != NULL, arg[3] != NULL);
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DeallocateGlobalStreamedSound(int* arg)
{
	//Audio::DeallocateGlobalStreamedSound();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::PlayGlobalStreamedSound(int* arg)
{
	//*arg = Audio::PlayGlobalStreamedSound();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::StopGlobalStreamedSound(int* arg)
{
	//*arg = Audio::StopGlobalStreamedSound();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::SetVolumePitchGlobalStreamedSound(int* arg)
{
	//*arg = Audio::SetVolumePitchGlobalStreamedSound((float)arg[1], (float)arg[2]);
}

void ScriptType_Builtin::GetDefaultFxVolumeVar(float* arg)
{
	*arg = 1.0f;
}

void ScriptType_Builtin::GetDefaultAmbienceVolumeVar(float* arg)
{
	*arg = 1.0f;
}

void ScriptType_Builtin::GetDefaultMusicVolumeVar(float* arg)
{
	*arg = 1.0f;
}

void ScriptType_Builtin::GetDefaultSpeaksVolumeVar(float* arg)
{
	*arg = 1.0f;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::SetVolModifierOnGroup(int* arg)
{
}

void ScriptType_Builtin::CutsceneDisableAware(int* arg)
{
	*arg = Script::CutsceneDisableAware;
}

void ScriptType_Builtin::IsDebugConsoleActive(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->IsDebugConsoleActive\n");
#else
	*arg = NULL;
#endif
}

void ScriptType_Builtin::DebugConsoleTextBox(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->DebugConsoleTextBox: %i %i %i %i %i\n", arg[1], arg[2], arg[3], arg[4], arg[5]);
#endif
}

void ScriptType_Builtin::DebugConsolePrint(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->DebugConsolePrint: \"%s\"\n", (const char*)arg[1]);
#endif
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::GlobalKillAllEmmiters(int* arg)
{
	//ParticleSystem::KillEmmiters(false, false);
}

void ScriptType_Builtin::GetVersionNumber(char** arg)
{
	char procname[64] = {};
	HANDLE prochnd = NULL;

	sprintf(procname, "v7_%04d", GetCurrentProcessId() ^ 0x19EA3FD3);
	prochnd = OpenEvent(EVENT_MODIFY_STATE, FALSE, procname);
	*arg = new char[6];	//	TODO: who deletes this?

	if (prochnd)
	{
		CloseHandle(prochnd);
		strcpy(*arg, "v1.00");
	}
	else
		strcpy(*arg, "v1,00");
}

void ScriptType_Builtin::GetConfigString(int* arg)
{
	if (GameConfig::g_Config->m_ConfigurationVariables->IsVariableSet((const char*)arg[1]))
	{
		*(char**)&(*arg) = new char[45];
		String valstr = GameConfig::g_Config->m_ConfigurationVariables->GetParamValueString((const char*)arg[1]);
		strcpy(*(char**)&(*arg), valstr.m_szString);
	}
	else
		*arg = NULL;
}

void ScriptType_Builtin::GetConfigTruth(int* arg)
{
	if (GameConfig::g_Config->m_ConfigurationVariables->IsVariableSet((const char*)arg[1]))
		*arg = GameConfig::g_Config->m_ConfigurationVariables->GetParamValueBool((const char*)arg[1]);
	else
		*arg = NULL;
}

void ScriptType_Builtin::GetSessionVariableString(char* arg)
{
	if (GameConfig::g_Config->m_SessionVariables->IsVariableSet((const char*)arg[1]))
	{
		*(char**)&(*arg) = new char[45];
		String valstr = GameConfig::g_Config->m_SessionVariables->GetParamValueString((const char*)arg[1]);
		strcpy(*(char**)&(*arg), valstr.m_szString);
	}
	else
		*arg = NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::SetSessionVariableString(char* arg)
{
	//GameConfig::g_Config->m_SessionVariables->SetParamValueString((const char*)*arg, (const char*)arg[1]);
}

void ScriptType_Builtin::GetSessionVariableTruth(char* arg)
{
	if (GameConfig::g_Config->m_SessionVariables->IsVariableSet((const char*)arg[1]))
		*arg = GameConfig::g_Config->m_SessionVariables->GetParamValueBool((const char*)arg[1]);
	else
		*arg = false;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::SetSessionVariableTruth(char* arg)
{
	//GameConfig::g_Config->m_SessionVariables->SetParamValueTruth((char*)*arg, (char*)arg[1]);
}

void ScriptType_Builtin::SetCurrentCountryCode(char* arg)
{
	Script::SetCountryCode((char*)*arg);
}

void ScriptType_Builtin::GetCurrentCountryCode(char* arg)
{
	*(char**)(&(*arg)) = new char[4];
	strcpy(*(char**)(&(*arg)), Script::GetCurrentCountryCode());
}

void ScriptType_Builtin::SetDiscErrorMessage(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->SetDiscErrorMessage: %p %i\n", *arg, arg[1]);
#endif
}

void ScriptType_Builtin::SetLodFactor(float* arg)
{
	Script::LodFactor = *arg;
}

void ScriptType_Builtin::LoadScene(char** arg)
{
	if (!*arg || !**arg || !strlen(*arg))
		return;

	GameConfig::g_Config->m_SceneName = *arg;
}

void ScriptType_Builtin::GetSystemLanguage(int* arg)
{
	*arg = GetSystemLanguageCode();
}

void ScriptType_Builtin::StartCleanupDashboard(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->StartCleanupDashboard: %i\n", *arg);
#endif
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::SetScreenResolution(int* arg)
{
	//g_GfxInternal->SetScreenResolution(*arg, arg[1]);
	//g_Blocks->ResetSceneChildrenNodes(NULL);
}

void ScriptType_Builtin::GetScreenResolution(float* arg)
{
	Vector2<int> res;
	g_GfxInternal->GetScreenResolution(res);

	*arg = res.x;
	arg[1] = res.y;
	arg[2] = 0.f;	//	NOTE: why?
}

void ScriptType_Builtin::IsScreenResolutionAvailable(int* arg)
{
	*arg = g_GfxInternal->IsScreenResolutionAvailable(arg[1], arg[2]);
}

void ScriptType_Builtin::IsWideScreen(int* arg)
{
	*arg = GfxInternal::IsWideScreen();
}

void ScriptType_Builtin::SetVirtualHudScreenSize(float* arg)
{
	g_ScreenProperties.SetHudScreenSize(*arg, arg[1], 1.f, 1.f);
}

void ScriptType_Builtin::GetVirtualHudScreenSize(float* arg)
{
	*arg = g_ScreenProperties.m_fVirtualHudScreensizeWidth;
	arg[1] = g_ScreenProperties.m_fVirtualHudScreensizeHeight;
	arg[2] = 0.f;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::GetScreenTopInVirtualUnits(float* arg)
{
	//*arg = g_ScreenProperties.GetScreenTop();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::GetScreenBottomInVirtualUnits(float* arg)
{
	//*arg = g_ScreenProperties.GetScreenBottom();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::GetScreenLeftInVirtualUnits(float* arg)
{
	//*arg = g_ScreenProperties.GetScreenLeft();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::GetScreenRightInVirtualUnits(float* arg)
{
	//*arg = g_ScreenProperties.GetScreenRight();
}

void ScriptType_Builtin::DisableCurrentLoadScreen(int* arg)
{
	g_LoadScreenInfo->Deactivate();
}

void ScriptType_Builtin::GetEditorActive(bool* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->GetEditorActive: %c\n", false);
#endif

	*arg = false;
}

void ScriptType_Builtin::DumptableCreate(int* arg)
{
	DumpTable_Element dtel;
	DumpTable.AddElement(&dtel);

	*arg = DumpTable.m_CurrIndex;
}

void ScriptType_Builtin::DumptableCreateFromFile(int* arg)
{
	String buff;
	File dtfile((const char*)*arg, 1, true);

	while (!dtfile.ReadIfNotEOF())
	{
		char buf[4] = {};
		dtfile.Read(buf, sizeof(buf));
		buff.Append(buf);
	}

	DumpTable_Element dtel(buff.m_szString);
	DumpTable.AddElement(&dtel);

	*arg = DumpTable.m_CurrIndex;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableAddIntegerColumn(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableAddNumberColumn(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableAddStringColumn(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableSetNumRows(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableSetIntegerValue(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableSetNumberValue(float* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableSetStringValue(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableWriteToFile(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableClose(int* arg)
{
}

void ScriptType_Builtin::EditorReloadAllAssets(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->EditorReloadAllAssets: %s\n", (const char*)*arg);
#endif
}

void ScriptType_Builtin::EditorSelectNode(int* arg)
{
#ifdef INCLUDE_FIXES
	debug("pBuiltinModule->EditorSelectNode: %p\n", *arg);
#endif
}

void ScriptType_Builtin::GetRegion(int* arg)
{
	*arg = g_Blocks->GetRegionId();
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::GetMessageId(int* arg)
{
	//*arg = Script::GetCommandId((const char*)arg[1]);
}

void ScriptType_Builtin::QuitGame(int* arg)
{
	g_Window->QuitGame();
}

void ScriptType_Builtin::GetCoverdemoPlayMode(int* arg)
{
	*arg = g_Window->GetCoverdemoPlayMode();
}

void ScriptType_Builtin::GetCoverdemoInactiveTimeoutSec(int* arg)
{
	*arg = g_Window->GetCoverdemoInactiveTimeoutSec();
}

void ScriptType_Builtin::GetCoverdemoGameplayTimeoutSec(int* arg)
{
	*arg = g_Window->GetCoverdemoGameplayTimeoutSec();
}

void ScriptType_Builtin::CoverdemoExit(int*)
{
	exit(0);
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::Register()
{
	tBuiltin = new ScriptType_Builtin();
}

BuiltinHandler::BuiltinHandler(const char* _prot, void* (*_hndlr)(void*), const char* _name)
{
	m_Prototype = _prot;
	m_Handler = _hndlr;
	m_Name = _name;
}

DumpTable_Element::DumpTable_Element()
{
	MESSAGE_CLASS_CREATED(DumpTable_Element);

	m_Columns;
	m_NumRows = NULL;
}

#pragma message(TODO_IMPLEMENTATION)
DumpTable_Element::DumpTable_Element(const char* const s)
{
}