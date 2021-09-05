#include "IndexBuffer.h"
#include "GfxInternal_Dx9.h"

IndexBuffer::IndexBuffer(const unsigned int totalindicies, const unsigned int primitivetypeindex, const unsigned int a3)
{
 MESSAGE_CLASS_CREATED(IndexBuffer);

 m_BufferPtr = nullptr;
 m_Direct3DIndexBuffer = nullptr;
 m_Flags = a3;
 m_PrimitiveTypeIndex = primitivetypeindex;
 m_PrimitiveType = PrimitiveTypes[primitivetypeindex];
 m_TotalIndicies = totalindicies;
 field_14 &= 0xFFFFFFFE;
 m_LockMode = 0;

 m_BufferPtr = new char[4 * totalindicies];
 CreateDirect3DBuffer();

 IndexBufferMap->insert({ IndexBufferMap->size(), this });
}

IndexBuffer::IndexBuffer(IndexBuffer* rhs, const int a2)
{
 MESSAGE_CLASS_CREATED(IndexBuffer);

 m_BufferPtr = nullptr;
 m_Direct3DIndexBuffer = nullptr;
 m_Flags = (a2 & 4) ? rhs->m_Flags : a2;
 m_PrimitiveTypeIndex = rhs->m_PrimitiveTypeIndex;
 m_PrimitiveType = PrimitiveTypes[rhs->m_PrimitiveTypeIndex];
 m_TotalIndicies = rhs->m_TotalIndicies;
 field_14 ^= (field_14 ^ rhs->field_14) & 1;
 m_LockMode = 0;
 rhs->m_LockMode = 2;

 m_BufferPtr = new char[m_TotalIndicies * 4];
 memcpy(m_BufferPtr, rhs->m_BufferPtr, m_TotalIndicies * 2);
 
 if (rhs->m_LockMode == 1)
  rhs->FillFromBufferedData();

 CreateDirect3DBuffer();
 FillFromBufferedData();

 IndexBufferMap->insert({ IndexBufferMap->size(), this });
}

IndexBuffer::~IndexBuffer()
{
 MESSAGE_CLASS_DESTROYED(IndexBuffer);

 RELEASE_SAFE(m_Direct3DIndexBuffer);

 for (auto it = IndexBufferMap->begin(); it != IndexBufferMap->end(); it++)
 {
  if (it->second == this)
  {
   IndexBufferMap->erase(it);
   break;
  }
 }
}

char* IndexBuffer::LockAndGetBufferPtr(const int mode)
{
 m_LockMode = mode;
 return m_BufferPtr;
}

unsigned short* IndexBuffer::GetBufferPtr() const
{
 return (unsigned short*)m_BufferPtr;
}

void IndexBuffer::UnlockBuffer()
{
 if (m_LockMode == 1)
  FillFromBufferedData();

 m_LockMode = NULL;
}

void IndexBuffer::CreateDirect3DBuffer()
{
 LPDIRECT3DINDEXBUFFER9 ib = nullptr;
 g_GfxInternal_Dx9->m_Direct3DDevice->CreateIndexBuffer(m_TotalIndicies * sizeof(short), (m_Flags & 1) == 0 ? D3DUSAGE_WRITEONLY : D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL);

 RELEASE_SAFE(m_Direct3DIndexBuffer);

 if (ib)
  m_Direct3DIndexBuffer = ib;
 else
  m_Direct3DIndexBuffer = nullptr;
}

void IndexBuffer::DestroyDirect3DBuffer()
{
 RELEASE_SAFE(m_Direct3DIndexBuffer);
}

void IndexBuffer::FillFromBufferedData()
{
 void* data = nullptr;
 if (SUCCEEDED(m_Direct3DIndexBuffer->Lock(0, 0, &data, (m_Flags & 1) != 0 ? D3DLOCK_DISCARD : NULL)) && data)
 {
  size_t buflen = 4 * ((m_TotalIndicies * 2) >> 2);
  memcpy(data, m_BufferPtr, buflen);
  memcpy((char*)data + buflen, &m_BufferPtr[buflen], (m_TotalIndicies * 2) & 3);
  m_Direct3DIndexBuffer->Unlock();
 }
}

#pragma message(TODO_IMPLEMENTATION)
void IndexBuffer::CreateFromAssetData(void* asset)
{
}

const D3DPRIMITIVETYPE IndexBuffer::PrimitiveTypes[] =
{
 D3DPT_TRIANGLESTRIP,
 D3DPT_TRIANGLELIST,
 D3DPT_TRIANGLEFAN,
 D3DPT_POINTLIST,
 D3DPT_LINELIST,
 D3DPT_LINESTRIP
};

std::map<int, IndexBuffer*>* IndexBuffer::IndexBufferMap;

void IndexBuffer::CreateIndexBufferMap()
{
 IndexBufferMap = new std::map<int, IndexBuffer*>;
}

void IndexBuffer::DestroyIndexBufferMap()
{
 IndexBufferMap->clear();
 delete IndexBufferMap;
}

void IndexBuffer::FillMapFromBuffer()
{
 if (IndexBufferMap->begin() == IndexBufferMap->end())
  return;

 for (auto it = IndexBufferMap->begin(); it != IndexBufferMap->end(); it++)
 {
  it->second->CreateDirect3DBuffer();
  it->second->FillFromBufferedData(); // NOTE: original code has it inlined.
 }
}