#include "NothingType.h"

NothingType::NothingType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
 MESSAGE_CLASS_CREATED(NothingType);
}

NothingType::~NothingType()
{
 MESSAGE_CLASS_DESTROYED(NothingType);
}