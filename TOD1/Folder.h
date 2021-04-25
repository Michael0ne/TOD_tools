#pragma once
#include "Node.h"
#include "AssetManager.h"

#define FOLDER_CLASS_SIZE 88
#define FOLDER_ASSETINFO_SIZE 24

struct AssetInfo
{
	struct ActualAssetInfo
	{
		void*		m_ResourceDataBufferPtr = nullptr;	//	NOTE: actual resource data read from file written here.
		void*		m_ResourceAllocatedAlignedBufferPtr = nullptr;	//	NOTE: pointer to buffer with resource data size written here.
		unsigned int m_ResourceDataBufferSize = NULL;
	};
	ActualAssetInfo	m_AssetInfo_Shared;
	ActualAssetInfo	m_AssetInfo_Localised;
	
	inline AssetInfo()	//	NOTE: constructor always inlined.
		: m_AssetInfo_Shared(), m_AssetInfo_Localised()
	{
		MESSAGE_CLASS_CREATED(AssetInfo);
	};
};

ASSERT_CLASS_SIZE(AssetInfo, 24);

class Folder_ : public Node
{
protected:
	int				m_BlockId;
	AssetInfo*		m_AssetBlockInfo;

	static void		GetResourcePathRelative(String& outPath, String resourceName, ResType::BlockTypeNumber blockType, const char* languageCode);	//	@882DF0
public:
	Folder_();

	static void		Register();	//	@87E810
	static Folder_* Create();	//	@87E730
};

extern EntityType*	tFolder;	//	@A3D810

ASSERT_CLASS_SIZE(Folder_, 88);