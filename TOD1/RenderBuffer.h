#pragma once
#include "MemoryAllocators.h"

#define RENDERBUFFER_DEFAULT_BUFFER_SIZE 128
#define RENDERBUFFER_MAX_PARAMS_SIZE 250

class RenderBuffer
{
private:
	int		m_MaxParams;
	int*	m_ParamsArray;
	int		m_CurrentParamIndex;
	int		m_PrevParamIndex;
	int		m_AllocatorId;

public:
	RenderBuffer(unsigned int maxParams, AllocatorIndex allocatorType);	//	@415400
	RenderBuffer() {};

	static void	CreateRenderBuffer();	//	@436070

	static int	Buffer[RENDERBUFFER_DEFAULT_BUFFER_SIZE];	//	@A35B98
};

extern RenderBuffer* g_RenderBuffer;	//	@A35E60

ASSERT_CLASS_SIZE(RenderBuffer, 20);