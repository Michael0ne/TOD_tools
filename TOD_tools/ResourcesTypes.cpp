#include "ResourcesTypes.h"
#include "Blocks.h"

namespace ResType
{
	unsigned int Resource::LastOpenResourceIndex;

#pragma message(TODO_IMPLEMENTATION)
	Base::Base(const char* type, void* typePtr)
	{
		MESSAGE_CLASS_CREATED(Base);

		m_ResourceTypeName.Set(type);
		m_ResourceIndex = ResTypeList.m_nCurrIndex;
		ResTypeList.AddElement(this);
		//m_Creator = creator;
		field_2C = NULL;
		field_2D = NULL;
		
		SetResourceAlignment(16, 0);
		SetResourceAlignment(16, 1);
		SetResourceAlignment(16, 2);

		m_ActualResourceType = typePtr;
		//	_TypeInfo_Destroy(m_Creator);
	}

	void Base::SetResourceAlignment(unsigned int size, unsigned int index)
	{
		m_Alignment[index] = size;

		if (ResourceAlignment[index] < size)
			ResourceAlignment[index] = size;
	}

#pragma message(TODO_IMPLEMENTATION)

	void Resource::SetUnkFlag(unsigned char a1, int, int)
	{
		field_18 ^= (field_18 ^ (a1 << 18)) & 0x40000;
	}

	int Resource::GetUnkFlag()
	{
		return (field_18 >> 18) & 1;
	}

	void Resource::GetResourcesDir(String& outDir, PlatformId platformId)
	{
		outDir = String();
	}

	int Resource::stub9()
	{
		return (((field_18 >> 19) & 1) != NULL) + 1;
	}

#pragma message(TODO_IMPLEMENTATION)
	void Resource::GetResourceName(String& outName, int a2)
	{
		if (a2)
			(*(void(__stdcall*)(String*, const char*, int, int))0x851800)(&outName, m_ResourcePath, NULL, NULL);
		else
			outName.Set(m_ResourcePath);
	}

	void Resource::DestroyResource()
	{
		field_10 = NULL;
		field_14 = NULL;
	}

	Resource::Resource(bool a1)
	{
		MESSAGE_CLASS_CREATED(Resource);

		if (TotalResourcesCreated == NULL)
			OpenResourcesList.SetCapacity(RESTYPE_MAX_OPEN_RESOURCES);

		field_10 = NULL;
		field_14 = NULL;
		field_18 = NULL;

		//	NOTE: field_4 is somehow initialized here. Haven't figured it out yet...

		m_GlobalResourceId = a1 ? NULL : g_Blocks->InsertTypeListItem(this);

		field_18 = field_18 & 0xFFF1FFFF | 0x10000;
	}

	Resource::~Resource()
	{
		MESSAGE_CLASS_DESTROYED(Resource);

		if (m_GlobalResourceId > 0)
			g_Blocks->m_ResourceTypesList.m_pElements[m_GlobalResourceId] = nullptr;

		--TotalResourcesCreated;

		delete m_ResourcePath;
	}

	const char* Resource::AddResToOpenListAndReturnName()
	{
		unsigned int _currresind = LastOpenResourceIndex;
		LastOpenResourceIndex = (LastOpenResourceIndex + 1) % RESTYPE_MAX_OPEN_RESOURCES;

		String resPath;
		g_Blocks->GetInternalFileName(resPath, m_ResourcePath);

		OpenResourcesList.m_pElements[_currresind] = &resPath;

		return OpenResourcesList.m_pElements[_currresind]->m_szString;
	}

	void Resource::_8513E0(unsigned char a1)
	{
		field_18 ^= (field_18 ^ (a1 << 19)) & 0x80000;
	}

	void* Texture::GetInstancePtr() const
	{
		return (void*)rtTexture;
	}

