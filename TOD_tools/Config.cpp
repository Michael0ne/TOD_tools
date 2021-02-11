#include "Config.h"

#include "Blocks.h"
#include "ScriptTypes.h"
#include "Scratchpad.h"
#include "SceneSaveLoad.h"
#include "Light.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "InputGameController.h"
#include "StreamedSoundBuffers.h"
#include "GfxInternal.h"
#include "Font.h"
#include "Random.h"
#include "LoadScreenInfo.h"
#include "Scene.h"
#include "Progress.h"
#include "RenderBuffer.h"
#include "SaveSlot.h"
#include "LogDump.h"
#include "EditorCamera.h"
#include "ZipArch.h"
#include "ResourcesTypes.h"
#include "ScriptDatabase.h"

namespace Script
{
	String Filesystem;
	String ControlType;
	String Region;
	String VersionName;
	String StreamedSoundExt;
	String LanguageMode;
	String ScriptsPath;
	List<StringTuple> DirectoryMappings;
}

namespace GameConfig
{
	Config* g_Config = nullptr;

	Config::Config()
	{
		MESSAGE_CLASS_CREATED(Config);

		m_Initialized = false;
		m_GameName;
		m_ConfigFilePath;
		m_sUnkString_1;
		m_sUnkString_2;
		m_SceneName;
		m_ConfigurationVariables = nullptr;
		m_SessionVariables = nullptr;
		field_4C = 0;
		m_ShouldStartGame = false;
		m_vBackgroundSize;
		m_nGlobalPropertiesListCRC = 0;
		m_nGlobalCommandsListCRC = 0;
		m_nTypesListCRC = 0;
		m_TotalGlobalProperties = 0;
		m_TotalGlobalCommands = 0;
		m_TotalTypes = 0;
		m_GlobalPropertiesListCRC = 0;
		m_GlobalCommandsListCRC = 0;
		m_TypesListCRC = 0;
		m_UninitialiseCallback = nullptr;
	}

