#include "VertexBuffer.h"
#include "GfxInternal_Dx9.h"

const VertexBuffer::VertexDeclaration VertexBuffer::VertexDeclarations[] =
{
 { 20, D3DFVF_DIFFUSE | D3DFVF_XYZRHW },
 { 24, D3DFVF_TEX1 | D3DFVF_XYZRHW},
 { 28, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW },
 { 24, D3DFVF_NORMAL | D3DFVF_XYZ },
 { 16, D3DFVF_DIFFUSE | D3DFVF_XYZ },
 { 28, D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ },
 { 32, D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ },
 { 40, D3DFVF_TEX2 | D3DFVF_NORMAL | D3DFVF_XYZ },
 { 24, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ },
 { 36, D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_XYZ },
 { 48, D3DFVF_TEX1 | D3DFVF_NORMAL | D3DFVF_XYZ },
 { 40, D3DFVF_NORMAL | D3DFVF_XYZ }
};
std::map<int, VertexBuffer*>* VertexBuffer::VertexBufferMap;

VertexBuffer::VertexBuffer(int FVFindex, int size, int flags)
{
    m_BufferPtr = nullptr;
    m_Direct3DVertexBuffer = nullptr;
    field_16 = NULL;
    m_InitialVerticiesCapacity = size;
    m_Stride = VertexDeclarations[FVFindex].m_Stride;
    m_FVF = VertexDeclarations[FVFindex].m_FVF;
    m_Length = size * m_Stride;
    m_VerticiesTotal = NULL;
    m_Flags = flags;
    m_FVFIndex = FVFindex;
    m_LockMode = 0;
    m_BufferPtr = new char[m_Length];

    CreateDirect3DBuffer();
    VertexBufferMap->insert({ VertexBufferMap->size(), this });
}

VertexBuffer::~VertexBuffer()
{
    MESSAGE_CLASS_DESTROYED(VertexBuffer);

    for (auto it = VertexBufferMap->begin(); it != VertexBufferMap->end(); it++)
    {
        if (it->second == this)
        {
            VertexBufferMap->erase(it);
            break;
        }
    }

    RELEASE_SAFE(m_Direct3DVertexBuffer);
    delete[] m_BufferPtr;
}

#pragma message(TODO_IMPLEMENTATION)
char* VertexBuffer::LockAndGetBufferPtr(const int mode)
{
    m_LockMode = mode;

    if (mode != 3)
        return m_BufferPtr;

    char* buf = nullptr;
    m_Direct3DVertexBuffer->Lock(0, 0, (void**)&buf, m_Flags & 1 ? D3DLOCK_DISCARD : NULL);

    // TODO: this actually returns 2 vectors (sizeof = 32).
    return buf;
}

void VertexBuffer::UnlockBuffer()
{
    if (m_LockMode == 1)
        FillFromBufferedData();
    else
        if (m_LockMode == 3)
            m_Direct3DVertexBuffer->Unlock();

    m_LockMode = 0;
}

int VertexBuffer::SetData(const unsigned int verticies, const void* indata, void* outdata)
{
    if (m_VerticiesTotal + verticies >= m_InitialVerticiesCapacity)
    {
        if (outdata)
            m_Direct3DVertexBuffer->Lock(0, verticies * m_Stride, &outdata, D3DLOCK_NOOVERWRITE);
        else
            m_Direct3DVertexBuffer->Lock(0, 0, &outdata, D3DLOCK_DISCARD);

        m_VerticiesTotal = verticies;
    }
    else
    {
        m_Direct3DVertexBuffer->Lock(m_VerticiesTotal * m_Stride, verticies * m_Stride, &outdata, D3DLOCK_NOOVERWRITE);
        m_VerticiesTotal += verticies;
    }

    memcpy(outdata, indata, verticies * m_Stride);
    m_Direct3DVertexBuffer->Unlock();

    return m_VerticiesTotal - verticies;
}

void VertexBuffer::CreateDirect3DBuffer()
{
    LPDIRECT3DVERTEXBUFFER9 vertbuff = nullptr;
    g_GfxInternal_Dx9->m_Direct3DDevice->CreateVertexBuffer(m_Length, m_Flags & 1 ? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY : D3DUSAGE_WRITEONLY, m_FVF, D3DPOOL_DEFAULT, &vertbuff, nullptr);

    if (vertbuff != m_Direct3DVertexBuffer)
        RELEASE_SAFE(m_Direct3DVertexBuffer);

    m_Direct3DVertexBuffer = vertbuff;
}

void VertexBuffer::FillFromBufferedData()
{
    void* data = nullptr;
    if (SUCCEEDED(m_Direct3DVertexBuffer->Lock(0, 0, &data, (m_Flags & 1) != 0 ? D3DLOCK_DISCARD : NULL)) && data)
    {
        size_t buflen = 4 * (m_Length >> 2);
        memcpy(data, m_BufferPtr, buflen);
        memcpy((char*)data + buflen, &m_BufferPtr[buflen], m_Length & 3);
        m_Direct3DVertexBuffer->Unlock();
    }
}

char* VertexBuffer::GetBufferPtr() const
{
    return m_BufferPtr;
}

void VertexBuffer::CreateVerticesMap()
{
    VertexBufferMap = new std::map<int, VertexBuffer*>();
}

void VertexBuffer::DestroyVertexBufferMap()
{
    VertexBufferMap->clear();
    delete VertexBufferMap;
}

void VertexBuffer::FillMapFromBuffer()
{
    if (VertexBufferMap->begin() == VertexBufferMap->end())
        return;

    for (auto it = VertexBufferMap->begin(); it != VertexBufferMap->end(); it++)
    {
        it->second->CreateDirect3DBuffer();
        it->second->FillFromBufferedData(); // NOTE: original code has it inlined.
    }
}