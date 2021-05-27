#include "Mesh.h"
#include "BuiltinType.h"

Mesh::Mesh(const unsigned int a1, const char a2, const char a3)
{
	MESSAGE_CLASS_CREATED(Mesh);

	field_54 = a1;
	field_50 = field_50 & 0xFFFFFFE0 | a2 & 1 | (8 * (a3 & 3));
	field_58 = 0;
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

	m_FacesList[faceind].field_10 = texuv;
}

void Mesh::GetFaceColor(ColorRGB& clr, const unsigned int faceind) const
{
	clr = m_FacesList[faceind].field_30;
}

void Mesh::AddFace1(const unsigned int faceind, const float x, const float y, const float z)
{
	if (m_FacesList.size() < faceind)
		m_FacesList.resize(faceind + 1);

	m_FacesList[faceind].field_0 = { x, y, z, 0 };
}

void Mesh::AddFace2(const unsigned int faceind, const float x, const float y, const float z)
{
	const float s = sqrtf(x * x + y * y + z * z);

	if (m_FacesList.size() < faceind)
		m_FacesList.resize(faceind + 1);

	Vector4f& v = m_FacesList[faceind].field_20;
	if (s < 0.00001f)
		v = BuiltinType::InVector;
	else
		v = { (float)(1.0 / s) * x, (float)(1.0 / s) * y, (float)(1.0 / s) * z, 0 };
}

Mesh::Face::Face()
{
	MESSAGE_CLASS_CREATED(Face);

	field_60 = 0;
	field_0 = BuiltinType::ZeroVector;
	field_20 = { 0, 1, 0, 0 };
}