	Config::~Config()
	{
		MESSAGE_CLASS_DESTROYED(Config);

		if (m_Initialized)
			UninitialiseGame();
	}

#pragma message(TODO_IMPLEMENTATION)
	void Config::Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId)
	{
#ifdef INCLUDE_FIXES
		clock_t timeStart = clock();
#endif

		CoInitialize(0);

		//	Set filename for configuration file.
		if (szConfigFilename && *szConfigFilename)
			m_ConfigFilePath = szConfigFilename;
		else
			m_ConfigFilePath = CONFIG_CONFIGFILE;

		//	Set gamename.
		m_GameName = CONFIG_GAMENAME;

		//	Try and look for configuration variables file.
		//	TODO: CreateBuffer implementation!
		if (File::FindFileEverywhere(m_ConfigFilePath.m_szString)) {
			LogDump::LogA("Initialising engine with '%s'\n", m_ConfigFilePath.m_szString);

			m_ConfigurationVariables = new Session_Variables(m_ConfigFilePath.m_szString, 1);;
		}else
			m_ConfigurationVariables = new Session_Variables(0);

		Session_Variables* pProfileVariables = nullptr;

		//	Try and look for profile variables file.
		if (File::FindFileEverywhere("/profile.txt"))
			pProfileVariables = new Session_Variables("/profile.txt", 0);

		if (m_ConfigurationVariables->IsVariableSet("filecheck"))
			Script::FileCheck = m_ConfigurationVariables->GetParamValueBool("filecheck") == 0;

		if (m_ConfigurationVariables->IsVariableSet("control_type"))
			Script::ControlType = m_ConfigurationVariables->GetParamValueString("control_type");

		if (m_ConfigurationVariables->IsVariableSet("forcefeedback"))
			Script::ForceFeedback = m_ConfigurationVariables->GetParamValueBool("forcefeedback");

		Script::LoadBlocks = true;
		if (m_ConfigurationVariables->IsVariableSet("loadblocks"))
			Script::LoadBlocks = m_ConfigurationVariables->GetParamValueBool("loadblocks");

		//	Try and initialize Blocks class.
		g_Blocks = new Blocks(Script::LoadBlocks);

		//	Init script types.
		InitScriptTypes();

		//	Init scratchpad (mostly used in CollisionProbe calculations).
		g_Scratchpad = new Scratchpad();

		//	Init SceneNode (contains rewind buffer).
		g_SceneSaveLoad = new SceneSaveLoad();

		//	Init resources types.
		ResType::Texture::CreateInstance();
		ResType::Font::CreateInstance();
		ResType::Text::CreateInstance();
		ResType::Model::CreateInstance();
		ResType::Fragment::CreateInstance();
		ResType::Movie::CreateInstance();
		ResType::Cutscene::CreateInstance();
		ResType::Sound::CreateInstance();
		ResType::StreamedSoundInfo::CreateInstance();
		ResType::Animation::CreateInstance();
		ResType::MeshColor::CreateInstance();

		//	Init unknown matricies.
		CreateUnknownMatricies();

		//	Register script entities.
		//	TODO: implementation!
		InitEntitiesDatabase();

		//	Init unknown maps (lists), somehow related to renderer.
		//	TODO: implementation!
		(*(void(__cdecl*)())0x460AB0)();
		(*(void(__cdecl*)())0x4651B0)();	//	NOTE: init some verticies map.

		//	Init rendered textures map.
		GfxInternal_Dx9_Texture::InitTexturesMap();

		//	Init renderer commands buffer.
		RenderBuffer::CreateRenderBuffer();

		//	Init lights.
		Light::InitLightsList();

		if (m_ConfigurationVariables->IsVariableSet("ps2_max_texture_size"))
			Script::Ps2MaxTextureSize = m_ConfigurationVariables->GetParamValueInt("ps2_max_texture_size");

		if (m_ConfigurationVariables->IsVariableSet("overridelights"))
			Light::OverrideLights(m_ConfigurationVariables->GetParamValueBool("overridelights"));

		Script::Fullscreen = true;
		if (m_ConfigurationVariables->IsVariableSet("fullscreen"))
			Script::Fullscreen = m_ConfigurationVariables->GetParamValueBool("fullscreen");

		Script::Filesystem = "blocks.naz, sounds.naz, videos00.naz, videos01.naz";
		if (m_ConfigurationVariables->IsVariableSet("filesystem"))
			Script::Filesystem = m_ConfigurationVariables->GetParamValueString("filesystem");

#ifdef INCLUDE_FIXES
		m_UninitialiseCallback = new ConfigCallback();
#else
		m_UninitialiseCallback = nullptr;
#endif

		//	If log dump file is here - open it.
		if (m_ConfigurationVariables->IsVariableSet("logdumpfile")) {
			LogDump::OpenLogDump(m_ConfigurationVariables->GetParamValueString("logdumpfile").m_szString);
		}

		//	If we have 'profile.txt' available, check directory mappings and other stuff.
		if (pProfileVariables) {
			char DirectorymappingStr[22];
			unsigned int index = 0;

			memset(&DirectorymappingStr, 0, sizeof(DirectorymappingStr));

			sprintf(DirectorymappingStr, "directorymapping%d", index++);

			while (pProfileVariables->IsVariableSet(DirectorymappingStr)) {
				String dirmapping = pProfileVariables->GetParamValueString(DirectorymappingStr);

				if (strchr(dirmapping.m_szString, '>') != nullptr)
				{
					//	NOTE: potential place for buffer overflow right here, original code uses heap allocation, but this is redundant here really, adjust buffer size if any errors happen.
					char direntry[128] = {};
					strncpy_s(direntry, sizeof(direntry), dirmapping.m_szString, strchr(dirmapping.m_szString, '>') - dirmapping.m_szString);

					File::AddDirectoryMappingsListEntry(direntry, strchr(dirmapping.m_szString, '>') + 1);
				}

				sprintf(DirectorymappingStr, "directorymapping%d", index++);
			}
		}

		//	Is this the testing build?
		String TestingPath;
		GetInternalGameName(TestingPath);

		if (_stricmp(TestingPath.m_szString, "testing"))
		{
			m_GameName.Append(" *");
			m_GameName.Append(TestingPath.m_szString);
			m_GameName.Append("* ");
		}

		m_GameName.Append(" (");

		//	Scripts search path if 'profile.txt' is available.
		Script::ScriptsPath = "/data/scripts/stable/";

		if (pProfileVariables && pProfileVariables->IsVariableSet("script_searchpath"))
			Script::ScriptsPath = pProfileVariables->GetParamValueString("script_searchpath");

#ifdef INCLUDE_FIXES
		m_GameName.Append(" scripts: ");
		m_GameName.Append(Script::ScriptsPath.m_szString);
		m_GameName.Append(")");
#else
		m_GameName.Append(" scripts: baked)");
		m_GameName = CONFIG_GAMENAME;
#endif

		if (m_ConfigurationVariables->IsVariableSet("relax_build_version_check"))
			Script::RelaxBuildVersionCheck = m_ConfigurationVariables->GetParamValueBool("relax_build_version_check");

		//	What icon should we use?
		if (_stricmp(TestingPath.m_szString, "stable"))
			Script::IconResourceId = 103;
		else
			Script::IconResourceId = 101;

		//	Detect and set language.
		Script::LanguageStringsOffset = 0;

		//	TODO: implementation for utilities functions!
		if (Utils::IsFileAvailable("/es_sounds.zip") || Utils::IsFileAvailable("/es_sounds.naz"))
			Script::LanguageStringsOffset = 4;
		if (Utils::IsFileAvailable("/fr_sounds.zip") || Utils::IsFileAvailable("/fr_sounds.naz"))
			Script::LanguageStringsOffset = 1;
		if (Utils::IsFileAvailable("/de_sounds.zip") || Utils::IsFileAvailable("/de_sounds.naz"))
			Script::LanguageStringsOffset = 3;
		if (Utils::IsFileAvailable("/it_sounds.zip") || Utils::IsFileAvailable("/it_sounds.naz"))
			Script::LanguageStringsOffset = 2;
		if (Utils::IsFileAvailable("/uk_sounds.zip") || Utils::IsFileAvailable("/uk_sounds.naz"))
			Script::LanguageStringsOffset = 0;

		if (m_ConfigurationVariables->IsVariableSet("language_mode"))
			Script::SetCountryCode(m_ConfigurationVariables->GetParamValueString("language_mode").m_szString);

		Script::LanguageMode = Script::CountryCodes[Script::LanguageStringsOffset];

		//	Create required devices - window, mouse, keyboard, gamepad.
		g_Window = new Window(m_GameName.m_szString, 1, CONFIG_MENU_RESOURCEID, Script::Filesystem.m_szString, !Script::IconResourceId ? nIconResId : Script::IconResourceId);

		g_InputMouse = new Input::Mouse();
		g_InputKeyboard = new Input::Keyboard();
		g_InputGamepad[0] = new Input::Gamepad(0, 1);

		Input::Gamepad::GetGameControllerByIndex(0);

		//	Init saves directories information (ps2 emulation and pc).
		SaveSlots[SAVE_SLOT_0] = new SaveSlot(SAVE_SLOT_0);
		SaveSlots[SAVE_SLOT_1] = new SaveSlot(SAVE_SLOT_1);
		SaveSlots[SAVE_SLOT_8] = new SaveSlot(SAVE_SLOT_8);

		Script::SavePlatformPS2 = true;

		//	Detect platform.
		if (m_ConfigurationVariables->IsVariableSet("save_platform"))
		{
			String platform = m_ConfigurationVariables->GetParamValueString("save_platform");

			//	For PS2 we want 2 memory cards and harddisk available.
			if (platform.Equal("PS2")) {
				const char* szMemcard0[2] = {
					"/savegames/memorycard0/",
					"/savegames/memorycard1/"
				};

				int memcardindex = 0;

				do {
					//	TODO: implementation for utility function!
					Utils::CreateDirectoriesRecursive(szMemcard0[memcardindex]);
					SaveSlots[memcardindex]->m_SaveFolderPath = szMemcard0[memcardindex];

					if (!SaveSlots[memcardindex]->IsFormatted())
						SaveSlots[memcardindex]->FormatCard();

					memcardindex++;
				} while (memcardindex < 2);
			}

			const char szHarddisk[] = "/savegames/harddisk/";
			Utils::CreateDirectoriesRecursive(szHarddisk);
			SaveSlots[SAVE_SLOT_8]->m_SaveFolderPath = szHarddisk;

			if (!SaveSlots[SAVE_SLOT_8]->IsFormatted())
				SaveSlots[SAVE_SLOT_8]->FormatCard();
		}else{
			//	For PC, just figure out system user data directory.
			Script::SavePlatformPS2 = false;
			Script::FileCheck = true;

			GetUserDocumentsDir(SaveSlots[SAVE_SLOT_8]->m_SaveFolderPath);
			SaveSlots[SAVE_SLOT_8]->m_SaveFolderPath.Append(CONFIG_SAVEDIR);
		}

		Script::CutsceneDisableAware = false;
		if (m_ConfigurationVariables->IsVariableSet("cutscene_disable_aware"))
			Script::CutsceneDisableAware = m_ConfigurationVariables->GetParamValueBool("cutscene_disable_aware");

		Script::CutsceneForceCompleteLodUpdates = false;
		if (m_ConfigurationVariables->IsVariableSet("cutscene_force_complete_lod_updates"))
			Script::CutsceneForceCompleteLodUpdates = m_ConfigurationVariables->GetParamValueBool("cutscene_force_complete_lod_updates");

		//	Default streamed sound files extension.
		if (m_ConfigurationVariables->IsVariableSet("streamed_sound_ext"))
			Script::StreamedSoundExt = m_ConfigurationVariables->GetParamValueString("streamed_sound_ext");
		else
			Script::StreamedSoundExt = "ogg";

		//	Figure out sound renderer.
		if (m_ConfigurationVariables->IsVariableSet("soundrenderer")) {
			String soundrenderer = m_ConfigurationVariables->GetParamValueString("soundrenderer");
			Audio::SoundSystemType rendererid = Audio::SOUND_SYSTEM_UNDEFINED;

			soundrenderer.ToLowerCase();

			if (soundrenderer.Equal("directsound"))
				rendererid = Audio::SOUND_SYSTEM_DSOUND;
			if (soundrenderer.Equal("dieselpower"))
				rendererid = Audio::SOUND_SYSTEM_DIESELPOWER;
			if (soundrenderer.Equal("auto"))
				rendererid = Audio::SOUND_SYSTEM_AUTOSELECT;

			if (rendererid != 0)
				Audio::RememberSoundRenderer(rendererid);
		}

		//	Init sound renderer.
		g_StreamedSoundBuffers = new Audio::StreamedSoundBuffers(1, 44100);

		if (m_ConfigurationVariables->IsVariableSet("sound_max_concurrent_sounds"))
			g_StreamedSoundBuffers->m_nMaxConcurrentSounds = m_ConfigurationVariables->GetParamValueInt("sound_max_concurrent_sounds");

		//	Override default volume values.
		if (m_ConfigurationVariables->IsVariableSet("change_sound_group_volume_scaling") && m_ConfigurationVariables->GetParamValueBool("change_sound_group_volume_scaling")) {
			if (m_ConfigurationVariables->IsVariableSet("default_fx_volume_var"))
				Audio::SetDefaultFxVolume(m_ConfigurationVariables->GetParamValueFloat("default_fx_volume_var"));

			if (m_ConfigurationVariables->IsVariableSet("default_ambience_volume_var"))
				Audio::SetDefaultAmbienceVolume(m_ConfigurationVariables->GetParamValueFloat("default_ambience_volume_var"));

			if (m_ConfigurationVariables->IsVariableSet("default_music_volume_var"))
				Audio::SetDefaultMusicVolume(m_ConfigurationVariables->GetParamValueFloat("default_music_volume_var"));

			if (m_ConfigurationVariables->IsVariableSet("default_speaks_volume_var"))
				Audio::SetDefaultSpeaksVolume(m_ConfigurationVariables->GetParamValueFloat("default_speaks_volume_var"));
		}

		//	Sound is enabled if not overridden.
		g_StreamedSoundBuffers->m_Sound = !m_ConfigurationVariables->IsVariableSet("sound") ||
													m_ConfigurationVariables->GetParamValueBool("sound");

		//	NOTE: what is this parameter?
		Vector3<float> vBackground = Vector3<float>();
		if (m_ConfigurationVariables->IsVariableSet("background"))
			m_ConfigurationVariables->GetParamValueVector3("background", vBackground, ',');

		m_vBackgroundSize.x = vBackground.x * (float)(1/255);
		m_vBackgroundSize.y = vBackground.y * (float)(1/255);
		m_vBackgroundSize.z = vBackground.z * (float)(1/255);

		//	Override screen size.
		Vector2<int> ScreenSize = Vector2<int>();
		if (m_ConfigurationVariables->IsVariableSet("screensize"))
			m_ConfigurationVariables->GetParamValueVector2("screensize", ScreenSize, ',');

		//	Widescreen emulation
		if (m_ConfigurationVariables->IsVariableSet("widescreen_emulation")) {
			String sWidescreenType = m_ConfigurationVariables->GetParamValueString("widescreen_emulation");

			if (sWidescreenType.Equal("pal")) {
				GfxInternal::WideScreen = true;
				ScreenSize.x = 720;
				ScreenSize.y = 576;

				LogDump::LogA("EMULATING Pal-Wide\n");
			}

			if (sWidescreenType.Equal("ntsc")) {
				GfxInternal::WideScreen = true;
				ScreenSize.x = 720;
				ScreenSize.y = 480;

				LogDump::LogA("EMULATING Ntsc-Wide\n");
			}

			if (sWidescreenType.Equal("hdtv")) {
				GfxInternal::WideScreen = true;
				ScreenSize.x = 1280;
				ScreenSize.y = 720;

				LogDump::LogA("EMULATING HDTV-Wide\n");
			}
		}

		if (m_ConfigurationVariables->IsVariableSet("FSAA"))
			GfxInternal::FSAA = m_ConfigurationVariables->GetParamValueBool("FSAA");

		if (m_ConfigurationVariables->IsVariableSet("ratioxy"))
			GfxInternal::RatioXY = m_ConfigurationVariables->GetParamValueFloat("ratioxy");

		Vector3<float> ScreenBuffers[31];

		for (unsigned int i = 0; i < 31; i++)
			ScreenBuffers[i];

		ScreenBuffers[11].x = ScreenBuffers[12].x = 2.f;

		g_GfxInternal = new GfxInternal(&ScreenSize, 32, 16, (Script::Fullscreen ? (GfxInternal::FSAA != 0 ? 0x200 : 0) : 130), 31, 20, ScreenBuffers);

		//	Set region.
		Script::Region = Script::IsRegionEurope() ? "europe" : "usa";

		if (m_ConfigurationVariables->IsVariableSet("region"))
			Script::Region = m_ConfigurationVariables->GetParamValueString("region").m_szString;

		g_Blocks->SetRegionId(GetRegionId(&Script::Region));
		LogDump::LogA("Using region: %s\n", Script::Region.m_szString);

		if (m_ConfigurationVariables->IsVariableSet("virtual_hud_screensize")) {
			Vector2<float> VirtualHudSize;
			m_ConfigurationVariables->GetParamValueVector2("virtual_hud_screensize", VirtualHudSize, ',');

			g_ScreenProperties.SetHudScreenSize(VirtualHudSize.x, VirtualHudSize.y, 1.0, 1.0);
		}

		if (m_ConfigurationVariables->IsVariableSet("screen_safe_area"))
			g_ScreenProperties.SetSafeArea(m_ConfigurationVariables->GetParamValueFloat("screen_safe_area"));

		g_GfxInternal->SetClearColorForBufferIndex(*((ColorRGB*)&m_vBackgroundSize), -1);

		//	TODO: what is this?
		GfxInternal::_A08704[0].field_0 = 0;
		GfxInternal::_A08704[1].field_0 = 0;
		GfxInternal::_A08704[5].field_0 = 0;

		if (m_ConfigurationVariables->IsVariableSet("version_name"))
			Script::VersionName = m_ConfigurationVariables->GetParamValueString("version_name").m_szString;

		//	NOTE: this is unused.
		Script::_A1B98D = 0;

		//	Initialize baked font.
		//	TODO: implementation!
		Font::MakeCharactersMap((void*)0xA1B698);
		g_Font = new Font((const void*)0xA1B698);

		//	Load NAZ archives into memory.
		File::ReadZipDirectories(Script::Filesystem.m_szString);

		//	Parse collmat file
		EnumMaterialsInCollmat();
		//	Parse facecolmat file
		EnumFaceColMaterials();

		//	Init random numbers generator.
		Random::Init((int)__rdtsc());

		//	Load screen and progress class.
		g_LoadScreenInfo = new LoadScreenInfo("");
		g_Progress = new Progress();

		if (m_ConfigurationVariables->IsVariableSet("show_hud") &&
			m_ConfigurationVariables->GetParamValueBool("show_hud"))
			Script::ShowHud = true;

		if (m_ConfigurationVariables->IsVariableSet("check_original_asset") &&
			!m_ConfigurationVariables->GetParamValueBool("check_original_asset"))
			Script::CheckOriginalAsset = false;

		if (m_ConfigurationVariables->IsVariableSet("warning_window"))
			Script::WarningShow = m_ConfigurationVariables->GetParamValueBool("warning_window");

		//	NOTE: this adjusts size for global entities lists.
		//(*(void(__cdecl*)(int, int))0x8C66E0)(8192, 512);

		//Script::LoadScripts();

		//	Since scripts loaded and ready, calculate CRC and remember it.
		//	TODO: implementation!
		//m_GlobalPropertiesListCRC = (*(int(__cdecl*)())0x873440)();
		//m_GlobalCommandsListCRC = (*(int (__cdecl*)())0x871DD0)();
		//m_TypesListCRC = (*(int(__cdecl*)())0x862CF0)();

		//	Instantiate scene.
		bool SceneSet = false;
		if (m_ConfigurationVariables->IsVariableSet("scenefile"))
		{
			const char* sceneFile = m_ConfigurationVariables->GetParamValueString("scenefile").m_szString;
			LogDump::LogA("Opening scene %s\n", sceneFile);

			//SceneSet = OpenScene(sceneFile);
		}

		//if (!SceneSet)
			//if (!(SceneSet = OpenScene("/data/Overdose_THE_GAME/OverdoseIntro.scene")))
				//tScene->CreateNode();

		//	TODO: implementation! Scene is not initialized here!
		if (m_ConfigurationVariables->IsVariableSet("fixedframerate"))
		{
			Scene::SceneInstance->m_FixedFramerate = true;
			Scene::SceneInstance->m_FixedFramerateVal = 1.0f / m_ConfigurationVariables->GetParamValueFloat("fixedframerate");
		}

		if (!Script::Fullscreen)
		{
			Camera* sceneCamera = (Camera*)Scene::SceneInstance->m_FirstChild;
			if (sceneCamera)
			{
				do
				{
					EditorCamera* editorCamera = (EditorCamera*)sceneCamera;
					if (editorCamera)
					{
						while (tEditorCamera != editorCamera)
						{
							editorCamera = (EditorCamera*)editorCamera->m_Parent;
							if (!editorCamera)
								break;
						}
						Scene::SceneInstance->m_GameCamera = sceneCamera;
						Scene::SceneInstance->UpdateActiveCameraPosition();
					}
					sceneCamera = (Camera*)sceneCamera->m_NextSibling;
				} while (sceneCamera);
			}
		}

		if (m_ConfigurationVariables->IsVariableSet("frame_console_marker"))
			Script::FrameConsoleMarker = m_ConfigurationVariables->GetParamValueBool("frame_console_marker");

		Script::CheckDataSanity = false;
		Script::CheckDivisionByZero = false;

		if (pProfileVariables) {
			if (pProfileVariables->IsVariableSet("check_data_sanity"))
				Script::CheckDataSanity = pProfileVariables->GetParamValueBool("check_data_sanity");

			if (pProfileVariables->IsVariableSet("check_division_by_zero"))
				Script::CheckDivisionByZero = pProfileVariables->GetParamValueBool("check_division_by_zero");
		}

		//	Start editor or game.
		m_ShouldStartGame = false;
		if (SceneSet)
			m_ShouldStartGame = true;

		if (m_ConfigurationVariables->IsVariableSet("starteditor") &&
			m_ConfigurationVariables->GetParamValueBool("starteditor"))
			m_ShouldStartGame = false;

		if (m_ShouldStartGame)
			Scene::SceneInstance->Start();

		if (m_ConfigurationVariables->IsVariableSet("ps2_play_ctrl") &&
			m_ConfigurationVariables->GetParamValueBool("ps2_play_ctrl"))
			Script::Ps2PlayCtrl = true;

		m_Initialized = 1;

		if (m_ConfigurationVariables->IsVariableSet("min_fade_dist"))
			Script::MinFadeDist = m_ConfigurationVariables->GetParamValueFloat("min_fade_dist");

		if (m_ConfigurationVariables->IsVariableSet("lod_and_fade"))
			Script::LodAndFade = m_ConfigurationVariables->GetParamValueBool("lod_and_fade");

		if (m_ConfigurationVariables->IsVariableSet("check_dangling_refs"))
			Script::CheckDanglingRefs = m_ConfigurationVariables->GetParamValueBool("check_dangling_refs");

		if (m_ConfigurationVariables->IsVariableSet("fix_dangling_refs"))
			Script::FixDanglingRefs = m_ConfigurationVariables->GetParamValueBool("fix_dangling_refs");

		if (m_ConfigurationVariables->IsVariableSet("simulate_release_build")) {
			Script::SimulateReleaseBuild = m_ConfigurationVariables->GetParamValueBool("simulate_release_build");
			LogDump::LogA("Script::SimulateReleaseBuild == %i\n", Script::SimulateReleaseBuild);
		}

#ifdef INCLUDE_FIXES
		g_Window->SetCursorReleased(true);
#else
		g_Window->SetCursorReleased(false);
#endif

		if (pProfileVariables)
			delete pProfileVariables;

#ifdef INCLUDE_FIXES
		debug("Game init complete! Took %i ms\n", clock() - timeStart);
#endif
	}

