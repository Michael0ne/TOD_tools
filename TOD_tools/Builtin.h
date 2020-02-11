#pragma once

#include "stdafx.h"

#include "Node.h"
#include "List.h"
#include "KapowStringsPool.h"

//	TODO: implementation!
void Random__Init(int* seed);
int Random__Integer(signed int maxvalue);
float Random__Number();

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

class Builtin : Node
{
private:

	List<Builtin_Handler> m_MethodsList;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;

private:
	Builtin() {}

public:
	Builtin(const Builtin&) = delete;

	static Builtin& Get() {
		static Builtin instance;

		return instance;
	}

public:
	//	Actual class methods.

	//	MATH OPERATIONS
	static void Sin(Handler_Params<float, float, 1>& params) { params.output = sin(params.input[0]); }
	static void Cos(Handler_Params<float, float, 1>& params) { params.output = cos(params.input[0]); }
	static void Tan(Handler_Params<float, float, 1>& params) { params.output = tan(params.input[0]); }
	static void Asin(Handler_Params<float, float, 1>& params) { params.output = asin(params.input[0]); }
	static void Acos(Handler_Params<float, float, 1>& params) { params.output = acos(params.input[0]); }
	static void Atan(Handler_Params<float, float, 1>& params) { params.output = atan(params.input[0]); }
	static void Abs(Handler_Params<int, int, 1>& params) { params.output = params.input < 0 ? -params.input[0] : params.input[0]; }
	static void FAbs(Handler_Params<float, float, 1>& params) { params.output = fabs(params.input[0]); }
	static void Sqrt(Handler_Params<float, float, 1>& params) { params.output = sqrt(params.input[0]); }
	static void Floor(Handler_Params<float, float, 1>& params) { params.output = floor(params.input[0]); }
	static void Ceil(Handler_Params<float, float, 1>& params) { params.output = ceil(params.input[0]); }
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
};

static_assert(sizeof(Builtin) == 0x98, MESSAGE_WRONG_CLASS_SIZE("Builtin"));