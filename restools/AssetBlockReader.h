#pragma once
#include "GenericResourceReader.h"

#define RESTORE_POINTER(type, ptr, field) \
    ptr->field = (type*)((int)ptr + offsetof(type, field) + (int)ptr->field)

#define READ_FIELD_VALUE(field, fieldType, ptr) \
    field = **(fieldType**)ptr; \
    *ptr += sizeof(fieldType)

#define READ_FIELD_VALUE_POINTER(field, fieldType, ptr) \
    field = (fieldType*)(*ptr + **(uint32_t**)ptr); \
    *ptr += sizeof(uint32_t)

#define READ_LIST_VALUE(field, ptr) \
    *(uint32_t*)field = (uint32_t)(*ptr + **(uint32_t**)ptr); \
    *ptr += sizeof(uint32_t); \
    *(uint32_t*)((uint32_t)field + 4) = **(uint32_t**)ptr; \
    *ptr += sizeof(uint32_t); \
    *(uint32_t*)((uint32_t)field + 8) = **(uint32_t**)ptr; \
    *ptr += sizeof(uint32_t); \
    *(uint32_t*)((uint32_t)field + 12) = **(uint32_t**)ptr; \
    *ptr += sizeof(uint32_t); \

#define ALIGN_4BYTES(x) ((uint32_t)(x) & 0xFFFFFFFC)
#define ALIGN_4BYTESUP(x) ((uint32_t)(x + 3) & 0xFFFFFFFC)
#define ALIGN_8BYTESUP(x) ((uint32_t)(x + 7) & 0xFFFFFFF8)
#define ALIGN_16BYTESUP(x) ((uint32_t)(x + 15) & 0xFFFFFFF8)

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

    struct vec2
    {
        float           x;
        float           y;
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

    enum tAssetType
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
        tAssetType  AssetType;
        char       *Name;
        uint32_t    GlobalId;
        uint32_t    field_C;
        UINT64      EngineTimestamp;
        uint32_t    Flags;

        CompiledAsset(unsigned char** infobuffer);
        
        virtual void    PrintInfo() const;
        virtual void    SkipNameRead(unsigned char** infobuffer);
        virtual void    SkipSpecificData(unsigned char** infobuffer) = 0;
        virtual void    SkipEndAlignment(uint8_t** infobuffer);
        virtual void    DumpData(const AssetBlockReader* reader) = 0;
    };

    struct CompiledTextureAsset : CompiledAsset
    {
        static const char* const    TextureFormatString[];

        enum DDS_FLAGS
        {
            DDSD_CAPS = 0x1,
            DDSD_HEIGHT = 0x2,
            DDSD_WIDTH = 0x4,
            DDSD_PITCH = 0x8,
            DDSD_PIXELFORMAT = 0x1000,
            DDSD_MIPMAPCOUNT = 0x20000,
            DDSD_LINEARSIZE = 0x80000,
            DDSD_DEPTH = 0x800000
        };

        enum DDS_PIXELFORMAT
        {
            DDPF_ALPHAPIXELS = 1,
            DDPF_FOURCC = 4,
            DDPF_RGB = 64,
            DDPF_YUV = 512,
            DDPF_LUDDPF_ALPHA = 2,
            DMINANCE = 131072
        };

        struct DDS_HEADER
        {
            unsigned int	size;
            unsigned int    flags;
            unsigned int	height;
            unsigned int	width;
            unsigned int	pitchOrLinearSize;
            unsigned int	depth;
            unsigned int	mipMapCount;
            unsigned int	reserved[11];

            struct DDS_PIXELFORMAT
            {
                unsigned int	size;
                unsigned int    flags;
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
            int                 m_Format;
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

        //  Utility.
        int             m_FileSize;
        char           *m_DataBuffer;

        CompiledTextureAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer);
        virtual void    DumpData(const AssetBlockReader* reader);
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
            int				field_4;    //  NOTE: maybe bits per sample.
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
            short			field_38;
            short           field_3A;
            CompiledTextureAsset::GfxTexture   *m_FontTexture;

            Glyph          *m_GlyphsList;
            unsigned int    m_GlyphsInList;
            int             field_48;
            int             field_4C;

            int            *m_GlyphsMap;
            int				field_54;
            float			m_GlyphsTotalWidth;
            float			field_5C;
            float			field_60;
        };

        unsigned int    field_1C;
        unsigned int   *field_20;
        Font           *m_FontInfo;

        //  Utility.
        int             m_FileSize;
        char           *m_DataBuffer;

        CompiledFontAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
        virtual void    DumpData(const AssetBlockReader* reader);
    };

    struct CompiledTextAsset : CompiledAsset
    {
        struct Dictionary
        {
            Dictionary         *m_Next;
            Dictionary         *m_Previous;
            unsigned short      m_Contents;

            static Dictionary*  GetCharacterInfo(Dictionary* dic);  //  @861760

            static int Offset;  //  @A3CE84
            static unsigned char* Indicy;   //  @A3CE80
        };

        unsigned int    field_1C;
        unsigned int   *m_List_1_Elements;
        unsigned int    m_List_1_Size;
        unsigned int    field_28[2];

        unsigned short *m_TextIndicies_Elements;
        unsigned int    m_TextIndicies_Size;
        unsigned int    field_38[2];

        unsigned char  *m_List_3_Elements;
        unsigned int    m_List_3_Size;
        unsigned int    field_48[2];

        Dictionary     *m_CharactersMap;
        unsigned int    field_54;

        CompiledTextAsset(unsigned char** infobuffer);

        void            GetGameString(const unsigned short indicieslistindex, unsigned char* outString, unsigned int* maxlength, const bool contents) const;

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
        virtual void    DumpData(const AssetBlockReader* reader);
    };

    struct CompiledModelAsset : CompiledAsset
    {
        struct TextureReference
        {
            int32_t        *TextureAsset;
            char           *Name;
        };

        struct Face
        {
            vec4            Position;
            vec2            TexCoordU;
            vec2            TexCoordV;
            vec4            Normal;
            vec4            Color;
            uint32_t        field_40;
            uint32_t        field_44;
            uint32_t        field_48;
            uint32_t        field_4C;
            float_t         field_50;
            float_t         field_54;
            float_t         field_58;
            float_t         field_5C;
            uint32_t        field_60;
        };

        struct BaseMesh
        {
            Face           *FacesList;
            size_t          FacesListSize;
            uint32_t        field_8[2];

            uint16_t       *IndiciesList;
            size_t          IndiciesListSize;
            uint32_t        field_18[2];

            uint32_t       *field_20;
            size_t          field_24;
            uint32_t        field_28[2];

            uint32_t       *field_30;
            size_t          field_34;
            uint32_t        field_38[2];

            uint32_t       *field_40;
            size_t          field_44;
            uint32_t        field_48[2];

            uint32_t        Flags;
            uint32_t        IsTrianglesList;
            uint32_t        field_58;
            vec4            field_5C;
        };

        struct VertexBuffer
        {
            size_t          Capacity;
            size_t          ListSize;
            uint32_t        FVF;
            uint32_t        Length;
            char           *Buffer;
            uint16_t        Stride;
            uint32_t        field_16;
            uint32_t        FVFIndex;
            uint32_t        Flags;
            uint32_t        LockMode;
            void           *D3DVertexBuffer;
        };

        struct IndexBuffer
        {
            uint32_t        ListSize;
            uint32_t        PrimitiveTypeIndex;
            uint32_t        PrimitiveType;
            char           *Buffer;
            uint32_t        field_14;
            uint32_t        LockMode;
            uint32_t        Flags;
            void           *D3DIndexBuffer;
        };

        struct MeshBuffer
        {
            BaseMesh       *Mesh;
            uint32_t        field_4;
            uint32_t        field_8;
            VertexBuffer   *VertexBufferPtr;
            IndexBuffer    *IndexBufferPtr;
            String          field_14;
            vec4            field_24;
            vec4            field_34;
        };

        struct SkinnedMesh
        {
            MeshBuffer         *MeshBuffer;
            BaseMesh           *Mesh;

            TextureReference   *TextureSetsReferencesList;
            size_t              TextureSetsReferencesListSize;
            uint32_t            field_10[2];

            uint32_t           *TextureSetsList;
            size_t              TextureSetsListSize;
            uint32_t            field_20[2];

            uint32_t           *EnvMapCoefficientsList;
            size_t              EnvMapCoefficientsListSize;
            uint32_t            field_30[2];

            uint32_t            Flags;
        };

        struct Mesh
        {
            vec3            Position;
            vec4            Orientation;
            char           *Name;
            char           *field_20;

            SkinnedMesh    *SkinnedMeshesList;
            size_t          SkinnedMeshesListSize;
            uint32_t        field_2C[2];

            uint32_t       *SurfaceProperties;
            size_t          SurfacePropertiesSize;
            uint32_t        field_3C[2];
            int32_t        *field_44;   //  NOTE: ptr to list.

            uint32_t       *FacesToSplit;
            size_t          FacesToSplitSize;
            uint32_t        field_50[2];

            int32_t         ParentIndex;
            uint32_t        field_5C;
            uint32_t        field_60;
            float_t         field_64;
            float_t         field_68;
            float_t         field_6C;
            uint32_t        field_70;
            uint32_t        field_74;
            uint32_t        field_78;
        };

        uint32_t            field_1C;

        TextureReference   *TextureReferencesList;
        size_t              TextureReferencesListSize;
        uint32_t            field_28[2];

        Mesh               *MeshList;
        size_t              MeshListSize;
        uint32_t            field_38[2];

        uint32_t           *PhysAttachmentsList;
        vec4                BoundingRadius;
        uint32_t           *field_54;
        uint32_t           *field_58;
        uint8_t             field_5C;

        CompiledModelAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
        virtual void    DumpData(const AssetBlockReader* reader);
    };

    struct CompiledFragmentAsset : CompiledAsset
    {
        unsigned int    field_1C;
        unsigned int   *field_20;   //  NOTE: some dictionary...
        unsigned int    field_24;

        CompiledFragmentAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
        virtual void    DumpData(const AssetBlockReader* reader);
    };

    struct CompiledMovieAsset : CompiledAsset
    {
        unsigned int    field_1C;
        unsigned int    field_20;
        unsigned int    field_24;

        CompiledMovieAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    DumpData(const AssetBlockReader* reader);
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
        virtual void    DumpData(const AssetBlockReader* reader);
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
        virtual void    DumpData(const AssetBlockReader* reader);
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
        virtual void    DumpData(const AssetBlockReader* reader);
    };

    struct CompiledAnimationAsset : CompiledAsset
    {
        struct AnimationPivotData
        {
            uint32_t        field_0;
            uint32_t        field_4;
            uint32_t        field_8;
        };

        struct List1Data
        {
            uint32_t        field_0;
            uint32_t        field_4;
            uint32_t        field_8;
            uint32_t        field_C;
            uint32_t        field_10;
            uint32_t        field_14;
        };

        struct List2Data
        {
            uint32_t        field_0;
            uint32_t        field_4;
            uint32_t        field_8;
            uint32_t        field_C;
            uint32_t        field_10;
            uint32_t        field_14;
            uint32_t        field_18;
            uint32_t        field_1C;
            uint32_t        field_20;
            uint32_t        field_24;
            uint32_t        field_28;
            uint32_t        field_2C;
            uint32_t        field_30;
        };

        uint32_t            field_1C;
        uint32_t            field_20;
        uint32_t            LoopMode;
        uint32_t            field_28;

        List1Data          *List_1;
        uint32_t            List1_Size;
        uint32_t            field_34;
        uint32_t            field_38;

        List2Data          *List_2;
        uint32_t            List2_Size;
        uint32_t            field_44;
        uint32_t            field_48;

        uint8_t            *List_3;
        uint32_t            List3_Size;
        uint32_t            field_54;
        uint32_t            field_58;

        AnimationPivotData *field_5C;
        uint32_t            field_60;
        uint16_t            field_64;
        uint16_t            field_66;

        CompiledAnimationAsset(unsigned char** infobuffer);

        virtual void    PrintInfo() const override;
        virtual void    SkipSpecificData(unsigned char** infobuffer) override;
        virtual void    DumpData(const AssetBlockReader* reader);
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
        virtual void    DumpData(const AssetBlockReader* reader);
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
    mutable std::vector<CompiledAsset*> m_AssetsList;

public:
    AssetBlockReader(LPCSTR filename, LPCSTR ext = ".main");
    ~AssetBlockReader();

    virtual void	ReadInfo() override;
    virtual void	PrintInfo() const override;
    virtual void	DumpData() const override;
};