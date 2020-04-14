#include "ResourcesTypes.h"

namespace Types
{
	namespace Resources
	{
		//	TODO: implementation!
		void TypeInfo::RegisterBase(const char* szTypeName, void* (__cdecl* pCreator)())
		{
			void(__thiscall * _Register)(TypeInfo * _this, const char* _typename, void* (__cdecl* _creator)()) = (void(__thiscall*)(TypeInfo*, const char*, void* (__cdecl*)()))0x852440;

			_Register(this, szTypeName, pCreator);

			m_sTypename.Set(szTypeName);
			m_nId = g_TypesList.m_nCurrIndex;

			g_TypesList.AddElement(this);

			lpVtbl = pCreator;
			field_2C = 0;
			field_2D = 0;

			m_UnkBufferArray[1] = 16;
			m_UnkBufferArray[2] = 16;
			m_UnkBufferArray[0] = 16;

			if (*g_UnkTypeBufferSizes[1] < 16)
				*g_UnkTypeBufferSizes[1] = 16;

			if (*g_UnkTypeBufferSizes[2] < 16)
				*g_UnkTypeBufferSizes[2] = 16;

			if (*g_UnkTypeBufferSizes[0] < 16)
				*g_UnkTypeBufferSizes[0] = 16;

			void* lpUnk = (*(void*(__thiscall*)(TypeInfo*))lpVtbl)(this);
			lpTypeVtbl = (TypeInfo__vtable*)lpUnk;

			(*(void(__cdecl*)(void*))0x851FC0)(lpUnk);	//	Call creator
		}

		void TypeInfo::InitUnkBuffer(unsigned int size, unsigned int index)
		{
			m_UnkBufferArray[index] = size;

			if (*g_UnkTypeBufferSizes[index] < size)
				*g_UnkTypeBufferSizes[index] = size;
		}

		void Texture::Register()
		{
			m_sExtensionList.Add(new String("bmp"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(128, 2);
			InitUnkBuffer(16, 0);
		}

		void Font::Register()
		{
			m_sExtensionList.Add(new String("ttf"));
			m_sExtensionList.Add(new String("font"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(128, 2);
			InitUnkBuffer(16, 0);

			field_2C = 1;
		}

		void Text::Register()
		{
			m_sExtensionList.Add(new String("txt"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);

			field_2C = 1;
		}

		void Model::Register()
		{
			m_sExtensionList.Add(new String("model"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);
		}

		void Fragment::Register()
		{
			m_sExtensionList.Add(new String("fragment"));
			m_sExtensionList.Add(new String("scene"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);

			field_2D = 1;
		}

		void Movie::Register()
		{
			m_sExtensionList.Add(new String("wmv"));
			m_sExtensionList.Add(new String("bik"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);
		}

		void Cutscene::Register()
		{
			m_sExtensionList.Add(new String("cutscene"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);
		}

		void Sound::Register()
		{
			m_sExtensionList.Add(new String("wav"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);

			field_2C = 1;
		}

		void StreamedSoundInfo::Register()
		{
			m_sExtensionList.Add(new String("stream"));
			m_sExtensionList.Add(new String("ogg"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);

			field_2C = 1;
		}

		void Animation::Register()
		{
			m_sExtensionList.Add(new String("animation"));

			InitUnkBuffer(16, 1);
			InitUnkBuffer(16, 2);
			InitUnkBuffer(16, 0);

			field_2D = 1;
		}

		void MeshColor::Register()
		{
			m_sExtensionList.Add(new String("meshcolor"));
			m_sExtensionList.Add(new String("lighting"));
		}
	}
}