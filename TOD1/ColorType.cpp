#include "ColorType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "Node.h"

ColorType::ColorType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
    MESSAGE_CLASS_CREATED(ColorType);
}

ColorType::~ColorType()
{
    MESSAGE_CLASS_DESTROYED(ColorType);
}

String& ColorType::PrintFormattedValue(String& outstr, void* val, int precision) const
{
    char str[16] = {};
    sprintf(str, "%x", *(int*)val);

    outstr = str;
    return outstr;
}

void ColorType::CallGetterFunction(const Node* callerNode, EntityGetterFunction getterPtr, int a3, int virtualMethodIndex, int a5, int* const outResult) const
{
    ((IntegerType*)this)->CallGetterFunction(callerNode, getterPtr, a3, virtualMethodIndex, a5, outResult);
}

void ColorType::CallSetterFunction(const void* data, Node* callerNode, EntitySetterFunction setterPtr, int a4, int virtualMethodIndex, int a6) const
{
    ((NumberType*)this)->CallSetterFunction(data, callerNode, setterPtr, a4, virtualMethodIndex, a6);
}