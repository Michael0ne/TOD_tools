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

    struct DXT1Texture
    {
        struct Color
        {
            union
            {
                struct
                {
                    //unsigned char   Blue : 5;
                    //unsigned char   Green : 6;
                    //unsigned char   Red : 5;
                    unsigned char   Blue : 5;
                    unsigned char   GreenLow : 3;
                    unsigned char   GreenHigh : 3;
                    unsigned char   Red : 5;
                }               m_ColorBits;
                unsigned short  m_Color;
            };

            Color(const unsigned int clr)
            {
                m_Color = (unsigned short)clr;
            }

            inline unsigned char   GetNormalGreenColor() const
            {
                return (m_ColorBits.GreenHigh << 3) | m_ColorBits.GreenLow;
            }
        };

        struct BitmapWord
        {
            union
            {
                struct
                {
                    unsigned char   _00 : 2;
                    unsigned char   _01 : 2;
                    unsigned char   _02 : 2;
                    unsigned char   _03 : 2;

                    unsigned char   _10 : 2;
                    unsigned char   _11 : 2;
                    unsigned char   _12 : 2;
                    unsigned char   _13 : 2;

                    unsigned char   _20 : 2;
                    unsigned char   _21 : 2;
                    unsigned char   _22 : 2;
                    unsigned char   _23 : 2;

                    unsigned char   _30 : 2;
                    unsigned char   _31 : 2;
                    unsigned char   _32 : 2;
                    unsigned char   _33 : 2;
                }               m_WordBits;
                unsigned short  m_Word;
            };
        };

        Color   ColorArr[2];
        BitmapWord  WordArr;

        inline Color    GetColor() const
        {
            return (Color)((ColorArr[0].m_Color + ColorArr[1].m_Color) / 2);
        }
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

    enum TextureFormat
    {
        DXT1 = 7,
        DXT2 = 8,
        DXT3 = 9,
        DXT4 = 10,
        DXT5 = 11
    };

    struct GfxTexture
    {
        struct SurfaceMutable  *m_Mutable;
        struct IDirect3DTexture9*   m_Texture;
        char                   *m_TextureSurfaceBits;
        unsigned int            m_Resolution[2];
        unsigned int            m_SurfaceSize[2];
        TextureFormat           m_Format;
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
    TextureResourceReader(const char* const filename, PlatformDefinition platform = PC);
    ~TextureResourceReader();

    virtual void	ReadInfo() override;
    virtual void	PrintInfo() const override;
    virtual void    DumpData() const override;
};

static_assert(sizeof(TextureResourceReader::DXT1Texture) == 8, "Wrong size for DXT1Texture class!");