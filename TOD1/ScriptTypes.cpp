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

const Vector4f ScriptType_Builtin::ZeroVector = {};
const Vector4f ScriptType_Builtin::RightVector = { 1.f, 0.f, 0.f, 0.f };
const Vector4f ScriptType_Builtin::UpVector = { 0.f, 1.f, 0.f, 0.f };
const Vector4f ScriptType_Builtin::InVector = { 0.f, 0.f, 1.f, 0.f };
const Vector4f ScriptType_Builtin::LeftVector = { -1.f, 0.f, 0.f, 0.f };
const Vector4f ScriptType_Builtin::DownVector = { 0.f, -1.f, 0.f, 0.f };
const Vector4f ScriptType_Builtin::OutVector = { 0.f, 0.f, -1.f, 0.f };
const ColorRGB ScriptType_Builtin::ColorBlack = { 0.f, 0.f, 0.f, 1.f };
const ColorRGB ScriptType_Builtin::ColorRed = { 1.f, 0.f, 0.f, 1.f };
const ColorRGB ScriptType_Builtin::ColorGreen = { 0.f, 1.f, 0.f, 1.f };
const ColorRGB ScriptType_Builtin::ColorYellow = { 1.f, 1.f, 0.f, 1.f };
const ColorRGB ScriptType_Builtin::ColorDarkBlue = { 0.f, 0.f, 1.f, 1.f };
const ColorRGB ScriptType_Builtin::ColorPink = { 1.f, 0.f, 1.f, 1.f };
const ColorRGB ScriptType_Builtin::ColorBlue = { 0.f, 1.f, 1.f, 1.f };
const ColorRGB ScriptType_Builtin::ColorWhite = { 1.f, 1.f, 1.f, 1.f };

std::vector<DumpTable_Element>	DumpTable;
ScriptType_Builtin* tBuiltin;

ScriptType_Nothing* tNOTHING;
ScriptType_Number* tNUMBER;
ScriptType_Integer* tINTEGER;
ScriptType_Boolean* tBOOLEAN;
ScriptType_Vector* tVECTOR;
ScriptType_Quaternion* tQUATERNION;
ScriptType_Color* tCOLOR;
ScriptType_String* tSTRING;

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

ScriptType::ScriptType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType);

	m_TypeName = typeName;
	m_TypeId = typeId;
	m_Size = typeSize;

	TypesList.push_back(this);
	m_GlobalId = TypesList.size() - 1;
	TypesListCRCCalculated = false;
}

ScriptType::ScriptType(const ScriptType& _rhs)
{
	m_TypeName = _rhs.m_TypeName;
	m_TypeId = _rhs.m_TypeId;
	m_Size = _rhs.m_Size;
	m_GlobalId = _rhs.m_GlobalId;
}

unsigned int ScriptType::GetTypeSize_Impl(const ScriptType* type)
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
	TypesList.pop_back();
	TypesListCRCCalculated = false;
}

unsigned int ScriptType::GetTypeSize() const
{
	return GetTypeSize_Impl(this);
}

