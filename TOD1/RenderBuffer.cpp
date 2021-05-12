#include "RenderBuffer.h"
#include "GfxInternal.h"

RenderBuffer* g_RenderBuffer = nullptr;
int RenderBuffer::Buffer[RENDERBUFFER_DEFAULT_BUFFER_SIZE];

void RenderBuffer::SetBufferSize(unsigned int size)
{
	if (!size)
	{
		delete m_ParamsArray;

		m_CurrentParamIndex = 0;
		m_MaxParams = 0;

		return;
	}

	if (m_ParamsArray == Buffer || !m_ParamsArray)
		m_ParamsArray = (int*)MemoryManager::AllocateByType(m_AllocatorId, 4 * size);
	else
		m_ParamsArray = (int*)MemoryManager::Realloc(m_ParamsArray, 4 * size, false);

	if (m_ParamsArray)
	{
		m_MaxParams = size;
		if (m_CurrentParamIndex > size)
			m_CurrentParamIndex = size;
	}
	else
	{
		m_MaxParams = RENDERBUFFER_DEFAULT_BUFFER_SIZE;
		m_CurrentParamIndex = 0;
		m_ParamsArray = Buffer;

		g_GfxInternal->SetRenderBufferIsEmpty(false);
	}
}

RenderBuffer::RenderBuffer(unsigned int maxParams, AllocatorIndex allocatorType)
{
	MESSAGE_CLASS_CREATED(RenderBuffer);

	m_AllocatorId = allocatorType;
	m_MaxParams = maxParams;

	if (maxParams)
	{
		m_ParamsArray = (int*)MemoryManager::AllocatorsList[allocatorType]->Allocate_A(4 * maxParams, NULL, NULL);

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

void RenderBuffer::AdjustBufferSize(unsigned int size)
{
	unsigned int currentcapacity = (m_MaxParams * 3) >> 1;

	SetBufferSize(currentcapacity <= size ? size : currentcapacity);
}

void RenderBuffer::CreateRenderBuffer()
{
	g_RenderBuffer = new RenderBuffer(RENDERBUFFER_MAX_PARAMS_SIZE, DEFAULT);
}