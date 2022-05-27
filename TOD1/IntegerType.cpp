#include "IntegerType.h"
#include "NumberType.h"
#include "TruthType.h"
#include "Node.h"

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

int IntegerType::MakeFromString(const char* const input, char* const outdata) const
{
    return ParseNumberString(input, (int*)outdata);
}

void IntegerType::CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, int* const outResult) const
{
    int result;

    //  TODO: this is ridiculous!
    if (a5)
        result = *(int*)((Node*)(callerNode + a3 + virtualMethodIndex + *(int*)(*(int*)callerNode + virtualMethodIndex) + a5)->*(getterPtr))();
    else
        result = *(int*)((Node*)(callerNode + a3)->*(getterPtr))();

    *outResult = result;
}

bool IntegerType::NotEqualTo(const void* const arg1, const void* const arg2) const
{
    return *(int*)arg1 != *(int*)arg2;
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

bool IntegerType::IsValidValueForType(const void*) const
{
    return true;
}