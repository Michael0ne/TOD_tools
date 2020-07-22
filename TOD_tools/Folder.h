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

enum E_BLOCK_ID
{
	ID_ALL = 0,
	ID_MAP = 1,
	ID_SUBMAP = 2,
	ID_MISSION = 3,
	ID_CUTSCENE = 4,
	ID_PLAYERDATA = 5
};

class Folder_ : public Node
{
private:
	int	m_BlockId;
	Folder_Unknown_Struct* field_54;
public:
	Folder_() : Node(NODE_MASK_EMPTY)	//	NOTE: no actual constructor
	{
		MESSAGE_CLASS_CREATED(Folder_);
	}
};

static_assert(sizeof(Folder_) == FOLDER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Folder_));
static_assert(sizeof(Folder_Unknown_Struct) == FOLDER_UNK_STRUCT_SIZE, MESSAGE_WRONG_CLASS_SIZE(Folder_Unknown_Struct));