#pragma message(TODO_IMPLEMENTATION)
	void Config::InitEntitiesDatabase()
	{
		/*
		Entity::Register();
		Scripts::RegisterGlobalProperty("block_id:integer", 1);
		Node::Register();
		NodeSpatial::Register();
		NodeLogical::Register();
		Camera::Register();
		Model::Register();
		Folder::Register();
		Scene::Register();
		MockupBox::Register();
		EditorCamera::Register();
		Character::Register();
		Bone::Register();
		AnimSlot::Register();
		MotionAnimSlot::Register();
		AnimLayer::Register();
		CutscenePlayer::Register();
		MotionLayer::Register();
		CollisionBox::Register();
		CollisionSphere::Register();
		WayPoint::Register();
		TrackPoint::Register();
		Navigator::Register();
		SignPost::Register();
		Track::Register();
		ControlSetup::Register();
		Control::Register();
		Group::Register();
		CollisionProbe::Register();
		RigidBody::Register();
		OverdoseVehicle::Register();
		StretchModel::Register();
		SkyBox::Register();
		Fog::Register();
		Wind::Register();
		Cloth::Register();
		GFXEffect::Register();
		Shadow::Register();
		LensFlare::Register();
		LensFlareManager::Register();
		DecalManager::Register();
		SurroundGeometry::Register();
		DynamicSurroundGeometry::Register();
		Light::Register();
		Sprite::Register();
		TextBox::Register();
		TextSlot::Register();
		GuideBox::Register();
		Bullet::Register();
		SoundSlot::Register();
		SoundEmitter::Register();
		StaticLighting::Register();
		MoviePlayer::Register();
		ParticleSystem::Register();
		GEKeyFrame::Register();
		GeometryEffect::Register();
		ProfilerInput::Register();
		RealtimeFolder::Register();
		Builtin::Register();
		PlaceHolder::Register();
		CharacterPlaceHolder::Register();
		HavocPlaceHolder::Register();
		GoodiePlaceHolder::Register();
		WeaponPlaceHolder::Register();
		MemoryCards::Register();
		LoadScreenNode::Register();

		m_nGlobalPropertiesListCRC = (*(int (*)())0x873440)();
		m_nGlobalCommandsListCRC = (*(int (*)())0x871DD0)();
		m_nTypesListCRC = (*(int (*)())0x862CF0)();

		m_TotalGlobalProperties = (*(int (*)())0x872FB0)();
		m_TotalGlobalCommands = (*(int (*)())0x871A20)();
		m_TotalTypes = (*(int (*)())0x862B30)();
		*/
	}

