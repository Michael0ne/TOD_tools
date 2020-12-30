#pragma once

#include "List.h"
#include "Types.h"

namespace ResType
{
#define RESTYPE_CLASS_SIZE 60
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

	class Base
	{
	public:
		void* (__cdecl* m_Creator)();
		String			m_ResourceTypeName;
		void*			m_ActualResourceType;
		int				m_ResourceIndex;
		List<String>	m_ResourceExtensionsList;
		char			field_2C;
		char			field_2D;
		unsigned int	m_Alignment[3];

	public:
		Base();
		Base(const char* type, void* typePtr);	//	@852440

		inline void		SetResourceAlignment(unsigned int size, unsigned int index);	//	@852160
	};

	static unsigned int	ResourceAlignment[3];	//	@A3BE1C
	static List<Base>	ResTypeList = List<Base>(0x19300);	//	@A10F80

	//	NOTE: this class is actually inherited from another class, but parent doesn't seem to do anything important, so skipping it now.
	class Resource
	{
	protected:
		const char*		m_ResourcePath;
		int				m_GlobalResourceId;	//	NOTE: this is an index for Blocks global 'ResourceTypeList'.
		int				field_C;
		int				field_10;
		int				field_14;
		int				field_18;

	public:
		virtual			~Resource();	//	@851F90 scalar, actual dtor @8516C0
		virtual void*	GetInstancePtr() const { return nullptr; };	//	FIXME: this is pure virtual, but List class needs this class to be non-abstract, so this is it for now.
		virtual void	SetUnkFlag(unsigned char, int, int);
		virtual int		GetUnkFlag();
		virtual void	stub5(int) {};
		virtual void	GetResourcesDir(String& outDir, PlatformId platformId);
		virtual void	stub7(int) {};
		virtual char	SetResourcePlaceholder() { return NULL; };
		virtual int		stub9();	//	NOTE: retrieves some unknown flag.
		virtual void	GetResourceName(String& outName, int);
		virtual void	LoadResource(const char* resPath) {};
		virtual void	DestroyResource();
		virtual int		stub13() { return NULL; };

		Resource();	//	NOTE: this is not in EXE, but necessary for List class.
		Resource(bool);	//	@851D00

		const char*		AddResToOpenListAndReturnName();	//	@851720
		void			_8513E0(unsigned char);	//	@8513E0

		static unsigned int	LastOpenResourceIndex;	//	@A3BE14
	};

	static unsigned int TotalResourcesCreated = NULL;	//	@A3BE10
	static List<String>	OpenResourcesList = List<String>(0xC300);	//	@A10F00

	class Texture : Resource
	{
	protected:
		int				field_1C;
		int				field_20;
		int				field_24;
		class GfxInternal_Dx9_Texture* m_Texture;
		int				field_2C;

	public:
		virtual void*	GetInstancePtr() const override;

		Texture();	//	@853830

		void			GetTextureResolution(Vector2<int>& outRes);	//	@853650

		static void		CreateInstance();	//	@853870
	};

	class Font : Resource
	{
	protected:
		int				field_1C;
		int				field_20;
		int				field_24;

	public:
		virtual void*	GetInstancePtr() const override;

		Font();	//	@85B350

		static void		CreateInstance();	//	@85B460
	};

	class Text : Resource
	{
	protected:
		int				field_1C;
		List<int>		m_List_1;
		List<int>		m_List_2;
		List<int>		m_List_3;
		int				field_50;
		int				field_54;

	public:
		virtual void*	GetInstancePtr() const override;

		Text();	//	@861BD0

		static void		CreateInstance();	//	@861CE0
	};

	class Model : Resource
	{
	protected:
		int				field_1C;
		List<int>		m_List_1;
		List<int>		m_List_2;
		int				field_40;
		int				field_44;
		int				field_48;
		int				field_4C;
		int				field_50;
		int				field_54;
		int				field_58;
		int				field_5C;

	public:
		virtual void*	GetInstancePtr() const override;

		Model();	//	@8581F0

		static void		CreateInstance();	//	@858210
	};

	class Fragment : Resource
	{
	protected:
		int				field_1C;
		int*			field_20;
		int				field_24;

	public:
		virtual void*	GetInstancePtr() const override;

		Fragment();	//	@85DE30
		~Fragment();

		static void		CreateInstance();	//	@85DFA0
	};

	class Movie : Resource
	{
	protected:
		int				field_1C;
		int				field_20;
		int				field_24;

	public:
		virtual void*	GetInstancePtr() const override;

		Movie();	//	@85BC40

		static void		CreateInstance();	//	@85BC70
	};

	class Cutscene : Resource
	{
	protected:
		int				field_1C;
		List<int>		m_List_1;
		List<int>		m_List_2;
		int				field_40;
		int				field_44;
		int				field_48;
		String			m_String_1;
		int				field_5C;

	public:
		virtual void*	GetInstancePtr() const override;

		Cutscene();	//	@916080

		static void		CreateInstance();	//	@9164C0
	};

	class Sound : Resource
	{
	protected:
		int				field_1C;
		int*			field_20;
		int				field_24;

	public:
		virtual void*	GetInstancePtr() const override;

		Sound();	//	@85C3C0

		static void		CreateInstance();	//	@85C010
	};

	class StreamedSoundInfo : Resource
	{
	protected:
		int				field_1C;
		int*			m_MonoStream;
		int				field_24;

	public:
		virtual void*	GetInstancePtr() const override;

		StreamedSoundInfo();	//	@85C7E0

		static void		CreateInstance();	//	@85C8D0
	};

	class Animation : Resource
	{
	protected:
		int				field_1C;
		int				field_20;
		int				field_24;
		int				field_28;
		List<int>		m_List_1;
		List<int>		m_List_2;
		List<int>		m_List_3;
		int*			field_5C;
		int				field_60;
		short			field_64;
		short			field_66;

	public:
		virtual void*	GetInstancePtr() const override;

		Animation();	//	@900080

		static void		CreateInstance();	//	@900980
	};

	class MeshColor : Resource
	{
	protected:
		int				field_1C;
		List<int>		m_List_1;
		List<int>		m_List_2;
		int*			field_40;
		int*			field_44;
		int*			field_48;
		int*			field_4C;

	public:
		virtual void*	GetInstancePtr() const override;

		MeshColor();	//	@85E7F0

		static void		CreateInstance();	//	@85E970
	};

	static Base*		rtTexture;
	static Base*		rtFont;
	static Base*		rtText;
	static Base*		rtModel;
	static Base*		rtFragment;
	static Base*		rtMovie;
	static Base*		rtCutscene;
	static Base*		rtSound;
	static Base*		rtStreamedSoundInfo;
	static Base*		rtAnimation;
	static Base*		rtMeshColor;

	static_assert(sizeof(Base) == RESTYPE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Base));
}