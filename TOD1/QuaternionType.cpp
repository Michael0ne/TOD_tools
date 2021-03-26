#include "QuaternionType.h"
#include "BuiltinType.h"
#include "TruthType.h"
#include "VectorType.h"
#include "NumberType.h"

QuaternionType::QuaternionType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : BaseType(typeId, typeName, typeSize)
{
	MESSAGE_CLASS_CREATED(QuaternionType);
}

QuaternionType::~QuaternionType()
{
	MESSAGE_CLASS_DESTROYED(QuaternionType);
}

void* QuaternionType::stub3(void* a1) const
{
	*(Orientation*)a1 = BuiltinType::Orient;
	return a1;
}

String& QuaternionType::stub6(String& outstr, void* val, int precision) const
{
	char _outstr[16] = {};

	if (precision)
	{
		char formatstr[16] = {};

		sprintf(formatstr, "%%%d.%df %%%d.%df %%%d.%df %%%d.%df", precision >> 16, (short)precision, precision >> 16, (short)precision, precision >> 16, (short)precision, precision >> 16, (short)precision);
		sprintf(_outstr, formatstr, *((float*)val + 3), *(float*)val, *((float*)val + 1), *((float*)val + 2));

		outstr = _outstr;
		return outstr;
	}
	else
	{
		sprintf(_outstr, "%f %f %f %f", *((float*)val + 3), *(float*)val, *((float*)val + 1), *((float*)val + 2));

		outstr = _outstr;
		return outstr;
	}
}

int QuaternionType::stub7(char* a1, void* a2) const
{
	int digitsread = ParseFloatNumberString(a1, (float*)a2);
	if (digitsread < 0)
		return -1;

	if (!isspace(a1[digitsread]))
		return -1;

	for (char ch = a1[digitsread]; isspace(ch); ++digitsread);

	int digitsread_2 = ParseFloatNumberString(&a1[digitsread], (float*)a2 + 1);
	if (digitsread_2 < 0)
		return -1;

	if (!isspace(a1[digitsread + digitsread_2]))
		return -1;

	for (char ch = a1[digitsread + digitsread_2]; isspace(ch); ++digitsread_2);

	int digitsread_3 = ParseFloatNumberString(&a1[digitsread + digitsread_2], (float*)a2 + 2);
	if (digitsread_3 < 0)
		return -1;

	if (!isspace(a1[digitsread + digitsread_2 + digitsread_3]))
		return -1;

	for (char ch = a1[digitsread + digitsread_2 + digitsread_3]; isspace(ch); ++digitsread_3);

	int digitsread_4 = ParseFloatNumberString(&a1[digitsread + digitsread_2 + digitsread_3], (float*)a2 + 3);
	if (digitsread_4 < 0)
		return -1;

	for (char ch = a1[digitsread + digitsread_2 + digitsread_3 + digitsread_4]; isspace(ch); ++digitsread_4);

	return digitsread + digitsread_2 + digitsread_3 + digitsread_4;
}

void QuaternionType::stub13(int a1, void* a2, int a3, int a4, int a5, void* const a6) const
{
	int* retval = nullptr;

	if (a5)
		retval = ((int* (__thiscall*)(int, int))a2)(a1 + a3 + a4 + *(int*)(*(int*)(a4 + a1) + a5), (int)a2);
	else
		retval = ((int* (__thiscall*)(int, int))a2)(a3 + a1, (int)a2);

	*(int*)a6 = *retval;
	*((int*)a6 + 1) = retval[1];
	*((int*)a6 + 2) = retval[2];
	*((int*)a6 + 3) = retval[3];
}

void QuaternionType::stub14(int* a1, int a2, void* a3, int a4, int a5, int a6) const
{
	int* _a3 = (int*)a3;
	int _a4 = a4;
	int _a5 = a5;
	int _a6 = a6;

	*(int**)&a3 = (int*)*a1;
	a4 = a1[1];
	a5 = a1[2];
	a6 = a1[3];

	if (a6)
		((void(__thiscall*)(int, int**))_a3)(a2 + _a4 + _a5 + *(int*)(*(int*)(_a5 + a2) + _a6), (int**)&a3);
	else
		((void(__thiscall*)(int, int**))_a3)(_a4 + a2, (int**)&a3);
}