#pragma message(TODO_IMPLEMENTATION)
	void Config::UninitialiseGame()
	{
		//Scene::SceneInstance->Destroy();
		//ClearScriptLists();	//	@863380
		Light::ClearLightsList();

		g_Window->SetCursorReleased(true);

		delete m_ConfigurationVariables;
		delete g_Blocks;
		delete g_InputMouse;
		delete g_InputKeyboard;
		delete Input::Gamepad::GetGameControllerByIndex(0);
		delete g_GfxInternal;
		delete g_StreamedSoundBuffers;
		delete g_Window;

		CoUninitialize();

		if (m_UninitialiseCallback)
			m_UninitialiseCallback->UninitialiseGameCallback(1);

		m_Initialized = 0;
	}

	bool Config::OpenScene(const char* scene)
	{
		tScene->CreateNode();
		g_Blocks->SetSceneName(scene);
		Scene::SceneInstance->Load(scene);
		Scene::SceneInstance->UpdateLoadedBlocks(0, 0);
		Scene::SceneInstance->m_StartTimeMs = Performance::GetMilliseconds();
		Scene::SceneInstance->RefreshChildNodes();
		Scene::SceneInstance->FinishCreation("Scene instantiate all completed.");

		return true;
	}

#pragma message(TODO_IMPLEMENTATION)
	void Config::CreateUnknownMatricies()
	{	
	}

	void Session_Variables::LoadVariablesFile(const char* file, bool configvariables)
	{
		LogDump::LogA("Loading variable file '%s'...\n", file);	//	NOTE: actual EXE code doesn't have call to LogA, only sprintf.

		File file_(file, 1, true);
		ParseVariablesFile(&file_, configvariables);
	}

