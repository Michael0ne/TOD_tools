#include "SkinnedMeshBuffer.h"
#include "Mesh.h"
#include "MeshBuffer.h"

SkinnedMeshBuffer::SkinnedMeshBuffer()
{
    MESSAGE_CLASS_CREATED(SkinnedMeshBuffer);

    m_MeshBuffer = (MeshBuffer*)1;
    m_Mesh = (Mesh*)1;

    m_Flags._14 = 1;
    m_Flags._15 = 1;
    m_Flags._16 = 1;
    m_Flags._17 = 1;
    m_Flags._19 = 1;
    m_Flags._20 = 1;
}

SkinnedMeshBuffer::~SkinnedMeshBuffer()
{
    MESSAGE_CLASS_DESTROYED(SkinnedMeshBuffer);

    delete m_Mesh;
    delete m_MeshBuffer;
}