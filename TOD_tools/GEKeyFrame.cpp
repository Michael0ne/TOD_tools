#include "GEKeyFrame.h"

void GEKeyFrame::_484CC0(int)
{
	field_10 = field_10 | 10;
}

void GEKeyFrame::_88C300()
{
	*(char*)0xA3D890 = (char)1;
}

float GEKeyFrame::_8F8650(int, int)
{
	return -1.f;
}

Vector4f* GEKeyFrame::_484DC0(Vector4f* outVec)
{
	outVec->x = 0.f;
	outVec->y = 0.f;
	outVec->z = 0.f;
	outVec->a = 0.f;

	return outVec;
}

GEKeyFrame::GEKeyFrame(char allocationBit)
{
	MESSAGE_CLASS_CREATED(GEKeyFrame);

	field_4C = 0;
	field_40 = 0;
	field_34 = 0;
	field_44 = 0;
	field_38 = 0;
	field_12 = 0;
	field_28 = field_28 & 0xC000F000 | 40000000;
	m_pFragment = nullptr;

	if (allocationBit & 1)
		//	TODO: Create 'Position' object.
		;
	else
		m_pPosition = nullptr;

	if (allocationBit & 2)
		//	TODO: Create 'QuadTree' object.
		;
	else
		m_pQuadTree = nullptr;

	if (allocationBit & 4) {
		field_28 = field_28 | 0x80000000;
		//	TODO: Create 'Fragment' object.
	}else{
		field_28 = field_28 & 0x7FFFFFFF;
		m_pFragment = nullptr;
	}

	field_2C = field_2C | 0xFFFF;
	field_28 = field_28 | 0xF000;
	*(short*)(&field_2C + 2) = -1;
}