#pragma once

#include "stdafx.h"

#include "StringsPool.h"
#include "List.h"
#include "MemoryAllocators.h"

namespace Types {

	namespace Resources {

#define TYPEINFO_CLASS_SIZE 60

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

		class TypeInfo;
		class Texture;
		class Font;
		class Text;
		class Model;
		class Fragment;
		class Movie;
		class Cutscene;
		class Sound;
		class StreamedSoundInfo;
		class Animation;
		class MeshColor;

		struct TypeInfo__vtable
		{
			void(__thiscall* Release)(TypeInfo* _this, bool freememory);	//	@851F90
			TypeInfo* (__thiscall* GetThisPtr)(TypeInfo* _this);	//	pure virtual
			void(__thiscall* SetUnkFlag)(TypeInfo* _this, unsigned char unk1, int unk2, int unk3);	//	@851400
			unsigned int(__thiscall* GetUnkFlag)(TypeInfo* _this);	//	@851E80
			void(__cdecl* nullsub)();	//	@883EC0
			void (__stdcall* GetResourcesDir)(String* str, int platform);	//	@851EC0
			void(__cdecl* nullsub_1)();	//	@851420
			char(__cdecl* ReturnZero)();	//	@42F4F0
			int(__thiscall* _851EA0)(TypeInfo* _this);	//	@851EA0
			void(__thiscall* stub10)(TypeInfo* _this, String* str, int unk);	//	@851DB0
			void(__cdecl* nullsub_2)();	//	@883EC0
			void(__thiscall* _851E90)(TypeInfo* _this);	//	@851E90
		};

		class TypeInfo
		{
		protected:
			void* lpVtbl;	//	Actually, is this a creator?
			String m_sTypename;
			TypeInfo__vtable* lpTypeVtbl;
			int m_nId;
			List<String> m_sExtensionList;
			byte field_2C;
			byte field_2D;
			byte field_2E;
			byte field_2F;
			int m_UnkBufferArray[3];

		public:
			TypeInfo()
			{
				lpVtbl = nullptr;
				m_sTypename = String();
				lpTypeVtbl = nullptr;
				m_nId = 0;
				m_sExtensionList = List<String>();
				field_2C = 0;
				field_2D = 0;
				field_2E = 0;
				field_2F = 0;
				m_UnkBufferArray[0] = 0;
				m_UnkBufferArray[1] = 0;
				m_UnkBufferArray[2] = 0;

				debug("Types::Resources::TypeInfo created at %X\n", this);
			}

			~TypeInfo()
			{
				m_sExtensionList.Erase();

				debug("Types::Resources::TypeInfo destroyed!\n");
			}

			void* operator new(size_t size)
			{
				return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
			}

			void operator delete(void* ptr)
			{
				if (ptr)
					Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
			}

			void	RegisterBase(const char* szTypeName, void* (__cdecl* pCreator)());	//	@852440
			void	InitUnkBuffer(unsigned int size, unsigned int index);	//	@852160
		};

		static unsigned int* g_UnkTypeBufferSizes[] = {
			(unsigned int*)0xA3BE1C,
			(unsigned int*)0xA3BE20,
			(unsigned int*)0xA3BE24,
		};

		static List<TypeInfo>& g_TypesList = *(List<TypeInfo>*)0xA10F80;

		static Texture** g_Texture = (Texture**)0xA3BE28;

		class Texture : public TypeInfo
		{
		public:
			Texture()
			{
				RegisterBase(TYPERES_TEXTURE_NAME, (void * (__cdecl*)())0x853830);

				debug("Types::Resources::Texture created at %X\n", this);
			}

			~Texture()
			{
				debug("Types::Resources::Texture destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Texture) = new Texture();

					if (*g_Texture)
						(*g_Texture)->Register();
				}
			}

			void	Register();	//	@853870
		};

		static Font** g_Font = (Font**)0xA3BE48;

		class Font : public TypeInfo
		{
		public:
			Font()
			{
				RegisterBase(TYPERES_FONT_NAME, (void* (__cdecl*)())0x85B350);

				debug("Types::Resources::Font created at %X\n", this);
			}

			~Font()
			{
				debug("Types::Resources::Font destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Font) = new Font();

					if (*g_Font)
						(*g_Font)->Register();
				}
			}

			void	Register();	//	@85B460
		};

		static Text** g_Text = (Text**)0xA3CE7C;

		class Text : public TypeInfo
		{
		public:
			Text()
			{
				RegisterBase(TYPERES_TEXT_NAME, (void* (__cdecl*)())0x861BD0);

				debug("Types::Resources::Text created at %X\n", this);
			}

			~Text()
			{
				debug("Types::Resources::Text destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Text) = new Text();

					if (*g_Text)
						(*g_Text)->Register();
				}
			}

			void	Register();	//	@861CE0
		};

		static Model** g_Model = (Model**)0xA3BE30;

