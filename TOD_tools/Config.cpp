#include "Config.h"

#include "Blocks.h"
#include "ScriptTypes.h"
#include "Scratchpad.h"
#include "SceneNode.h"
#include "Light.h"
#include "Window.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "InputGameController.h"
#include "StreamedSoundBuffers.h"
#include "Renderer.h"
#include "Font.h"
#include "Random.h"
#include "LoadScreenInfo.h"
#include "Scene.h"
#include "Progress.h"
#include "RenderBuffer.h"
#include "SavesDirectoriesInformation.h"
#include "LogDump.h"

String Script::Filesystem = String();
String Script::ControlType = String();
String Script::Region = String();

namespace GameConfig
{
	Config* g_Config = nullptr;

	Config::Config()
	{
		MESSAGE_CLASS_CREATED(Config);

		field_0 = 0;
		m_sGameName = String();
		m_sConfigFilePath = String();
		m_sUnkString_1 = String();
		m_sUnkString_2 = String();
		m_sSceneName = String();
		m_pConfigurationVariables = nullptr;
		m_pSessionVariables = nullptr;
		field_4C = 0;
		field_50 = 0;
		m_vBackgroundSize = Vector4<float>();
		m_nGlobalPropertiesListCRC = 0;
		m_nGlobalCommandsListCRC = 0;
		m_nTypesListCRC = 0;
		m_nTotalGlobalProperties = 0;
		m_nTotalGlobalCommands = 0;
		m_nTotalTypes = 0;
		m_nCRCForScriptsListUnk = 0;
		m_nCRCForScriptsGlobalList = 0;
		m_nCRCForTypesList = 0;
	}