	Texture::Texture() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Texture);

		field_20 = 1;
		m_Texture = nullptr;
		field_24 &= 0xFFFFFFF8;
		_8513E0(1);
	}

	void Texture::CreateInstance()
	{
		rtTexture = new Base(RESTYPE_TEXTURE_NAME, (void*)new Texture());
		String ext(RESTYPE_TEXTURE_EXT);

		rtTexture->m_ResourceExtensionsList.Add(&ext);

		rtTexture->SetResourceAlignment(16, 1);
		rtTexture->SetResourceAlignment(128, 2);
		rtTexture->SetResourceAlignment(16, 0);
	}

	void* Font::GetInstancePtr() const
	{
		return (void*)rtFont;
	}

	Font::Font() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Font);

		field_20 = 1;
		field_24 = NULL;

		_8513E0(1);
	}

	void Font::CreateInstance()
	{
		rtFont = new Base(RESTYPE_FONT_NAME, (void*)new Font());
		String ext(RESTYPE_FONT_EXT_1);
		String ext_2(RESTYPE_FONT_EXT_2);

		rtFont->m_ResourceExtensionsList.Add(&ext);
		rtFont->m_ResourceExtensionsList.Add(&ext_2);

		rtFont->SetResourceAlignment(16, 1);
		rtFont->SetResourceAlignment(128, 2);
		rtFont->SetResourceAlignment(16, 0);

		rtFont->field_2C = true;
	}

	void* Text::GetInstancePtr() const
	{
		return (void*)rtText;
	}

	Text::Text() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Text);

		m_List_1 = List<int>(0x18B00);
		m_List_2 = List<int>(0x18B00);
		m_List_3 = List<int>(0x18B00);

		field_50 = NULL;
	}

	void Text::CreateInstance()
	{
		rtText = new Base(RESTYPE_TEXT_NAME, (void*)new Text());
		String ext(RESTYPE_TEXT_EXT);

		rtText->m_ResourceExtensionsList.Add(&ext);

		rtText->SetResourceAlignment(16, 1);
		rtText->SetResourceAlignment(16, 2);
		rtText->SetResourceAlignment(16, 0);

		rtText->field_2C = true;
	}

	void* Model::GetInstancePtr() const
	{
		return (void*)rtModel;
	}

	Model::Model() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Model);

		m_List_1 = List<int>(0x18B00);
		m_List_2 = List<int>(0x18B00);

		field_40 = NULL;
		field_54 = NULL;
		field_58 = 1;
		field_5C = 0xFFFFFFF0 & 0xFFFFFF2F | 0x20;
		field_44 = NULL;
		field_48 = NULL;
		field_4C = NULL;
		field_50 = NULL;

		_8513E0(1);
	}

	void Model::CreateInstance()
	{
		rtModel = new Base(RESTYPE_MODEL_NAME, (void*)new Model());
		String ext(RESTYPE_MODEL_EXT);

		rtModel->m_ResourceExtensionsList.Add(&ext);

		rtModel->SetResourceAlignment(16, 1);
		rtModel->SetResourceAlignment(16, 2);
		rtModel->SetResourceAlignment(16, 0);
	}

	void* Fragment::GetInstancePtr() const
	{
		return (void*)rtFragment;
	}

	Fragment::Fragment() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Fragment);

		field_20 = new int[6];
		field_24 &= 0xFFFFFFFD;
	}

	Fragment::~Fragment()
	{
		MESSAGE_CLASS_DESTROYED(Fragment);

		if (field_20)
			delete[] field_20;
	}

	void Fragment::CreateInstance()
	{
		rtFragment = new Base(RESTYPE_FRAGMENT_NAME, (void*)new Fragment());
		String ext(RESTYPE_FRAGMENT_EXT_1);
		String ext_1(RESTYPE_FRAGMENT_EXT_2);

		rtFragment->m_ResourceExtensionsList.Add(&ext);
		rtFragment->m_ResourceExtensionsList.Add(&ext_1);

		rtFragment->SetResourceAlignment(16, 1);
		rtFragment->SetResourceAlignment(16, 2);
		rtFragment->SetResourceAlignment(16, 0);

		rtFragment->field_2D = true;
	}

	void* Movie::GetInstancePtr() const
	{
		return (void*)rtMovie;
	}

	Movie::Movie() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Movie);
	}

	void Movie::CreateInstance()
	{
		rtMovie = new Base(RESTYPE_MOVIE_NAME, (void*)new Movie());
		String ext(RESTYPE_MOVIE_EXT_1);
		String ext_2(RESTYPE_MOVIE_EXT_2);

		rtMovie->m_ResourceExtensionsList.Add(&ext);
		rtMovie->m_ResourceExtensionsList.Add(&ext_2);

		rtMovie->SetResourceAlignment(16, 1);
		rtMovie->SetResourceAlignment(16, 2);
		rtMovie->SetResourceAlignment(16, 0);
	}

	void* Cutscene::GetInstancePtr() const
	{
		return (void*)rtCutscene;
	}

	Cutscene::Cutscene() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Cutscene);

		m_List_1 = List<int>(0x23B00);
		m_List_2 = List<int>(0x23B00);

		field_40 = 1;
		field_48 = 1;
		field_44 = 30;

		m_String_1 = String();
	}

	void Cutscene::CreateInstance()
	{
		rtCutscene = new Base(RESTYPE_CUTSCENE_NAME, (void*)new Cutscene());
		String ext(RESTYPE_CUTSCENE_EXT);

		rtCutscene->m_ResourceExtensionsList.Add(&ext);

		rtCutscene->SetResourceAlignment(16, 1);
		rtCutscene->SetResourceAlignment(16, 2);
		rtCutscene->SetResourceAlignment(16, 0);
	}

	void* Sound::GetInstancePtr() const
	{
		return (void*)rtSound;
	}

	Sound::Sound() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Sound);

		field_20 = nullptr;	//	NOTE: this object pointer has VMT. Maybe it's StreamBuffer?
	}

	void Sound::CreateInstance()
	{
		rtSound = new Base(RESTYPE_SOUND_NAME, (void*)new Sound());
		String ext(RESTYPE_SOUND_EXT);

		rtSound->m_ResourceExtensionsList.Add(&ext);

		rtSound->SetResourceAlignment(16, 1);
		rtSound->SetResourceAlignment(16, 2);
		rtSound->SetResourceAlignment(16, 0);

		rtSound->field_2C = true;
	}

	void* StreamedSoundInfo::GetInstancePtr() const
	{
		return (void*)rtStreamedSoundInfo;
	}

	StreamedSoundInfo::StreamedSoundInfo() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(StreamedSoundInfo);

		//m_MonoStream = new StreamBuffer();
		m_MonoStream = nullptr;
	}

	void StreamedSoundInfo::CreateInstance()
	{
		rtStreamedSoundInfo = new Base(RESTYPE_STREAMEDSOUNDINFO_NAME, (void*)new StreamedSoundInfo());
		String ext(RESTYPE_STREAMEDSOUNDINFO_EXT_1);
		String ext_2(RESTYPE_STREAMEDSOUNDINFO_EXT_2);

		rtStreamedSoundInfo->m_ResourceExtensionsList.Add(&ext);
		rtStreamedSoundInfo->m_ResourceExtensionsList.Add(&ext_2);

		rtStreamedSoundInfo->SetResourceAlignment(16, 1);
		rtStreamedSoundInfo->SetResourceAlignment(16, 2);
		rtStreamedSoundInfo->SetResourceAlignment(16, 0);

		rtStreamedSoundInfo->field_2C = true;
	}

	void* Animation::GetInstancePtr() const
	{
		return (void*)rtAnimation;
	}

	Animation::Animation() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Animation);

		m_List_1 = List<int>(0x22300);
		m_List_2 = List<int>(0x22300);
		m_List_3 = List<int>(0x22300);

		field_5C = nullptr;
		field_28 = NULL;
		field_64 = NULL;
		field_60 = NULL;
		field_66 = NULL;
	}

	void Animation::CreateInstance()
	{
		rtAnimation = new Base(RESTYPE_ANIMATION_NAME, (void*)new Animation());
		String ext(RESTYPE_ANIMATION_EXT);

		rtAnimation->m_ResourceExtensionsList.Add(&ext);

		rtAnimation->SetResourceAlignment(16, 1);
		rtAnimation->SetResourceAlignment(16, 2);
		rtAnimation->SetResourceAlignment(16, 0);

		rtAnimation->field_2D = true;
	}

	void* MeshColor::GetInstancePtr() const
	{
		return (void*)rtMeshColor;
	}

	MeshColor::MeshColor() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(MeshColor);

		m_List_1 = List<int>(0x20300);
		m_List_2 = List<int>(0x20300);

		field_40 = nullptr;
		field_48 = nullptr;
		field_4C = nullptr;	//	FIXME: replace with new.
		field_44 = nullptr;	//	FIXME: replace with new.
	}

	void MeshColor::CreateInstance()
	{
		rtMeshColor = new Base(RESTYPE_MESHCOLOR_NAME, (void*)new MeshColor());
		String ext(RESTYPE_MESHCOLOR_EXT_1);
		String ext_1(RESTYPE_MESHCOLOR_EXT_2);

		rtMeshColor->m_ResourceExtensionsList.Add(&ext);
		rtMeshColor->m_ResourceExtensionsList.Add(&ext_1);
	}
}