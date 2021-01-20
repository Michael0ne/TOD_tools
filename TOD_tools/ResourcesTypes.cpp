#include "ResourcesTypes.h"
#include "Blocks.h"

namespace ResType
{
	unsigned int Resource::LastOpenResourceIndex = NULL;

	ResourceBase::ResourceBase(const char* type, Resource* (*creator)())
	{
		MESSAGE_CLASS_CREATED(ResourceBase);

		m_ResourceTypeName = type;
		m_ResourceIndex = ResTypeList.m_CurrIndex;
		ResTypeList.AddElement(this);
		m_Creator = creator;
		field_2C = NULL;
		m_VerifyChecksum = false;
		
		SetResourceAlignment(16, 0);
		SetResourceAlignment(16, 1);
		SetResourceAlignment(16, 2);

		Resource* res_ = m_Creator();
		m_ResTypeMethods = (void*)(*((int*)res_));

		res_->~Resource();
		if (res_->field_18 & 0x10000)
			delete res_;
	}

	void ResourceBase::SetResourceAlignment(unsigned int size, unsigned int index)
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

	int Resource::GetUnkFlag() const
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
			outName = m_ResourcePath;
	}

	void Resource::DestroyResource()
	{
		m_ResourceTimestamp = NULL;
		field_14 = NULL;
	}

	Resource::Resource(bool a1)
	{
		MESSAGE_CLASS_CREATED(Resource);

		if (TotalResourcesCreated == NULL)
			OpenResourcesList.SetCapacityAndErase(RESTYPE_MAX_OPEN_RESOURCES);

		m_ResourceTimestamp = NULL;
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
			g_Blocks->m_ResourceTypesList.m_Elements[m_GlobalResourceId] = nullptr;

		--TotalResourcesCreated;

		delete m_ResourcePath;
	}

	const char* Resource::AddResToOpenListAndReturnName()
	{
		unsigned int _currresind = LastOpenResourceIndex;
		LastOpenResourceIndex = (LastOpenResourceIndex + 1) % RESTYPE_MAX_OPEN_RESOURCES;

		String resPath;
		g_Blocks->GetInternalFileName(resPath, m_ResourcePath);

		OpenResourcesList.m_Elements[_currresind] = &resPath;

		return OpenResourcesList.m_Elements[_currresind]->m_szString;
	}

	void Resource::_8513E0(unsigned char a1)
	{
		field_18 ^= (field_18^ (a1 << 19)) & 0x80000;
	}

	#pragma message(TODO_IMPLEMENTATION)
	void Resource::ApplyLoadedResource(ResourceHolder&)
	{
	}

	Resource* Texture::GetInstancePtr() const
{
		return (Resource*)rtTexture;
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
		rtTexture = new ResourceBase(RESTYPE_TEXTURE_NAME, (CREATOR)Create);
		String ext = RESTYPE_TEXTURE_EXT;

		rtTexture->m_ResourceExtensionsList.AddElement(&ext);

		rtTexture->SetResourceAlignment(16, 1);
		rtTexture->SetResourceAlignment(128, 2);
		rtTexture->SetResourceAlignment(16, 0);
	}

	Texture* Texture::Create()
	{
		return new Texture();
	}

	Resource* Font::GetInstancePtr() const
{
		return (Resource*)rtFont;
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
		rtFont = new ResourceBase(RESTYPE_FONT_NAME, (CREATOR)Create);
		String ext = RESTYPE_FONT_EXT_1;
		String ext_2 = RESTYPE_FONT_EXT_2;

		rtFont->m_ResourceExtensionsList.AddElement(&ext);
		rtFont->m_ResourceExtensionsList.AddElement(&ext_2);

		rtFont->SetResourceAlignment(16, 1);
		rtFont->SetResourceAlignment(128, 2);
		rtFont->SetResourceAlignment(16, 0);

		rtFont->field_2C = true;
	}

	Font* Font::Create()
	{
		return new Font();
	}

	Resource* Text::GetInstancePtr() const
{
		return (Resource*)rtText;
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
		rtText = new ResourceBase(RESTYPE_TEXT_NAME, (CREATOR)Create);
		String ext = RESTYPE_TEXT_EXT;

		rtText->m_ResourceExtensionsList.AddElement(&ext);

		rtText->SetResourceAlignment(16, 1);
		rtText->SetResourceAlignment(16, 2);
		rtText->SetResourceAlignment(16, 0);

		rtText->field_2C = true;
	}

	Text* Text::Create()
	{
		return new Text();
	}

	Resource* Model::GetInstancePtr() const
{
		return (Resource*)rtModel;
	}

	Model::Model() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Model);

		m_List_1 = List<int>(0x18B00);

		field_40 = NULL;
		field_54 = NULL;
		field_5C = 0xFFFFFFF0 & 0xFFFFFF2F | 0x20;

		_8513E0(1);
	}

	void Model::CreateInstance()
	{
		rtModel = new ResourceBase(RESTYPE_MODEL_NAME, (CREATOR)Create);
		String ext = RESTYPE_MODEL_EXT;

		rtModel->m_ResourceExtensionsList.AddElement(&ext);

		rtModel->SetResourceAlignment(16, 1);
		rtModel->SetResourceAlignment(16, 2);
		rtModel->SetResourceAlignment(16, 0);
	}

	Model* Model::Create()
	{
		return new Model();
	}

	Resource* Fragment::GetInstancePtr() const
{
		return (Resource*)rtFragment;
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
		rtFragment = new ResourceBase(RESTYPE_FRAGMENT_NAME, (CREATOR)Create);
		String ext = RESTYPE_FRAGMENT_EXT_1;
		String ext_1 = RESTYPE_FRAGMENT_EXT_2;

		rtFragment->m_ResourceExtensionsList.AddElement(&ext);
		rtFragment->m_ResourceExtensionsList.AddElement(&ext_1);

		rtFragment->SetResourceAlignment(16, 1);
		rtFragment->SetResourceAlignment(16, 2);
		rtFragment->SetResourceAlignment(16, 0);

		rtFragment->m_VerifyChecksum = true;
	}

	Fragment* Fragment::Create()
	{
		return new Fragment();
	}

	Resource* Movie::GetInstancePtr() const
{
		return (Resource*)rtMovie;
	}

	Movie::Movie() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Movie);
	}

	void Movie::CreateInstance()
	{
		rtMovie = new ResourceBase(RESTYPE_MOVIE_NAME, (CREATOR)Create);
		String ext = RESTYPE_MOVIE_EXT_1;
		String ext_2 = RESTYPE_MOVIE_EXT_2;

		rtMovie->m_ResourceExtensionsList.AddElement(&ext);
		rtMovie->m_ResourceExtensionsList.AddElement(&ext_2);

		rtMovie->SetResourceAlignment(16, 1);
		rtMovie->SetResourceAlignment(16, 2);
		rtMovie->SetResourceAlignment(16, 0);
	}

	Movie* Movie::Create()
	{
		return new Movie();
	}

	Resource* Cutscene::GetInstancePtr() const
{
		return (Resource*)rtCutscene;
	}

	Cutscene::Cutscene() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Cutscene);

		m_List_1 = List<int>(0x23B00);
		m_List_2 = List<int>(0x23B00);

		field_40 = 1;
		field_48 = 1;
		field_44 = 30;

		m_String_1;
	}

	void Cutscene::CreateInstance()
	{
		rtCutscene = new ResourceBase(RESTYPE_CUTSCENE_NAME, (CREATOR)Create);
		String ext = RESTYPE_CUTSCENE_EXT;

		rtCutscene->m_ResourceExtensionsList.AddElement(&ext);

		rtCutscene->SetResourceAlignment(16, 1);
		rtCutscene->SetResourceAlignment(16, 2);
		rtCutscene->SetResourceAlignment(16, 0);
	}

	Cutscene* Cutscene::Create()
	{
		return new Cutscene();
	}

	Resource* Sound::GetInstancePtr() const
{
		return (Resource*)rtSound;
	}

	Sound::Sound() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Sound);

		m_MonoStream = nullptr;
	}

	void Sound::CreateInstance()
	{
		rtSound = new ResourceBase(RESTYPE_SOUND_NAME, (CREATOR)Create);
		String ext = RESTYPE_SOUND_EXT;

		rtSound->m_ResourceExtensionsList.AddElement(&ext);

		rtSound->SetResourceAlignment(16, 1);
		rtSound->SetResourceAlignment(16, 2);
		rtSound->SetResourceAlignment(16, 0);

		rtSound->field_2C = true;
	}

	Sound* Sound::Create()
	{
		return new Sound();
	}

	Resource* StreamedSoundInfo::GetInstancePtr() const
{
		return (Resource*)rtStreamedSoundInfo;
	}

	#pragma message(TODO_IMPLEMENTATION)
	StreamedSoundInfo::StreamedSoundInfo() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(StreamedSoundInfo);

		//m_MonoStream = new StreamBuffer();
		m_MonoStream = nullptr;
	}

	void StreamedSoundInfo::CreateInstance()
	{
		rtStreamedSoundInfo = new ResourceBase(RESTYPE_STREAMEDSOUNDINFO_NAME, (CREATOR)Create);
		String ext = RESTYPE_STREAMEDSOUNDINFO_EXT_1;
		String ext_2 = RESTYPE_STREAMEDSOUNDINFO_EXT_2;

		rtStreamedSoundInfo->m_ResourceExtensionsList.AddElement(&ext);
		rtStreamedSoundInfo->m_ResourceExtensionsList.AddElement(&ext_2);

		rtStreamedSoundInfo->SetResourceAlignment(16, 1);
		rtStreamedSoundInfo->SetResourceAlignment(16, 2);
		rtStreamedSoundInfo->SetResourceAlignment(16, 0);

		rtStreamedSoundInfo->field_2C = true;
	}

	StreamedSoundInfo* StreamedSoundInfo::Create()
	{
		return new StreamedSoundInfo();
	}

	Resource* Animation::GetInstancePtr() const
{
		return (Resource*)rtAnimation;
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
		rtAnimation = new ResourceBase(RESTYPE_ANIMATION_NAME, (CREATOR)Create);
		String ext = RESTYPE_ANIMATION_EXT;

		rtAnimation->m_ResourceExtensionsList.AddElement(&ext);

		rtAnimation->SetResourceAlignment(16, 1);
		rtAnimation->SetResourceAlignment(16, 2);
		rtAnimation->SetResourceAlignment(16, 0);

		rtAnimation->m_VerifyChecksum = true;
	}

	Animation* Animation::Create()
	{
		return new Animation();
	}

	Resource* MeshColor::GetInstancePtr() const
{
		return (Resource*)rtMeshColor;
	}

	#pragma message(TODO_IMPLEMENTATION)
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
		rtMeshColor = new ResourceBase(RESTYPE_MESHCOLOR_NAME, (CREATOR)Create);
		String ext = RESTYPE_MESHCOLOR_EXT_1;
		String ext_1 = RESTYPE_MESHCOLOR_EXT_2;

		rtMeshColor->m_ResourceExtensionsList.AddElement(&ext);
		rtMeshColor->m_ResourceExtensionsList.AddElement(&ext_1);
	}

	MeshColor* MeshColor::Create()
	{
		return new MeshColor();
	}
}