#pragma once

#include "List.h"

namespace Types {

	namespace Resources {

#define BASE_CLASS_SIZE 60

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

		class Base
		{
		private:
			class TypeInfo* (__cdecl* m_Creator)();
			String			m_ResourceTypeName;
			class TypeInfo*		m_ActualResourceType;
			int				field_18;
			List<String>	m_ResourceExtensionsList;
			char			field_2C;
			char			field_2D;
			unsigned int	m_Alignment[3];

		public:
			Base(const char* type, void* (* creator)());	//	@852440

			void			SetResourceAlignment(unsigned int size, unsigned int index);	//	@852160

			static unsigned int	ResourceAlignment[3];	//	@A3BE1C
		};

		static_assert(sizeof(Base) == BASE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Base));
	}
}