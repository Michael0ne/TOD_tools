#include "Texture.h"
#include "LogDump.h"
#include "GfxInternal_Dx9.h"

std::map<unsigned int, Texture*>* Texture::TexturesMap;
D3DFORMAT Texture::SupportedTextureFormats[] =
{
 D3DFMT_R8G8B8,
 D3DFMT_A8R8G8B8,
 D3DFMT_R5G6B5,
 D3DFMT_A1R5G5B5,
 D3DFMT_A4R4G4B4,
 D3DFMT_P8,
 D3DFMT_UNKNOWN,
 D3DFMT_DXT1,
 D3DFMT_DXT2,
 D3DFMT_DXT3,
 D3DFMT_DXT4,
 D3DFMT_DXT5
};
unsigned int Texture::_A08980[] = { 0x180888, 0x208888, 0x100565, 0x101555, 0x104444, 0x8088888, 0x8048888, 0x40000, 0, 0, 0, 0, 0x208888, 0x100565, 0 };
int Texture::Formats[] =
{
 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 1, 14, 0, 1, 1, 2, 3, 14, 4, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 5
};

void Texture::CreateDirect3DTexture(const ScreenResolution& res, unsigned int formatindex, unsigned int levels)
{
    if (levels == 4 || levels == 3)
    {
        m_SurfaceSize = res;
    }
    else
    {
        m_SurfaceSize = { 1, 1 };

        if (res.x > 1)
            for (; m_SurfaceSize.x < res.x; m_SurfaceSize.x *= 2);
        if (res.y > 1)
            for (; m_SurfaceSize.y < res.y; m_SurfaceSize.y *= 2);
    }

    m_MipMapLevels = levels;
    m_Levels = m_Levels & 0xFE03 | 2;
    m_Texture = nullptr;

    HRESULT hr = g_GfxInternal_Dx9->m_Direct3DDevice->CreateTexture(
        m_SurfaceSize.x,
        m_SurfaceSize.y,
        levels != 1,
        levels == 3 ? 1 : 0,
        SupportedTextureFormats[formatindex],
        levels == 3 ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED,
        &m_Texture,
        nullptr);

#ifdef INCLUDE_FIXES
    if (!m_Texture)
    {
        debug("CreateDirect3DTexture({ %i, %i }, %i, %i) FAILED (%i)!\n", res.x, res.y, formatindex, levels, hr);
        return;
    }
#endif

    D3DSURFACE_DESC surfdesc;
    m_Texture->GetLevelDesc(0, &surfdesc);

    m_SurfaceSize = { surfdesc.Width, surfdesc.Height };

    switch (surfdesc.Format)
    {
    case D3DFMT_DXT5:
        m_Format = 11;
        break;
    case D3DFMT_DXT4:
        m_Format = 10;
        break;
    case D3DFMT_DXT3:
        m_Format = 9;
        break;
    case D3DFMT_DXT2:
        m_Format = 8;
        break;
    case D3DFMT_DXT1:
        m_Format = 7;
        break;
    default:
        m_Format = Formats[surfdesc.Format];
        break;
    }

    m_Resolution = res;
    field_20 = _A08980[m_Format];
    m_Format = formatindex;
}

unsigned int Texture::GetTextureFormat(D3DFORMAT fmt)
{
    switch (fmt)
    {
    case D3DFMT_DXT3:
        return 9;
    case D3DFMT_DXT1:
        return 7;
    case D3DFMT_DXT2:
        return 8;
    case D3DFMT_DXT4:
        return 10;
    case D3DFMT_DXT5:
        return 11;
    default:
        return Formats[(int)fmt];
    }
}

Texture::Texture(const ScreenResolution& resolution, unsigned int, unsigned int levels)
{
    MESSAGE_CLASS_CREATED(Texture);

    m_Mutable = nullptr;
    m_Texture = nullptr;
    m_TextureSurfaceBits = nullptr;
    m_Levels = 0xFDFE;
    CreateDirect3DTexture(resolution, 1, levels);
    GfxInternal_Dx9::RenderedTexturesMap.insert({ GfxInternal_Dx9::RenderedTexturesMap.size(), this });
}

Texture::Texture(MutableTextureBuffer* surf)
{
    MESSAGE_CLASS_CREATED(Texture);

    m_Mutable = nullptr;
    m_Texture = nullptr;
    m_TextureSurfaceBits = nullptr;
    surf->GetSurfaceByIndex(0);
    m_Levels &= 0xFFFFFDFE;

    m_Mutable = new MutableTextureBuffer(*surf);
    AdjustColors();

    TexturesMap->emplace(TexturesMap->size(), this);
}

Texture::~Texture()
{
    MESSAGE_CLASS_DESTROYED(Texture);

    for (unsigned int i = 0; i < 2; ++i)
        if (g_GfxInternal_Dx9->m_TexturesArray_2[i] == this)
            g_GfxInternal_Dx9->SetTextureIndex(nullptr, i);

    RELEASE_SAFE(m_Texture);

    auto it = TexturesMap->begin();
    for (; it != TexturesMap->end(); it++)
    {
        if (it->second == this)
        {
            TexturesMap->erase(it);
            break;
        }
    }

    delete[] m_TextureSurfaceBits;
    delete m_Mutable;
}