ScriptType* ScriptType::GetTypeByName(const char* name)
{
	if (!TypesList.size())
		return nullptr;

	for (std::vector<ScriptType*>::iterator it = TypesList.begin(); it != TypesList.end(); ++it)
		if (strncmp((*it)->m_TypeName.m_szString, name, strlen(name)) == NULL)
			return (*it);

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

			ScriptType* dictelscript = LoadScript(dict_element_type);
			if (dictelscript)
				new ScriptType_Dict(dictelscript);
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

			retn_type = glob_script->AssignScriptToEntity((ScriptType_Entity*)retn_type);
		}

		return retn_type;
	}

	char list_element_type[50] = {};
	strncpy(list_element_type, parenthopenpos + 1, parenthclospos - parenthopenpos - 1);
	ScriptType* listelscript = LoadScript(list_element_type);

	if (listelscript)
		return new ScriptType_List(listelscript);
	else
		return nullptr;
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
	static ScriptType_Nothing*	tyNothing = new ScriptType_Nothing(ScriptType::ScriptTypeId::TYPE_NOTHING, "nothing", ScriptType::ScriptTypeSize::TYPE_NOTHING_SIZE);
	static ScriptType_Number*	tyNumber = new ScriptType_Number(ScriptType::ScriptTypeId::TYPE_NUMBER, "number", ScriptType::ScriptTypeSize::TYPE_NUMBER_SIZE);
	static ScriptType_Integer*	tyInteger = new ScriptType_Integer(ScriptType::ScriptTypeId::TYPE_INTEGER, "integer", ScriptType::ScriptTypeSize::TYPE_INTEGER_SIZE);
	static ScriptType_Boolean*	tyBoolean = new ScriptType_Boolean(ScriptType::ScriptTypeId::TYPE_TRUTH, "truth", ScriptType::ScriptTypeSize::TYPE_TRUTH_SIZE);
	static ScriptType_Vector*	tyVector = new ScriptType_Vector(ScriptType::ScriptTypeId::TYPE_VECTOR, "vector", ScriptType::ScriptTypeSize::TYPE_VECTOR_SIZE);
	static ScriptType_Quaternion*	tyQuaternion = new ScriptType_Quaternion(ScriptType::ScriptTypeId::TYPE_QUATERNION, "quaternion", ScriptType::ScriptTypeSize::TYPE_QUATERNION_SIZE);
	static ScriptType_Color*	tyColor = new ScriptType_Color(ScriptType::ScriptTypeId::TYPE_COLOR, "color", ScriptType::ScriptTypeSize::TYPE_COLOR_SIZE);
	static ScriptType_String*	tyString = new ScriptType_String(ScriptType::ScriptTypeId::TYPE_STRING, "string", ScriptType::ScriptTypeSize::TYPE_STRING_SIZE);

	tNOTHING	= tyNothing;
	tNUMBER		= tyNumber;
	tINTEGER	= tyInteger;
	tBOOLEAN	= tyBoolean;
	tVECTOR		= tyVector;
	tQUATERNION	= tyQuaternion;
	tCOLOR		= tyColor;
	tSTRING		= tyString;
}

