#include "Mesh.h"
#include "BuiltinType.h"

Mesh::Mesh(const unsigned int a1, const char a2, const char a3)
{
	MESSAGE_CLASS_CREATED(Mesh);

	m_IsTrianglesList = a1;
	m_Flags.m_Flags = m_Flags.m_Flags & 0xFFFFFFE0 | a2 & 1 | (8 * (a3 & 3));
	field_58 = 0;
}

Mesh::Mesh(const Mesh& rhs)
{
	MESSAGE_CLASS_CREATED(Mesh);

	// NOTE: check when ready if simple copy is sufficient for 'fld_50'.
	m_Flags = rhs.m_Flags;
	m_FacesList = rhs.m_FacesList;
	m_IndiciesList = rhs.m_IndiciesList;
	field_58 = rhs.field_58;
	field_30 = rhs.field_30;
	field_40 = rhs.field_40;
}

Mesh::~Mesh()
{
	MESSAGE_CLASS_DESTROYED(Mesh);
}

void Mesh::SetFaceVertexIndex(const unsigned int faceind, const unsigned short vertind)
{
	if (m_IndiciesList.size() < faceind)
		m_IndiciesList.resize(faceind + 1);

	m_IndiciesList[faceind] = vertind;
}

void Mesh::AddFace(const unsigned int faceind, const Vector3f& face1, const Vector3f& face2, const Vector2f& texuv)
{
	AddFace1(faceind, face1.x, face1.y, face1.z);
	AddFace2(faceind, face2.x, face2.y, face2.z);

	if (m_FacesList.size() < faceind)
		m_FacesList.resize(faceind + 1);

	m_FacesList[faceind].m_TexCoord[0] = texuv;
}

void Mesh::GetFaceColor(ColorRGB& clr, const unsigned int faceind) const
{
	clr = m_FacesList[faceind].m_Color;
}

const bool Mesh::HasDiffuseFlag() const
{
	return m_Flags.m_FlagsBits.HasDiffuse;
}

void Mesh::GetFacePositionByIndex(Vector4f& outPos, const unsigned int ind) const
{
	outPos = m_FacesList[ind].m_Position;
}

unsigned short Mesh::GetIndiciesListElementByIndex(const unsigned int ind) const
{
	return m_IndiciesList[ind];
}

void Mesh::GetNormaPositionByIndex(Vector4f& outPos, const unsigned int ind) const
{
	outPos = m_FacesList[ind].m_Normal;
}

void Mesh::GetTexCoordByIndex(Vector2f& outCoords, const unsigned int ind, const TexCoord uv) const
{
	outCoords = m_FacesList[ind].m_TexCoord[uv];
}

void Mesh::AddFace1(const unsigned int faceind, const float x, const float y, const float z)
{
	if (m_FacesList.size() < faceind)
		m_FacesList.resize(faceind + 1);

	m_FacesList[faceind].m_Position = { x, y, z, 0 };
}

void Mesh::AddFace2(const unsigned int faceind, const float x, const float y, const float z)
{
	const float s = sqrtf(x * x + y * y + z * z);

	if (m_FacesList.size() < faceind)
		m_FacesList.resize(faceind + 1);

	Vector4f& v = m_FacesList[faceind].m_Normal;
	if (s < 0.00001f)
		v = BuiltinType::InVector;
	else
		v = { (float)(1.0 / s) * x, (float)(1.0 / s) * y, (float)(1.0 / s) * z, 0 };
}

Mesh::Face::Face()
{
	MESSAGE_CLASS_CREATED(Face);

	field_60 = 0;
	m_Position = BuiltinType::ZeroVector;
	m_Normal = { 0, 1, 0, 0 };
}