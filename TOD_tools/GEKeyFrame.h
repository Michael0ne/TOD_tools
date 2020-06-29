#pragma once

#include "stdafx.h"
#include "Types.h"
#include "StringsPool.h"

#define GEKEYFRAME_CLASS_SIZE 140

class AuxQuadTree;
class NodePosition;
class Fragment;
class Entity;

//	NOTE: inherited from EntityType.
class GEKeyFrame
{
private:
	int field_4;
	int field_8;
	int field_C;
	short field_10;
	short field_12;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	void* lpPositionVtbl;
	int field_28;
	int field_2C;
	AuxQuadTree* m_pQuadTree;
	int field_34;
	int field_38;
	NodePosition* m_pPosition;
	int field_40;
	int field_44;
	Fragment* m_pFragment;
	int field_4C;
	float m_fKeyTranslationX;
	float m_fKeyTranslationY;
	float m_fKeyTranslationZ;
	float m_f5C;
	float m_fKeyScaleX;
	float m_fKeyScaleY;
	float m_fKeyScaleZ;
	float m_f6C;
	float m_fKeyRotationX;
	float m_fKeyRotationY;
	float m_fKeyRotationZ;
	float m_f7C;
	float m_fKeyTime;
	float m_fKeyOpacity;
	float m_fKeyTextureScroll;

	//	NOTE: these methods should not be here, because this class does not override any of them from EntityType.
	virtual GEKeyFrame* destructor_scalar(bool freememory);	//	@86C6D0
	virtual void _88F770();	//	@88F770
	virtual void _484CC0(int);	//	@484CC0
	virtual Entity* _88EED0(const char* arg0);	//	@88EED0
	virtual void _88EC20(int);	//	@88EC20
	virtual void _88DE70();	//	@88DE70
	virtual void nullsub_1();
	virtual void _88C300();	//	@88C300
	virtual void nullsub_2();
	virtual char _484DB0(int, int);	//	@484DB0
	virtual float _8F8650(int, int);	//	@8F8650
	virtual void nullsub_3(int);
	virtual void _88C610();	//	@88C610	//	NOTE: thunk function
	virtual void nullsub_4(int);
	virtual void nullsub_5();
	virtual void nullsub_6(int);
	virtual String* _484E80(String* outString);	//	@484E80
	virtual Vector4f* _484DC0(Vector4f* outVec);	//	@484DC0
public:
	GEKeyFrame(char allocationBit);	//	@88D4B0
};

static_assert(sizeof(GEKeyFrame) == GEKEYFRAME_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GEKeyFrame));