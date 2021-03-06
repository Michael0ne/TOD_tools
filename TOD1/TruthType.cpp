#include "TruthType.h"

TruthType::TruthType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : BaseType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(TruthType);
}

TruthType::~TruthType()
{
	MESSAGE_CLASS_DESTROYED(TruthType);
}

void* TruthType::stub3(void* a1) const
{
	*(int*)a1 = 0;
	return a1;
}

String& TruthType::stub6(String& outstr, void* a2, int) const
{
	outstr = *(bool*)a2 ? "true" : "false";
	return outstr;
}

int TruthType::stub7(char* a1, void* a2) const
{
	if (strcmp(a1, "true") == NULL)
	{
		*(bool*)a2 = true;
		return 4;
	}

	if (strcmp(a1, "false") == NULL ||
		strcmp(a1, "0") == NULL)
	{
		*(bool*)a2 = false;
		return 5;
	}

	return -1;
}

void TruthType::stub13(int a1, void* a2, int a3, int a4, int a5, void* const a6) const
{
	if (a5)
		*(char*)a6 = ((char (__thiscall*)(int))a2)(a1 + a3 + a4 + *(int*)(*(int*)(a4 + a1) + a5));
	else
		*(char*)a6 = ((char (__thiscall*)(int))a2)(a3 + a1);
}

void TruthType::stub14(int* a1, int a2, void* a3, int a4, int a5, int a6) const
{
	if (a6)
		((void(__thiscall*)(int, int))a3)(a2 + a4 + a5 + *(int*)(*(int*)(a5 + a2) + a6), *a1 != 0);
	else
		((void(__thiscall*)(int, int))a3)(a4 + a2, *a1 != 0);
}

void TruthType::stub17(const char* const operation, int* outopid, BaseType** outoprestype, char* a4) const
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

	BaseType::stub17(operation, outopid, outoprestype, a4);
}

void TruthType::stub18(int operationId, void* params) const
{
	switch (operationId)
	{
	case 0:
		*(bool*)params = stub15((bool*)params + 2, (bool*)params + 1);
		break;
	case 1:
		*(bool*)params = stub16((bool*)params + 2, (bool*)params + 1);
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