#include "TextureAsset.h"
#include "GfxInternal.h"

AssetInstance* TextureAsset::Instance;

TextureAsset::TextureAsset() : Asset(false)
{
    MESSAGE_CLASS_CREATED(TextureAsset);

    m_Texture = new Tex;
    m_Texture_1 = nullptr;

    SetReferenceCount(1);
}

TextureAsset::~TextureAsset()
{
    MESSAGE_CLASS_DESTROYED(TextureAsset);

    delete m_Texture_1;
    delete m_Texture;
}

AssetInstance* TextureAsset::GetInstancePtr() const
{
    return Instance;
}

#pragma message(TODO_IMPLEMENTATION)
void TextureAsset::stub5(int)
{
}

void TextureAsset::GetResourcesDir(String& outDir, PlatformId platformId) const
{
    switch (platformId)
    {
    case PS2:
        outDir = "texture_ps2";
        break;
    case XBOX:
        outDir = "texture_x";
        break;
    case PC:
        outDir = "texture_pc";
        break;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void TextureAsset::ApplyAssetData(int* assetdata)
{
}

char TextureAsset::SetResourcePlaceholder()
{
    if (strstr(m_ResourcePath, "pinkyellowcheckers.bmp") == NULL)
        m_Texture_1 = (Texture*)g_GfxInternal->m_CheckerboardTextures[0]->m_Mutable;
    else
        m_Texture_1 = (Texture*)g_GfxInternal->m_CheckerboardTextures[0]->m_Texture;

    return 1;
}

ScreenResolution& TextureAsset::GetTextureResolution(ScreenResolution& outRes)
{
    if (m_Texture_1)
        outRes = m_Texture_1->m_Resolution;

    return outRes;
}

void TextureAsset::CreateInstance()
{
    Instance = new AssetInstance("texture", (CREATOR)Create);

    Instance->m_FileExtensions.push_back("bmp");
    Instance->SetAlignment(16, 1);
    Instance->SetAlignment(128, 2);
    Instance->SetAlignment(16, 0);
}

TextureAsset* TextureAsset::Create()
{
    return new TextureAsset();
}

TextureAsset::Tex::~Tex()
{
    MESSAGE_CLASS_DESTROYED(Tex);
}