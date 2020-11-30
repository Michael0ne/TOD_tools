#pragma once

#include "MemoryAllocators.h"

#define RENDERBUFFER_CLASS_SIZE 20
#define RENDERBUFFER_DEFAULT_COMMANDS 128

class RenderBuffer
{
private:
	int		m_MaxCommands;
	void*	m_BufferPtr;
	int		field_8;
	int		field_C;
	int		m_AllocatorId;

public:
	RenderBuffer(int maxCommands, int allocatorType);	//	@415400

	static void	CreateRenderBuffer();	//	@436070

	static int	Buffer;	//	@A35B98	//	NOTE: don't really know what type this is.
};

extern RenderBuffer* g_RenderBuffer;	//	@A35E60

static_assert(sizeof(RenderBuffer) == RENDERBUFFER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(RenderBuffer));