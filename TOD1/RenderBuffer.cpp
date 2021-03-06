#include "RenderBuffer.h"
#include "GfxInternal.h"

RenderBuffer* g_RenderBuffer = nullptr;
int RenderBuffer::Buffer[RENDERBUFFER_DEFAULT_BUFFER_SIZE];

RenderBuffer::RenderBuffer(int maxParams, AllocatorIndex allocatorType)
{
	MESSAGE_CLASS_CREATED(RenderBuffer);

	m_AllocatorId = allocatorType;
	m_MaxParams = maxParams;

	if (maxParams)
	{
		m_ParamsArray = (int*)Allocators::AllocatorsList[allocatorType]->Allocate_A(4 * maxParams, NULL, NULL);

		if (!m_ParamsArray)
		{
			m_ParamsArray = Buffer;
			m_MaxParams = RENDERBUFFER_DEFAULT_BUFFER_SIZE;
			g_GfxInternal->SetRenderBufferIsEmpty(true);
			m_CurrentParamIndex = NULL;
			m_PrevParamIndex = NULL;

			return;
		}
	}else
		m_ParamsArray = nullptr;

	m_CurrentParamIndex = NULL;
	m_PrevParamIndex = NULL;
}

void RenderBuffer::CreateRenderBuffer()
{
	g_RenderBuffer = new RenderBuffer(RENDERBUFFER_MAX_PARAMS_SIZE, DEFAULT);
}