bool QuaternionType::stub16(void* a1, void* a2) const
{
	float a = 0.f;
	float b = 0.f;

	float ab_w = fabs(*(float*)a1 - *(float*)a2);
	float ab_x = fabs(*((float*)a1 + 1) - *((float*)a2 + 1));
	float ab_y = fabs(*((float*)a1 + 2) - *((float*)a2 + 2));
	float ab_z = fabs(*((float*)a1 + 3) - *((float*)a2 + 3));

	if (ab_w >= 0.f)
	{
		a = ab_w;
		b = ab_w;
	}

	if (b <= ab_x)
	{
		a = ab_x;
		b = ab_x;
	}

	if (b <= ab_y)
	{
		a = ab_y;
		b = ab_y;
	}

	if (b <= ab_z)
		a = ab_z;

	//	FIXME: value @A3A064 is never initialized!
	return *(float*)0xA3A064 < a;
}

void QuaternionType::stub17(const char* const operation, int* outopid, BaseType** outoprestype, char* a4) const
{
	*a4 = 0;

	if (strcmp(operation, "*(quaternion)") == NULL)
	{
		*outopid = 0;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "==(quaternion)") == NULL)
	{
		*outopid = 1;
		*outoprestype = tTRUTH;
	}

	if (strcmp(operation, "!=(quaternion)") == NULL)
	{
		*outopid = 2;
		*outoprestype = tTRUTH;
	}

	if (strcmp(operation, "!") == NULL)
	{
		*outopid = 3;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "applytopoint(vector)") == NULL)
	{
		*outopid = 4;
		*outoprestype = tVECTOR;
	}

	if (strcmp(operation, "getangle") == NULL)
	{
		*outopid = 5;
		*outoprestype = tNUMBER;
	}

	if (strcmp(operation, "getaxis") == NULL)
	{
		*outopid = 6;
		*outoprestype = tVECTOR;
	}

	if (strcmp(operation, "length") == NULL)
	{
		*outopid = 7;
		*outoprestype = tNUMBER;
	}

	if (strcmp(operation, "interpolate(quaternion,number)") == NULL)
	{
		*outopid = 8;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "getnormalized") == NULL)
	{
		*outopid = 9;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "createangleaxis(number,vector)") == NULL)
	{
		*outopid = 10;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "createbaseaxis(vector,vector,vector)") == NULL)
	{
		*outopid = 11;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "rotatex(number)") == NULL)
	{
		*outopid = 12;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "rotatey(number)") == NULL)
	{
		*outopid = 13;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "rotatez(number)") == NULL)
	{
		*outopid = 14;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "rotateyintovector(vector)") == NULL)
	{
		*outopid = 15;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "rotatezintovector(vector)") == NULL)
	{
		*outopid = 16;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "w") == NULL)
	{
		*outopid = 17;
		*outoprestype = tNUMBER;
	}

	if (strcmp(operation, "w(number)") == NULL)
	{
		*outopid = 18;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "x") == NULL)
	{
		*outopid = 19;
		*outoprestype = tNUMBER;
	}

	if (strcmp(operation, "x(number)") == NULL)
	{
		*outopid = 20;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "y") == NULL)
	{
		*outopid = 21;
		*outoprestype = tNUMBER;
	}

	if (strcmp(operation, "y(number)") == NULL)
	{
		*outopid = 22;
		*outoprestype = tQUATERNION;
	}

	if (strcmp(operation, "z") == NULL)
	{
		*outopid = 23;
		*outoprestype = tNUMBER;
	}

	if (strcmp(operation, "z(number)") == NULL)
	{
		*outopid = 24;
		*outoprestype = tQUATERNION;
	}
}

#pragma message(TODO_IMPLEMENTATION)
void QuaternionType::stub18(int operationId, void* params) const
{
	//	TODO: a lot of quaternions maths here, probably do it later.
	switch (operationId)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	case 11:
		break;
	case 12:
		break;
	case 13:
		break;
	case 14:
		break;
	case 15:
		break;
	case 16:
		break;
	case 17:
		break;
	case 18:
		break;
	case 19:
		break;
	case 20:
		break;
	case 21:
		break;
	case 22:
		break;
	case 23:
		break;

	}
}

bool QuaternionType::stub20(void* a1) const
{
	return	(!isnan(*(float*)a1)) &&
			(!isnan(*((float*)a1 + 1))) &&
			(!isnan(*((float*)a1 + 2))) &&
			(!isnan(*((float*)a1 + 3)));
}