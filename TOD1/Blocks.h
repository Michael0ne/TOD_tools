#pragma once
#include "ResourcesTypes.h"
#include "Config.h"
#include <vector>

struct FastFindInfo
{
	unsigned int	m_NodeNameCRC;	//	NOTE: CRC for Node name OR model name (if it's a model).
	unsigned int	m_Index;	//	NOTE: maybe?
	class Node*		m_Node;
};

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
	ResType::BlockTypeNumber m_BlockType;
	char			field_8[256];
	int				field_108;
	Allocator*		m_Defragmentator;
	std::vector<int>m_DefragmentatorList;
	std::vector<FastFindInfo>	m_FastFindNodeVector;
	std::vector<class Entity*>	m_NodesList[6];
public:
	std::vector<ResType::Resource*>	m_ResourcesInstancesList;
protected:
	std::vector<String>	m_SceneNames;
	int				m_NodesInNodeList[6];
	int				field_1C8;
	char			field_1CC;
	int*			field_1D0;
	int				m_EngineVersionTimestamp;
	int				m_RegionId;
	bool			m_CheckTimestamp;
	std::vector<int>	m_AssetsList;
	bool			m_LoadBlocks;

private:
	void			AddTypesListItemAtPos(ResType::Resource* element, unsigned int index);	//	@8760C0
	unsigned int	_875570(unsigned int );	//	@875570

public:
	Blocks(bool loadBlocks);	//	@876E20
	~Blocks();	//	@877250

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void* operator new[](size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
	void operator delete[](void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}

	void			SetSceneName(const char* scenename);	//	@877F40
	void			RemoveLastSceneName();	//	@875650
	unsigned int	GetFreeResourceTypeListItem(unsigned int index);	//	@875540
	unsigned int	AddEntity(class Entity* ent);	//	@875FA0	//	NOTE: returns index
	void			SetRegion(signed int id);	//	@875434
	AllocatorIndex	GetAllocatorType() const;	//	@875360
	int				InsertTypeListItem(void* res);	//	@877A90
	void			GetPlatformSpecificPath(String& outStr, const char* respath, const char* resext, ResType::PlatformId platform);	//	@8776B0
	const char*		GetResourcePathSceneRelative(const char* const path);	//	@8773A0
	void			GetResourcePath(String& outStr, const char* path) const;	//	@875770
	void			IncreaseResourceReferenceCount(ResType::Resource*);	//	@875320
	void			DecreaseResourceReferenceCount(ResType::Resource*);	//	@875330
	const char*		GetCurrentSceneName() const;
	void			BuildFastFindNodeVector();	//	@877DA0
	void			FillFastFindNodeVector(Node* _baseNode, FastFindInfo* _ffi);	//	@877B00	//	NOTE: this goes through 'baseNode' children and fills vector.
	ResType::ResourceBase*	LoadResourceFile(const char* _pathandname);	//	@878AB0
	void*			LoadResourceBlock(class File*, int* resbufferptr, unsigned int* resdatasize, ResType::BlockTypeNumber resblockid);	//	@8759E0
	Entity*			_8755E0();	//	@8755E0
	Entity*			_875610(Entity*);	//	@875610
	int				GetRegion() const;	//	@875440
	String&			GetDataPath(String& outstr) const;	//	@8764E0
	ResType::Resource*	FindFirstFreeResource() const;	//	@879E00
	
	enum RegionCode
	{
		REGION_NOT_SET = -1,
		REGION_EUROPE = 0,
		REGION_USA = 1,
		REGION_ASIA = 2
	};

	static void		CorrectTextureResourcePath(String& outPath, const char* respath, RegionCode region, ResType::PlatformId platform);	//	@876500
	static RegionCode	GetRegionId(const String& region);	//	@875450

	static bool		ChecksumChecked;	//	@A3D7C9
};

extern Blocks* g_Blocks;

ASSERT_CLASS_SIZE(Blocks, 500);
ASSERT_CLASS_SIZE(AssetHeaderStruct_t, 136);