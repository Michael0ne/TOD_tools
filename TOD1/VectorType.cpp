#include "VectorType.h"
#include "BuiltinType.h"
#include "NumberType.h"
#include "TruthType.h"

VectorType::VectorType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
 MESSAGE_CLASS_CREATED(VectorType);
}

VectorType::~VectorType()
{
 MESSAGE_CLASS_DESTROYED(VectorType);
}

void* VectorType::stub3(void* a1) const
{
 *(Vector3f*)a1 = *(Vector3f*)&BuiltinType::ZeroVector;

 return a1;
}

String& VectorType::stub6(String& outstr, void* val, int precision) const
{
 char _outstr[32] = {};

 if (precision)
 {
  char formatstr[32] = {};

  sprintf(formatstr, "%%%d.%df %%%d.%df %%%d.%df", precision >> 16, (short)precision, precision >> 16, (short)precision, precision >> 16, (short)precision);
  sprintf(_outstr, formatstr, *(float*)val, *((float*)val + 1), *((float*)val + 2));

  outstr = _outstr;
  return outstr;
 }
 else
 {
  sprintf(_outstr, "%f %f %f", *(float*)val, *((float*)val + 1), *((float*)val + 2));

  outstr = _outstr;
  return outstr;
 }
}

int VectorType::stub7(char* a1, void* a2) const
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

 return digitsread + digitsread_2 + digitsread_3;
}

void VectorType::stub13(int a1, void* a2, int a3, int a4, int a5, void* const a6) const
{
 int* retval = nullptr;

 if (a5)
  retval = ((int* (__thiscall*)(int, int))a2)(a1 + a3 + a4 + *(int*)(*(int*)(a4 + a1) + a5), (int)a2);
 else
  retval = ((int* (__thiscall*)(int, int))a2)(a3 + a1, (int)a2);

 *(int*)a6 = *retval;
 *((int*)a6 + 1) = retval[1];
 *((int*)a6 + 2) = retval[2];
}

void VectorType::stub14(int* a1, int a2, void* a3, int a4, int a5, int a6) const
{
 int* _a3 = (int*)a3;
 int _a4 = a4;
 int _a5 = a5;
 int _a6 = a6;

 *(int**)&a3 = (int*)*a1;
 a4 = a1[1];
 a5 = a1[2];
 a6 = 0;

 if (a6)
  ((void(__thiscall*)(int, int**))_a3)(_a4 + a2, (int**)&a3);
 else
  ((void(__thiscall*)(int, int**))_a3)(_a4 + a2, (int**)&a3);
}

bool VectorType::stub16(void* a1, void* a2) const
{
 return (*(float*)a1 != *(float*)a2) ||
  (*((float*)a1 + 1) != *((float*)a2 + 1)) ||
  (*((float*)a1 + 2) != *((float*)a2 + 2));
}

void VectorType::stub17(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
{
 *a4 = 0;

 if (strcmp(operation, "+(vector)") == NULL)
 {
  *outopid = 0;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "-(vector)") == NULL)
 {
  *outopid = 1;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "*(vector)") == NULL)
 {
  *outopid = 2;
  *outoprestype = tNUMBER;
 }

 if (strcmp(operation, "*(number)") == NULL)
 {
  *outopid = 3;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "/(number)") == NULL)
 {
  *outopid = 4;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "^(vector)") == NULL)
 {
  *outopid = 5;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "==(vector)") == NULL)
 {
  *outopid = 6;
  *outoprestype = tTRUTH;
 }

 if (strcmp(operation, "!=(vector)") == NULL)
 {
  *outopid = 7;
  *outoprestype = tTRUTH;
 }

 if (strcmp(operation, "-") == NULL)
 {
  *outopid = 12;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "length") == NULL)
 {
  *outopid = 13;
  *outoprestype = tNUMBER;
 }

 if (strcmp(operation, "sqrlength") == NULL)
 {
  *outopid = 14;
  *outoprestype = tNUMBER;
 }

 if (strcmp(operation, "getnormalized") == NULL)
 {
  *outopid = 15;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "x") == NULL)
 {
  *outopid = 16;
  *outoprestype = tNUMBER;
 }

 if (strcmp(operation, "x(number)") == NULL)
 {
  *outopid = 17;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "y") == NULL)
 {
  *outopid = 18;
  *outoprestype = tNUMBER;
 }

 if (strcmp(operation, "y(number)") == NULL)
 {
  *outopid = 19;
  *outoprestype = tVECTOR;
 }

 if (strcmp(operation, "z") == NULL)
 {
  *outopid = 20;
  *outoprestype = tNUMBER;
 }

 if (strcmp(operation, "z(number)") == NULL)
 {
  *outopid = 21;
  *outoprestype = tVECTOR;
 }

 DataType::stub17(operation, outopid, outoprestype, a4);
}

