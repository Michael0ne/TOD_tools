#pragma once
#include "Node.h"
#include "Blocks.h"

#define FOLDER_CLASS_SIZE 88
#define FOLDER_ASSETINFO_SIZE 24

struct AssetInfo
{
	struct ActualAssetInfo
	{
		void*		m_ResourceDataBufferPtr;	//	NOTE: actual resource data read from file written here.
		void*		m_ResourceAllocatedAlignedBufferPtr;	//	NOTE: pointer to buffer with resource data size written here.
		unsigned int m_ResourceDataBufferSize;
	};
	ActualAssetInfo	m_AssetInfo_Shared;
	ActualAssetInfo	m_AssetInfo_Localised;
};

ASSERT_CLASS_SIZE(AssetInfo, 24);

class Folder_ : public Node
{
protected:
	int				m_BlockId;
	AssetInfo*		m_AssetBlockInfo;

	static void		GetResourcePathRelative(String& outPath, String resourceName, BlockTypeNumber blockType, const char* languageCode);	//	@882DF0
public:
	Folder_();

	static void		Register();	//	@87E810
	static Folder_* Create();	//	@87E730
};

extern ScriptType_Entity*	tFolder;	//	@A3D810

ASSERT_CLASS_SIZE(Folder_, 88);