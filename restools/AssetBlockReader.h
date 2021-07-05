#pragma once
#include "GenericResourceReader.h"

class AssetBlockReader : public GenericResourceReader
{
public:
    struct String
    {
        unsigned int    m_Length;
        char           *m_String;
        unsigned int    m_BitMask;
        unsigned char   m_ShortString;
    };

    struct vec3
    {
        float           x;
        float           y;
        float           z;
    };

    struct vec4
    {
        float           x;
        float           y;
        float           z;
        float           w;
    };

    enum AssetType
    {
        TEXTURE = 0,
        FONT,
        TEXT,
        MODEL,
        FRAGMENT,
        MOVIE,
        CUTSCENE,
        SOUND,
        STREAMEDSOUNDINFO,
        ANIMATION,
        MESHCOLOR
    };

    static const char*	AssetTypeIndexString[];
    static unsigned int	AssetInstanceSize[];
    static unsigned int	AssetTypeAlignment[][3];

    //	Compiled assets structs.
    //	NOTE: it's basically same as regular assets structs that are in Kapow, but instead of pointers we have their relative positions.
    //  TODO: merge derived classes with Kapow's.
    struct CompiledAsset
    {
        AssetType   m_AssetType;
        char       *m_AssetName;
        unsigned int    m_AssetGlobalId;
        unsigned int    field_C;
        UINT64      m_EngineTimestamp;
        unsigned int    m_Flags;

        CompiledAsset(unsigned char** infobuffer);
        
        virtual void    PrintInfo() const;
        virtual void    SkipNameRead(unsigned char** infobuffer);
        virtual void    SkipAlignment(unsigned char** infobuffer);
        virtual void    SkipSpecificData(unsigned char** infobuffer) = 0;
    };

    struct CompiledTextureAsset : CompiledAsset
    {
        enum TextureFormat
        {
            DXT1 = 7,
            DXT2 = 8,
            DXT3 = 9,
            DXT4 = 10,
            DXT5 = 11
        };

        static const char* const    TextureFormatString[];

        struct TextureInfo
        {
            int         field_0;
            int         field_4;
            int         field_8;
            int         field_C;
            int         field_10;
            int         field_14;
            int         field_18;
            int         field_1C;
            int         field_20;
            int         field_24;
            int         field_28;
            int         field_2C;
            int         field_30;
            int         field_34;
            int         field_38;
            int         field_3C;
            int         field_40;
            int         field_44;
            int         field_48;
            int         field_4C;
            int         field_50;
        };

        struct GfxTexture
        {
            struct SurfaceMutable      *m_Mutable;
            struct IDirect3DTexture9   *m_Texture;
            char               *m_TextureSurfaceBits;
            unsigned int        m_Resolution[2];
            unsigned int        m_SurfaceSize[2];
            TextureFormat       m_Format;
            int                 field_20;
            unsigned short      m_Levels;
            unsigned short      field_26;   //  NOTE: not used, only here for alignment.
            unsigned char       m_MipMapLevels;
        };

        unsigned int	m_BitsPerPixel;
        TextureInfo    *m_TextureInfo;
        unsigned int	field_24;
        GfxTexture     *m_GfxTexture;
        unsigned int	field_2C;

        CompiledTextureAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer);
    };

    struct CompiledFontAsset : CompiledAsset
    {
        struct Glyph
        {
            unsigned char   m_Character;
            float           m_X;
            float           m_Y;
            float           m_Width;
            float           m_Height;
            float           m_X1;
            float           m_X2;
            int             field_1C;
        };

        struct Font
        {
            int				field_0;
            int				field_4;
            int				field_8;
            int				field_C;
            int				field_10;
            float			m_HorizontalSpacing;
            float			m_VerticalSpacing;
            float			m_ScaleX;
            float			m_ScaleY;
            float			field_24;
            float			field_28;
            float			field_2C;
            int				field_30;
            float			field_34;
            int				field_38;
            CompiledTextureAsset::GfxTexture   *m_FontTexture;
            Glyph          *m_GlyphsList;
            unsigned int    m_GlyphsInList;
            int             field_48;
            int             field_4C;
            int				field_50;
            int				field_54;
            float			field_58;
            float			field_5C;
            float			field_60;
        };

        unsigned int    field_1C;
        unsigned int   *field_20;
        Font           *m_FontInfo;

        CompiledFontAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipAlignment(unsigned char** infobuffer) override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
    };

    struct CompiledTextAsset : CompiledAsset
    {
        struct Dictionary
        {
            Dictionary         *field_0;
            Dictionary         *field_4;
            unsigned short      field_8;
        };

        unsigned int    field_1C;
        unsigned int   *m_List_1_Elements;
        unsigned int    m_List_1_Size;
        unsigned int    field_28[2];

        unsigned int   *m_TextIndicies_Elements;
        unsigned int    m_TextIndicies_Size;
        unsigned int    field_38[2];

        unsigned int   *m_List_3_Elements;
        unsigned int    m_List_3_Size;
        unsigned int    field_48[2];

        Dictionary     *field_50;
        unsigned int    field_54;

        CompiledTextAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipAlignment(unsigned char** infobuffer) override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
    };

    struct CompiledModelAsset : CompiledAsset
    {
        struct TextureReference
        {
            unsigned int    field_0;
            char           *m_TextureName;
        };

        struct SkinnedMeshBuffer;

        struct Mesh
        {
            vec3            m_Position;
            float           field_C;
            float           field_10;
            float           field_14;
            float           field_18;
            char           *m_Name;
            char           *field_20;
            SkinnedMeshBuffer  *m_SkinnedMeshBuffer;
            int             m_TotalMeshes;
            int             field_2C;
            int             field_30;
            char           *field_34;
            int             field_38;
            int             field_3C;
            int             field_40;
            int             field_44;
            char           *field_48;
            int             field_4C;
            int             field_50;
            int             field_54;
            int             m_ParentPivotIndex; //  NOTE: or 'NextPivotIndex'.
            int             field_5C;
            int             field_60;
            float           field_64;
            float           field_68;
            float           field_6C;
            int             field_70;
            int             field_74;
            int             field_78;
        };

        unsigned int    field_1C;

        TextureReference* m_TextureResources_Elements;
        unsigned int    m_TextureResources_Size;
        int             field_28[2];

        Mesh           *m_MeshList_Elements;
        unsigned int    m_MeshList_Size;
        int             field_38[2];

        unsigned int   *field_40;
        vec4            m_BoundingRadius;
        unsigned int    field_54;
        unsigned int   *field_58;
        unsigned char   field_5C;

        CompiledModelAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipAlignment(unsigned char** infobuffer) override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
    };

    struct CompiledFragmentAsset : CompiledAsset
    {
        unsigned int    field_1C;
        unsigned int   *field_20;
        unsigned int    field_24;

        CompiledFragmentAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
    };

    struct CompiledMovieAsset : CompiledAsset
    {
        unsigned int    field_1C;
        unsigned int    field_20;
        unsigned int    field_24;

        CompiledMovieAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
    };

    struct CompiledCutsceneAsset : CompiledAsset
    {
        unsigned int    field_1C;
        std::vector<int>    m_List_1;
        std::vector<int>    m_List_2;
        unsigned int        field_40;
        unsigned int        field_44;
        unsigned int        field_48;
        unsigned int        m_String_1[4];
        unsigned int        field_5C;

        CompiledCutsceneAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
    };

    struct CompiledSoundAsset : CompiledAsset
    {
        struct StreamedWAV;

        struct StreamBuffer
        {
            unsigned int            field_0;
            unsigned int			m_SamplesPerSec;
            char					m_Channels;
            char					field_9;
            char					field_A;
            char					field_B;
            unsigned int			m_TotalChunks;
            unsigned int			m_SampledDataSize;
            float					field_14;
            float					m_BytesPerSec;
            int						field_1C;
            float					field_20;
            char                   *m_SampledData;
            String                 *m_SoundName;
            int                     field_2C[2];
            int						field_34;
            StreamedWAV            *m_AuxMonoStream;
            int                    *m_List_Elements;
            int                     m_List_Size;
            int                     field_44[2];
        };

        unsigned int        field_1C;
        StreamBuffer       *m_StreamBuffer;
        unsigned int        field_24;

        CompiledSoundAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
        virtual void    SkipAlignment(unsigned char** infobuffer) override;
    };

    struct CompiledStreamedSoundInfoAsset : CompiledAsset
    {
        struct StreamedWAV;

        struct SoundFile
        {
            int             m_WavChunkSize;
            int             field_4;
            int             m_Samples;
            float           m_BytesPerSample;
            int             m_AverageBytesPerSec;
            int             m_BlockAlign;
            int             m_Frequency;
            int             m_ChunkSize;
            int             m_SoundFormat;
            int             field_24;
            String          m_FileName;
            StreamedWAV    *m_StreamedWAV;
        };

        unsigned int        field_1C;
        SoundFile          *m_SoundFile;
        unsigned int        field_24;

        CompiledStreamedSoundInfoAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
        virtual void    SkipAlignment(unsigned char** infobuffer) override;
    };

    struct CompiledAnimationAsset : CompiledAsset
    {
        unsigned int        field_1C;
        unsigned int        field_20;
        unsigned int        field_24;
        unsigned int        field_28;

        char               *m_List_1_Elements;
        unsigned int        m_List_1_Size;
        unsigned int        field_34;
        unsigned int        field_38;

        char               *m_List_2_Elements;
        unsigned int        m_List_2_Size;
        unsigned int        field_44;
        unsigned int        field_48;

        char               *m_List_3_Elements;
        unsigned int        m_List_3_Size;
        unsigned int        field_54;
        unsigned int        field_58;

        unsigned int        field_5C;
        unsigned int        field_60;
        unsigned short      field_64;
        unsigned short      field_66;

        CompiledAnimationAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
    };

    struct CompiledMeshColorAsset : CompiledAsset
    {
        unsigned int        field_1C;
        std::vector<int>    m_List_1;
        std::vector<int>    m_List_2;
        unsigned int        field_40;
        unsigned int        field_44;
        unsigned int        field_48;
        unsigned int        field_4C;

        CompiledMeshColorAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
    };

    //  NOTE: this header is shared between all asset blocks (localised/non-localised).
    struct AssetHeaderShared
    {
        unsigned int	m_EngineTimestamp;
        unsigned int	m_PropertyChecksum;
        unsigned int	m_CommandsChecksum;
        int				m_ResourcesTotal;
        unsigned int	m_AssetsHeaderSize;
        unsigned int	m_MaxBufferSize;
    };

    AssetHeaderShared	m_SharedHeader;
    char*				m_AssetsInfoBuffer;
    char**				m_AssetsDataBuffer;
    char**				m_AssetsNames;
    int*				m_AssetsSizes;

public:
    AssetBlockReader(LPCSTR filename);
    ~AssetBlockReader();

    virtual void	ReadInfo() override;
    virtual void	PrintInfo() const override;
    virtual void	DumpData() const override;
};