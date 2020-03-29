#pragma once

#include "stdafx.h"
#include "Entity.h"

class Node : public Entity
{
public:
};

static_assert(sizeof(Node) == 0x78, MESSAGE_WRONG_CLASS_SIZE("Node"));