#pragma once
#include "Types.h"
#include "StringsPool.h"
#include <vector>

namespace ResType
{
#define RESTYPE_MAX_OPEN_RESOURCES 10

#define RESTYPE_TEXTURE_NAME "texture"
#define RESTYPE_TEXTURE_EXT "bmp"

#define RESTYPE_FONT_NAME "font"
#define RESTYPE_FONT_EXT_1 "ttf"
#define RESTYPE_FONT_EXT_2 "font"

#define RESTYPE_TEXT_NAME "textres"
#define RESTYPE_TEXT_EXT "txt"

#define RESTYPE_MODEL_NAME "modelres"
#define RESTYPE_MODEL_EXT "model"

#define RESTYPE_FRAGMENT_NAME "fragment"
#define RESTYPE_FRAGMENT_EXT_1 "fragment"
#define RESTYPE_FRAGMENT_EXT_2 "scene"

#define RESTYPE_MOVIE_NAME "movie"
#define RESTYPE_MOVIE_EXT_1 "wmv"
#define RESTYPE_MOVIE_EXT_2 "bik"

#define RESTYPE_CUTSCENE_NAME "cutscene"
#define RESTYPE_CUTSCENE_EXT "cutscene"

#define RESTYPE_SOUND_NAME "sound"
#define RESTYPE_SOUND_EXT "wav"

#define RESTYPE_STREAMEDSOUNDINFO_NAME "streamedsoundinfo"
#define RESTYPE_STREAMEDSOUNDINFO_EXT_1 "stream"
#define RESTYPE_STREAMEDSOUNDINFO_EXT_2 "ogg"

#define RESTYPE_ANIMATION_NAME "animation"
#define RESTYPE_ANIMATION_EXT "animation"

#define RESTYPE_MESHCOLOR_NAME "meshcolor"
#define RESTYPE_MESHCOLOR_EXT_1 "meshcolor"
#define RESTYPE_MESHCOLOR_EXT_2 "lighting"

	enum PlatformId
	{
		PLATFORM_PC = 0,
		PLATFORM_PS2 = 1,
		PLATFORM_XBOX = 2
	};

	enum ResourceBlockTypeNumber
	{
		RESTYPE_NONE = 0,
		RESTYPE_MAP = 1,
		RESTYPE_SUBMAP = 1,
		RESTYPE_MISSION = 2,
		RESTYPE_CUTSCENE = 3,
		RESTYPE_PLAYERDATA = 4
	};

	enum BlockTypeNumber
	{
		UNKNOWN = -1,
		NONE = 0,
		MAP = 1,
		SUBMAP = 2,
		MISSION = 3,
		CUTSCENE = 4,
		PLAYERDATA = 5,
		MAIN = 6
	};

	extern const char*	BlockTypeExtension[7];	//	@A11B64

	class ResourceBase
	{
	public:
		class Resource*	(__cdecl* m_Creator)();
		String			m_ResourceTypeName;
		void*			m_ResTypeMethods;
		int				m_ResourceIndex;
		std::vector<String>	m_ResourceExtensionsList;
		char			field_2C;
		bool			m_VerifyChecksum;
		unsigned int	m_Alignment[3];

	public:
		ResourceBase() {};
		ResourceBase(const char*, Resource* (*)());	//	@852440

		void			SetResourceAlignment(unsigned int size, unsigned int index);	//	@852160

		static void		AllocateResourceBlockBufferAligned(unsigned int pos, int** resBufStartPos, int** resBufSpace, BlockTypeNumber resblockid);	//	@852070
		static ResourceBlockTypeNumber GetResourceBlockTypeNumber(BlockTypeNumber resblockid);	//	@851FE0
	};

	extern unsigned int	ResourceAlignment[3];	//	@A3BE1C
	extern std::vector<ResourceBase*>	ResTypeList;	//	@A10F80

	ASSERT_CLASS_SIZE(ResourceBase, 60);

	struct ResourceHolder
	{
		class Resource* m_Resource;
		unsigned int	m_Status;	//	NOTE: 1 - loaded, to be loaded; 0 - ?

		ResourceHolder() {};
		~ResourceHolder() {};	//	NOTE: looks like always inlined.

		void			LoadResourceFromBlock(const char* _pathname) {};	//	@8FFC10
	};

	//	NOTE: this class is actually inherited from another class, but parent doesn't seem to do anything important, so skipping it now.
	#pragma pack(4)
	class Resource
	{
	public:
		typedef Resource* (*CREATOR)();

		const char*		m_ResourcePath;
		int				m_GlobalResourceId;	//	NOTE: this is an index for Blocks global 'ResourceTypeList'.
		int				field_C;
		UINT64			m_ResourceTimestamp;
		int				m_Flags;

	public:
		virtual			~Resource();	//	@851F90 scalar, actual dtor @8516C0
		virtual Resource* GetInstancePtr() const { return nullptr; };	//	FIXME: this is pure virtual, but List class needs this class to be non-abstract, so this is it for now.
		virtual void	SetUnkFlag(unsigned char, int, int);
		virtual int		GetUnkFlag() const;
		virtual void	stub5(int) {};
		virtual void	GetResourcesDir(String& outDir, PlatformId platformId);
		virtual void	ApplyAssetData(int*) {};
		virtual char	SetResourcePlaceholder() { return NULL; };
		virtual int		stub9();	//	NOTE: retrieves some unknown flag.
		virtual void	GetResourceName(String& outName, int);
		virtual void	LoadResource(const char* resPath) {};
		virtual void	DestroyResource();
		virtual int		stub13() { return NULL; };

		Resource() {};
		Resource(bool);	//	@851D00

