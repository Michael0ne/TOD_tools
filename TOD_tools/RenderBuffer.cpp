#include "RenderBuffer.h"
#include "Renderer.h"

RenderBuffer* g_RenderBuffer = nullptr;
int RenderBuffer::Buffer = NULL;

RenderBuffer::RenderBuffer(int maxCommands, int allocatorType)
{
	MESSAGE_CLASS_CREATED(RenderBuffer);

	m_AllocatorId = allocatorType;
	m_MaxCommands = maxCommands;

	if (maxCommands)
	{
		m_BufferPtr = Allocators::AllocatorsList[DEFAULT]->Allocate_A(4 * maxCommands, NULL, NULL);

		if (!m_BufferPtr)
		{
			m_BufferPtr = &Buffer;
			m_MaxCommands = RENDERBUFFER_DEFAULT_COMMANDS;
			g_Renderer->_420170(1);
			field_8 = NULL;
			field_C = NULL;

			return;
		}
	}else
		m_BufferPtr = nullptr;

	field_8 = NULL;
	field_C = NULL;
}

void RenderBuffer::CreateRenderBuffer()
{
	g_RenderBuffer = new RenderBuffer(250, 0);
}