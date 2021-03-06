#pragma once

#include "ResourcesTypes.h"
#include "Config.h"
#include "Entity.h"

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

struct FastFindInfo
{
	unsigned int	m_NodeNameCRC;	//	NOTE: CRC for Node name OR model name (if it's a model).
	unsigned int	m_Index;	//	NOTE: maybe?
	class Node*		m_Node;
};

#define ASSETHEADERSTRUCT_SIZE 136

struct AssetHeaderStruct_t
{
	unsigned char m_AssetId[36];
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;

	struct AssetHeaderStruct_1
	{
		char m_OriginalKey[32];
		
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

		AssetHeaderStruct_1();	//	@401050
		void	_401450(char* key, char* keydata);	//	@401450
		void	_4010C0(const char* key);	//	@4010C0
		void	_4011A0(char* key);	//	@4011A0
	} field_38;
};

//	TODO: probably rename it to 'CurrentAssetBlock'.
class Blocks
{
	friend class Scene;
protected:
	unsigned char	field_0;
	BlockTypeNumber m_BlockType;
	char			field_8[256];
	int				field_108;
	Allocator*		m_Defragmentator;
	List<int>		m_UnkList_1;	//	NOTE: this seems to be related to Defragmentator object.
	List<FastFindInfo>	m_FastFindNodeVector;
	List<class Entity>		m_NodesList[6];
public:
	List<ResType::Resource> m_ResourcesInstancesList;
protected:
	List<String>	m_SceneNames;
	int				m_NodesInNodeList[6];
	int				field_1C8;
	char			field_1CC;
	int*			field_1D0;
	int				m_EngineVersionTimestamp;
	int				m_RegionId;
	bool			m_CheckTimestamp;
	List<int>		m_AssetsList;
	bool			m_LoadBlocks;

private:
	void			AddTypesListItemAtPos(ResType::Resource* element, unsigned int index);	//	@8760C0
	unsigned int	_875570(unsigned int );	//	@875570

	static void		AllocateResourceBlockBufferAligned(unsigned int, int** resBufStartPos, int* resBufSpace, BlockTypeNumber);	//	@852070

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
	void			RemoveLastSceneName();	//	@875650
	int				GetFreeResourceTypeListItem(unsigned int index);	//	@875540
	unsigned int	AddEntity(class Entity* ent);	//	@875FA0	//	NOTE: returns index
	void			SetRegionId(signed int id);	//	@875434
	AllocatorIndex	GetAllocatorType() const;	//	@875360
	int				InsertTypeListItem(void* res);	//	@877A90
	void			GetFullResourcePath(String& outStr, const char* respath, const char* resext, ResType::PlatformId platform);	//	@8776B0
	void			GetInternalFileName(String& outName, const char* str);	//	@8773A0
	void			GetResourcePath(String& outStr, const char* path) const;	//	@875770
	void			IncreaseResourceReferenceCount(ResType::Resource*);	//	@875320
	void			DecreaseResourceReferenceCount(ResType::Resource*);	//	@875330
	const char*		GetCurrentSceneName() const;
	void			BuildFastFindNodeVector();	//	@877DA0
	void			FillFastFindNodeVector(Node* _baseNode, FastFindInfo* _ffi);	//	@877B00	//	NOTE: this goes through 'baseNode' children and fills vector.
	ResType::ResourceBase*	LoadResourceFile(const char* _pathandname);	//	@878AB0
	void*			LoadResourceBlock(class File*, void* resbufferptr, unsigned int* resdatasize, BlockTypeNumber resblockid);	//	@8759E0
	Entity*			_8755E0();	//	@8755E0
	Entity*			_875610(Entity*);	//	@875610
	int				GetRegionId() const;	//	@875440
	
	static void		GetPlatformSpecificResourcePath(String& outPath, const char* respath, GameConfig::CountryCodes region, ResType::PlatformId platform);	//	@876500
	static ResourceBlockTypeNumber GetResourceBlockTypeNumber(BlockTypeNumber resourceBlockId);	//	@851FE0

	static const char* BlockTypeExtension[];	//	@A11B64
	static bool		ChecksumChecked;	//	@A3D7C9
};

extern Blocks* g_Blocks;

static_assert(sizeof(Blocks) == BLOCKS_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Blocks));
static_assert(sizeof(AssetHeaderStruct_t) == ASSETHEADERSTRUCT_SIZE, MESSAGE_WRONG_CLASS_SIZE(AssetHeaderStruct_t));