		const char*		AddResToOpenListAndReturnName();	//	@851720
		void			_8513E0(unsigned char);	//	@8513E0
		void			ApplyLoadedResource(ResourceHolder&);
		
		static void		Destroy(Resource* res);	//	@851FC0

		static unsigned int	LastOpenResourceIndex;	//	@A3BE14
	};

	static unsigned int TotalResourcesCreated = NULL;	//	@A3BE10
	static std::vector<String>	OpenResourcesList;	//	@A10F00

	ASSERT_CLASS_SIZE(Resource, 28);

	class Texture : public Resource
	{
		struct Tex
		{
			unsigned int	field_0;
			unsigned int	field_4;
			unsigned short	field_8;
			unsigned char	field_A;
			String			field_E;
			unsigned int	field_12;
			unsigned int	field_16;
			unsigned int	field_1A;
			unsigned int	field_1E;
			unsigned short	field_22;
			unsigned int	field_26;
			unsigned int	field_2A;
			unsigned int	field_2E;
			unsigned int	field_32;
			unsigned int	field_36;
			unsigned int	field_3A;
			unsigned int	field_3E;
			unsigned int	field_42;
		};
	protected:
		int				field_1C;
		Tex*			field_20;
		int				field_24;
		class GfxInternal_Dx9_Texture* m_Texture;
		int				field_2C;

	private:
		inline Texture();	//	NOTE: always inlined.

	public:
		virtual ~Texture() = default;
		virtual Resource* GetInstancePtr() const override;

		void			GetTextureResolution(Vector2<int>& outRes);	//	@853650

		static void		CreateInstance();	//	@853870
		static Texture* Create();	//	@853830
	};

	class Font : public Resource
	{
	protected:
		int				field_1C;
		int				field_20;
		int				field_24;

	private:
		inline Font();	//	NOTE: always inlined.

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@85B460
		static Font*	Create();	//	@85B350
	};

	class Text : public Resource
	{
	protected:
		int				field_1C;
		int				m_List_1[4];
		int				m_List_2[4];
		int				m_List_3[4];
		int				field_50;
		int				field_54;

	private:
		inline Text();	//	NOTE: always inlined.

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@861CE0
		static Text*	Create();	//	@861BD0
	};

	class Model : public Resource
	{
		struct TextureReference
		{
			Texture*	m_TextureResource;
			unsigned int field_4;
			char*		m_TexturePath;
		};
	protected:
		int				field_1C;
		int				m_TextureResources[4];	//	FIXME: make this actual list!
		int				m_PivotList[4];
		int*			field_40;
		Vector4f		m_BoundingRadius;
		int*			field_54;
		int*			field_58;
		int				field_5C;

	private:
		inline Model();	//	NOTE: always inlined.

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@858210
		static Model*	Create();	//	@8581F0
	};

	class Fragment : public Resource
	{
	protected:
		int				field_1C;
		int*			field_20;
		int				field_24;

	private:
		Fragment();	//	@85DD80
		~Fragment();

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@85DFA0
		static Fragment* Create();	//	@85DE30

		void			ApplyFragmentResource(unsigned int entityId, bool);	//	@85D990
	};

	class Movie : public Resource
	{
	protected:
		int				field_1C;
		int				field_20;
		int				field_24;

	private:
		inline Movie();	//	NOTE: no constructor.

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@85BC70
		static Movie*	Create();	//	@85BC40
	};

	class Cutscene : public Resource
	{
	protected:
		int				field_1C;
		int				m_List_1[4];
		int				m_List_2[4];
		int				field_40;
		int				field_44;
		int				field_48;
		String			m_String_1;
		int				field_5C;

	private:
		Cutscene();	//	@916080

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@9164C0
		static Cutscene* Create();	//	@916100
	};

	class Sound : public Resource
	{
		friend class SoundSlot;
	protected:
		int				field_1C;
		class StreamBuffer*	m_MonoStream;
		int				field_24;

	private:
		Sound();	//	@85C3C0

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@85C010
		static Sound*	Create();	//	@85C430
	};

	class StreamedSoundInfo : public Resource
	{
	protected:
		int				field_1C;
		int*			m_MonoStream;
		int				field_24;

	private:
		StreamedSoundInfo();	//	@85C7E0

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@85C8D0
		static StreamedSoundInfo* Create();	//	@85CDA0
	};

	class Animation : public Resource
	{
	protected:
		int				field_1C;
		int				field_20;
		int				field_24;
		int				field_28;
		int				m_List_1[4];
		int				m_List_2[4];
		int				m_List_3[4];
		int*			field_5C;
		int				field_60;
		short			field_64;
		short			field_66;

	private:
		Animation();	//	@900080

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@900980
		static Animation* Create();	//	@900EF0
	};

	class MeshColor : public Resource
	{
	protected:
		int				field_1C;
		int				m_List_1[4];
		int				m_List_2[4];
		int*			field_40;
		int*			field_44;
		int*			field_48;
		int*			field_4C;

	private:
		MeshColor();	//	@85E7F0

	public:
		virtual Resource* GetInstancePtr() const override;

		static void		CreateInstance();	//	@85E970
		static MeshColor* Create();	//	@85E950
	};

	static ResourceBase*		rtTexture;
	static ResourceBase*		rtFont;
	static ResourceBase*		rtText;
	static ResourceBase*		rtModel;
	static ResourceBase*		rtFragment;
	static ResourceBase*		rtMovie;
	static ResourceBase*		rtCutscene;
	static ResourceBase*		rtSound;
	static ResourceBase*		rtStreamedSoundInfo;
	static ResourceBase*		rtAnimation;
	static ResourceBase*		rtMeshColor;
}