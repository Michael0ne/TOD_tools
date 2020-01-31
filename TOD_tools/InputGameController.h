#pragma once

#include "stdafx.h"

class InputGameController
{
private:
	BYTE	_pad[228];
};

static_assert(sizeof(InputGameController) == 228, MESSAGE_WRONG_CLASS_SIZE("InputGameController"));