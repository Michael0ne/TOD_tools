#pragma once

#include "stdafx.h"
#include "Entity.h"

#define TEXTSLOT_CLASS_SIZE 120

class TextSlot : Entity
{
};

static_assert(sizeof(TextSlot) == TEXTSLOT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TextSlot));