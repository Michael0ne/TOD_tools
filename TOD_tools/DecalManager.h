#pragma once

#include "stdafx.h"
#include "Types.h"

#define DECAL_MANAGER_CLASS_SIZE 100

class DecalManager
{
private:
	void* lpVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	void* lpPositionVtbl;
	int field_28;
	int field_2C;
	int* field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	int field_50;
	int* m_szTexture;
	int field_58;
	char m_nBufferSize;
	char m_nPerFrameFillSize;
	__int16 field_5E;
	int* field_60;

public:
	void		SetDecal(float* params);	//	@8D3D80
};