#pragma message(TODO_IMPLEMENTATION)
	void Session_Variables::ParseVariablesFile(File* file, bool configvariables)
	{
		//file->WriteFromBuffer();
		//int filesize = file->GetPosition();
		//file->WriteBufferAndSetToStart();

		//char* buffer = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(filesize + 1, NULL, NULL);
		//int bytesread = file->Read(buffer, filesize);
		//buffer[bytesread] = NULL;
	}

	Session_Variables::Session_Variables(int)
	{
		MESSAGE_CLASS_CREATED(Session_Variables);

		m_PlainValues = { 17 };
		m_Keys = { 17 };
		field_20 = { 17 };
		field_30 = { 17 };
		field_40 = { 17 };
		field_50 = { 17 };

		m_TotalVariables = NULL;
		field_64 = NULL;
	}

	Session_Variables::Session_Variables(const char* file, bool configvariables)
	{
		MESSAGE_CLASS_CREATED(Session_Variables);

		m_PlainValues = { 17 };
		m_Keys = { 17 };
		field_20 = { 17 };
		field_30 = { 17 };
		field_40 = { 17 };
		field_50 = { 17 };

		LoadVariablesFile(file, configvariables);
	}

#pragma message(TODO_IMPLEMENTATION)
	Session_Variables::~Session_Variables()
	{
		MESSAGE_CLASS_DESTROYED(Session_Variables);

		(*(void (__thiscall*)(Session_Variables*))0x4107B0)(this);
	}

