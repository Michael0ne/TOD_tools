#include "StringType.h"

StringType::StringType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : BaseType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(StringType);
}

StringType::~StringType()
{
	MESSAGE_CLASS_DESTROYED(StringType);
}