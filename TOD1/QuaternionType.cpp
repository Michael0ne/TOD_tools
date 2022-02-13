#include "QuaternionType.h"
#include "BuiltinType.h"
#include "TruthType.h"
#include "VectorType.h"
#include "NumberType.h"
#include <directxmath/include/DirectXMath.h>

QuaternionType::QuaternionType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize) : DataType(typeId, typeName, typeSize)
{
    MESSAGE_CLASS_CREATED(QuaternionType);
}

QuaternionType::~QuaternionType()
{
    MESSAGE_CLASS_DESTROYED(QuaternionType);
}

void* QuaternionType::ReturnNew(void* a1) const
{
    *(Orientation*)a1 = BuiltinType::Orient;
    return a1;
}

String& QuaternionType::PrintFormattedValue(String& outstr, void* val, int precision) const
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

int QuaternionType::StrToType(char* a1, void* a2) const
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

void QuaternionType::stub13(int a1, int(__thiscall* procptr)(void*, void*), int a3, int a4, int a5, void* const outResult) const
{
    int* retval = nullptr;

    if (a5)
        retval = (int*)procptr((void*)(a1 + a3 + a4 + *(int*)(*(int*)(a1 + a4))), nullptr);
    else
        retval = (int*)procptr((void*)(a1 + a3), nullptr);

    *(int*)outResult = *retval;
    *((int*)outResult + 1) = retval[1];
    *((int*)outResult + 2) = retval[2];
    *((int*)outResult + 3) = retval[3];
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

bool QuaternionType::NotEqualTo(void* a1, void* a2) const
{
    float a = 0.f;
    float b = 0.f;

    float ab_w = fabsf(*(float*)a1 - *(float*)a2);
    float ab_x = fabsf(*((float*)a1 + 1) - *((float*)a2 + 1));
    float ab_y = fabsf(*((float*)a1 + 2) - *((float*)a2 + 2));
    float ab_z = fabsf(*((float*)a1 + 3) - *((float*)a2 + 3));

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

    // FIXME: value @A3A064 is never initialized!
    return _A3A064 < a;
}

void QuaternionType::ParseOperationString(const char* const operation, int* outopid, DataType** outoprestype, char* a4) const
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
void QuaternionType::PerformOperation(int operationId, void* params) const
{
    switch (operationId)
    {
    case 0:
    {
        Orientation* firstvec = (Orientation*)((float*)params + tQUATERNION->m_Size);
        Orientation* secondvec = (Orientation*)((float*)params + (tQUATERNION->m_Size * 2));

        *((Orientation*)params) =
        {
         (secondvec->z * firstvec->x) + ((firstvec->y * secondvec->x) - (secondvec->y * firstvec->x)),
         (firstvec->x * secondvec->z) + ((secondvec->x * firstvec->z) + ((secondvec->y * firstvec->w) - (firstvec->y * secondvec->w))),
         ((firstvec->y * secondvec->z) + ((secondvec->y * firstvec->z) + ((firstvec->x * secondvec->w) - (secondvec->x * firstvec->w)))),
         ((firstvec->z * secondvec->z) - (((firstvec->y * secondvec->y) + (firstvec->x * secondvec->x)) + (firstvec->w * secondvec->w)))
        };
    }
    break;
    case 1:
        *(bool*)params = AreEqual((int*)params + 5, (int*)params + 1);
        break;
    case 2:
        *(bool*)params = NotEqualTo((int*)params + 5, (int*)params + 1);
        break;
    case 3:
    {
        Orientation* secondvec = (Orientation*)((float*)params + tQUATERNION->m_Size);
        *((Orientation*)params) = { 0.f - secondvec->w, 0.f - secondvec->x, 0.f - secondvec->y, 0.f - secondvec->z };
    }
    break;
    case 4:
    {
        // NOTE: this was painful :( .
        Orientation* firstvec = (Orientation*)((float*)params + tVECTOR->m_Size);
        Orientation* secondvec = (Orientation*)((float*)params + tVECTOR->m_Size);
        float delim = 1.f / sqrtf((secondvec->w * secondvec->w + secondvec->z * secondvec->z) + (secondvec->y * secondvec->y + secondvec->x * secondvec->x));
        Orientation thirdvec =
        {
         ((secondvec->z * delim) * firstvec->w) + ((firstvec->y * (0.f - (secondvec->x * delim))) - ((0.f - (secondvec->y * delim)) * firstvec->x)),
         (firstvec->x * (secondvec->z * delim)) + ((0.f - (secondvec->y * delim) - (firstvec->y * (0.f - (secondvec->w * delim))))),
         (firstvec->y * (secondvec->z * delim)) + ((firstvec->x * (0.f - (secondvec->w * delim))) - (0.f - (secondvec->x * delim) * firstvec->w)),
         (0.f - ((firstvec->y * (0.f - (secondvec->y * delim)) + (firstvec->x * (0.f - (secondvec->x * delim)))) + (firstvec->w * (0.f - (secondvec->w * delim)))))
        };

        *(Vector3f*)params =
        {
         (thirdvec.z * secondvec->w) + ((secondvec->z * thirdvec.w) + ((thirdvec.x * secondvec->y) - (thirdvec.y * secondvec->x))),
         (thirdvec.z * secondvec->x) + ((thirdvec.x * secondvec->z) + ((thirdvec.y * secondvec->w) - (secondvec->y * thirdvec.w))),
         (thirdvec.z * secondvec->y) + ((thirdvec.y * secondvec->z) + ((secondvec->x * thirdvec.w) - (thirdvec.x * secondvec->w)))
        };
    }
    break;
    case 5:
    {
        Orientation* firstvec = (Orientation*)((float*)params + tNUMBER->m_Size);

        if (firstvec->z >= 1.f || firstvec->z <= -1.f)
            *(float*)params = 0.f;
        else
            *(float*)params = acosf(firstvec->z) * 2;
    }
    break;
    case 6:
    {
        Orientation* firstvec = (Orientation*)((float*)params + tVECTOR->m_Size);
        float inv = 1.f / sqrtf(1.f - firstvec->z * firstvec->z);

        if (firstvec->z >= 1.f || firstvec->z <= -1.f)
            *(Vector3f*)params = { 1.f, 0.f, 0.f };
        else
            *(Vector3f*)params = { firstvec->w * inv, firstvec->x * inv, firstvec->y * inv };
    }
    break;
    case 7:
    {
        Orientation* firstvec = (Orientation*)((float*)params + tNUMBER->m_Size);

        *(float*)params = sqrtf(firstvec->w * firstvec->w + firstvec->z * firstvec->z + firstvec->y * firstvec->y + firstvec->x * firstvec->x);
    }
    break;
    case 8:
    {
        DirectX::FXMVECTOR* firstvec = (DirectX::FXMVECTOR*)((float*)params + tNUMBER->m_Size + 2 * tQUATERNION->m_Size);
        DirectX::FXMVECTOR* secondvec = (DirectX::FXMVECTOR*)((float*)params + tQUATERNION->m_Size);
        float t = *((float*)params + tQUATERNION->m_Size * 2);
        DirectX::XMVECTOR outquat = DirectX::XMQuaternionSlerp(*firstvec, *secondvec, t);

        *(Orientation*)params = { outquat.m128_f32[0], outquat.m128_f32[1], outquat.m128_f32[2], outquat.m128_f32[3] };
    }
    break;
    case 9:
    {
        Orientation* secondvec = (Orientation*)((float*)params + tQUATERNION->m_Size);
        float delim = 1.f / sqrtf(secondvec->w * secondvec->w + secondvec->z * secondvec->z + secondvec->y * secondvec->y + secondvec->x * secondvec->x);

        *(Orientation*)params = { secondvec->w * delim, secondvec->x * delim, secondvec->y * delim, secondvec->z * delim };
    }
    break;
    case 10:
    {
        Orientation* firstvec = (Orientation*)((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size);
        float t = *((float*)params + tQUATERNION->m_Size) * -0.5f;
        float tcosf = cosf(t);
        float tsinf = sinf(t);
        float mul = fabsf(tsinf) >= std::numeric_limits<float>::epsilon() ? tsinf : 0.f;

        *(Orientation*)params =
        {
         mul * firstvec->w,
         mul * firstvec->x,
         mul * firstvec->y,
         fabsf(tcosf) >= std::numeric_limits<float>::epsilon() ? tcosf : 0.f
        };
    }
    break;
    case 11:
    {
        Orientation* firstvec = (Orientation*)((float*)params + tQUATERNION->m_Size);
        Orientation* secondvec = (Orientation*)((float*)params + tQUATERNION->m_Size + tVECTOR->m_Size);
        Orientation* thirdvec = (Orientation*)((float*)params + (tVECTOR->m_Size * 2) + tQUATERNION->m_Size);
        DirectX::XMMATRIX mat =
        {
         { firstvec->w, firstvec->x, firstvec->y, 0 },
         { secondvec->w, secondvec->x, secondvec->y, 0 },
         { thirdvec->w, thirdvec->x, thirdvec->y, 0 },
         { 0, 0, 0, 1 }
        };

        *(DirectX::XMVECTOR*)&params = DirectX::XMQuaternionRotationMatrix(mat);
    }
    break;
    case 12:
        *(Orientation*)params = { sinf(*((float*)params + tQUATERNION->m_Size) * -0.5f), 0.f, 0.f, cosf(*((float*)params + tQUATERNION->m_Size) * -0.5f) };
        break;
    case 13:
        *(Orientation*)params = { 0.f, sinf(*((float*)params + tQUATERNION->m_Size) * -0.5f), 0.f, cosf(*((float*)params + tQUATERNION->m_Size) * -0.5f) };
        break;
    case 14:
        *(Orientation*)params = { 0.f, 0.f, sinf(*((float*)params + tQUATERNION->m_Size) * -0.5f), cosf(*((float*)params + tQUATERNION->m_Size) * -0.5f) };
        break;
    case 15:
    {
        Orientation* firstvec = (Orientation*)((float*)params + tQUATERNION->m_Size);
        float delim = 1.f / sqrtf(firstvec->w * firstvec->w + firstvec->y * firstvec->y + firstvec->x * firstvec->x);

        if (delim * firstvec->x >= 0.99999899f || delim * firstvec->x <= -0.99999899f)
            *(Orientation*)params = BuiltinType::Orient;
        else
        {
            // TODO: ...
        }
    }
    break;
    case 16:
        // TODO: ...
        break;
    case 17:
        *(float*)params = *((float*)params + tNUMBER->m_Size + 3);
        break;
    case 18:
        *(Orientation*)params =
        {
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 1),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 2),
         *((float*)params + tQUATERNION->m_Size)
        };
        break;
    case 19:
        *(float*)params = *((float*)params + tNUMBER->m_Size);
        break;
    case 20:
        *(Orientation*)params =
        {
         *((float*)params + tQUATERNION->m_Size),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 1),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 2),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 3)
        };
        break;
    case 21:
        *(float*)params = *((float*)params + tNUMBER->m_Size + 1);
        break;
    case 22:
        *(Orientation*)params =
        {
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size),
         *((float*)params + tQUATERNION->m_Size),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 2),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 3)
        };
        break;
    case 23:
        *(float*)params = *((float*)params + tNUMBER->m_Size + 2);
        break;
    case 24:
        *(Orientation*)params =
        {
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 1),
         *((float*)params + tQUATERNION->m_Size),
         *((float*)params + tQUATERNION->m_Size + tNUMBER->m_Size + 3),
        };
        break;
    }
}

bool QuaternionType::IsValidValueForType(void* a1) const
{
    return (!isnan(*(float*)a1)) &&
        (!isnan(*((float*)a1 + 1))) &&
        (!isnan(*((float*)a1 + 2))) &&
        (!isnan(*((float*)a1 + 3)));
}