	void Config::Process(LPSTR lpCmdLine, int unk, const char* szConfigFilename, signed int nIconResId)
	{
#ifdef INCLUDE_FIXES
		clock_t timeStart = clock();
#endif

		CoInitialize(0);

		//	Set filename for configuration file.
		if (szConfigFilename && *szConfigFilename)
			m_sConfigFilePath = String(szConfigFilename);
		else
			m_sConfigFilePath = String(CONFIG_CONFIGFILE);

		//	Set gamename.
		m_sGameName = String(CONFIG_GAMENAME);

		//	Try and look for configuration variables file.
		//	TODO: CreateBuffer implementation!
		if (Utils::FindFileEverywhere(m_sConfigFilePath.m_szString)) {
			debug("Initialising engine with '%s'\n", m_sConfigFilePath.m_szString);

			m_pConfigurationVariables = new Session_Variables(m_sConfigFilePath.m_szString, 1);;
		}else
			m_pConfigurationVariables = new Session_Variables(0);

		Session_Variables* pProfileVariables = nullptr;

		//	Try and look for profile variables file.
		if (Utils::FindFileEverywhere("/profile.txt"))
			pProfileVariables = new Session_Variables("/profile.txt", 0);

		if (m_pConfigurationVariables->IsVariableSet("filecheck"))
			Script::FileCheck = m_pConfigurationVariables->GetParamValueBool("filecheck") == 0;

		if (m_pConfigurationVariables->IsVariableSet("control_type"))
			Script::ControlType = m_pConfigurationVariables->GetParamValueString("control_type");

		if (m_pConfigurationVariables->IsVariableSet("forcefeedback"))
			Script::ForceFeedback = m_pConfigurationVariables->GetParamValueBool("forcefeedback");

		Script::LoadBlocks = true;
		if (m_pConfigurationVariables->IsVariableSet("loadblocks"))
			Script::LoadBlocks = m_pConfigurationVariables->GetParamValueBool("loadblocks");

		//	Try and initialize Blocks class.
		//	TODO: Blocks implementation!
		g_Blocks = new Blocks(Script::LoadBlocks);

		//	Init script types.
		ScriptTypes::Init();

		//	Init scratchpad (mostly used in CollisionProbe calculations).
		g_Scratchpad = new Scratchpad();

		//	Init SceneNode (contains rewind buffer).
		tSceneNode = new SceneNode();

		//	Init resources types.
		//	TODO: implementation for Register class!
		/*
		Types::Resources::Texture::Init();
		Types::Resources::Font::Init();
		Types::Resources::Text::Init();
		Types::Resources::Model::Init();
		Types::Resources::Fragment::Init();
		Types::Resources::Movie::Init();
		Types::Resources::Cutscene::Init();
		Types::Resources::Sound::Init();
		Types::Resources::StreamedSoundInfo::Init();
		Types::Resources::Animation::Init();
		Types::Resources::MeshColor::Init();
		*/

		//	Init unknown matricies.
		CreateUnknownMatricies();

		//	Register script entities.
		//	TODO: implementation!
		InitEntitiesDatabase();

		//	Init unknown maps (lists), somehow related to renderer.
		//	TODO: implementation!
		(*(void(__cdecl*)())0x460AB0)();
		(*(void(__cdecl*)())0x4651B0)();

		//	Init rendered textures map.
		//	TODO: implementation!
		(*(void(__cdecl*)())0x464120)();

		//	Init renderer commands buffer.
		RenderBuffer::CreateRenderBuffer();

		//	Init lights.
		//	TODO: implementation!
		(*(void(__cdecl*)())0x881070)();

		if (m_pConfigurationVariables->IsVariableSet("ps2_max_texture_size"))
			Script::Ps2MaxTextureSize = m_pConfigurationVariables->GetParamValueInt("ps2_max_texture_size");

		if (m_pConfigurationVariables->IsVariableSet("overridelights"))
			Light::OverrideLights(m_pConfigurationVariables->GetParamValueBool("overridelights"));

		Script::Fullscreen = true;
		if (m_pConfigurationVariables->IsVariableSet("fullscreen"))
			Script::Fullscreen = m_pConfigurationVariables->GetParamValueBool("fullscreen");

		Script::Filesystem = String("blocks.naz, sounds.naz, videos00.naz, videos01.naz");
		if (m_pConfigurationVariables->IsVariableSet("filesystem"))
			Script::Filesystem = m_pConfigurationVariables->GetParamValueString("filesystem");

		//	TODO: what does this do?
		field_54 = 0;

		//	If log dump file is here - open it.
		if (m_pConfigurationVariables->IsVariableSet("logdumpfile")) {
			LogDump::OpenLogDump(m_pConfigurationVariables->GetParamValueString("logdumpfile").m_szString);
		}

		//	If we have 'profile.txt' available, check directory mappings and other stuff.
		//	TODO: implementation!
		if (pProfileVariables) {
			char szDirectorymapping[24];
			int index = 0;

			memset(&szDirectorymapping, 0, sizeof(szDirectorymapping));

			sprintf(szDirectorymapping, "directorymapping%d", index);

			while (pProfileVariables->IsVariableSet(szDirectorymapping)) {
				String dirmapping = pProfileVariables->GetParamValueString(szDirectorymapping);

				//AddDirectoryMappingsListEntry(dirmapping);
			}
		}

		//	Is this the testing build?
		String sTestingPath = String();
		_GetDeveloperPath(&sTestingPath);

		if (_stricmp(sTestingPath.m_szString, "testing")) {
			//	NOTE: append ' **' to this string. Why?
			sTestingPath.Append(" **");
		}

		sTestingPath.Append("(");

		//	Scripts search path if 'profile.txt' is available.
		Script::ScriptsPath = String("/data/scripts/stable/");

		if (pProfileVariables)
			if (pProfileVariables->IsVariableSet("script_searchpath"))
				Script::ScriptsPath = pProfileVariables->GetParamValueString("script_searchpath");

		//	NOTE: SetScriptsPath not necessary, since variable is already set above.
		sTestingPath.Append(" scripts: baked)");
		sTestingPath.Append(CONFIG_GAMENAME);

		if (m_pConfigurationVariables->IsVariableSet("relax_build_version_check"))
			Script::RelaxBuildVersionCheck = m_pConfigurationVariables->GetParamValueBool("relax_build_version_check");

		//	What icon should we use?
		if (_stricmp(sTestingPath.m_szString, "stable"))
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

		//	TODO: implementation for SetCountryCode!
		if (m_pConfigurationVariables->IsVariableSet("language_mode"))
			SetCountryCode(m_pConfigurationVariables->GetParamValueString("language_mode").m_szString);

		Script::LanguageMode = String(*Script::CountryCodes[Script::LanguageStringsOffset]);

		//	Create required devices - window, mouse, keyboard, gamepad.
		g_Window = new Window(m_sGameName.m_szString, 1, CONFIG_MENU_RESOURCEID, Script::Filesystem.m_szString, !Script::IconResourceId ? nIconResId : Script::IconResourceId);

		g_InputMouse = new Input::Mouse();
		g_InputKeyboard = new Input::Keyboard();
		g_InputGamepad[0] = new Input::Gamepad(0, 1);

		Input::Gamepad::GetGameControllerByIndex(0);

		//	Init saves directories information (ps2 emulation and pc).
		g_SavesDirsInfo[SAVE_SLOT_0] = new SavesDirectoriesInformation(SAVE_SLOT_0);
		g_SavesDirsInfo[SAVE_SLOT_1] = new SavesDirectoriesInformation(SAVE_SLOT_1);
		g_SavesDirsInfo[SAVE_SLOT_8] = new SavesDirectoriesInformation(SAVE_SLOT_8);

		Script::SavePlatformPS2 = true;

		//	Detect platform.
		if (m_pConfigurationVariables->IsVariableSet("save_platform")) {
			String platform = m_pConfigurationVariables->GetParamValueString("save_platform");

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
					g_SavesDirsInfo[memcardindex]->m_sSaveFolderPath.Set(szMemcard0[memcardindex]);

					if (!g_SavesDirsInfo[memcardindex]->IsFormatted())
						g_SavesDirsInfo[memcardindex]->FormatCard();

					memcardindex++;
				} while (memcardindex < 2);
			}

