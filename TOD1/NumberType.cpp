#include "NumberType.h"
#include "VectorType.h"
#include "TruthType.h"

NumberType::NumberType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(NumberType);
}

NumberType::~NumberType()
{
	MESSAGE_CLASS_DESTROYED(NumberType);
}

void* NumberType::stub3(void* a1) const
{
	*(float*)a1 = 0.f;
	return a1;
}

String& NumberType::stub6(String& outstr, void* val, int precision) const
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

int NumberType::stub7(char* operation, void* outval) const
{
	return ParseFloatNumberString(operation, (float*)outval);
}

void NumberType::stub13(int a1, void* a2, int a3, int a4, int a5, void* const a6) const
{
	if (a5)
		*(float*)a6 = ((float(__thiscall*)(int, void*))a2)(a1 + a3 + a4 + *(int*)(*(int*)(a4 + a1) + a5), a2);
	else
		*(float*)a6 = ((float(__thiscall*)(int, void*))a2)(a3 + a1, a2);
}

void NumberType::stub14(int* a1, int a2, void* a3, int a4, int a5, int a6) const
{
	if (a6)
		(*(int(__thiscall*)(int, int))a3)(a2 + a4 + a5 + *(int*)(*(int*)(a5 + a2) + a6), *a1);
	else
		(*(int(__thiscall*)(int, int))a3)(a4 + a2, *a1);
}

bool NumberType::stub16(void* a1, void* a2) const
{
	return *(float*)a1 != *(float*)a2;
}

void NumberType::stub17(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
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

	DataType::stub17(operation, outopid, outoprestype, a4);
}

void NumberType::stub18(int operationId, void* params) const
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
		*(float*)params = stub15((float*)params + 2, (float*)params + 1);
		break;
	case 7:
		*(float*)params = stub16((float*)params + 2, (float*)params + 1);
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

bool NumberType::stub20(void* a1) const
{
	return !isinf(*(float*)a1);
}