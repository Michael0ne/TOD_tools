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
	UNKNOWN = -1,
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
	unsigned char	field_0;
	BlockTypeNumber m_BlockType;
	unsigned char	field_8[255];
	int				field_108;
	Allocator*		field_10C;
	List<int>		m_UnkList_1;
	List<int>		m_UnkList_2;
	List<int>		m_UnkList_3;
	List<int>		m_UnkList_4;
	List<int>		m_UnkList_5;
	List<int>		m_UnkList_6;
	List<int>		m_UnkList_7;
	List<int>		m_UnkList_8;
public:
	List<class Resource> m_ResourceTypesList;
protected:
	List<String>	m_SceneNames;
	int				field_1B0[6];
	int				field_1C8;
	char			field_1CC;
	int				field_1D0;
	int				field_1D4;
	int				m_RegionId;
	char			field_1DC;
	List<int>		m_UnkList_9;
	bool			m_LoadBlocks;

private:
	void			AddTypesListItemAtPos(Resource* element, unsigned int index);	//	@8760C0

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