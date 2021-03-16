#include "ResourcesTypes.h"
#include "Blocks.h"

namespace ResType
{
	unsigned int Resource::LastOpenResourceIndex = NULL;
	const char* BlockTypeExtension[7] =
	{
		".", "map", "submap", "mission", "cutscene", "playerdata", "main"
	};
	unsigned int	ResourceAlignment[3];
	std::vector<ResourceBase*>	ResTypeList;

	ResourceBase::ResourceBase(const char* type, Resource* (*creator)())
	{
		MESSAGE_CLASS_CREATED(ResourceBase);

		m_ResourceTypeName = type;
		m_ResourceIndex = ResTypeList.size();
		ResTypeList.push_back(this);
		m_Creator = creator;
		field_2C = NULL;
		m_VerifyChecksum = false;
		
		SetResourceAlignment(16, 0);
		SetResourceAlignment(16, 1);
		SetResourceAlignment(16, 2);

		Resource* res_ = m_Creator();
		m_ResTypeMethods = (void*)*((int*)res_);
		delete res_;
	}

	void ResourceBase::SetResourceAlignment(unsigned int size, unsigned int index)
	{
		m_Alignment[index] = size;

		if (ResourceAlignment[index] < size)
			ResourceAlignment[index] = size;
	}

	void ResourceBase::AllocateResourceBlockBufferAligned(unsigned int pos, int** resBufStartPos, int** resBufSpace, BlockTypeNumber resblockid)
	{
		int* allocspace = (int*)Allocators::AllocateByType(GetResourceBlockTypeNumber(resblockid), pos + ResType::ResourceAlignment[0]);
		*resBufSpace = allocspace;
		*resBufStartPos = (int*)(~(ResType::ResourceAlignment[0] - 1) & ((unsigned int)allocspace + ResType::ResourceAlignment[0] - 1));
	}

	ResourceBlockTypeNumber ResourceBase::GetResourceBlockTypeNumber(BlockTypeNumber resblockid)
	{
		if (!resblockid ||
			memcmp(BlockTypeExtension[resblockid], "map", 4) ||
			memcmp(BlockTypeExtension[resblockid], "submap", 7))
			return RESTYPE_MAP;

		if (!memcmp(BlockTypeExtension[resblockid], "mission", 8))
			return RESTYPE_MISSION;

		if (!memcmp(BlockTypeExtension[resblockid], "cutscene", 9))
			return RESTYPE_CUTSCENE;

		if (!memcmp(BlockTypeExtension[resblockid], "playerdata", 11))
			return RESTYPE_PLAYERDATA;

		return RESTYPE_NONE;
	}

#pragma message(TODO_IMPLEMENTATION)
	void Resource::SetUnkFlag(unsigned char a1, int, int)
	{
		m_Flags ^= (m_Flags ^ (a1 << 18)) & 0x40000;
	}

	int Resource::GetUnkFlag() const
{
		return (m_Flags >> 18) & 1;
	}

	void Resource::GetResourcesDir(String& outDir, PlatformId platformId)
	{
		outDir = String();
	}

	int Resource::stub9()
	{
		return (((m_Flags >> 19) & 1) != NULL) + 1;
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
	}

	Resource::Resource(bool a1)
	{
		MESSAGE_CLASS_CREATED(Resource);

		if (TotalResourcesCreated == NULL)
			OpenResourcesList.reserve(RESTYPE_MAX_OPEN_RESOURCES);

		m_ResourceTimestamp = NULL;
		m_Flags = NULL;
		m_ResourcePath = nullptr;
		m_GlobalResourceId = a1 ? NULL : g_Blocks->InsertTypeListItem(this);
		m_Flags = m_Flags & 0xFFF1FFFF | 0x10000;
	}

	Resource::~Resource()
	{
		MESSAGE_CLASS_DESTROYED(Resource);

		if (m_GlobalResourceId > 0)
			g_Blocks->m_ResourcesInstancesList[m_GlobalResourceId] = nullptr;

		--TotalResourcesCreated;

		if (m_ResourcePath)
			delete m_ResourcePath;
	}

	const char* Resource::AddResToOpenListAndReturnName()
	{
		unsigned int _currresind = LastOpenResourceIndex;
		LastOpenResourceIndex = (LastOpenResourceIndex + 1) % RESTYPE_MAX_OPEN_RESOURCES;

		String resPath;
		g_Blocks->GetInternalFileName(resPath, m_ResourcePath);

		OpenResourcesList[_currresind] = resPath;

		return OpenResourcesList[_currresind].m_szString;
	}

	void Resource::_8513E0(unsigned char a1)
	{
		m_Flags ^= (m_Flags^ (a1 << 19)) & 0x80000;
	}

	#pragma message(TODO_IMPLEMENTATION)
	void Resource::ApplyLoadedResource(ResourceHolder&)
	{
	}

	void Resource::Destroy(Resource* res)
	{
		res->~Resource();
		if (res->m_Flags & 0x10000)
			delete res;
	}

