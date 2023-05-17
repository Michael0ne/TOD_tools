#include "TruthType.h"
#include "Node.h"

TruthType::TruthType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
    MESSAGE_CLASS_CREATED(TruthType);
}

TruthType::~TruthType()
{
    MESSAGE_CLASS_DESTROYED(TruthType);
}

void* TruthType::ReturnNew(void* a1) const
{
    *(int*)a1 = 0;
    return a1;
}

String& TruthType::PrintFormattedValue(String& outstr, void* a2, int) const
{
    outstr = *(bool*)a2 ? "true" : "false";
    return outstr;
}

int TruthType::MakeFromString(const char* const input, char* const outdata) const
{
    if (strcmp(input, "true") == NULL)
    {
        *(bool*)outdata = true;
        return 4;
    }

    if (strcmp(input, "false") == NULL ||
        strcmp(input, "0") == NULL)
    {
        *(bool*)outdata = false;
        return 5;
    }

    return -1;
}

void TruthType::CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, uint8_t* const outResult) const
{
    bool result = 0;

    //  TODO: this is ridiculous!
    if (a5)
        result = *(bool*)((Node*)(callerNode + a3 + virtualMethodIndex + *(int*)(*(int*)callerNode + virtualMethodIndex) + a5)->*(getterPtr))();
    else
        result = *(bool*)((Node*)(callerNode + a3)->*(getterPtr))();

    *outResult = result;
}

void TruthType::CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const
{
    //  TODO: this is ridiculous!
    if (a6)
        ((Node*)(callerNode + a4 + virtualMethodIndex + *(int*)(*(int*)(virtualMethodIndex + callerNode) + a6))->*(setterPtr))(*(const void**)data);
    else
        ((Node*)(callerNode + a4)->*(setterPtr))(*(const void**)data);
}

void TruthType::ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
{
    *a4 = 1;

    if (strcmp(operation, "==(truth)") == NULL)
    {
        *outopid = 0;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "!=(truth)") == NULL)
    {
        *outopid = 1;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "!") == NULL)
    {
        *outopid = 2;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "||(truth)") == NULL)
    {
        *outopid = 3;
        *outoprestype = tTRUTH;
    }

    if (strcmp(operation, "&&(truth)") == NULL)
    {
        *outopid = 4;
        *outoprestype = tTRUTH;
    }

    DataType::ParseOperationString(operation, outopid, outoprestype, a4);
}

void TruthType::PerformOperation(int operationId, void* params) const
{
    switch (operationId)
    {
    case 0:
        *(bool*)params = AreEqual((bool*)params + 2, (bool*)params + 1);
        break;
    case 1:
        *(bool*)params = NotEqualTo((bool*)params + 2, (bool*)params + 1);
        break;
    case 2:
        *(bool*)params = *((bool*)params + 1) == false;
        break;
    case 3:
        if (*((bool*)params + 2) || *((bool*)params + 1))
            *(bool*)params = true;
        else
            *(bool*)params = false;
        break;
    case 4:
        if (*((bool*)params + 2) && *((bool*)params + 1))
            *(bool*)params = true;
        else
            *(bool*)params = false;
        break;
    default:
        break;
    }
}