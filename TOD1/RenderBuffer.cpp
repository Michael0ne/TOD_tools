#include "RenderBuffer.h"
#include "GfxInternal.h"

RenderBuffer* g_RenderBuffer = nullptr;
int RenderBuffer::Buffer[RENDERBUFFER_DEFAULT_BUFFER_SIZE];
int RenderBuffer::MeshBuffersDrawn = 0;

void RenderBuffer::SetBufferSize(const unsigned int size)
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

RenderBuffer::RenderBuffer(const unsigned int maxParams, AllocatorIndex allocatorType)
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
	}
	else
		m_ParamsArray = nullptr;

	m_CurrentParamIndex = NULL;
	m_PrevParamIndex = NULL;
}

RenderBuffer::RenderBuffer()
{
	MESSAGE_CLASS_CREATED(RenderBuffer);
}

void RenderBuffer::AdjustBufferSize(const unsigned int size)
{
	unsigned int currentcapacity = (m_MaxParams * 3) >> 1;

	SetBufferSize(currentcapacity <= size ? size : currentcapacity);
}

void RenderBuffer::PushModelMatrix(const DirectX::XMMATRIX& mat)
{
	constexpr size_t matsize = sizeof(mat) / 4;
	if (m_MaxParams < m_CurrentParamIndex + matsize)
		AdjustBufferSize(matsize);

	for (unsigned int r = 0; r < 4; ++r)
		for (unsigned int c = 0; c < 3; ++c)
			*(float*)&m_ParamsArray[m_CurrentParamIndex++] = mat.r[r].m128_f32[c];
}

void RenderBuffer::PopMatrix(DirectX::XMMATRIX& mat)
{
	constexpr size_t matargsize = sizeof(mat) / 4;
	m_PrevParamIndex = m_CurrentParamIndex - matargsize;

	mat = *(DirectX::XMMATRIX*)&m_ParamsArray[m_PrevParamIndex];
	mat.r[0].m128_f32[3] = 0;
	mat.r[1].m128_f32[3] = 0;
	mat.r[2].m128_f32[3] = 0;
	mat.r[3].m128_f32[3] = 1;

	m_PrevParamIndex += matargsize;
	m_CurrentParamIndex -= matargsize;
}

void RenderBuffer::PopVector2i(Vector2<int>& vec)
{
	constexpr size_t vecargsize = sizeof(vec) / 4;
	m_PrevParamIndex = m_CurrentParamIndex - vecargsize;
	vec = *(Vector2<int>*) & m_ParamsArray[m_PrevParamIndex];

	m_PrevParamIndex += vecargsize;
	m_CurrentParamIndex -= vecargsize;
}

void RenderBuffer::PopVector2i(Vector2<unsigned int>& vec)
{
	constexpr size_t vecargsize = sizeof(vec) / 4;
	m_PrevParamIndex = m_CurrentParamIndex - vecargsize;
	vec = *(Vector2<unsigned int>*) & m_ParamsArray[m_PrevParamIndex];

	m_PrevParamIndex += vecargsize;
	m_CurrentParamIndex -= vecargsize;
}

void RenderBuffer::PopVector2f(Vector2f& vec)
{
	vec = *(Vector2f*)&m_ParamsArray[m_PrevParamIndex];
	m_PrevParamIndex += (sizeof(vec) / 4);
}

void RenderBuffer::PopVector3f(Vector3f& vec)
{
	vec = *(Vector3f*)&m_ParamsArray[m_PrevParamIndex];

	m_PrevParamIndex += (sizeof(vec) / 4);
}

void RenderBuffer::PopVector4f(Vector4f& vec)
{
	constexpr size_t vecargsize = sizeof(vec) / 4;
	m_PrevParamIndex = m_CurrentParamIndex - vecargsize;
	vec = *(Vector4f*)&m_ParamsArray[m_PrevParamIndex];

	m_PrevParamIndex += vecargsize;
	m_CurrentParamIndex -= vecargsize;
}

void RenderBuffer::PopFloat(float& f)
{
	m_PrevParamIndex = m_CurrentParamIndex - sizeof(f);
	f = *(float*)&m_ParamsArray[m_PrevParamIndex];

	m_PrevParamIndex += sizeof(f);
	m_CurrentParamIndex -= sizeof(f);
}

