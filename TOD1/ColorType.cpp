#include "ColorType.h"
#include "IntegerType.h"
#include "NumberType.h"

ColorType::ColorType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
 MESSAGE_CLASS_CREATED(ColorType);
}

ColorType::~ColorType()
{
 MESSAGE_CLASS_DESTROYED(ColorType);
}

String& ColorType::stub6(String& outstr, void* val, int precision) const
{
 char str[16] = {};
 sprintf(str, "%x", *(int*)val);

 outstr = str;
 return outstr;
}

void ColorType::stub13(int a1, void* a2, int a3, int a4, int a5, void* const a6) const
{
 ((IntegerType*)this)->stub13(a1, a2, a3, a4, a5, a6);
}

void ColorType::stub14(int* a1, int a2, void* a3, int a4, int a5, int a6) const
{
 ((NumberType*)this)->stub14(a1, a2, a3, a4, a5, a6);
}