unsigned int GetTypesChecksum()
{
	if (TypesListCRCCalculated)
		return TypesListCRC;

#ifdef INCLUDE_FIXES
	char checksum_str[16376] = {};
#else
	char checksum_str[102400] = {};
#endif
	unsigned int checksum_str_len = NULL;

	if (TypesList.size() > 0)
	{
		for (std::vector<ScriptType*>::iterator it = TypesList.begin(); it != TypesList.end(); ++it)
		{
			if ((*it)->m_TypeId != ScriptType::ScriptTypeId::TYPE_ENTITY)
				continue;

			if (checksum_str_len + strlen((*it)->m_TypeName.m_szString) > sizeof(checksum_str))
				break;
			else
				checksum_str_len += strlen((*it)->m_TypeName.m_szString);

			if (*checksum_str == NULL)
				strcpy(checksum_str, (*it)->m_TypeName.m_szString);
			else
				strcat(checksum_str, (*it)->m_TypeName.m_szString);
		}
	}

	TypesListCRC = Utils::CalcCRC32(checksum_str, checksum_str_len);
	TypesListCRCCalculated = true;
	return TypesListCRC;
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

template <class C>
void ScriptType_Entity::RegisterScript(const char* const scriptname, void (C::*scriptproc)(void*), const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7)
{

}

template <class C>
void ScriptType_Entity::RegisterProperty(const ScriptType* returntype, const char* const propertyname, void* (C::* getter)(int) const, const int a4, const int a5, const int a6, void (C::* setter)(int), const int a8, const int a9, const int a10, const char* const a11, const int a12, const int a13, const int argumentstotal)
{
	char buf[26] = {};
	sprintf(buf, "%s:%s", propertyname, returntype->m_TypeName.m_szString);
	unsigned int propindmask = RegisterGlobalProperty(buf, true) | 0x7FFF0000;

	if (argumentstotal < 0)
		m_PropertiesList_1.emplace_back(returntype, nullptr, getter, a4, a5, a6, setter, a8, a9, a10, (propindmask ^ (0xFFFF0000 * (m_PropertiesList_1.size() + field_70))) & 0x7FFF0000 ^ propindmask, 0);
	else
	{
		if (argumentstotal - field_6C >= m_PropertiesList.size())
			m_PropertiesList.insert(m_PropertiesList.begin(), argumentstotal - field_6C + 1, {});
		m_PropertiesList.emplace_back(returntype, nullptr, getterprocptr, a4, a5, a6, setterprocptr, a8, a9, a10, (propindmask ^ (argumentstotal << 16)) & 0x7FFF0000 ^ propindmask, 0);
	}
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Entity::_86E9B0()
{
}

ScriptType_Entity* ScriptType_Entity::GetScriptEntityByName(const char* name)
{
	if (!TypesList.size())
		return nullptr;

	for (std::vector<ScriptType*>::iterator it = TypesList.begin(); it != TypesList.end(); ++it)
		if (strncmp((*it)->m_TypeName.m_szString, name, strlen(name)) == NULL)
			return (ScriptType_Entity*)(*it);

	return nullptr;
}

ScriptType_List::ScriptType_List(const ScriptType* elementsType) : ScriptType(TYPE_LIST, "list", TYPE_LIST_SIZE)
{
	MESSAGE_CLASS_CREATED(ScriptType_List);

	m_ListElementsType = (ScriptType*)elementsType;
	m_ListElementSize = elementsType->m_Size;

	//	NOTE: is this correct?
	char buffer[64] = {};
	sprintf(buffer, "list(%s)", elementsType->m_TypeName.m_szString);
	m_TypeName = buffer;

	if (m_TypeId == 3 || m_TypeId == 8 || m_TypeId == 9 || m_TypeId == 11)
		m_IsTypeId3_8_9_11 = true;
	else
		m_IsTypeId3_8_9_11 = false;
}

ScriptType_Dict::ScriptType_Dict(const ScriptType* elementsType) : ScriptType(TYPE_DICT, "dict", TYPE_DICT_SIZE)
{
	MESSAGE_CLASS_CREATED(ScriptType_Dict);

	m_ElementsType = (ScriptType*)elementsType;

	//	NOTE: is this correct?
	char buffer[64] = {};
	sprintf(buffer, "dict(%s)", elementsType->m_TypeName.m_szString);
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
	*arg = (float)sin(arg[1]);
}

void ScriptType_Builtin::Cos(float* arg)
{
	*arg = (float)cos(arg[1]);
}

void ScriptType_Builtin::Tan(float* arg)
{
	*arg = (float)tan(arg[1]);
}

void ScriptType_Builtin::Asin(float* arg)
{
	*arg = (float)asin(arg[1]);
}

void ScriptType_Builtin::Acos(float* arg)
{
	*arg = (float)acos(arg[1]);
}

void ScriptType_Builtin::Atan(float* arg)
{
	*arg = (float)atan(arg[1]);
}

void ScriptType_Builtin::Abs(int* arg)
{
	*arg = abs(arg[1]);
}

void ScriptType_Builtin::Fabs(float* arg)
{
	*arg = (float)fabs(arg[1]);
}

void ScriptType_Builtin::Sqrt(float* arg)
{
	*arg = (float)sqrt(arg[1]);
}

void ScriptType_Builtin::Floor(float* arg)
{
	*arg = (float)floor(arg[1]);
}

void ScriptType_Builtin::Ceil(float* arg)
{
	*arg = (float)ceil(arg[1]);
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
	*arg = g_InputKeyboard->m_Acquired ? g_InputKeyboard->m_ButtonStates_1[arg[1]] >> 7 : NULL;
}

void ScriptType_Builtin::IsKeyPressed(int* arg)
{
	*arg = g_InputKeyboard->m_Acquired ? g_InputKeyboard->m_ButtonStates_1[arg[1]] & 1 : NULL;
}

void ScriptType_Builtin::IsKeyReleased(int* arg)
{
	*arg = g_InputKeyboard->m_Acquired ? (g_InputKeyboard->m_ButtonStates_1[arg[1]] & 2) != NULL : NULL;
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

void ScriptType_Builtin::NumberToInteger(void* arg)
{
	*((int*)arg) = (int)(((float*)arg)[1]);
}

void ScriptType_Builtin::IntegerToNumber(void* arg)
{
	*((float*)arg) = (float)((int*)arg)[1];
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
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_DIESELPOWER);
		break;
	case 2:
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_DSOUND);
		break;
	case 3:
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_UNDEFINED);	//	FIXME: this should be 0, but no enum for it right now.
		break;
	default:
		StreamedSoundBuffers::RememberSoundRenderer(SOUND_SYSTEM_AUTOSELECT);
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
		String configstr;
		GameConfig::g_Config->m_ConfigurationVariables->GetParamValueString(configstr, (const char*)arg[1]);
		*arg = (int)_strdup(configstr.m_szString);
	}
	else
		*arg = NULL;
}