		class Model : public TypeInfo
		{
		public:
			Model()
			{
				RegisterBase(TYPERES_MODEL_NAME, (void* (__cdecl*)())0x8581F0);

				debug("Typeres::Resources::Model created at %X\n", this);
			}

			~Model()
			{
				debug("Typeres::Resources::Model destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Model) = new Model();

					if (*g_Model)
						(*g_Model)->Register();
				}
			}

			void	Register();	//	@858210
		};

		static Fragment** g_Fragment = (Fragment**)0xA3BE58;

		class Fragment : public TypeInfo
		{
		public:
			Fragment()
			{
				RegisterBase(TYPERES_FRAGMENT_NAME, (void* (__cdecl*)())0x85DE30);

				debug("Types::Resources::Fragment created at %X\n", this);
			}

			~Fragment()
			{
				debug("Types::Resources::Fragment destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Fragment) = new Fragment();

					if (*g_Fragment)
						(*g_Fragment)->Register();
				}
			}

			void	Register();	//	@85DFA0
		};

		static Movie** g_Movie = (Movie**)0xA3BE4C;

		class Movie : public TypeInfo
		{
		public:
			Movie()
			{
				RegisterBase(TYPERES_MOVIE_NAME, (void* (__cdecl*)())0x85BC40);

				debug("Types::Resources::Movie created at %X\n", this);
			}

			~Movie()
			{
				debug("Types::Resources::Movie destroyed!");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Movie) = new Movie();

					if (*g_Movie)
						(*g_Movie)->Register();
				}
			}

			void	Register();	//	@85BC70
		};

		static Cutscene** g_Cutscene = (Cutscene**)0xA3E12C;

		class Cutscene : public TypeInfo
		{
		public:
			Cutscene()
			{
				RegisterBase(TYPERES_CUTSCENE_NAME, (void* (__cdecl*)())0x916100);

				debug("Types::Resources::Cutscene created at %X\n", this);
			}

			~Cutscene()
			{
				debug("Types::Resources::Cutscene destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Cutscene) = new Cutscene();

					if (*g_Cutscene)
						(*g_Cutscene)->Register();
				}
			}

			void	Register();	//	@9164C0
		};

		static Sound** g_Sound = (Sound**)0xA3BE50;

		class Sound : public TypeInfo
		{
		public:
			Sound()
			{
				RegisterBase(TYPERES_SOUND_NAME, (void* (__cdecl*)())0x85C430);

				debug("Types::Resources::Sound created at %X\n", this);
			}

			~Sound()
			{
				debug("Types::Resources::Sound destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Sound) = new Sound();

					if (*g_Sound)
						(*g_Sound)->Register();
				}
			}

			void	Register();	//	@85C010
		};

		static StreamedSoundInfo** g_StreamedSoundInfo = (StreamedSoundInfo**)0xA3BE54;

		class StreamedSoundInfo : public TypeInfo
		{
		public:
			StreamedSoundInfo()
			{
				RegisterBase(TYPERES_STREAMEDSOUNDINFO_NAME, (void* (__cdecl*)())0x85CDA0);

				debug("Types::Resources::StreamedSoundInfo created at %X\n", this);
			}

			~StreamedSoundInfo()
			{
				debug("Types::Resources::StreamedSoundInfo destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_StreamedSoundInfo) = new StreamedSoundInfo();

					if (*g_StreamedSoundInfo)
						(*g_StreamedSoundInfo)->Register();
				}
			}

			void	Register();	//	@85C8D0
		};

		static Animation** g_Animation = (Animation**)0xA3E0FC;

		class Animation : public TypeInfo
		{
		public:
			Animation()
			{
				RegisterBase(TYPERES_ANIMATION_NAME, (void* (__cdecl*)())0x900EF0);

				debug("Types::Resources::Animation created at %X\n", this);
			}

			~Animation()
			{
				debug("Types::Resources::Animation destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_Animation) = new Animation();

					if (*g_Animation)
						(*g_Animation)->Register();
				}
			}

			void	Register();	//	@900980
		};

		static MeshColor** g_MeshColor = (MeshColor**)0xA3CE78;

		class MeshColor : public TypeInfo
		{
		public:
			MeshColor()
			{
				RegisterBase(TYPERES_MESHCOLOR_NAME, (void* (__cdecl*)())0x85E950);

				debug("Types::Resources::MeshColor created at %X\n", this);
			}

			~MeshColor()
			{
				debug("Types::Resources::MeshColor destroyed!\n");
			}

			static void Init()
			{
				if (!Allocators::Released) {
					(*g_MeshColor) = new MeshColor();

					if (*g_MeshColor)
						(*g_MeshColor)->Register();
				}
			}

			void	Register();	//	@85E970
		};
	}
}

static_assert(sizeof(Types::Resources::TypeInfo) == TYPEINFO_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(TypeInfo));
