#include "DataType.h"
#include "NothingType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "VectorType.h"
#include "QuaternionType.h"
#include "ColorType.h"
#include "StringType.h"
#include "DictType.h"
#include "ListType.h"
#include "ScriptDatabase.h"

#include "LogDump.h"
#include "Globals.h"

class NothingType* tNOTHING; // @A3CE94
class NumberType* tNUMBER; // @A3CEC0
class IntegerType* tINTEGER; // @A3CEB8
class TruthType* tTRUTH; // @A3CEC4
class VectorType* tVECTOR; // @A3CEB4
class QuaternionType* tQUATERNION; // @A3CE98
class ColorType* tCOLOR; // @A3CEA4
class StringType* tSTRING; // @A3CEB0

bool     TypesListCRCCalculated;
std::vector<DataType*> TypesList;
unsigned int   TypesListCRC;
float     _A3A064;

DataType::~DataType()
{
    MESSAGE_CLASS_DESTROYED(DataType);

    RemoveTypeFromList(m_TypeName.m_Str);
}

int DataType::GetSize(int* dummy, int* list) const
{
    if (list)
        list[m_TypeId] += m_Size * 4;

    return m_Size * 4;
}

void* DataType::ReturnNew(void*) const
{
    return nullptr;
}

void DataType::Delete(char*)
{
    return;
}

void DataType::Clone(const int* from, int* to)
{
    CopyValue(to, from, m_Size);
}

String& DataType::PrintFormattedValue(String& outstr, void*, int) const
{
    outstr = {};
    return outstr;
}

int DataType::MakeFromString(const char* const input, char* const outdata) const
{
    return -1;
}

int DataType::MakeCopy(char* a1)
{
    char str[16] = {};

    int result = CopyAndAllocate(a1, str);
    Delete(str);

    return result;
}

int DataType::CopyNoAllocate(const char* const from, char* to)
{
    if (to == from)
        return m_Size;

    if (from >= to)
    {
        char* lastsym = &to[m_Size - 1];
        if (m_Size - 1 >= 0)
        {
            int len = &from[m_Size] - &to[m_Size];
            for (int _size = m_Size; _size; --_size)
            {
                *(lastsym + len) = *lastsym;
                --lastsym;
            }

            return m_Size;
        }
    }

    if (m_Size <= 0)
        return m_Size;

    char* firstsym = to;
    for (int _size = m_Size; _size; --_size)
    {
        *(firstsym + (from - to)) = *to;
        firstsym++;
    }

    return m_Size;
}

int DataType::CopyAndAllocate(const char* const from, char* to)
{
    return CopyNoAllocate(from, to);
}

int DataType::AsString(const char* const from, String& outString, int format)
{
    char str[16] = {};
    String str1;

    int result = CopyAndAllocate(from, str);
    outString = PrintFormattedValue(str1, str, format);
    Delete(str);

    return result;
}

int DataType::MakeFromString_A(const char* const inputstr, char* outtype, int* const outsize)
{
    char str[16] = {};

    int result = MakeFromString(inputstr, str);
    *outsize = CopyNoAllocate(str, outtype);
    Delete(str);

    return result;
}

void DataType::CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, int* const outResult) const
{
    return;
}

void DataType::CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const
{
    return;
}

bool DataType::AreEqual(const void* const a1, const void* const a2) const
{
    return NotEqualTo(a1, a2) == false;
}

bool DataType::NotEqualTo(const void* const a1, const void* const a2) const
{
    return true;
}

void DataType::ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char*) const
{
    *outopid = -1;
}

void DataType::PerformOperation(int operationId, void* params) const
{
    return;
}

char DataType::IsReferenced(int* a1, int a2)
{
    return true;
}

bool DataType::IsValidValueForType(const void*) const
{
    return true;
}

void DataType::CopyValue(int* to, const int* from, const size_t size)
{
    if (to == from)
        return;

    if (to >= from)
    {
        char* _a1 = (char*)&to[size - 1];
        char* _a2 = (char*)&from[size - 1];

        if (size - 1 >= 0)
        {
            for (int i = size; i; --i)
                *_a1-- = *_a2--;
        }
    }
    else
        if (size > 0)
        {
            char* _a1 = (char*)to;
            char* _a2 = (char*)from;

            for (int i = size; i; --i)
                *_a1++ = *_a2++;
        }

}