#pragma message(TODO_IMPLEMENTATION)
	bool Session_Variables::IsVariableSet(const char* variableName)
	{
		bool(__thiscall * _IsVariableSet)(Session_Variables* _this, const char* _varname) = (bool (__thiscall*)(Session_Variables*, const char*))0x410080;
		
		return _IsVariableSet(this, variableName);
	}

#pragma message(TODO_IMPLEMENTATION)
	bool Session_Variables::GetParamValueBool(const char* variableName)
	{
		bool(__thiscall * _GetParamValueBool)(Session_Variables * _this, const char* _varname) = (bool(__thiscall*)(Session_Variables*, const char*))0x410900;
		
		return _GetParamValueBool(this, variableName);
	}

#pragma message(TODO_IMPLEMENTATION)
	int Session_Variables::GetParamValueInt(const char* variableName)
	{
		int(__thiscall * _GetParamInt)(Session_Variables * _this, const char* _varname) = (int(__thiscall*)(Session_Variables*, const char*))0x410A30;

		return _GetParamInt(this, variableName);
	}

#pragma message(TODO_IMPLEMENTATION)
	float Session_Variables::GetParamValueFloat(const char* variableName)
	{
		float(__thiscall * _GetParamFloat)(Session_Variables * _this, const char* _varname) = (float(__thiscall*)(Session_Variables*, const char*))0x410AC0;

		return _GetParamFloat(this, variableName);
	}

