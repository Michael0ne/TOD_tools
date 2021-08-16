#include "MeshBuffer.h"

#pragma message(TODO_IMPLEMENTATION)
MeshBuffer::MeshBuffer(Mesh* mutablemesh, const int a2)
{
	MESSAGE_CLASS_CREATED(MeshBuffer);

	m_Mesh = nullptr;
	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;

	m_Mesh = new Mesh(*mutablemesh);

	field_4 &= 0xFFFFFFF5;
	field_4 &= 0xFFFFE00F;

	FillFromMesh(*mutablemesh, a2);
}

MeshBuffer::~MeshBuffer()
{
	MESSAGE_CLASS_DESTROYED(MeshBuffer);
}

#pragma message(TODO_IMPLEMENTATION)
void MeshBuffer::FillFromMesh(const Mesh& mutablemesh, const int a2)
{
	if (!mutablemesh.m_FacesList.size())
		return;

	if ((mutablemesh.m_Flags.m_Flags & 24) == 8)
		m_VertexBuffer = new VertexBuffer(mutablemesh.HasDiffuseFlag() ? 9 : 6, mutablemesh.m_FacesList.size(), a2);
	else
		m_VertexBuffer = new VertexBuffer(7, mutablemesh.m_FacesList.size(), a2);

	m_IndexBuffer = new IndexBuffer(mutablemesh.m_IndiciesList.size(), mutablemesh.m_IsTrianglesList == 1, a2);

	m_VertexBuffer->LockAndGetBufferPtr(1);
	m_IndexBuffer->LockAndGetBufferPtr(1);

	if (m_VertexBuffer->m_InitialVerticiesCapacity > 0)
	{
		const unsigned short vbstride = m_VertexBuffer->m_Stride;
		unsigned int offset = NULL;

		for (unsigned int i = 0; i < m_VertexBuffer->m_InitialVerticiesCapacity; ++i)
		{
			char* vb = m_VertexBuffer->GetBufferPtr() + offset;

			Vector4f facepos;
			Vector4f normalpos;
			mutablemesh.GetFacePositionByIndex(facepos, i);
			mutablemesh.GetNormaPositionByIndex(normalpos, i);
			
			*(Vector3f*)vb = {facepos.x, facepos.y, facepos.z};

			//	TODO: check coords sanity.

			*(Vector3f*)vb = { normalpos.x, normalpos.y, normalpos.z};

			if (mutablemesh.HasDiffuseFlag())
			{
				ColorRGB faceclr;
				mutablemesh.GetFaceColor(faceclr, i);

				if (mutablemesh.m_Flags.m_FlagsBits.IsBGRColor)
					*(unsigned int*)vb++ = ( (unsigned int)( (faceclr.b + faceclr.g + faceclr.r) * 0.33333334 * 255.0 ) << 24 ) | 0xFFFFFF;
				else
					*(unsigned int*)vb++ = D3DCOLOR_DWORD(faceclr.r, faceclr.g, faceclr.b, faceclr.a);
			}

			if (mutablemesh.m_Flags.m_FlagsBits.TexCoordsLevel)
			{
				for (unsigned int j = 0; j < 2; ++j)
				{
					Vector2f texcoords;
					mutablemesh.GetTexCoordByIndex(texcoords, i, (Mesh::TexCoord)j);

					*(Vector2f*)vb = texcoords;
				}
			}

			offset += vbstride;
		}
	}

	for (unsigned int i = 0; i < m_IndexBuffer->m_TotalIndicies; ++i)
		m_IndexBuffer->GetBufferPtr()[i] = mutablemesh.GetIndiciesListElementByIndex(i);

	m_VertexBuffer->UnlockBuffer();
	m_IndexBuffer->UnlockBuffer();

	//	TODO: fill max coords values here.
}