void DataType::RemoveTypeFromList(const char* const name)
{
    TypesList.pop_back();
    TypesListCRCCalculated = false;
}

unsigned int DataType::GetTypeSize_Impl(const DataType* type)
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

int DataType::ParseFloatNumberString(const char* const numberstr, float* const outval)
{
    bool negativenum = false;
    int intpart = 0, fractpart = 0;
    char* currchar = (char*)numberstr;

    if (*numberstr == '-')
    {
        negativenum = true;
        currchar++;
    }
    else
        if (*numberstr != '+')
            negativenum = false;
        else
            currchar++;

    int digitsread = ParseNumberString(currchar, &intpart);
    if (digitsread)
    {
        char* lastcharpos = &currchar[digitsread];
        if (digitsread)
            digitsread = intpart;
        else
            intpart = 0;

        if (*lastcharpos == '.')
        {
            digitsread = ParseNumberString(lastcharpos + 1, &fractpart);
            if (digitsread >= 0)
                if (negativenum)
                    *outval = 0.f - ((float)fractpart / powf(10.f, (float)digitsread) + intpart);
                else
                    *outval = (float)fractpart / powf(10.f, (float)digitsread) + intpart;
        }
        else
            *outval = negativenum ? (float)-digitsread : (float)digitsread;

        return digitsread; // TODO: check this.
    }
    else
        return -1;
}

int DataType::ParseNumberString(const char* const numberstr, int* const outnumber)
{
    bool negativenum = false;
    char* currchar = (char*)numberstr;

    if (*numberstr == '-')
    {
        negativenum = true;
        currchar++;
    }
    else
        if (*numberstr != '+')
            negativenum = false;
        else
            currchar++;

    int digit = NULL;
    if (isdigit(*currchar))
    {
        do
        {
            digit = *currchar + 10 * digit - 48;
        } while (isdigit(*++currchar));
    }

    if (negativenum)
        digit = -digit;

    *outnumber = digit;

    // NOTE: returns how many digits this number has.
    return currchar - numberstr;
}

DataType::DataType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize)
{
    MESSAGE_CLASS_CREATED(DataType);

    m_TypeName = typeName;
    m_TypeId = typeId;
    m_Size = typeSize;

    m_GlobalId = TypesList.size();
    TypesList.push_back(this);
    TypesListCRCCalculated = false;
}

unsigned int DataType::GetTypeSize() const
{
    return GetTypeSize_Impl(this);
}

//  NOTE: simple in terms of 'has no pointers inside it'.
const bool DataType::IsSimpleType(const DataType* t)
{
    return
        t->m_TypeId != TYPE_STRING &&
        t->m_TypeId != TYPE_LIST &&
        t->m_TypeId != TYPE_DICT &&
        t->m_TypeId != TYPE_STRUCT;
}

DataType* DataType::GetTypeByName(const char* name)
{
    if (!TypesList.size())
        return nullptr;

    const size_t nameLen = strlen(name);
    for (size_t i = 0; i < TypesList.size(); ++i)
    {
        int j = 0;
        while (true)
        {
            if (j == TypesList[i]->m_TypeName.m_Length)
                break;

            if (tolower(TypesList[i]->m_TypeName.m_Str[j]) == tolower(name[j]))
                j++;
            else
                break;
        }

        if (j == nameLen)
            return TypesList[i];
    }

#if defined(INCLUDE_FIXES) && defined(VERBOSELOG)
    debug("GetTypeByName(\"%s\") FAILED!", name);
#endif
    return nullptr;
}

DataType* DataType::LoadScript(const char* script)
{
    DataType* type_ = GetTypeByName(script);
    if (type_)
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

            DataType* dictelscript = LoadScript(dict_element_type);
            if (dictelscript)
                new DictType(dictelscript);
        }

        String script_name, script_type;
        if (!ParseVariableString(script, script_name, script_type))
            return nullptr;

        String script_complete_name = script_name;
        script_complete_name.Append("(");
        script_complete_name.Append(script_type.m_Str);
        script_complete_name.Append(")");

        DataType* rettype = GetTypeByName(script_complete_name.m_Str);
        if (!rettype)
        {
            rettype = (DataType*)GetScriptEntityByName(script_type.m_Str);
            if (!rettype)
                return nullptr;

            Scriptbaked* glob_script = Scriptbaked::GetGlobalScriptByName(script_name.m_Str);
            if (!glob_script)
            {
                LogDump::LogA("Unable to load script '%s'\n", script_name.m_Str);
                return nullptr;
            }

            rettype = (DataType*)glob_script->AssignScriptToEntity((EntityType*)rettype);
        }

        return rettype;
    }

    char list_element_type[128] = {};
    strncpy(list_element_type, parenthopenpos + 1, parenthclospos - parenthopenpos - 1);
    DataType* listelscript = LoadScript(list_element_type);

    if (listelscript)
        return new ListType(listelscript);
    else
        return nullptr;
}