unsigned int Texture::GetSizeForLevel(const unsigned char lvl) const
{
    unsigned int widthn = m_SurfaceSize.x >> lvl;
    unsigned int heightn = m_SurfaceSize.y >> lvl;

    if (widthn < 1) widthn = 1;
    if (heightn < 1) heightn = 1;

    if (!(m_Levels & 1))
        return (widthn * heightn * (m_BitsPerPixel & 63)) >> 3;
    else
        return (8 * (widthn / 4 >= 1 ? widthn / 4 : 1) * (heightn / 4 >= 1 ? heightn : 1));
}

void Texture::SetTextureForStage(const unsigned int stage) const
{
    g_GfxInternal_Dx9->m_Direct3DDevice->SetTexture(stage, (LPDIRECT3DTEXTURE9)ALIGN_4BYTES(m_Texture));
}

void Texture::AdjustColors()
{
    ScreenResolution surfsize, surfsizenorm;
    m_Mutable->GetTextureResolution(surfsizenorm, 0);
    CreateDirect3DTexture(surfsizenorm, 1, m_Mutable->m_Surfaces.size() > 1);
    m_Resolution = m_Mutable->GetImageResolution(surfsize, 0);

    if ((unsigned char)(m_Levels >> 1) != m_Mutable->m_Surfaces.size())
    {
        LogDump::LogA("ERROR - mTex has %i levels, texture has %i\n", m_Mutable->m_Surfaces.size(), (unsigned char)(m_Levels >> 1));
        LogDump::LogA("Mutable:\n");

        for (unsigned int i = 0; i < m_Mutable->m_Surfaces.size(); ++i)
        {
            ScreenResolution s;
            m_Mutable->GetTextureResolution(s, i);
            LogDump::LogA("Level %i: size=(%i,%i)\n", i, s.x, s.y);
        }

        LogDump::LogA("Surface size: (%i,%i)\n", m_SurfaceSize.x, m_SurfaceSize.y);
    }

    if ((m_Levels & 510) != 0)
    {
        for (unsigned int level = 0; level < (unsigned char)(m_Levels >> 1); ++level)
        {
            D3DSURFACE_DESC surfdesc;
            D3DLOCKED_RECT surfrect;
            ScreenResolution texres;
            m_Texture->GetLevelDesc(level, &surfdesc);
            m_Mutable->GetTextureResolution(texres, level);

            if (FAILED(m_Texture->LockRect(level, &surfrect, nullptr, NULL)))
                LogDump::LogA("Error - could not lock this tex level %i\n", level);

            char* surfcolors = m_Mutable->GetColorPalletteForSurfaceByIndex(level);
            m_Mutable->GetTotalColorsForSurfaceByIndex(level);

            for (unsigned int x = 0; x < texres.x; ++x)
                for (unsigned int y = 0; y < texres.y; ++y)
                    *((char*)surfrect.pBits + 4 * x + y * surfrect.Pitch) = *(surfcolors + (4 * (x + texres.x * y)) + 3) | ((*(surfcolors + (4 * (x + texres.x * y)) + 2) | *(surfcolors + (4 * (x + texres.x * y)) + 1) << 8) << 8);

            m_Texture->UnlockRect(level);
        }
    }
}

const unsigned int Texture::GetTextureSizeBytes() const
{
    unsigned int s = 0;
    unsigned int l = m_Levels >> 1;
    for (unsigned int lvl = 0; lvl < l; s += GetSizeForLevel(lvl++));

    return s;
}

LPDIRECT3DSURFACE9 Texture::GetSurfaceLevel(const unsigned int level) const
{
    LPDIRECT3DSURFACE9 surf = nullptr;
    m_Texture->GetSurfaceLevel(level, &surf);

    return surf;
}

LPDIRECT3DTEXTURE9 Texture::GetDirect3DTexture() const
{
    return m_Texture;
}

bool Texture::FillTextureInfo()
{
    D3DSURFACE_DESC desc;
    HRESULT texlvldesc = m_Texture->GetLevelDesc(0, &desc);

    m_SurfaceSize = { desc.Width, desc.Height };
    m_Format = GetTextureFormat(desc.Format);
    field_20 = _A08980[m_Format];

    return SUCCEEDED(texlvldesc);
}

// NOTE: disabling it doesn't seem to affect anything.
void Texture::DrawAllTextures()
{
    unsigned int memoryusage = 0;
    LogDump::LogA("rendering a frame with all (%i) textures!\n", TexturesMap->size());

    if (TexturesMap->cbegin() != TexturesMap->cend())
    {
        for (auto it = TexturesMap->cbegin(); it != TexturesMap->cend(); ++it)
        {
            g_GfxInternal_Dx9->RenderTexturedQuad2D_1(*it->second, {}, { 2.f, 2.f }, { 1.f, 1.f, 1.f, 1.f });
            unsigned int levelsize = 0;
            unsigned int texlevel = 0;

            for (unsigned int level = (unsigned int)it->second->m_Levels >> 1; texlevel < level; levelsize += it->second->GetSizeForLevel(texlevel++));
            memoryusage += texlevel;
        }
    }

    LogDump::LogA("Current texture memory usage = %i\n", memoryusage);
}

void Texture::InitTexturesMap()
{
    TexturesMap = new std::map<unsigned int, Texture*>;
}