#pragma once

#include "List.h"

#define BLOCKS_CLASS_SIZE 500

enum ResourceBlockTypeNumber
{
	RESTYPE_NONE = 0,
	RESTYPE_MAP = 1,
	RESTYPE_SUBMAP = 1,
	RESTYPE_MISSION = 2,
	RESTYPE_CUTSCENE = 3,
	RESTYPE_PLAYERDATA = 4
};

enum BlockTypeNumber
{
	NONE = 0,
	MAP = 1,
	SUBMAP = 2,
	MISSION = 3,
	CUTSCENE = 4,
	PLAYERDATA = 5,
	MAIN = 6
};

class Blocks
{
protected:
	int field_0;
	BlockTypeNumber m_nBlockType;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	int field_A8;
	int field_AC;
	int field_B0;
	int field_B4;
	int field_B8;
	int field_BC;
	int field_C0;
	int field_C4;
	int field_C8;
	int field_CC;
	int field_D0;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;
	int field_100;
	int field_104;
	int field_108;
	int field_10C;
	int field_110;
	int field_114;
	int field_118;
	int field_11C;
	int field_120;
	int field_124;
	int field_128;
	int field_12C;
	List<int> m_UnkList_1;
	List<int> m_UnkList_2;
	List<int> m_UnkList_3;
	List<int> m_UnkList_4;
	List<int> m_UnkList_5;
	List<int> m_UnkList_6;
public:
	List<class Resource> m_ResourceTypesList;
protected:
	List<String>	m_SceneNames;
	int field_1B0;
	int field_1B4;
	int field_1B8;
	int field_1BC;
	int field_1C0;
	int field_1C4;
	int field_1C8;
	int field_1CC;
	int field_1D0;
	int field_1D4;
	signed int m_nRegionId;
	int field_1DC;
	int field_1E0;
	int field_1E4;
	int field_1E8;
	int field_1EC;
	char m_bLoad;
	char field_1F1;
	char field_1F2;
	char field_1F3;

public:
	Blocks(bool loadBlocks);	//	@876E20
	~Blocks();	//	@877250

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void			SetSceneName(const char* szSceneName);	//	@877F40
	int				GetFreeResourceTypeListItem(unsigned int index);	//	@875540
	unsigned int	AddEntity(class Entity* ent);	//	@875FA0	//	NOTE: returns index
	void			SetRegionId(signed int id);	//	@875434
	signed int		GetAllocatorType();	//	@875360
	int				InsertTypeListItem(void* res);	//	@877A90
	void			GetInternalFileName(String& outName, const char* str);	//	@8773A0

	static ResourceBlockTypeNumber GetResourceBlockTypeNumber(BlockTypeNumber resourceBlockId);	//	@851FE0

	static const char* BlockTypeExtension[];	//	@A11B64
};

extern Blocks* g_Blocks;

static_assert(sizeof(Blocks) == BLOCKS_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Blocks));