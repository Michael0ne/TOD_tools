#pragma once

#include "stdafx.h"
#include "Entity.h"

class TextSlot : Entity
{
};

static_assert(sizeof(TextSlot) == 0x78, MESSAGE_WRONG_CLASS_SIZE("TextSlot"));