void ScriptType_Builtin::GetConfigTruth(int* arg)
{
	*arg = GameConfig::g_Config->m_ConfigurationVariables->IsVariableSet((const char*)arg[1]) ? GameConfig::g_Config->m_ConfigurationVariables->GetParamValueBool((const char*)arg[1]) : 0;
}

void ScriptType_Builtin::GetSessionVariableString(int* arg)
{
	if (GameConfig::g_Config->m_SessionVariables->IsVariableSet((const char*)arg[1]))
	{
		String sessionstr;
		GameConfig::g_Config->m_SessionVariables->GetParamValueString(sessionstr, (const char*)arg[1]);
		*arg = (int)_strdup(sessionstr.m_szString);
	}
	else
		*arg = NULL;
}

void ScriptType_Builtin::SetSessionVariableString(int* arg)
{
	GameConfig::g_Config->m_SessionVariables->SetParamValue((const char*)arg[0], (const char*)arg[1]);
}

void ScriptType_Builtin::GetSessionVariableTruth(char* arg)
{
	*arg = GameConfig::g_Config->m_SessionVariables->IsVariableSet((const char*)arg[1]) ? GameConfig::g_Config->m_SessionVariables->GetParamValueBool((const char*)arg[1]) : *arg = false;
}

void ScriptType_Builtin::SetSessionVariableTruth(int* arg)
{
	GameConfig::g_Config->m_SessionVariables->SetParamValueBool((const char*)arg[0], (bool)arg[1]);
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
	ScreenResolution res;
	g_GfxInternal->GetScreenResolution(res);

	*arg = (float)res.x;
	arg[1] = (float)res.y;
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

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableCreate(int* arg)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptType_Builtin::DumptableCreateFromFile(int* arg)
{
	/*
	String buff;
	File dtfile((const char*)*arg, 1, true);

	while (!dtfile.ReadIfNotEOF())
	{
		char buf[5] = {};
		dtfile.Read(buf, sizeof(buf) - 1);
		buff.Append(buf);
	}

	DumpTable_Element dtel(buff.m_szString);
	DumpTable.AddElement(&dtel);

	*arg = DumpTable.m_CurrIndex;
	*/
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
	*arg = g_Blocks->GetRegion();
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

ScriptType_Nothing::ScriptType_Nothing(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_Nothing);
}

ScriptType_Number::ScriptType_Number(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_Number);
}

ScriptType_Integer::ScriptType_Integer(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_Integer);
}

ScriptType_Boolean::ScriptType_Boolean(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_Boolean);
}

ScriptType_Vector::ScriptType_Vector(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_Vector);
}

ScriptType_Quaternion::ScriptType_Quaternion(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_Quaternion);
}

ScriptType_Color::ScriptType_Color(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_Color);
}

ScriptType_String::ScriptType_String(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(ScriptType_String);
}

ScriptType_Entity::ScriptFuncDesc::ScriptFuncDesc(const void* const scriptprocptr, int a2, int a3, int a4)
{
	m_ScriptProcPtr = (void (*)(class Entity*, void* params))scriptprocptr;
	field_4 = a2;
	field_8 = a3;
	field_C = a4;
}

ScriptType_Entity::ScriptMethodDesc::ScriptMethodDesc(const ScriptType* rettype, int* a2, GETTER getterprocptr, unsigned int a4, unsigned int a5, unsigned int a6, SETTER setterprocptr, unsigned int a8, unsigned int a9, int a10, unsigned int a11, unsigned int a12)
{
	m_ReturnType = (ScriptType*)rettype;
	field_4 = a2;
	m_Getter = getterprocptr;
	field_C = a4;
	field_10 = a5;
	field_14 = a6;
	m_Setter = setterprocptr;
	field_1C = a8;
	field_20 = a9;
	field_24 = a10;
	field_28 = a11;
	field_2C = a12;
}