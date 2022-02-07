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

int DataType::stub2(int* a1, int* a2)
{
    if (a2)
        a2[m_TypeId] += m_Size * 4;

    return m_Size * 4;
}

void* DataType::stub3(void*) const
{
    return nullptr;
}

void DataType::stub4(char*)
{
    return;
}

void DataType::stub5(int* a1, int* a2)
{
    _4893C0(a1, a2, m_Size);
}

String& DataType::stub6(String& outstr, void*, int) const
{
    outstr = {};
    return outstr;
}

int DataType::stub7(char*, void*) const
{
    return -1;
}

int DataType::stub8(char* a1)
{
    char str[16] = {};

    int result = stub10(a1, str);
    stub4(str);

    return result;
}

int DataType::stub9(char* a1, char* a2)
{
    if (a1 == a2)
        return m_Size;

    if (a2 >= a1)
    {
        char* lastsym = &a1[m_Size - 1];
        if (m_Size - 1 >= 0)
        {
            int len = &a2[m_Size] - &a1[m_Size];
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

    char* firstsym = a1;
    for (int _size = m_Size; _size; --_size)
    {
        *(firstsym + (a2 - a1)) = *a1;
        firstsym++;
    }

    return m_Size;
}

int DataType::stub10(char* a1, char* a2)
{
    return stub9(a1, a2);
}

int DataType::stub11(char* a1, String& a2, int a3)
{
    char str[16] = {};
    String str1;

    int result = stub10(a1, str);
    a2 = stub6(str1, str, a3);
    stub4(str);

    return result;
}

int DataType::stub12(char* a1, char* a2, int* a3)
{
    char str[16] = {};

    int result = stub7(a1, str);
    *a3 = stub9(str, a2);
    stub4(str);

    return result;
}

void DataType::stub13(int, void*, int, int, int, void* const) const
{
    return;
}

void DataType::stub14(int*, int, void*, int, int, int) const
{
    return;
}

bool DataType::stub15(void* a1, void* a2) const
{
    return stub16(a1, a2) == false;
}

bool DataType::stub16(void* a1, void* a2) const
{
    return true;
}

void DataType::stub17(const char* const operation, int* outopid, DataType** outoprestype, char*) const
{
    *outopid = -1;
}

void DataType::stub18(int operationId, void* params) const
{
    return;
}

char DataType::stub19(int a1, int a2)
{
    return true;
}

bool DataType::stub20(void*) const
{
    return true;
}

void DataType::_4893C0(int* a1, int* a2, int a3)
{
    if (a1 == a2)
        return;

    if (a1 >= a2)
    {
        char* _a1 = (char*)&a1[a3 - 1];
        char* _a2 = (char*)&a2[a3 - 1];

        if (a3 - 1 >= 0)
        {
            for (int i = a3; i; --i)
                *_a1-- = *_a2--;
        }
    }
    else
        if (a3 > 0)
        {
            char* _a1 = (char*)a1;
            char* _a2 = (char*)a2;

            for (int i = a3; i; --i)
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
    case TYPE_SCRIPT:
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

DataType* DataType::GetTypeByName(const char* name)
{
    if (!TypesList.size())
        return nullptr;

    for (std::vector<DataType*>::iterator it = TypesList.begin(); it != TypesList.end(); ++it)
        if (strncmp((*it)->m_TypeName.m_Str, name, strlen(name)) == NULL)
            return (*it);

#if defined(INCLUDE_FIXES) && defined(VERBOSELOG)
    debug("GetTypeByName(\"%s\") FAILED!", name);
#endif
    return nullptr;
}

DataType* DataType::LoadScript(const char* script)
{
    if (DataType* type_ = GetTypeByName(script))
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

            GlobalScript* glob_script = GlobalScript::GetGlobalScriptByName(script_name.m_Str);
            if (!glob_script)
            {
                LogDump::LogA("Unable to load script '%s'\n", script_name.m_Str);
                return nullptr;
            }

            rettype = (DataType*)glob_script->AssignScriptToEntity((EntityType*)rettype);
        }

        return rettype;
    }

    char list_element_type[50] = {};
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
    static NothingType* tyNothing       = new NothingType(ScriptTypeId::TYPE_NOTHING, "nothing", ScriptTypeSize::TYPE_NOTHING_SIZE);
    static NumberType* tyNumber         = new NumberType(ScriptTypeId::TYPE_NUMBER, "number", ScriptTypeSize::TYPE_NUMBER_SIZE);
    static IntegerType* tyInteger       = new IntegerType(ScriptTypeId::TYPE_INTEGER, "integer", ScriptTypeSize::TYPE_INTEGER_SIZE);
    static TruthType* tyBoolean         = new TruthType(ScriptTypeId::TYPE_TRUTH, "truth", ScriptTypeSize::TYPE_TRUTH_SIZE);
    static VectorType* tyVector         = new VectorType(ScriptTypeId::TYPE_VECTOR, "vector", ScriptTypeSize::TYPE_VECTOR_SIZE);
    static QuaternionType* tyQuaternion = new QuaternionType(ScriptTypeId::TYPE_QUATERNION, "quaternion", ScriptTypeSize::TYPE_QUATERNION_SIZE);
    static ColorType* tyColor           = new ColorType(ScriptTypeId::TYPE_COLOR, "color", ScriptTypeSize::TYPE_COLOR_SIZE);
    static StringType* tyString         = new StringType(ScriptTypeId::TYPE_STRING, "string", ScriptTypeSize::TYPE_STRING_SIZE);

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
    GlobalScript::ScriptsList.clear();
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