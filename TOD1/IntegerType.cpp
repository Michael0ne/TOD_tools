#include "IntegerType.h"
#include "NumberType.h"
#include "TruthType.h"

IntegerType::IntegerType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
    MESSAGE_CLASS_CREATED(IntegerType);
}

IntegerType::~IntegerType()
{
    MESSAGE_CLASS_DESTROYED(IntegerType);
}

void* IntegerType::ReturnNew(void* a1) const
{
    *(int*)a1 = 0;
    return a1;
}

String& IntegerType::PrintFormattedValue(String& outstr, void* val, int precision) const
{
    char _outstr[16] = {};

    if (precision)
    {
        char formatstr[16] = {};

        sprintf(formatstr, "%%%dd", precision >> 16);
        sprintf(_outstr, formatstr, *(int*)val);

        outstr = _outstr;
        return outstr;
    }
    else
    {
        sprintf(_outstr, "%d", *(int*)val);

        outstr = _outstr;
        return outstr;
    }
}

int IntegerType::StrToType(char* operation, void* outval) const
{
    return ParseNumberString(operation, (int*)outval);
}

void IntegerType::stub13(int a1, int(__thiscall* procptr)(void*, void*), int a3, int a4, int a5, void* const a6) const
{
    if (a5)
        *(int*)a6 = procptr((void*)(a1 + a3 + a4 + *(int*)(*(int*)(a1 + a4) + a5)), nullptr);
    else
        *(int*)a6 = procptr((void*)(a1 + a3), nullptr);
}

bool IntegerType::NotEqualTo(void* a1, void* a2) const
{
    return *(int*)a1 != *(int*)a2;
}

void IntegerType::ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
{
    *a4 = 1;

    if (strcmp(operation, "+(integer)") == NULL)
    {
        *outopid = 0;
        *outoprestype = tINTEGER;
    }

    if (strcmp(operation, "-(integer)") == NULL)
    {
        *outopid = 1;
        *outoprestype = tINTEGER;
    }

    if (strcmp(operation, "*(integer)") == NULL)
    {
        *outopid = 2;
        *outoprestype = tINTEGER;
    }

    if (strcmp(operation, "/(integer)") == NULL)
    {
        *outopid = 3;
        *outoprestype = tINTEGER;
    }

    if (strcmp(operation, "%(integer)") == NULL)
    {
        *outopid = 4;
        *outoprestype = tINTEGER;
    }
    if (strcmp(operation, "^(integer)") == NULL)
    {
        *outopid = 5;
        *outoprestype = tINTEGER;
    }

    if (strcmp(operation, "^(number)") == NULL)
    {
        *outopid = 6;
        *outoprestype = tNUMBER;
    }

    if (strcmp(operation, "==(integer)") == NULL)
    {
        *outopid = 7;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "!=(integer)") == NULL)
    {
        *outopid = 8;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, ">(integer)") == NULL)
    {
        *outopid = 9;
        *outoprestype = tTRUTH;
    }
    if (strcmp(operation, "<(integer)") == NULL)
    {
        *outopid = 10;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, ">=(integer)") == NULL)
    {
        *outopid = 11;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "<=(integer)") == NULL)
    {
        *outopid = 12;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "-") == NULL)
    {
        *outopid = 13;
        *outoprestype = tINTEGER;
    }

    DataType::ParseOperationString(operation, outopid, outoprestype, a4);
}

void IntegerType::PerformOperation(int operationId, void* params) const
{
    switch (operationId)
    {
    case 0:
        *(int*)params = *((int*)params + 1) + *((int*)params + 2);
        break;
    case 1:
        *(int*)params = *((int*)params + 2) - *((int*)params + 1);
        break;
    case 2:
        *(int*)params = *((int*)params + 2) * *((int*)params + 1);
        break;
    case 3:
        *(float*)params = *((float*)params + 2) / *((float*)params + 1);
        break;
    case 4:
        *(int*)params = *((int*)params + 2) % *((int*)params + 1);
        break;
    case 5:
        *(int*)params = (int)pow(*((float*)params + 2), *((float*)params + 1));
        break;
    case 6:
        *(float*)params = (float)pow(*((float*)params + 2), *((float*)params + 1));
        break;
    case 7:
        *(bool*)params = AreEqual((int*)params + 2, (int*)params + 1);
        break;
    case 8:
        *(bool*)params = NotEqualTo((int*)params + 2, (int*)params + 1);
        break;
    case 9:
        *(bool*)params = *((int*)params + 2) > *((int*)params + 1);
        break;
    case 10:
        *(bool*)params = *((int*)params + 2) < *((int*)params + 1);
        break;
    case 11:
        *(bool*)params = *((int*)params + 2) >= *((int*)params + 1);
        break;
    case 12:
        *(bool*)params = *((int*)params + 2) <= *((int*)params + 1);
        break;
    case 13:
        *(int*)params = -*((int*)params + 1);
        break;
    default:
        break;
    }
}

bool IntegerType::IsValidValueForType(void*) const
{
    return true;
}