#pragma message(TODO_IMPLEMENTATION)
	const Vector2<int>& Session_Variables::GetParamValueVector2(const char* variableName, Vector2<int>& outVec, char delimiter)
	{
		const Vector2<int>& (__thiscall * _GetParamVec2i)(Session_Variables * _this, Vector2<int>* _outvec, const char* _varname, char _delim) = (const Vector2<int>&(__thiscall*)(Session_Variables*, Vector2<int>*, const char*, char))0x410B50;

		return _GetParamVec2i(this, &outVec, variableName, delimiter);
	}

#pragma message(TODO_IMPLEMENTATION)
	const Vector2<float>& Session_Variables::GetParamValueVector2(const char* variableName, Vector2<float>& outVec, char delimiter)
	{
		const Vector2<float>&(__thiscall * _GetParamVec2)(Session_Variables * _this, Vector2<float>*, const char* _varname, char _delim) = (const Vector2<float>&(__thiscall*)(Session_Variables*, Vector2<float>*, const char*, char))0x410BE0;

		return _GetParamVec2(this, &outVec, variableName, delimiter);
	}

#pragma message(TODO_IMPLEMENTATION)
	const Vector4f& Session_Variables::GetParamValueVector4(const char* variableName, Vector4f& outVec, char delimiter)
	{
		const Vector4f&(__thiscall * _GetParamVec4)(Session_Variables * _this, Vector4f* _outvec, const char* _varname, char _delim) = (const Vector4f&(__thiscall*)(Session_Variables*, Vector4f*, const char*, char))0x410C70;

		return _GetParamVec4(this, &outVec, variableName, delimiter);
	}

