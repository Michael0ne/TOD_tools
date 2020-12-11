#pragma once

#include "Node.h"

#define FOLDER_CLASS_SIZE 88
#define FOLDER_UNK_STRUCT_SIZE 24

struct Folder_Unknown_Struct
{
	int	field_0;
	int	field_4;
	int	field_8;
	int	field_C;
	int	field_10;
	int	field_14;
};

enum BlockTypeId
{
	NONE = 0,
	MAP = 1,
	SUBMAP = 2,
	MISSION = 3,
	CUTSCENE = 4,
	PLAYERDATA = 5,
	MAIN = 6
};

class Folder_ : public Node
{
private:
	int	m_BlockId;
	Folder_Unknown_Struct* field_54;

	void GetResourcePathRelative(String& outPath, String resourceName, BlockTypeId blockType, const char* languageCode);	//	@882DF0

	static const char*	BlockTypeExtension[];	//	@A11B64
public:
	Folder_() : Node(NODE_MASK_EMPTY)	//	NOTE: no actual constructor
	{
		MESSAGE_CLASS_CREATED(Folder_);
	}
};

static_assert(sizeof(Folder_) == FOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Folder_));
static_assert(sizeof(Folder_Unknown_Struct) == FOLDER_UNK_STRUCT_SIZE, MESSAGE_WRONG_CLASS_SIZE(Folder_Unknown_Struct));