#include "StringType.h"

StringType::StringType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(TYPE_STRING, "string", TYPE_STRING_SIZE)
{
    MESSAGE_CLASS_CREATED(StringType);
}

StringType::~StringType()
{
    MESSAGE_CLASS_DESTROYED(StringType);
}

int StringType::GetSize(int* a1, int* a2)
{
    int retsize = Size * 4;

    if (*a1)
        retsize += strlen((const char*)*a1) + 1;

    if (a2)
        a2[3] += retsize;

    return retsize;
}

void* StringType::ReturnNew(void* a1) const
{
    *(int*)a1 = NULL;
    return a1;
}

void StringType::Delete(char* a1)
{
    delete (char*)*a1;
}

void StringType::Clone(int* a1, int* a2)
{
    if (*a1)
        *(char**)&a2 = new char[45];
    else
        *(char**)&a2 = nullptr;
}