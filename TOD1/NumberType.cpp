#include "NumberType.h"
#include "VectorType.h"
#include "TruthType.h"
#include "Node.h"

NumberType::NumberType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
    MESSAGE_CLASS_CREATED(NumberType);
}

NumberType::~NumberType()
{
    MESSAGE_CLASS_DESTROYED(NumberType);
}

void* NumberType::ReturnNew(void* a1) const
{
    *(float*)a1 = 0.f;
    return a1;
}

String& NumberType::PrintFormattedValue(String& outstr, void* val, int precision) const
{
    char _outstr[16] = {};

    if (precision)
    {
        char formatstr[16] = {};

        sprintf(formatstr, "%%%d.%df", precision >> 16, (unsigned short)precision);
        sprintf(_outstr, formatstr, *(float*)val);

        outstr = _outstr;
        return outstr;
    }
    else
    {
        sprintf(_outstr, "%f", *(float*)val);

        outstr = _outstr;
        return outstr;
    }
}

int NumberType::MakeFromString(const char* const input, char* const outdata) const
{
    return ParseFloatNumberString(input, (float*)outdata);
}

void NumberType::CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, int* const outResult) const
{
    int result;

    //  TODO: this is ridiculous!
    if (a5)
        result = *(int*)((Node*)(callerNode + a3 + virtualMethodIndex + *(int*)(*(int*)callerNode + virtualMethodIndex) + a5)->*(getterPtr))();
    else
        result = *(int*)((Node*)(callerNode + a3)->*(getterPtr))();

    *outResult = result;
}

void NumberType::CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const
{
    //  TODO: this is ridiculous!
    if (a6)
        ((Node*)(callerNode + a4 + virtualMethodIndex + *(int*)(*(int*)(virtualMethodIndex + callerNode) + a6))->*(setterPtr))(*(const void**)data);
    else
        ((Node*)(callerNode + a4)->*(setterPtr))(*(const void**)data);
}

bool NumberType::NotEqualTo(const void* const arg1, const void* const arg2) const
{
    return *(float*)arg1 != *(float*)arg2;
}

void NumberType::ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
{
    *a4 = 1;

    if (strcmp(operation, "+(number)") == NULL)
    {
        *outopid = 0;
        *outoprestype = tNUMBER;
    }

    if (strcmp(operation, "-(number)") == NULL)
    {
        *outopid = 1;
        *outoprestype = tNUMBER;
    }

    if (strcmp(operation, "*(number)") == NULL)
    {
        *outopid = 2;
        *outoprestype = tNUMBER;
    }

    if (strcmp(operation, "/(number)") == NULL)
    {
        *outopid = 3;
        *outoprestype = tNUMBER;
    }

    if (strcmp(operation, "^(number)") == NULL)
    {
        *outopid = 4;
        *outoprestype = tNUMBER;
    }

    if (strcmp(operation, "*(vector)") == NULL)
    {
        *outopid = 5;
        *outoprestype = tVECTOR;
    }

    if (strcmp(operation, "==(number)") == NULL)
    {
        *outopid = 6;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "!=(number)") == NULL)
    {
        *outopid = 7;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, ">(number)") == NULL)
    {
        *outopid = 8;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "<(number)") == NULL)
    {
        *outopid = 9;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, ">=(number)") == NULL)
    {
        *outopid = 10;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "<=(number)") == NULL)
    {
        *outopid = 11;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "-") == NULL)
    {
        *outopid = 12;
        *outoprestype = tNUMBER;
    }

    DataType::ParseOperationString(operation, outopid, outoprestype, a4);
}

void NumberType::PerformOperation(int operationId, void* params) const
{
    switch (operationId)
    {
    case 0:
        *(float*)params = *((float*)params + 2) / *((float*)params + 1);
        break;
    case 1:
        *(float*)params = *((float*)params + 2) / *((float*)params + 1);
        break;
    case 2:
        *(float*)params = *((float*)params + 2) / *((float*)params + 1);
        break;
    case 3:
        *(float*)params = *((float*)params + 2) / *((float*)params + 1);
        break;
    case 4:
        *(float*)params = powf(*((float*)params + 2), *((float*)params + 1));
        break;
    case 5:
    {
        int vecsize = tVECTOR->m_Size;

        *(float*)params = ((float*)params)[vecsize] * ((float*)params)[vecsize * 2];
        *(float*)((float*)params + 1) = ((float*)params)[vecsize + 1] * ((float*)params)[vecsize * 2];
        *(float*)((float*)params + 2) = ((float*)params)[vecsize + 2] * ((float*)params)[vecsize * 2];
    }
    break;
    case 6:
        *(float*)params = AreEqual((float*)params + 2, (float*)params + 1);
        break;
    case 7:
        *(float*)params = NotEqualTo((float*)params + 2, (float*)params + 1);
        break;
    case 8:
        if (*((float*)params + 2) > *((float*)params + 1))
            *(bool*)params = true;
        else
            *(bool*)params = false;
        break;
    case 9:
        if (*((float*)params + 1) <= *((float*)params + 2))
            *(bool*)params = false;
        else
            *(bool*)params = true;
        break;
    case 10:
        if (*((float*)params + 2) < *((float*)params + 1))
            *(bool*)params = false;
        else
            *(bool*)params = true;
        break;
    case 11:
        if (*((float*)params + 1) < *((float*)params + 2))
            *(bool*)params = false;
        else
            *(bool*)params = true;
        break;
    case 12:
        *(float*)params = 0.f - *((float*)params + 1);
        break;
    default:
        break;
    }
}

bool NumberType::IsValidValueForType(const void* a1) const
{
    return !isinf(*(float*)a1);
}