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

    struct DDS_HEADER
    {
        unsigned int	size;
        enum DDSFLAGS
        {
            DDSD_CAPS = 0x1,
            DDSD_HEIGHT = 0x2,
            DDSD_WIDTH = 0x4,
            DDSD_PITCH = 0x8,
            DDSD_PIXELFORMAT = 0x1000,
            DDSD_MIPMAPCOUNT = 0x20000,
            DDSD_LINEARSIZE = 0x80000,
            DDSD_DEPTH = 0x800000
        }               flags;
        unsigned int	height;
        unsigned int	width;
        unsigned int	pitchOrLinearSize;
        unsigned int	depth;
        unsigned int	mipMapCount;
        unsigned int	reserved[11];

        struct DDS_PIXELFORMAT
        {
            unsigned int	size;
            enum
            {
                DDPF_ALPHAPIXELS = 1,
                DDPF_ALPHA = 2,
                DDPF_FOURCC = 4,
                DDPF_RGB = 64,
                DDPF_YUV = 512,
                DDPF_LUMINANCE = 131072
            }               flags;
            char			fourcc[4];
            unsigned int	RGBBitCount;
            unsigned int	RBitMask;
            unsigned int	GBitMask;
            unsigned int	BBitMask;
            unsigned int	ABitMask;
        }				ddspf;

        enum DDSCAPS
        {
            DDSCAPS_COMPLEX = 0x8,
            DDSCAPS_MIPMAP = 0x400000,
            DDSCAPS_TEXTURE = 0x1000
        }               caps;
        unsigned int	caps2;
        unsigned int	caps3;
        unsigned int	caps4;
        unsigned int	reserved2;

        static const unsigned int magick;
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