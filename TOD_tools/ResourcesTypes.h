#pragma once

#include "List.h"

namespace Types {

	namespace Resources {

#define TYPEINFO_CLASS_SIZE 44
#define TYPEINFO_TEXTURE_CLASS_SIZE 48
#define TYPEINFO_FONT_CLASS_SIZE 40

#define TYPERES_TEXTURE_NAME "texture"
#define TYPERES_FONT_NAME "font"
#define TYPERES_TEXT_NAME "textres"
#define TYPERES_MODEL_NAME "modelres"
#define TYPERES_FRAGMENT_NAME "fragment"
#define TYPERES_MOVIE_NAME "movie"
#define TYPERES_CUTSCENE_NAME "cutscene"
#define TYPERES_SOUND_NAME "sound"
#define TYPERES_STREAMEDSOUNDINFO_NAME "streamedsoundinfo"
#define TYPERES_ANIMATION_NAME "animation"
#define TYPERES_MESHCOLOR_NAME "meshcolor"

		enum ePlatform
		{
			PLATFORM_PC = 0,
			PLATFORM_PS2 = 1,
			PLATFORM_XBOX = 2
		};

		class TypeInfo
		{
		protected:
			const char*		m_TypeName;
			int				m_GlobalListIndex;
			int				field_C;
			int				field_10;
			int*			field_14;	//	NOTE: result of scalar destructor call is written here. Why?
			int				m_Id;
			List<String>	m_ExtensionList;

		public:
			TypeInfo(const char* _typename, void* (*creator)());	//	@852440
			virtual ~TypeInfo()
			{
				MESSAGE_CLASS_DESTROYED(TypeInfo);

				m_ExtensionList.Erase();
			}

			virtual void*	GetInstancePtr() = 0;
			virtual char	_851400(char unk1, int unk2, int unk3);	//	@851400
			virtual int		_851E80();	//	@851E80
			virtual void	_853720(int unk1);	//	@853720
			virtual String* GetResourceDirectory(String* outDir, ePlatform platform);	//	@851EC0
			virtual void	stub7(int unk);
			virtual char	stub8();
			virtual int		_851EA0();	//	@851EA0
			virtual String* stub10(String* outStr, int unk1);	//	@851DB0
			virtual int		stub11(int unk1);	//	@883EC0
			virtual void	ResetFields10_14();	//	@851E90
		};

		class Texture : public TypeInfo
		{
		protected:
			int				field_30;

		public:
			Texture() : TypeInfo(TYPERES_TEXTURE_NAME, CreateTextureResourceType)
			{
				MESSAGE_CLASS_CREATED(Texture);
			}
			~Texture();

			static void* CreateTextureResourceType();
		};

		class Font : public TypeInfo
		{
		protected:

		public:
			Font() : TypeInfo(TYPERES_FONT_NAME, CreateFontResourceType)
			{
				MESSAGE_CLASS_CREATED(Font);
			}
			~Font();

			static void* CreateFontResourceType();
		};

		//static Texture& Instance = *(Texture*)0xA3BE28;	//	@A3BE28
		//static Font& Instance = *(Font*)0xA3BE48;	//	@A3BE48

		static_assert(sizeof(TypeInfo) == TYPEINFO_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TypeInfo));
		//static_assert(sizeof(Texture) == TYPEINFO_TEXTURE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Texture));
		//static_assert(sizeof(Font) == TYPEINFO_FONT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Font));
	}
}