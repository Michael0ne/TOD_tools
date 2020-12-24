#pragma once

#include "Node.h"
#include "Blocks.h"

#define FOLDER_CLASS_SIZE 88
#define FOLDER_ASSETINFO_SIZE 24

struct AssetInfo
{
	int	field_0;	//	NOTE: could be ID.
	int	field_4;
	int	field_8;
	int	field_C;
	int	field_10;
	int	field_14;
};

//	NOTE: this is used on all .main files. And maybe another resource types, not sure now.
//	NOTE: asset files are LITTLE endian.
struct MainAssetBlock
{
	unsigned int	m_EngineTimestamp;	//	NOTE: the game reads it as timestamp, but if represented as timestamp - it's just garbage.
	unsigned int	m_CRCPropertiesList;
	unsigned int	m_CRCCommandsList;
	int				m_ResourcesInFile;	//	NOTE: this value can be negavite (-1).
	unsigned int	m_ResourceElementSize;
	unsigned int	m_ResourceBufferSize;	//	NOTE: this is the size for the buffer that game allocates when reading asset block file.
};

class Folder_ : public Node
{
private:
	int	m_BlockId;
	AssetInfo* m_AssetBlockInfo;

	void GetResourcePathRelative(String& outPath, String resourceName, BlockTypeNumber blockType, const char* languageCode);	//	@882DF0
public:
	Folder_() : Node(NODE_MASK_EMPTY)	//	NOTE: no actual constructor
	{
		MESSAGE_CLASS_CREATED(Folder_);
	}
};

static_assert(sizeof(Folder_) == FOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Folder_));
static_assert(sizeof(AssetInfo) == FOLDER_ASSETINFO_SIZE, MESSAGE_WRONG_CLASS_SIZE(AssetInfo));