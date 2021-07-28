#pragma once
#include "GenericResourceReader.h"

class TextureResourceReader : public GenericResourceReader
{
public:
    struct vec2
    {
        unsigned int    w, h;

        vec2(unsigned w_, unsigned h_)
            : w(w_), h(h_)
        {};
    };

private:
    struct AssetHeader
    {
        unsigned int			m_Timestamp;
        unsigned int            m_AssetStructSize;
        unsigned int            m_AssetDataSize;
        bool                    m_IsLocalised;
        unsigned int            m_LocalisationStringLength;
        char                   *m_LocalisationString;
    };

    struct TextureInfo
    {
        int                     field_0;
        int                     field_4;
        int                     field_8;
        int                     field_C;
        int                     field_10;
        int                     field_14;
        int                     field_18;
        int                     field_1C;
        int                     field_20;
        int                     field_24;
        int                     field_28;
        int                     field_2C;
        int                     field_30;
        int                     field_34;
        int                     field_38;
        int                     field_3C;
        int                     field_40;
        int                     field_44;
        int                     field_48;
        int                     field_4C;
        int                     field_50;
    };

    struct GfxTexture
    {
        struct SurfaceMutable  *m_Mutable;
        struct IDirect3DTexture9*   m_Texture;
        char                   *m_TextureSurfaceBits;
        unsigned int            m_Resolution[2];
        unsigned int            m_SurfaceSize[2];
        int                     m_Format;
        char                    field_20;
        char                    field_21;
        char                    m_BitsPerPixel;
        char                    field_23;
        unsigned short          m_Levels;
        unsigned short          field_26;   //  NOTE: not used, only here for alignment.
        unsigned char           m_MipMapLevels;

        unsigned int            GetSizeForLevel(const unsigned char level) const;
        const vec2              GetSize(const unsigned char level) const;
    };

    AssetHeader                 m_Header;
    char                       *m_AssetStruct;
    char                       *m_AssetData;
    mutable TextureInfo        *m_TextureInfo;
    mutable GfxTexture         *m_GfxTexture;
    mutable unsigned int        m_BitsPerPixel;

    static const std::string	PlatformExtension[];
    static const char* const    AssetTypeString[];
    static const char* const    TextureFormatString[];
public:
    TextureResourceReader(const char* const filename);
    TextureResourceReader(const char* const filename, PlatformDefinition platform);
    ~TextureResourceReader();

    virtual void	ReadInfo() override;
    virtual void	PrintInfo() const override;
    virtual void    DumpData() const override;
};