#pragma message(TODO_IMPLEMENTATION)
	const Vector3<float>& Session_Variables::GetParamValueVector3(const char* variableName, Vector3<float>& outVec, char delimiter)
	{
		const Vector3<float>& (__thiscall * _GetParamVec3)(Session_Variables * _this, const char* _varname, float* _outX, float* _outY, float* _outZ, char _delim) = (const Vector3<float> & (__thiscall*)(Session_Variables*, const char*, float*, float*, float*, char))0x410D90;

		return _GetParamVec3(this, variableName, &outVec.x, &outVec.y, &outVec.z, delimiter);
	}

#pragma message(TODO_IMPLEMENTATION)
	void Session_Variables::GetParamValueString(const char* variableName, String& outStr)
	{
		void (__thiscall * _GetParamString)(Session_Variables * _this, String* _outstr, const char* _varname) = (void (__thiscall*)(Session_Variables*, String*, const char*))0x410E30;

		_GetParamString(this, &outStr, variableName);
	}

#pragma message(TODO_IMPLEMENTATION)
	const String& Session_Variables::GetParamValueString(const char* variableName)
	{
		String* outStr = nullptr;
		const String& (__thiscall * _GetParamString)(Session_Variables * _this, String * _outstr, const char* _varname) = (const String& (__thiscall*)(Session_Variables*, String*, const char*))0x410E30;

		return _GetParamString(this, outStr, variableName);
	}

#pragma message(TODO_IMPLEMENTATION)
	void Session_Variables::SetParamValue(const char* variableName, char* value)
	{
		void(__thiscall * _SetParamValue)(Session_Variables * _this, const char* _varname, char* _val) = (void(__thiscall*)(Session_Variables*, const char*, char*))0x4114E0;

		_SetParamValue(this, variableName, value);
	}

#pragma message(TODO_IMPLEMENTATION)
	void Session_Variables::SetParamValueBool(const char* variableName, int value)
	{
		void(__thiscall * _SetParamValueBool)(Session_Variables * _this, const char* _varname, int _val) = (void(__thiscall*)(Session_Variables*, const char*, int))0x4116D0;

		_SetParamValueBool(this, variableName, value);
	}

	void InitialiseGame(LPSTR cmdline)
	{
		g_Config = new Config();

		g_Config->Process(cmdline, NULL, nullptr, NULL);
		g_Window->Process(Scene::GameUpdate);

		delete g_Config;
	}

	CountryCodes GetRegionId(String* regionStr)
	{
		if (regionStr->Equal("europe"))
			return COUNTRY_EUROPE;

		if (regionStr->Equal("usa"))
			return COUNTRY_USA;

		if (regionStr->Equal("asia"))
			return COUNTRY_ASIA;

		return COUNTRY_UNKNOWN;
	}

#pragma message(TODO_IMPLEMENTATION)
	void EnumMaterialsInCollmat()
	{
		(*(void(*)())0x87D330)();
	}

	void EnumFaceColMaterials()
	{
		if (FaceColList.m_CurrIndex > 0)
			return;

		if (!File::FindFileEverywhere("/FaceColl.mat"))
			return;

		File faceColFile("/FaceColl.mat", 1, true);

		if (!faceColFile.IsFileOpen())
			return;

		String buffer;

		while (faceColFile.ReadString(&buffer)) {
			buffer.ToLowerCase();
			FaceColList.AddElement(&buffer);
		}
	}

	void GetInternalGameName(String& outStr)
	{
#ifdef INCLUDE_FIXES
		const char devpath[] = "E:/Develop/KapowSystems/TOD1/Libs/Toolbox/Functions.cpp";
#else
		const char devpath[] = __FILE__;
		String::ConvertBackslashes((char*)devpath);
#endif
		const char* kapowsystems_pos = strstr(devpath, "KapowSystems");

		if (kapowsystems_pos)
		{
			char buf[64] = {};
			strcpy(buf, devpath + (kapowsystems_pos - devpath + 13));
			*strchr(buf, '/') = NULL;
			
			outStr = buf;
		}
	}

	void ConfigCallback::UninitialiseGameCallback(int)
	{
#ifdef INCLUDE_FIXES
		LogDump::LogA("Uninitialise game callback called!\n");
#endif
	}
}