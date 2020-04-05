#pragma once

#include "stdafx.h"
#include "Entity.h"

#define NODE_CLASS_SIZE 120

class Node : public Entity
{
public:
};

static_assert(sizeof(Node) == NODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Node"));