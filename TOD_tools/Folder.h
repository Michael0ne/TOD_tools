#pragma once

#include "Node.h"
#include "Blocks.h"

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

class Folder_ : public Node
{
private:
	int	m_BlockId;
	Folder_Unknown_Struct* field_54;

	void GetResourcePathRelative(String& outPath, String resourceName, BlockTypeNumber blockType, const char* languageCode);	//	@882DF0
public:
	Folder_() : Node(NODE_MASK_EMPTY)	//	NOTE: no actual constructor
	{
		MESSAGE_CLASS_CREATED(Folder_);
	}
};

static_assert(sizeof(Folder_) == FOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Folder_));
static_assert(sizeof(Folder_Unknown_Struct) == FOLDER_UNK_STRUCT_SIZE, MESSAGE_WRONG_CLASS_SIZE(Folder_Unknown_Struct));