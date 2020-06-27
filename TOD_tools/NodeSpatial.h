#pragma once

#include "stdafx.h"
#include "MemoryAllocators.h"
#include "StringsPool.h"
#include "Types.h"

#define NODE_SPATIAL_CLASS_SIZE 84

class Entity;

//	NOTE: actual parent of each script entity is 'GEKeyFrame'.
//	NOTE: field 0 is pointer to virtual table in exe, not used here for correctness.
class NodeSpatial
{
private:
	int		field_4;
	int		field_8;
	int		field_C;
	int		field_10;
	int		field_14;
	int		field_18;
	int		field_1C;
	int		field_20;
	int		field_24;
	int		field_28;
	int		field_2C;
	int		field_30;
	int		field_34;
	int		field_38;
	int		field_3C;
	int		field_40;
	int		field_44;
	int		field_48;
	int		field_4C;
	float	m_f50;

private:
	//	NOTE: vtable at 9CE368.
	virtual void	Destroy(bool aligned);	//	@86C6D0
	virtual int		_88F770();	//	@88F770
	virtual void	_484CC0(int unused);	//	@484CC0
	virtual Entity* FindNode(const char* szNodeName);	//	@88EED0
	virtual void*	_88EC20(int unk0);	//	@88EC20	//	NOTE: this could return AuxQuadTree.
	virtual void	RefreshQuadTree();	//	@88DE70
	virtual void	nullsub_0();	//	@8CB190
	virtual void	_88C300();	//	@88C300	//	NOTE: this justs sets byte at A3D890 to 1.
	virtual void	nullsub_1();	//	@8CB190
	virtual char	_484DB0(int unk0, int unk1);	//	@484DB0	//	NOTE: returns zero.
	virtual float	_8F8650(int unk0, int unk1);	//	@8F8650	//	NOTE: return float -1.
	virtual void	_88C600(int unk0);	//	@88C600
	virtual void	_88C610();	//	@88C610
	virtual void	nullsub_2(int unused);	//	@883EC0
	virtual void	nullsub_3();	//	@8CB190
	virtual void	nullsub_4(int unused);	//	@883EC0
	virtual String* ReturnEmptyString(String* str);	//	@484E80
	virtual Vector4f _892130(Vector4f* unk0);	//	@892130

public:
	NodeSpatial(unsigned int allocatorIndex);	//	@892160
};

static_assert(sizeof(NodeSpatial) == NODE_SPATIAL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(NodeSpatial));