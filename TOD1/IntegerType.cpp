#include "IntegerType.h"
#include "NumberType.h"
#include "TruthType.h"

IntegerType::IntegerType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : BaseType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(IntegerType);
}

IntegerType::~IntegerType()
{
	MESSAGE_CLASS_DESTROYED(IntegerType);
}

void* IntegerType::stub3(void* a1) const
{
	*(int*)a1 = 0;
	return a1;
}

String& IntegerType::stub6(String& outstr, void* val, int precision) const
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

int IntegerType::stub7(char* operation, void* outval) const
{
	return ParseNumberString(operation, (int*)outval);
}

void IntegerType::stub13(int a1, void* a2, int a3, int a4, int a5, void* const a6) const
{
	if (a5)
		*(int*)a6 = ((int (__thiscall*)(char*))a2)((char*)a1 + a3 + a4 + *(int*)(*(int*)(a1 + a4) + a5));
	else
		*(int*)a6 = ((int (__thiscall*)(char*))a2)((char*)a1 + a3);
}

bool IntegerType::stub16(void* a1, void* a2) const
{
	return *(int*)a1 != *(int*)a2;
}

void IntegerType::stub17(const char* const operation, int* outopid, BaseType** outoprestype, char* a4) const
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

	BaseType::stub17(operation, outopid, outoprestype, a4);
}

void IntegerType::stub18(int operationId, void* params) const
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
		*(bool*)params = stub15((int*)params + 2, (int*)params + 1);
		break;
	case 8:
		*(bool*)params = stub16((int*)params + 2, (int*)params + 1);
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

bool IntegerType::stub20(void*) const
{
	return true;
}