#include "Mesh.h"
#include "BuiltinType.h"

Mesh::Mesh(const unsigned int isTrianglesList, const bool hasDiffuse, const char TexCoordsLevel)
{
    MESSAGE_CLASS_CREATED(Mesh);

    m_IsTrianglesList = isTrianglesList;
    m_Flags.HasDiffuse = hasDiffuse;
    m_Flags.TexCoordsLevel = TexCoordsLevel;
    field_58 = 0;
}

Mesh::Mesh(const Mesh& rhs)
{
    MESSAGE_CLASS_CREATED(Mesh);

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

void Mesh::SetVertexIndex(const unsigned int faceind, const unsigned short vertind)
{
    if (faceind >= m_IndiciesList.size())
        m_IndiciesList.resize(faceind + 1);

    m_IndiciesList[faceind] = vertind;
}

void Mesh::AddVertex(const unsigned int faceind, const Vector3f& face, const Vector3f& normal, const Vector2f& texuv)
{
    SetFacePosition(faceind, face.x, face.y, face.z);
    SetFaceNormal(faceind, normal.x, normal.y, normal.z);

    if (faceind >= m_FacesList.size())
        m_FacesList.resize(faceind + 1);

    m_FacesList[faceind].m_TexCoord[0] = texuv;
}

void Mesh::GetFaceColor(ColorRGB& clr, const unsigned int faceind) const
{
    clr = m_FacesList[faceind].m_Color;
}

const bool Mesh::HasDiffuseFlag() const
{
    return m_Flags.HasDiffuse;
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

void Mesh::SetFacePosition(const unsigned int faceind, const float x, const float y, const float z)
{
    if (faceind >= m_FacesList.size())
        m_FacesList.resize(faceind + 1);

    m_FacesList[faceind].m_Position = { x, y, z, 0 };
}

void Mesh::SetFaceNormal(const unsigned int faceind, const float x, const float y, const float z)
{
    const float s = sqrtf(x * x + y * y + z * z);

    if (faceind >= m_FacesList.size())
        m_FacesList.resize(faceind + 1);

    Vector4f& v = m_FacesList[faceind].m_Normal;
    if (s < 0.00001f)
        v = BuiltinType::InVector;
    else
        v = { (float)(1.0 / s) * x, (float)(1.0 / s) * y, (float)(1.0 / s) * z, 0 };
}

MeshFace::MeshFace()
{
    MESSAGE_CLASS_CREATED(MeshFace);

    field_60 = 0;
    m_Position = BuiltinType::ZeroVector;
    m_Normal = { 0, 1, 0, 0 };
}