void VectorType::stub18(int operationId, void* params) const
{
 switch (operationId)
 {
 case 0:
 {
  float* firstvec = &((float*)params)[tVECTOR->m_Size];
  float* secondvec = &((float*)params)[tVECTOR->m_Size + tVECTOR->m_Size];

  *(float*)params = *firstvec + *secondvec;
  *((float*)params + 1) = firstvec[1] + secondvec[1];
  *((float*)params + 2) = firstvec[2] + secondvec[2];
 }
 break;
 case 1:
 {
  float* firstvec = &((float*)params)[tVECTOR->m_Size];
  float* secondvec = &((float*)params)[tVECTOR->m_Size + tVECTOR->m_Size];

  *(float*)params = *secondvec - *firstvec;
  *((float*)params + 1) = secondvec[1] - firstvec[1];
  *((float*)params + 2) = secondvec[2] - firstvec[2];
 }
 break;
 case 2:
 {
  float* firstvec = &((float*)params)[tNUMBER->m_Size];
  float* secondvec = &((float*)params)[tNUMBER->m_Size + tVECTOR->m_Size];

  *(float*)params = (*firstvec * *secondvec) + (firstvec[2] * secondvec[2]) + (firstvec[1] * secondvec[1]);
 }
 break;
 case 3:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size + tNUMBER->m_Size];
  float mult = ((float*)params)[tVECTOR->m_Size];

  *(float*)params = *secondvec * mult;
  *((float*)params + 1) = secondvec[1] * mult;
  *((float*)params + 2) = secondvec[2] * mult;
 }
 break;
 case 4:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size + tNUMBER->m_Size];
  float delim = 1.f / *((float*)params + tVECTOR->m_Size);

  *(float*)params = delim * *secondvec;
  *((float*)params + 1) = delim * secondvec[1];
  *((float*)params + 2) = delim * secondvec[2];
 }
 break;
 case 5:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size];
  float* thirdvec = &((float*)params)[tVECTOR->m_Size * 2];

  *(float*)params = (secondvec[2] * thirdvec[1]) - (thirdvec[2] * secondvec[1]);
  *((float*)params + 1) = (thirdvec[2] * *secondvec) - (secondvec[2] * *thirdvec);
  *((float*)params + 2) = (secondvec[1] * *thirdvec) - (thirdvec[1] * *secondvec);
 }
 break;
 case 6:
  *(bool*)params = stub15((float*)params + 4, (float*)params + 1);
  break;
 case 7:
  *(bool*)params = stub16((float*)params + 4, (float*)params + 1);
  break;
 case 12:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size];

  *(float*)params = 0.f - *secondvec;
  *((float*)params + 1) = 0.f - secondvec[1];
  *((float*)params + 2) = 0.f - secondvec[2];
 }
 break;
 case 13:
 {
  float* secondvec = &((float*)params)[tNUMBER->m_Size];
  *(float*)params = sqrtf(*secondvec * *secondvec + secondvec[2] * secondvec[2] + secondvec[1] * secondvec[1]);
 }
 break;
 case 14:
 {
  float* secondvec = &((float*)params)[tNUMBER->m_Size];
  *(float*)params = ((*secondvec * *secondvec) + (secondvec[2] * secondvec[2])) + (secondvec[1] * secondvec[1]);
 }
 break;
 case 15:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size];

  if (((*secondvec * *secondvec) + (secondvec[1] * secondvec[1])) + (secondvec[2] * secondvec[2]) == 0.f)
  {
   *(float*)params = *secondvec;
   *((float*)params + 1) = secondvec[1];
   *((float*)params + 2) = secondvec[2];
  }
  else
  {
   float inv = 1.f / sqrtf(((*secondvec * *secondvec) + (secondvec[1] * secondvec[1])) + (secondvec[2] * secondvec[2]));
   *(float*)params = *secondvec / inv;
   *((float*)params + 1) = secondvec[1] / inv;
   *((float*)params + 2) = secondvec[2] / inv;
  }
 }
 break;
 case 16:
  *(float*)params = ((float*)params)[tNUMBER->m_Size];
  break;
 case 17:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size + tNUMBER->m_Size];
  *secondvec = ((float*)params)[tVECTOR->m_Size];

  *(float*)params = *secondvec;
  *((float*)params + 1) = secondvec[1];
  *((float*)params + 2) = secondvec[2];
 }
 break;
 case 18:
  *(float*)params = ((float*)params)[tNUMBER->m_Size + 1];
  break;
 case 19:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size + tNUMBER->m_Size];
  secondvec[1] = ((float*)params)[tVECTOR->m_Size];

  *(float*)params = *secondvec;
  *((float*)params + 1) = secondvec[1];
  *((float*)params + 2) = secondvec[2];
 }
 break;
 case 20:
  *(float*)params = ((float*)params)[tNUMBER->m_Size + 2];
  break;
 case 21:
 {
  float* secondvec = &((float*)params)[tVECTOR->m_Size + tNUMBER->m_Size];
  secondvec[2] = ((float*)params)[tVECTOR->m_Size];

  *(float*)params = *secondvec;
  *((float*)params + 1) = secondvec[1];
  *((float*)params + 2) = secondvec[2];
 }
 break;
 default:
  break;
 }
}

bool VectorType::stub20(void* a1) const
{
 return (!isnan(*(float*)a1)) &&
  (!isnan(*((float*)a1 + 1))) &&
  (!isnan(*((float*)a1 + 2)));
}