			const char szHarddisk[] = "/savegames/harddisk/";
			Utils::CreateDirectoriesRecursive(szHarddisk);
			g_SavesDirsInfo[SAVE_SLOT_8]->m_sSaveFolderPath.Set(szHarddisk);

			//	TODO: implementation for memorycards class methods!
			if (!g_SavesDirsInfo[SAVE_SLOT_8]->IsFormatted())
				g_SavesDirsInfo[SAVE_SLOT_8]->FormatCard();
		}else{
			//	For PC, just figure out system user data directory.
			Script::SavePlatformPS2 = false;

			String sUserDocDir;
			GetUserDocumentsDir(sUserDocDir);

			sUserDocDir.Append(CONFIG_SAVEDIR);

			Script::FileCheck = true;

			g_SavesDirsInfo[SAVE_SLOT_8]->m_sSaveFolderPath.Set(sUserDocDir.m_szString);
		}

		Script::CutsceneDisableAware = false;
		if (m_pConfigurationVariables->IsVariableSet("cutscene_disable_aware"))
			Script::CutsceneDisableAware = m_pConfigurationVariables->GetParamValueBool("cutscene_disable_aware");

		Script::CutsceneForceCompleteLodUpdates = false;
		if (m_pConfigurationVariables->IsVariableSet("cutscene_force_complete_lod_updates"))
			Script::CutsceneForceCompleteLodUpdates = m_pConfigurationVariables->GetParamValueBool("cutscene_force_complete_lod_updates");

		//	Default streamed sound files extension.
		if (m_pConfigurationVariables->IsVariableSet("streamed_sound_ext"))
			Script::StreamedSoundExt = m_pConfigurationVariables->GetParamValueString("streamed_sound_ext");
		else
			Script::StreamedSoundExt = String("ogg");

		//	Figure out sound renderer.
		if (m_pConfigurationVariables->IsVariableSet("soundrenderer")) {
			String soundrenderer = m_pConfigurationVariables->GetParamValueString("soundrenderer");
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
		//	TODO: implementation for StreamedSoundBuffers class!
		Audio::g_StreamedSoundBuffers = new Audio::StreamedSoundBuffers(1, 44100);

		if (m_pConfigurationVariables->IsVariableSet("sound_max_concurrent_sounds"))
			Audio::g_StreamedSoundBuffers->m_nMaxConcurrentSounds = m_pConfigurationVariables->GetParamValueInt("sound_max_concurrent_sounds");

		//	Override default volume values.
		if (m_pConfigurationVariables->IsVariableSet("change_sound_group_volume_scaling") && m_pConfigurationVariables->GetParamValueBool("change_sound_group_volume_scaling")) {
			if (m_pConfigurationVariables->IsVariableSet("default_fx_volume_var"))
				Audio::SetDefaultFxVolume(m_pConfigurationVariables->GetParamValueFloat("default_fx_volume_var"));

			if (m_pConfigurationVariables->IsVariableSet("default_ambience_volume_var"))
				Audio::SetDefaultAmbienceVolume(m_pConfigurationVariables->GetParamValueFloat("default_ambience_volume_var"));

			if (m_pConfigurationVariables->IsVariableSet("default_music_volume_var"))
				Audio::SetDefaultMusicVolume(m_pConfigurationVariables->GetParamValueFloat("default_music_volume_var"));

			if (m_pConfigurationVariables->IsVariableSet("default_speaks_volume_var"))
				Audio::SetDefaultSpeaksVolume(m_pConfigurationVariables->GetParamValueFloat("default_speaks_volume_var"));
		}

		//	Sound is enabled if not overridden.
		Audio::g_StreamedSoundBuffers->m_Sound = !m_pConfigurationVariables->IsVariableSet("sound") ||
													m_pConfigurationVariables->GetParamValueBool("sound");

		//	NOTE: what is this parameter?
		Vector3<float> vBackground = Vector3<float>();
		if (m_pConfigurationVariables->IsVariableSet("background"))
			m_pConfigurationVariables->GetParamValueVector3("background", vBackground, ',');

		m_vBackgroundSize.x = vBackground.x * (float)0.0039215689;
		m_vBackgroundSize.y = vBackground.y * (float)0.0039215689;
		m_vBackgroundSize.z = vBackground.z * (float)0.0039215689;

		//	Override screen size.
		Vector2<float> vScreensize = Vector2<float>();
		if (m_pConfigurationVariables->IsVariableSet("screensize"))
			m_pConfigurationVariables->GetParamValueVector2("screensize", vScreensize, ',');

		//	Widescreen emulation
		if (m_pConfigurationVariables->IsVariableSet("widescreen_emulation")) {
			String sWidescreenType = m_pConfigurationVariables->GetParamValueString("widescreen_emulation");

			if (sWidescreenType.Equal("pal")) {
				Renderer::WideScreen = true;
				vScreensize.x = 720;
				vScreensize.y = 576;

				debug("EMULATING Pal-Wide\n");
			}

			if (sWidescreenType.Equal("ntsc")) {
				Renderer::WideScreen = true;
				vScreensize.x = 720;
				vScreensize.y = 480;

				debug("EMULATING Ntsc-Wide\n");
			}

			if (sWidescreenType.Equal("hdtv")) {
				Renderer::WideScreen = true;
				vScreensize.x = 1280;
				vScreensize.y = 720;

				debug("EMULATING HDTV-Wide\n");
			}
		}

		if (m_pConfigurationVariables->IsVariableSet("FSAA"))
			Renderer::FSAA = m_pConfigurationVariables->GetParamValueBool("FSAA");

		if (m_pConfigurationVariables->IsVariableSet("ratioxy"))
			Renderer::RatioXY = m_pConfigurationVariables->GetParamValueFloat("ratioxy");

		//	NOTE: is this backbuffer size?
		Vector3<float> vBuff;
		//void* buff = (void*)malloc(31 * 4);
		//memset(buff, 0, 31 * 4);

		//	Create renderer, this also creates and initializes Direct3D device.
		//	TODO: implementation!
		if (!Allocators::Released)
			if (g_Renderer = new Renderer())
				if (Script::Fullscreen)
					g_Renderer->CreateRenderer(&vScreensize, 32, 16, (Renderer::FSAA != 0 ? 0x200 : 0) | 0x80, 31, 20, &vBuff);
				else
					g_Renderer->CreateRenderer(&vScreensize, 32, 16, 130, 31, 20, &vBuff);

		//	Set region.
		Script::Region = "europe";

		//	NOTE: this is set to return 0 (false) always, so maybe get rid of it?
		if (!(*(int(__cdecl*)())0x420160)())
			Script::Region.Set("usa");

		if (m_pConfigurationVariables->IsVariableSet("region"))
			Script::Region.Set(m_pConfigurationVariables->GetParamValueString("region").m_szString);

		g_Blocks->SetRegionId(GetRegionId(&Script::Region));
		debug("Using region: %s\n", Script::Region.m_szString);

		if (m_pConfigurationVariables->IsVariableSet("virtual_hud_screensize")) {
			Vector2<float> vHudSize;
			m_pConfigurationVariables->GetParamValueVector2("virtual_hud_screensize", vHudSize, ',');

			Renderer::g_ScreenProperties.SetHudScreenSize(vHudSize.x, vHudSize.y, 1.0, 1.0);
		}

		if (m_pConfigurationVariables->IsVariableSet("screen_safe_area"))
			Renderer::g_ScreenProperties.SetSafeArea(m_pConfigurationVariables->GetParamValueFloat("screen_safe_area"));

		//	NOTE: maybe this is 'SetBackBufferSize'?
		//	TODO: implementation!
		g_Renderer->_41FDF0(&m_vBackgroundSize, -1);

		//	TODO: what is this?
		Renderer::_A08704 = 0;
		Renderer::_A0870C = 0;
		Renderer::_A0872C = 0;

		if (m_pConfigurationVariables->IsVariableSet("version_name"))
			Script::VersionName = m_pConfigurationVariables->GetParamValueString("version_name");

		//	TODO: what is this?
		Script::_A1B98D = 0;

		//	Initialize baked font.
		//	TODO: implementation!
		Font::MakeCharactersMap();

		//	TODO: implementation!
		if (!Allocators::Released)
			if (g_Font = new Font())
				g_Font->Init();

		//	Load assets.
		//	TODO: implementation for OpenZip.
		ReadZipDirectories(Script::Filesystem.m_szString);

		//	Parse collmat file
		EnumMaterialsInCollmat();
		//	Parse facecolmat file
		EnumFaceColMaterials();

		//	Init random numbers generator.
		Random::Init((int)__rdtsc());

		//	Load screen and progress class.
		g_LoadScreenInfo = new LoadScreenInfo("");
		g_Progress = new Progress();

		if (m_pConfigurationVariables->IsVariableSet("show_hud") &&
			m_pConfigurationVariables->GetParamValueBool("show_hud"))
			Script::ShowHud = true;

		if (m_pConfigurationVariables->IsVariableSet("check_original_asset") &&
			!m_pConfigurationVariables->GetParamValueBool("check_original_asset"))
			Script::CheckOriginalAsset = false;

		if (m_pConfigurationVariables->IsVariableSet("warning_window"))
			Script::WarningShow = m_pConfigurationVariables->GetParamValueBool("warning_window");

		//	NOTE: Something related to scripts?
		(*(void(__cdecl*)(int, int))0x8C66E0)(8192, 512);

		//	NOTE: Init global entities? Really large function.
		(*(void(__cdecl*)())0x7A1F60)();

		//	Since scripts loaded and ready, calculate CRC and remember it.
		//	TODO: implementation!
		m_nCRCForScriptsListUnk = (*(int(__cdecl*)())0x873440)();
		m_nCRCForScriptsGlobalList = (*(int (__cdecl*)())0x871DD0)();
		m_nCRCForTypesList = (*(int(__cdecl*)())0x862CF0)();

		//	Instantiate scene.
		bool SceneSet = false;
		if (m_pConfigurationVariables->IsVariableSet("scenefile")) {
			const char* sceneFile = m_pConfigurationVariables->GetParamValueString("scenefile").m_szString;
			debug("Opening scene %s\n", sceneFile);

			SceneSet = OpenScene(sceneFile);
		}

		if (!SceneSet)
			if (!(SceneSet = OpenScene("/data/Overdose_THE_GAME/OverdoseIntro.scene")))
				tScene->GetScriptEntity()->CreateNode();

		//	TODO: implementation! Scene is not initialized here!
		if (m_pConfigurationVariables->IsVariableSet("fixedframerate"))
			tScene->SetFixedFramerate(m_pConfigurationVariables->GetParamValueFloat("fixedframerate"));

		//	NOTE: Adjust cameras matricies if not in full screen?
		//	Also, needs correct implementation.
		if (!Script::Fullscreen)
		{
			//	TODO: update cameras here.
		}

		if (m_pConfigurationVariables->IsVariableSet("frame_console_marker"))
			Script::FrameConsoleMarker = m_pConfigurationVariables->GetParamValueBool("frame_console_marker");

		Script::CheckDataSanity = false;
		Script::CheckDivisionByZero = false;

		if (pProfileVariables) {
			if (pProfileVariables->IsVariableSet("check_data_sanity"))
				Script::CheckDataSanity = pProfileVariables->GetParamValueBool("check_data_sanity");

			if (pProfileVariables->IsVariableSet("check_division_by_zero"))
				Script::CheckDivisionByZero = pProfileVariables->GetParamValueBool("check_division_by_zero");
		}

		//	Start editor or game.
		field_50 = 0;
		if (SceneSet)
			field_50 = 1;

		if (m_pConfigurationVariables->IsVariableSet("starteditor") &&
			m_pConfigurationVariables->GetParamValueBool("starteditor"))
			field_50 = 0;

		if (field_50)
			tScene->Start();

		if (m_pConfigurationVariables->IsVariableSet("ps2_play_ctrl") &&
			m_pConfigurationVariables->GetParamValueBool("ps2_play_ctrl"))
			Script::Ps2PlayCtrl = true;

		if (m_pConfigurationVariables->IsVariableSet("min_fade_dist"))
			Script::MinFadeDist = m_pConfigurationVariables->GetParamValueFloat("min_fade_dist");

		if (m_pConfigurationVariables->IsVariableSet("lod_and_fade"))
			Script::LodAndFade = m_pConfigurationVariables->GetParamValueBool("lod_and_fade");

		if (m_pConfigurationVariables->IsVariableSet("check_dangling_refs"))
			Script::CheckDanglingRefs = m_pConfigurationVariables->GetParamValueBool("check_dangling_refs");

		if (m_pConfigurationVariables->IsVariableSet("fix_dangling_refs"))
			Script::FixDanglingRefs = m_pConfigurationVariables->GetParamValueBool("fix_dangling_refs");

		//	NOTE: this is unused. Keep it here?
		String* _str = (String*)0xA0B4C4;
		Utils::SetWarningString(_str->m_szString);

		if (m_pConfigurationVariables->IsVariableSet("simulate_release_build")) {
			Script::SimulateReleaseBuild = m_pConfigurationVariables->GetParamValueBool("simulate_release_build");
			debug("Script::SimulateReleaseBuild == %i\n", Script::SimulateReleaseBuild);
		}

		g_Window->SetCursorReleased(false);

		field_0 = 1;

		if (pProfileVariables)
			delete pProfileVariables;

		//	Initialization completed. Game is running.
#ifdef INCLUDE_FIXES
		debug("Game init complete! Took %i ms\n", clock() - timeStart);
#endif
	}

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
		*/

		m_nGlobalPropertiesListCRC = (*(int (*)())0x873440)();
		m_nGlobalCommandsListCRC = (*(int (*)())0x871DD0)();
		m_nTypesListCRC = (*(int (*)())0x862CF0)();

		m_nTotalGlobalProperties = (*(int (*)())0x872FB0)();
		m_nTotalGlobalCommands = (*(int (*)())0x871A20)();
		m_nTotalTypes = (*(int (*)())0x862B30)();
	}

	//	TODO: implementation!
	void Config::UninitialiseGame()
	{
		//	g_Scene->_895E40();
		//	ScriptTypes::Release();
		//	g_Light->ClearList();
		g_Window->SetCursorReleased(true);

		if (m_pConfigurationVariables)
			//	m_pConfigurationVariables->Release();	//	@107B0
			delete m_pConfigurationVariables;

		if (g_Blocks)
			//	g_Blocks->Release();	//	@877250
			delete g_Blocks;

		if (g_InputMouse)
			delete g_InputMouse;

		if (g_InputKeyboard)
			//	g_InputKeyboard->Release();	//	@43AF20
			delete g_InputKeyboard;

		if (Input::Gamepad::GetGameControllerByIndex(0))
			//	g_InputGamepad->Release();	//	@439D60
			delete g_InputGamepad[0];

		if (g_Renderer)
			//	g_Renderer->Release();	//	@421470
			delete g_Renderer;

		if (g_Window) {
			g_Window->Release();	//	@43C230
			delete g_Window;
		}

		CoUninitialize();

		if (field_54)
			(*(void (__stdcall*)(int))field_54)(1);

		field_0 = 0;

		debug("Game uninitialized!\n");
	}

	//	TODO: implementation!
	bool Config::OpenScene(const char* scene)
	{
		return (*(bool(__thiscall*)(Config*, const char*))0x93CE00)(this, scene);
	}

	//	TODO: implementation!
	void Config::CreateUnknownMatricies()
	{
		
	}

	bool Session_Variables::IsVariableSet(const char* variableName)
	{
		bool(__thiscall * _IsVariableSet)(Session_Variables* _this, const char* _varname) = (bool (__thiscall*)(Session_Variables*, const char*))0x410080;
		
		return _IsVariableSet(this, variableName);
	}

	bool Session_Variables::GetParamValueBool(const char* variableName)
	{
		bool(__thiscall * _GetParamValueBool)(Session_Variables * _this, const char* _varname) = (bool(__thiscall*)(Session_Variables*, const char*))0x410900;
		
		return _GetParamValueBool(this, variableName);
	}

	int Session_Variables::GetParamValueInt(const char* variableName)
	{
		int(__thiscall * _GetParamInt)(Session_Variables * _this, const char* _varname) = (int(__thiscall*)(Session_Variables*, const char*))0x410A30;

		return _GetParamInt(this, variableName);
	}

	float Session_Variables::GetParamValueFloat(const char* variableName)
	{
		float(__thiscall * _GetParamFloat)(Session_Variables * _this, const char* _varname) = (float(__thiscall*)(Session_Variables*, const char*))0x410AC0;

		return _GetParamFloat(this, variableName);
	}

	const Vector2<int>& Session_Variables::GetParamValueVector2(const char* variableName, Vector2<int>& outVec, char delimiter)
	{
		const Vector2<int>& (__thiscall * _GetParamVec2i)(Session_Variables * _this, Vector2<int>* _outvec, const char* _varname, char _delim) = (const Vector2<int>&(__thiscall*)(Session_Variables*, Vector2<int>*, const char*, char))0x410B50;

		return _GetParamVec2i(this, &outVec, variableName, delimiter);
	}

	const Vector2<float>& Session_Variables::GetParamValueVector2(const char* variableName, Vector2<float>& outVec, char delimiter)
	{
		const Vector2<float>&(__thiscall * _GetParamVec2)(Session_Variables * _this, Vector2<float>*, const char* _varname, char _delim) = (const Vector2<float>&(__thiscall*)(Session_Variables*, Vector2<float>*, const char*, char))0x410BE0;

		return _GetParamVec2(this, &outVec, variableName, delimiter);
	}

	const Vector4f& Session_Variables::GetParamValueVector4(const char* variableName, Vector4f& outVec, char delimiter)
	{
		const Vector4f&(__thiscall * _GetParamVec4)(Session_Variables * _this, Vector4f* _outvec, const char* _varname, char _delim) = (const Vector4f&(__thiscall*)(Session_Variables*, Vector4f*, const char*, char))0x410C70;

		return _GetParamVec4(this, &outVec, variableName, delimiter);
	}

	const Vector3<float>& Session_Variables::GetParamValueVector3(const char* variableName, Vector3<float>& outVec, char delimiter)
	{
		const Vector3<float>& (__thiscall * _GetParamVec3)(Session_Variables * _this, const char* _varname, float* _outX, float* _outY, float* _outZ, char _delim) = (const Vector3<float> & (__thiscall*)(Session_Variables*, const char*, float*, float*, float*, char))0x410D90;

		return _GetParamVec3(this, variableName, &outVec.x, &outVec.y, &outVec.z, delimiter);
	}

	void Session_Variables::GetParamValueString(const char* variableName, String& outStr)
	{
		void (__thiscall * _GetParamString)(Session_Variables * _this, String* _outstr, const char* _varname) = (void (__thiscall*)(Session_Variables*, String*, const char*))0x410E30;

		_GetParamString(this, &outStr, variableName);
	}

	const String& Session_Variables::GetParamValueString(const char* variableName)
	{
		String* outStr = nullptr;
		const String& (__thiscall * _GetParamString)(Session_Variables * _this, String * _outstr, const char* _varname) = (const String& (__thiscall*)(Session_Variables*, String*, const char*))0x410E30;

		return _GetParamString(this, outStr, variableName);
	}

	void Session_Variables::SetParamValue(const char* variableName, char* value)
	{
		void(__thiscall * _SetParamValue)(Session_Variables * _this, const char* _varname, char* _val) = (void(__thiscall*)(Session_Variables*, const char*, char*))0x4114E0;

		_SetParamValue(this, variableName, value);
	}

	void Session_Variables::SetParamValueBool(const char* variableName, int value)
	{
		void(__thiscall * _SetParamValueBool)(Session_Variables * _this, const char* _varname, int _val) = (void(__thiscall*)(Session_Variables*, const char*, int))0x4116D0;

		_SetParamValueBool(this, variableName, value);
	}

	void ReadZipDirectories(const char* szFileSystem)
	{
		if (szFileSystem && *szFileSystem) {
			int nCurrentArchiveIndex = 0;
			char szZipName[255];

			memset(&szZipName, 0, sizeof(szZipName));

			while (true) {
				int nCharIndex = 0;
				do {
					char cCurrentChar = szFileSystem[nCurrentArchiveIndex];

					if (!cCurrentChar)
						break;
					if (cCurrentChar == ' ' || cCurrentChar == ',')
						break;

					++nCurrentArchiveIndex;
					szZipName[nCharIndex++] = cCurrentChar;
				} while (nCharIndex < 255);

				szZipName[nCharIndex] = 0;

				while (true) {
					char cCurrentChar = szFileSystem[nCurrentArchiveIndex];

					if (cCurrentChar != ' ' && cCurrentChar != ',')
						break;

					++nCurrentArchiveIndex;
				}

				OpenZip(szZipName);

				String sZipNameLocalised;

				sZipNameLocalised.Set(Script::LanguageMode.m_szString);
				sZipNameLocalised.Append("_");
				sZipNameLocalised.Append(szZipName);

				OpenZip(sZipNameLocalised.m_szString);

				if (!szFileSystem[nCurrentArchiveIndex])
					break;

				memset(&szZipName, 0, sizeof(szZipName));
			}
		}
	}

	void OpenZip(const char* szZipPath)
	{
		//	TODO: implementation!
		(*(void (*)(const char*))0x419100)(szZipPath);
	}

	void AddDirectoryMappingsListEntry(const char* szDir, const char* szDirTo)
	{
		void(__cdecl * _AddDirMappingList)(const char* _dir, const char* _dirto) = (void(__cdecl*)(const char*, const char*))0x418F90;

		_AddDirMappingList(szDir, szDirTo);
	}

	void SetCountryCode(const char* szCode)
	{
		unsigned int languageIndex = 0;

		while (languageIndex++ < CONFIG_LANGUAGES) {
			if (Script::CountryCodes[languageIndex] && strncmp(szCode, *Script::CountryCodes[languageIndex], 2) == 0)
				break;
		}

		Script::LanguageStringsOffset = languageIndex;
	}

	signed int GetRegionId(String* regionStr)
	{
		/*
		 *	Europe - 0
		 *	USA - 1
		 *	Asia - 2
		*/

		if (regionStr->Equal("europe"))
			return 0;

		if (regionStr->Equal("usa"))
			return 1;

		if (regionStr->Equal("asia"))
			return 2;

		return -1;
	}

	void EnumMaterialsInCollmat()
	{
		(*(void(*)())0x87D330)();
	}

	void EnumFaceColMaterials()
	{
		if (FaceColList.m_nCurrIndex > 0)
			//	Already loaded.
			return;

		if (!Utils::FindFileEverywhere("/FaceColl.mat"))
			return;

		FileInternal faceColFile("/FaceColl.mat", 1, true);

		if (!faceColFile.IsFileOpen())
			return;

		String buffer;

		while (faceColFile.ReadString(&buffer)) {
			buffer.ToLowerCase();
			FaceColList.Add(&buffer);
		}
	}

	//	NOTE:	what does this do?
	void _GetDeveloperPath(String* outStr)
	{
		String sDevPath = "E:\\Develop\\KapowSystems\\TOD1\\Libs\\Toolbox\\Functions.cpp";

		sDevPath.ConvertBackslashes();
		sDevPath.ToLowerCase();

		char* szKapSysStr = strstr(sDevPath.m_szString, "KapowSystems");

		if (szKapSysStr) {
			//	TODO: this is incomplete. Fix if necessary.
			String tempstr = String();
			sDevPath.Substring(&tempstr, szKapSysStr - sDevPath.m_szString + 13, 0x7FFFFFFE);
			sDevPath.m_nLength = tempstr.m_nLength;
			
			outStr->Append(sDevPath.m_szString);
		}else{
			outStr->m_nLength = 0;
			outStr->m_szString = &outStr->m_pEmpty;
			outStr->m_nBitMask |= 0x80000000;
		}
	}
}