#pragma once

#include "stdafx.h"

#include "Entity.h"
#include "List.h"
#include "StringsPool.h"
#include "MemoryAllocators.h"

#define BUILTIN_CLASS_SIZE 152

//	TODO: implementation!
void	Random__Init(int* seed);
int		Random__Integer(signed int maxvalue);
float	Random__Number();

enum eTypes {
	TYPE_NOTHING,	//	Nothing!
	TYPE_NUMBER,	//	Float
	TYPE_INTEGER,	//	Integer
	TYPE_STRING,	//	String
	TYPE_TRUTH,		//	Boolean
	TYPE_VECTOR,	//	Vector4 OR Vector3
	TYPE_QUATERNION,//	Quaternion
	TYPE_COLOR,		//	ColorRGB
	TYPE_LIST,		//	Actual list
	TYPE_DICT,		//	Dictionary
	TYPE_ENTITY,	//	Entity (base node class)
	TYPE_STRUCT,	//	Structure (?)
	TOTAL_TYPES
};

template <typename Out, typename InType, int size>
struct Handler_Params {
	Out	output;
	InType	input[size];
};

/*
 *	Used like this:
 *	Handler_Params<float, float, 1> args = {NULL, 90};
 *	Builtin::GetSin(&args);
 *	args contents are: { 1, 90 }. Handy!
 *	NOTE! Number of actual "inputs" are variadic, but output is always first element!
*/

struct Builtin_Handler
{
	String m_sProto;
	void *m_pFunction;
	String m_sName;
};

class Builtin : Entity
{
private:

	List<Builtin_Handler> m_MethodsList;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;

public:
	Builtin() {
		m_MethodsList = List<Builtin_Handler>();
		field_88 = 0;
		field_8C = 0;
		field_90 = 0;
		field_94 = 0;

		debug("Builtin created at %X\n", this);
	}

	~Builtin()
	{
		debug("Builtin destroyed!\n");
	}

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	//	MATH OPERATIONS
	static void Sin(Handler_Params<float, float, 1>& params) { params.output = (float)sin(params.input[0]); }
	static void Cos(Handler_Params<float, float, 1>& params) { params.output = (float)cos(params.input[0]); }
	static void Tan(Handler_Params<float, float, 1>& params) { params.output = (float)tan(params.input[0]); }
	static void Asin(Handler_Params<float, float, 1>& params) { params.output = (float)asin(params.input[0]); }
	static void Acos(Handler_Params<float, float, 1>& params) { params.output = (float)acos(params.input[0]); }
	static void Atan(Handler_Params<float, float, 1>& params) { params.output = (float)atan(params.input[0]); }
	static void Abs(Handler_Params<int, int, 1>& params) { params.output = params.input < 0 ? -params.input[0] : params.input[0]; }
	static void FAbs(Handler_Params<float, float, 1>& params) { params.output = (float)fabs(params.input[0]); }
	static void Sqrt(Handler_Params<float, float, 1>& params) { params.output = (float)sqrt(params.input[0]); }
	static void Floor(Handler_Params<float, float, 1>& params) { params.output = (float)floor(params.input[0]); }
	static void Ceil(Handler_Params<float, float, 1>& params) { params.output = (float)ceil(params.input[0]); }
	static void Clamp(Handler_Params<float, float, 2>& params) {
		if (params.input[1] <= params.input[0])
			if (params.input[0] <= params.input[2])
				params.output = params.input[0];
			else
				params.output = params.input[2];
		else
			params.output = params.input[1];
	}
	static void Testbits(Handler_Params<bool, int, 1>& params) { params.output = (params.input[0] & params.input[1]) != 0; }
	static void SetBit(Handler_Params<int, int, 1>& params) { params.output = params.input[0] | (1 << params.input[1]); }
	static void GetBit(Handler_Params<bool, int, 1>& params) { params.output = (params.input[0] & (1 << params.input[1])) != 0; }
	static void Rand_Seed(Handler_Params<int, int, 1>& params) { Random__Init(&params.input[0]); }
	static void Rand_Integer(Handler_Params<int, int, 1>& params) { params.output = Random__Integer(params.input[0]); }
	static void Rand_Number(Handler_Params<float, int, 1>& params) { params.output = Random__Number(); }

	static const Vector4f& m_RightVector;
	static const Vector4f& m_UpVector;
	static const Vector4f& m_InVector;
	static const Quaternion<float>& m_Orientation;
	static const Vector4f& m_LeftVector;
	static const Vector4f& m_DownVector;
	static const Vector4f& m_OutVector;
	static const Vector4f& m_vUnkColor;	//	NOTE: not a color!
};

extern Builtin* g_Builtin;

static_assert(sizeof(Builtin) == BUILTIN_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Builtin"));