EntityType* DataType::GetScriptEntityByName(const char* name)
{
    if (!TypesList.size())
        return nullptr;

    for (std::vector<DataType*>::iterator it = TypesList.begin(); it != TypesList.end(); ++it)
        if ((*it)->m_TypeId == TYPE_ENTITY && strncmp((*it)->m_TypeName.m_Str, name, strlen(name)) == NULL)
            return (EntityType*)(*it);

    return nullptr;
}

bool DataType::ParseVariableString(const char* variable, String& variableName, String& variableType)
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

void DataType::InitScriptTypes()
{
    static NothingType* tyNothing = new NothingType(ScriptTypeId::TYPE_NOTHING, "nothing", ScriptTypeSize::TYPE_NOTHING_SIZE);
    static NumberType* tyNumber = new NumberType(ScriptTypeId::TYPE_NUMBER, "number", ScriptTypeSize::TYPE_NUMBER_SIZE);
    static IntegerType* tyInteger = new IntegerType(ScriptTypeId::TYPE_INTEGER, "integer", ScriptTypeSize::TYPE_INTEGER_SIZE);
    static TruthType* tyBoolean = new TruthType(ScriptTypeId::TYPE_TRUTH, "truth", ScriptTypeSize::TYPE_TRUTH_SIZE);
    static VectorType* tyVector = new VectorType(ScriptTypeId::TYPE_VECTOR, "vector", ScriptTypeSize::TYPE_VECTOR_SIZE);
    static QuaternionType* tyQuaternion = new QuaternionType(ScriptTypeId::TYPE_QUATERNION, "quaternion", ScriptTypeSize::TYPE_QUATERNION_SIZE);
    static ColorType* tyColor = new ColorType(ScriptTypeId::TYPE_COLOR, "color", ScriptTypeSize::TYPE_COLOR_SIZE);
    static StringType* tyString = new StringType(ScriptTypeId::TYPE_STRING, "string", ScriptTypeSize::TYPE_STRING_SIZE);

    tNOTHING = tyNothing;
    tNUMBER = tyNumber;
    tINTEGER = tyInteger;
    tTRUTH = tyBoolean;
    tVECTOR = tyVector;
    tQUATERNION = tyQuaternion;
    tCOLOR = tyColor;
    tSTRING = tyString;
}

void DataType::ClearScriptLists()
{
    ClearGlobalScriptList();
    GlobalCommand::ClearGlobalCommands();
    GlobalProperty::ClearGlobalProperties();

    TypesList.clear();
    TypesListCRCCalculated = false;
}

void DataType::ClearGlobalScriptList()
{
    Scriptbaked::ScriptsList.clear();
}

unsigned int DataType::GetTypesListChecksum()
{
    if (TypesListCRCCalculated)
        return TypesListCRC;

#ifdef INCLUDE_FIXES
    char checksum_str[16366] = {};
#else
    char checksum_str[102400] = {};
#endif
    unsigned int checksum_str_len = NULL;

    if (TypesList.size() > 0)
    {
        for (std::vector<DataType*>::iterator it = TypesList.begin(); it != TypesList.end(); ++it)
        {
            if ((*it)->m_TypeId != DataType::ScriptTypeId::TYPE_ENTITY)
                continue;

            if (checksum_str_len + strlen((*it)->m_TypeName.m_Str) > sizeof(checksum_str))
                break;
            else
                checksum_str_len += strlen((*it)->m_TypeName.m_Str);

            if (*checksum_str == NULL)
                strcpy(checksum_str, (*it)->m_TypeName.m_Str);
            else
                strcat(checksum_str, (*it)->m_TypeName.m_Str);
        }
    }

    TypesListCRC = Utils::CalcCRC32(checksum_str, checksum_str_len);
    TypesListCRCCalculated = true;
    return TypesListCRC;
}

unsigned int DataType::GetTypesListSize()
{
    return TypesList.size();
}