void RenderBuffer::PopBool(bool& b)
{
	m_PrevParamIndex = m_CurrentParamIndex - sizeof(b);
	b = *(bool*)&m_ParamsArray[m_PrevParamIndex];

	m_PrevParamIndex += sizeof(b);
	m_CurrentParamIndex -= sizeof(b);
}

void RenderBuffer::PopQuaternion(Orientation& q)
{
	q = *(Orientation*)&m_ParamsArray[m_PrevParamIndex];
	m_PrevParamIndex += sizeof(q) / 4;
}

void RenderBuffer::PopColor(ColorRGB& clr)
{
	clr = *(ColorRGB*)&m_ParamsArray[m_PrevParamIndex];
	m_PrevParamIndex += sizeof(clr) / 4;
}

void RenderBuffer::PushColor(const ColorRGB& clr)
{
	if (m_MaxParams < m_CurrentParamIndex + sizeof(clr))
		AdjustBufferSize(m_CurrentParamIndex + sizeof(clr));

	*(float*)&m_ParamsArray[m_CurrentParamIndex++] = clr.r;
	*(float*)&m_ParamsArray[m_CurrentParamIndex++] = clr.g;
	*(float*)&m_ParamsArray[m_CurrentParamIndex++] = clr.b;
	*(float*)&m_ParamsArray[m_CurrentParamIndex++] = clr.a;
}

void RenderBuffer::PushFloat(const float& f)
{
	if (m_MaxParams < m_CurrentParamIndex + sizeof(f))
		AdjustBufferSize(m_CurrentParamIndex + sizeof(f));

	*(float*)&m_ParamsArray[m_CurrentParamIndex++] = f;
}

void RenderBuffer::PushInt(const int& i)
{
	if (m_MaxParams < m_CurrentParamIndex + sizeof(i))
		AdjustBufferSize(m_CurrentParamIndex + sizeof(i));

	m_ParamsArray[m_CurrentParamIndex++] = i;
}

void RenderBuffer::PushBool(const bool& b)
{
	if (m_MaxParams < m_CurrentParamIndex + sizeof(b))
		AdjustBufferSize(m_CurrentParamIndex + sizeof(b));

	m_ParamsArray[m_CurrentParamIndex++] = b != false;
}

void RenderBuffer::PushVector2i(const Vector2<int>& vec)
{
	constexpr size_t vecargsize = sizeof(vec) / 4;
	if (m_MaxParams < m_CurrentParamIndex + vecargsize)
		AdjustBufferSize(m_CurrentParamIndex + vecargsize);

	m_ParamsArray[m_CurrentParamIndex++] = vec.x;
	m_ParamsArray[m_CurrentParamIndex++] = vec.y;
}

void RenderBuffer::PushVector2i(const ScreenResolution& vec)
{
	constexpr size_t vecargsize = sizeof(vec) / 4;
	if (m_MaxParams < m_CurrentParamIndex + vecargsize)
		AdjustBufferSize(m_CurrentParamIndex + vecargsize);

	*(unsigned int*)&m_ParamsArray[m_CurrentParamIndex++] = vec.x;
	*(unsigned int*)&m_ParamsArray[m_CurrentParamIndex++] = vec.y;
}

void RenderBuffer::PushVector2f(const Vector2f& vec)
{
	constexpr size_t vecargsize = sizeof(vec) / 4;
	if (m_MaxParams < m_CurrentParamIndex + vecargsize)
		AdjustBufferSize(m_CurrentParamIndex + vecargsize);

	*(float*)&m_ParamsArray[m_CurrentParamIndex++] = vec.x;
	*(float*)&m_ParamsArray[m_CurrentParamIndex++] = vec.y;
}

void RenderBuffer::PushMatrix(const DirectX::XMMATRIX& mat, const unsigned int ind)
{
	DirectX::XMMATRIX& matarr = *(DirectX::XMMATRIX*)&m_ParamsArray[ind];

	matarr.r[0] = mat.r[0];
	matarr.r[1] = mat.r[1];
	matarr.r[2] = mat.r[2];
}

void RenderBuffer::CreateRenderBuffer()
{
	g_RenderBuffer = new RenderBuffer(RENDERBUFFER_MAX_PARAMS_SIZE, DEFAULT);
}