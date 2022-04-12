#include "ModelAsset.h"
#include "SkinnedMeshBuffer.h"
#include "GfxInternal.h"

AssetInstance* ModelAsset::Instance;

int ModelAsset::PivotIndexByName(const char* const pivotname) const
{
    if (m_PivotsList.size() <= 0)
        return -1;

    for (unsigned int i = 0; i < m_PivotsList.size(); ++i)
        if (_stricmp(m_PivotsList[i].m_Name, pivotname) == NULL)
            return i;

    return -1;
}

#pragma message(TODO_IMPLEMENTATION)
void ModelAsset::GetPivotMatrix(DirectX::XMMATRIX& outMat, const int pivotIndex) const
{
}

Vector4f& ModelAsset::GetBoundingRadius(Vector4f& outBoundings) const
{
    outBoundings = m_BoundingRadius;
    return outBoundings;
}

#pragma message(TODO_IMPLEMENTATION)
void ModelAsset::CalculateBounds()
{
}

void ModelAsset::AddPhysAttachmentRef(const int listIndex, const int physAttachmentIndex)
{
    if (physAttachmentIndex == -1)
        return;

    auto& attlist = *m_PhysAttachmentsList;

    attlist.resize(listIndex + 1);
    if (!attlist[listIndex].m_Attachments.size())
        attlist[listIndex].m_Attachments.resize(64);

    if (m_PivotsList[physAttachmentIndex].m_ParentIndex != -1)
        AddPhysAttachmentRef(listIndex, m_PivotsList[physAttachmentIndex].m_ParentIndex);
    attlist[listIndex].m_Attachments[physAttachmentIndex] = 1;
}

Texture* ModelAsset::GetPivotMeshTexture(const unsigned int pivotIndex, const unsigned int skinnedMeshIndex, const unsigned int textureSetIndex) const
{
    const SkinnedMeshBuffer* skMesh = m_PivotsList[pivotIndex].m_SkinnedMeshesList[skinnedMeshIndex];

    if (!skMesh->m_TextureSets.size() || textureSetIndex >= skMesh->m_TextureSets.size())
        return nullptr;

    if (skMesh->m_TextureSets[textureSetIndex] < 0)
        return nullptr;

    return m_TextureResources[skMesh->m_TextureSets[textureSetIndex]].m_TextureResource->m_Texture_1;
}

const float ModelAsset::GetPivotMeshEnvMapCoefficient(const unsigned int pivotIndex, const unsigned int skinnedMeshIndex, const unsigned int textureSetIndex) const
{
    const SkinnedMeshBuffer* skMesh = m_PivotsList[pivotIndex].m_SkinnedMeshesList[skinnedMeshIndex];
    if (textureSetIndex >= skMesh->m_EnvMapCoefficients.size())
        return 0.5f;
    else
        return skMesh->m_EnvMapCoefficients[textureSetIndex];
}

void ModelAsset::CreateInstance()
{
    Instance = new AssetInstance("modelres", (CREATOR)Create);

    Instance->m_FileExtensions.push_back("model");
    Instance->SetAlignment(16, 1);
    Instance->SetAlignment(16, 2);
    Instance->SetAlignment(16, 0);
}

ModelAsset* ModelAsset::Create()
{
    return new ModelAsset;
}

ModelAsset::ModelAsset() : Asset(false)
{
    MESSAGE_CLASS_CREATED(ModelAsset);

    m_PhysAttachmentsList = 0;
    field_54 = 0;
    field_58 = (int*)1;
    m_BoundingRadius = {};
    SetReferenceCount(1);
    m_PhysAttachmentsListsTotal = 47;
}

int ModelAsset::LoadTexture(const char* const texname)
{
    String fname;
    File::ExtractFileName(fname, texname);

    if (strstr(fname.m_Str, "nvisible") || strstr(fname.m_Str, "NVISIBLE"))
        return -1;

    String respath;
    String existrespath;
    unsigned int i = 0;
    g_AssetManager->GetResourcePath(respath, texname);
    if (m_TextureResources.size())
    {
        while (true)
        {
            g_AssetManager->GetResourcePath(existrespath, m_TextureResources[i].m_TextureResource->m_ResourcePath);
            if (strncmp(existrespath.m_Str, respath.m_Str, existrespath.m_Length) == NULL)
                return i;
        }
    }

    AssetLoader assload(texname);
    m_TextureResources.push_back(
        { (TextureAsset*)assload.m_AssetPtr, 0 }
    );
    g_AssetManager->IncreaseResourceReferenceCount(assload.m_AssetPtr); //  NOTE: why? destructor will set this value back.

    return m_TextureResources.size() - 1;
}

AssetInstance* ModelAsset::GetInstancePtr() const
{
    return Instance;
}

char ModelAsset::SetResourcePlaceholder()
{
    Mesh mesh;
    SkinnedMeshBuffer skinnedmeshbuffer;

    skinnedmeshbuffer.m_Mesh = g_GfxInternal->m_Mesh;
    skinnedmeshbuffer.m_MeshBuffer = g_GfxInternal->m_MeshBuffer;

    skinnedmeshbuffer.m_TextureReferences.push_back(
        { LoadTexture("pinkyellowcheckers.bmp") }
    );

    mesh.m_SkinnedMeshesList.push_back(&skinnedmeshbuffer);
    m_PivotsList.push_back(mesh);

    CalculateBounds();

    return 1;
}

#pragma message(TODO_IMPLEMENTATION)
void ModelAsset::_856E60()
{
}

#pragma message(TODO_IMPLEMENTATION)
ModelAsset::Mesh::Mesh()
{
    MESSAGE_CLASS_CREATED(Mesh);
}

#pragma message(TODO_IMPLEMENTATION)
ModelAsset::Mesh::Mesh(const Mesh& rhs)
{
    MESSAGE_CLASS_CREATED(Mesh);
}

#pragma message(TODO_IMPLEMENTATION)
ModelAsset::Mesh::~Mesh()
{
    MESSAGE_CLASS_DESTROYED(Mesh);
}