	Resource* Texture::GetInstancePtr() const
	{
		return (Resource*)rtTexture;
	}

	Texture::Texture() : Resource(false)
	{
		MESSAGE_CLASS_CREATED(Texture);

		m_Texture = nullptr;
		field_24 &= 0xFFFFFFF8;
		_8513E0(1);
	}

	void Texture::CreateInstance()
	{
		rtTexture = new ResourceBase(RESTYPE_TEXTURE_NAME, (CREATOR)Create);

		rtTexture->m_ResourceExtensionsList.push_back(RESTYPE_TEXTURE_EXT);

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

		rtFont->m_ResourceExtensionsList.push_back(RESTYPE_FONT_EXT_1);
		rtFont->m_ResourceExtensionsList.push_back(RESTYPE_FONT_EXT_2);

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

		//m_List_1 = List<int>(0x18B00);
		//m_List_2 = List<int>(0x18B00);
		//m_List_3 = List<int>(0x18B00);

		field_50 = NULL;
	}

	void Text::CreateInstance()
	{
		rtText = new ResourceBase(RESTYPE_TEXT_NAME, (CREATOR)Create);

		rtText->m_ResourceExtensionsList.push_back(RESTYPE_TEXT_EXT);

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

		//m_TextureResources = List<TextureReference>(0x18B00);

		field_40 = NULL;
		field_54 = NULL;
		field_5C = 0xFFFFFFF0 & 0xFFFFFF2F | 0x20;

		_8513E0(1);
	}

	void Model::CreateInstance()
	{
		rtModel = new ResourceBase(RESTYPE_MODEL_NAME, (CREATOR)Create);

		rtModel->m_ResourceExtensionsList.push_back(RESTYPE_MODEL_EXT);

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

		rtFragment->m_ResourceExtensionsList.push_back(RESTYPE_FRAGMENT_EXT_1);
		rtFragment->m_ResourceExtensionsList.push_back(RESTYPE_FRAGMENT_EXT_2);

		rtFragment->SetResourceAlignment(16, 1);
		rtFragment->SetResourceAlignment(16, 2);
		rtFragment->SetResourceAlignment(16, 0);

		rtFragment->m_VerifyChecksum = true;
	}

	Fragment* Fragment::Create()
	{
		return new Fragment();
	}

	#pragma message(TODO_IMPLEMENTATION)
	void Fragment::ApplyFragmentResource(unsigned int entityId, bool)
	{
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

		rtMovie->m_ResourceExtensionsList.push_back(RESTYPE_MOVIE_EXT_1);
		rtMovie->m_ResourceExtensionsList.push_back(RESTYPE_MOVIE_EXT_2);

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

		//m_List_1 = List<int>(0x23B00);
		//m_List_2 = List<int>(0x23B00);

		field_40 = 1;
		field_48 = 1;
		field_44 = 30;

		m_String_1;
	}

	void Cutscene::CreateInstance()
	{
		rtCutscene = new ResourceBase(RESTYPE_CUTSCENE_NAME, (CREATOR)Create);

		rtCutscene->m_ResourceExtensionsList.push_back(RESTYPE_CUTSCENE_EXT);

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

		rtSound->m_ResourceExtensionsList.push_back(RESTYPE_SOUND_EXT);

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

		rtStreamedSoundInfo->m_ResourceExtensionsList.push_back(RESTYPE_STREAMEDSOUNDINFO_EXT_1);
		rtStreamedSoundInfo->m_ResourceExtensionsList.push_back(RESTYPE_STREAMEDSOUNDINFO_EXT_2);

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

		//m_List_1 = List<int>(0x22300);
		//m_List_2 = List<int>(0x22300);
		//m_List_3 = List<int>(0x22300);

		field_5C = nullptr;
		field_28 = NULL;
		field_64 = NULL;
		field_60 = NULL;
		field_66 = NULL;
	}

	void Animation::CreateInstance()
	{
		rtAnimation = new ResourceBase(RESTYPE_ANIMATION_NAME, (CREATOR)Create);

		rtAnimation->m_ResourceExtensionsList.push_back(RESTYPE_ANIMATION_EXT);

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

		//m_List_1 = List<int>(0x20300);
		//m_List_2 = List<int>(0x20300);

		field_40 = nullptr;
		field_48 = nullptr;
		field_4C = nullptr;	//	FIXME: replace with new.
		field_44 = nullptr;	//	FIXME: replace with new.
	}

	void MeshColor::CreateInstance()
	{
		rtMeshColor = new ResourceBase(RESTYPE_MESHCOLOR_NAME, (CREATOR)Create);

		rtMeshColor->m_ResourceExtensionsList.push_back(RESTYPE_MESHCOLOR_EXT_1);
		rtMeshColor->m_ResourceExtensionsList.push_back(RESTYPE_MESHCOLOR_EXT_2);
	}

	MeshColor* MeshColor::Create()
	{
		return new MeshColor();
	}
}