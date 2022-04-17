#include "DecalManager.h"
#include "StringType.h"
#include "IntegerType.h"
#include "VertexBuffer.h"
#include "FrameBuffer.h"

EntityType* tDecalManager;

DecalManager* DecalManager::Create(AllocatorIndex)
{
    return new DecalManager;
}

void DecalManager::Register()
{
    tDecalManager = new EntityType("DecalManager");
    tDecalManager->InheritFrom(tNode);
    tDecalManager->SetCreator((CREATOR)Create);

    tDecalManager->RegisterProperty(tSTRING, "texture", (EntityGetterFunction)&GetTexture, (EntitySetterFunction)&SetTexture, "control=resource|type=*.bmp");
    tDecalManager->RegisterProperty(tINTEGER, "BufferSize", (EntityGetterFunction)&GetBufferSize, (EntitySetterFunction)&SetBufferSize, "control=slider|min=1|max=100");
    tDecalManager->RegisterProperty(tINTEGER, "PerFrameFillSize", (EntityGetterFunction)&GetPerFrameFillSize, (EntitySetterFunction)&SetPerFrameFillSize, "control=slider|min=1|max=10");

    tDecalManager->RegisterScript("SetDecal(vector,vector,number)", (EntityFunctionMember)&SetDecal);
    tDecalManager->RegisterScript("SetDecal(vector,vector,number,number,number,number)", (EntityFunctionMember)&SetDecal_A);

    tDecalManager->PropagateProperties();
}

DecalManager::DecalInfo::DecalInfo()
{
    MESSAGE_CLASS_CREATED(DecalManager::DecalInfo);

    m_VertexBuffer = nullptr;
    m_BufferSize = 0;
    m_PerFrameFillSize = 2;
    m_DecalsInThisFrame = 0;
    m_VBFreeSpace = 0;
}

DecalManager::DecalInfo::~DecalInfo()
{
    MESSAGE_CLASS_DESTROYED(DecalManager::DecalInfo);

    delete m_VertexBuffer;
}

void DecalManager::DecalInfo::SubmitFrameBufferData(FrameBuffer* fb)
{
    fb->SubmitEnableMipMappingCommand(false);
    fb->SubmitSetTextureAddressModeCommand(1, 0);

    if (m_VBFreeSpace < m_BufferSize)
    {
        fb->SubmitRenderVertexBufferCommand(m_VertexBuffer, 3 * (m_VBFreeSpace + m_PerFrameFillSize), m_BufferSize - m_VBFreeSpace);
        if (m_VBFreeSpace)
            fb->SubmitRenderVertexBufferCommand(m_VertexBuffer, 0, m_VBFreeSpace);
    }
    else
    {
        fb->SubmitRenderVertexBufferCommand(m_VertexBuffer, 3 * (m_VBFreeSpace - m_BufferSize), m_BufferSize);
    }

    fb->SubmitSetTextureAddressModeCommand(0, 0);
    fb->SubmitEnableMipMappingCommand(true);
    m_DecalsInThisFrame = 0;
}

DecalManager::~DecalManager()
{
    MESSAGE_CLASS_DESTROYED(DecalManager);

    delete m_DecalInfo;
    delete m_FrameBuffer;
}

const char* DecalManager::GetTexture() const
{
    return m_Texture.m_AssetPtr ? m_Texture.m_AssetPtr->GetName() : nullptr;
}

void DecalManager::SetTexture(const char* texturepath)
{
    AssetLoader assload(texturepath);
    m_Texture.m_AssetPtr = (TextureAsset*)assload.m_AssetPtr;

    m_Id._3 = true;
}

const int DecalManager::GetBufferSize() const
{
    return m_BufferSize;
}

void DecalManager::SetBufferSize(const char buffersize)
{
    m_BufferSize = buffersize > 255 ? -1 : buffersize;

    TryInstantiate();
}

const int DecalManager::GetPerFrameFillSize() const
{
    return m_PerFrameFillSize;
}

void DecalManager::SetPerFrameFillSize(const int size)
{
    m_PerFrameFillSize = size > 20 ? 20 : size;

    TryInstantiate();
}

#pragma message(TODO_IMPLEMENTATION)
void DecalManager::SetDecal(int* args)
{
}

#pragma message(TODO_IMPLEMENTATION)
void DecalManager::SetDecal_A(int* args)
{
}