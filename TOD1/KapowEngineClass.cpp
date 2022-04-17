#include "KapowEngineClass.h"

#include "AssetManager.h"
#include "DataType.h"
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
#include "MemoryCard.h"
#include "LogDump.h"
#include "EditorCamera.h"
#include "ZipArch.h"
#include "ScriptDatabase.h"
#include "NodeSpatial.h"
#include "NodeLogical.h"
#include "Model.h"
#include "CollisionProbe.h"
#include "VirtualHud.h"
#include "LoadScreen.h"
#include "SkyBox.h"
#include "FontAsset.h"
#include "TextAsset.h"
#include "ModelAsset.h"
#include "Group.h"
#include "Control.h"
#include "Track.h"
#include "ControlSetup.h"
#include "Fog.h"
#include "Wind.h"
#include "TextSlot.h"
#include "GEKeyFrame.h"
#include "CollisionBox.h"
#include "CollisionSphere.h"
#include "GuideBox.h"
#include "BuiltinType.h"
#include "MockupBox.h"
#include "Character.h"
#include "Bone.h"
#include "AnimSlot.h"
#include "MotionAnimSlot.h"
#include "MotionLayer.h"
#include "CutscenePlayer.h"
#include "WayPoint.h"
#include "TrackPoint.h"
#include "Navigator.h"
#include "SignPost.h"
#include "RigidBody.h"
#include "OverdoseVehicle.h"
#include "StretchModel.h"
#include "Cloth.h"
#include "GFXEffect.h"
#include "Shadow.h"
#include "LensFlare.h"
#include "LensFlareManager.h"
#include "DecalManager.h"
#include "SurroundGeometry.h"
#include "DynamicSurroundGeometry.h"
#include "TextBox.h"
#include "Bullet.h"
#include "SoundSlot.h"
#include "SoundEmitter.h"
#include "StaticLighting.h"
#include "MoviePlayer.h"
#include "ParticleSystem.h"
#include "GeometryEffect.h"
#include "ProfilerInput.h"
#include "RealtimeFolder.h"
#include "BuiltinType.h"
#include "PlaceHolder.h"
#include "CharacterPlaceHolder.h"
#include "HavocPlaceHolder.h"
#include "GoodiePlaceHolder.h"
#include "WeaponPlaceHolder.h"
#include "MemoryCards.h"

namespace Script
{
    String Filesystem;
    String ControlType;
    String Region;
    String VersionName;
    String StreamedSoundExt;
    String LanguageMode;
    String ScriptsPath;
    std::vector<StringTuple> DirectoryMappings;
}

KapowEngineClass* g_KapowEngine = nullptr;
String KapowEngineClass::_A1B9F8 = {};

std::list<String> FaceCollList;
File* CollMatFile;
std::map<String, unsigned int> CollMatProperties;
std::map<String, FaceColl> CollMatMaterialsTypes;

KapowEngineClass::KapowEngineClass()
{
    MESSAGE_CLASS_CREATED(KapowEngineClass);

    m_Initialized = false;
    m_GameName;
    m_ConfigFilePath;
    m_String_1;
    m_String_2;
    m_SceneName;
    m_ConfigurationVariables = nullptr;
    m_SessionVariables = nullptr;
    field_4C = 0;
    m_ShouldStartGame = false;
    m_Background;
    m_PropertiesBuiltinChecksum = 0;
    m_CommandsBuiltinChecksum = 0;
    m_TypesBuiltinChecksum = 0;
    m_PropertiesTotal = 0;
    m_CommandsTotal = 0;
    m_TypesTotal = 0;
    m_PropertiesLoadedChecksum = 0;
    m_CommandsLoadedChecksum = 0;
    m_TypesLoadedChecksum = 0;
    m_UninitialiseCallback = nullptr;
}

KapowEngineClass::~KapowEngineClass()
{
    MESSAGE_CLASS_DESTROYED(KapowEngineClass);

    if (m_Initialized)
        UninitialiseGame();
}

#pragma message(TODO_IMPLEMENTATION)
void KapowEngineClass::Init(LPSTR, int, const char* configFileName, signed int iconResId)
{
#ifdef INCLUDE_FIXES
    clock_t timeStart = clock();
#endif

    CoInitialize(0);

    if (configFileName && *configFileName)
        m_ConfigFilePath = configFileName;
    else
        m_ConfigFilePath = CONFIG_CONFIGFILE;

    m_GameName = KAPOW_GAMENAME;

    if (File::FindFileEverywhere(m_ConfigFilePath.m_Str))
    {
        LogDump::LogA("Initialising engine with '%s'\n", m_ConfigFilePath.m_Str);

        m_ConfigurationVariables = new ConfigVariables(m_ConfigFilePath.m_Str, 1);
    }
    else
        m_ConfigurationVariables = new ConfigVariables(0);

    ConfigVariables* profileVariables = nullptr;
    if (File::FindFileEverywhere(CONFIG_PROFILEFILE))
        profileVariables = new ConfigVariables(CONFIG_PROFILEFILE, 0);

    if (m_ConfigurationVariables->IsVariableSet("filecheck"))
        Script::FileCheck = m_ConfigurationVariables->GetParamValueBool("filecheck") == 0; // NOTE: wtf?

    if (m_ConfigurationVariables->IsVariableSet("control_type"))
        m_ConfigurationVariables->GetParamValueString(Script::ControlType, "control_type");

    if (m_ConfigurationVariables->IsVariableSet("forcefeedback"))
        Script::ForceFeedback = m_ConfigurationVariables->GetParamValueBool("forcefeedback");

    Script::LoadBlocks = true;
    if (m_ConfigurationVariables->IsVariableSet("loadblocks"))
        Script::LoadBlocks = m_ConfigurationVariables->GetParamValueBool("loadblocks");

    g_AssetManager = new AssetManager(Script::LoadBlocks);

    DataType::InitScriptTypes();

    g_Scratchpad = new Scratchpad();

    g_SceneSaveLoad = new SceneSaveLoad();

    TextureAsset::CreateInstance();
    FontAsset::CreateInstance();
    TextAsset::CreateInstance();
    ModelAsset::CreateInstance();
    FragmentAsset::CreateInstance();
    /*
    MovieAsset::CreateInstance();
    CutsceneAsset::CreateInstance();
    SoundAsset::CreateInstance();
    StreamedSoundInfoAsset::CreateInstance();
    AnimationAsset::CreateInstance();
    MeshColorAsset::CreateInstance();
    */

    CreateUnknownMatricies();

    InitEntitiesDatabase();

    IndexBuffer::CreateIndexBufferMap();
    VertexBuffer::CreateVerticesMap();
    Texture::InitTexturesMap();

    RenderList::CreateRenderBuffer();

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
        m_ConfigurationVariables->GetParamValueString(Script::Filesystem, "filesystem");

#ifdef INCLUDE_FIXES
    m_UninitialiseCallback = new ConfigCallback();
#else
    m_UninitialiseCallback = nullptr;
#endif

    if (m_ConfigurationVariables->IsVariableSet("logdumpfile"))
    {
        String logdumpfilestr;
        m_ConfigurationVariables->GetParamValueString(logdumpfilestr, "logdumpfile");
        LogDump::OpenLogDump(logdumpfilestr.m_Str);

    }

    if (profileVariables)
    {
        char DirectorymappingStr[22] = {};
        unsigned int index = 0;

        sprintf(DirectorymappingStr, "directorymapping%d", index++);

        while (profileVariables->IsVariableSet(DirectorymappingStr))
        {
            String dirmapping;
            profileVariables->GetParamValueString(dirmapping, DirectorymappingStr);

            if (strchr(dirmapping.m_Str, '>') != nullptr)
            {
                // NOTE: potential place for buffer overflow right here, original code uses heap allocation, but this is redundant here really, adjust buffer size if any errors happen.
                char direntry[128] = {};
                strncpy_s(direntry, sizeof(direntry), dirmapping.m_Str, strchr(dirmapping.m_Str, '>') - dirmapping.m_Str);

                File::AddDirectoryMappingsListEntry(direntry, strchr(dirmapping.m_Str, '>') + 1);
            }

            sprintf(DirectorymappingStr, "directorymapping%d", index++);
        }
    }

    String TestingPath;
    GetInternalGameName(TestingPath);

    if (_stricmp(TestingPath.m_Str, "testing"))
    {
        m_GameName.Append(" *");
        m_GameName.Append(TestingPath.m_Str);
        m_GameName.Append("* ");
    }

    m_GameName.Append(" (");

    Script::ScriptsPath = CONFIG_SCRIPTS_PATH_STABLE;
    if (profileVariables && profileVariables->IsVariableSet("script_searchpath"))
        profileVariables->GetParamValueString(Script::ScriptsPath, "script_searchpath");

#ifdef INCLUDE_FIXES
    m_GameName.Append(" scripts: ");
    m_GameName.Append(Script::ScriptsPath.m_Str);
    m_GameName.Append(")");
#else
    m_GameName.Append(" scripts: baked)");
    m_GameName = KAPOW_GAMENAME;
#endif

    if (m_ConfigurationVariables->IsVariableSet("relax_build_version_check"))
        Script::RelaxBuildVersionCheck = m_ConfigurationVariables->GetParamValueBool("relax_build_version_check");

    if (_stricmp(TestingPath.m_Str, "stable"))
        Script::IconResourceId = 103;
    else
        Script::IconResourceId = 101;

    Script::LanguageStringsOffset = 0;

    if (File::IsFileValid("/es_sounds.zip") || File::IsFileValid("/es_sounds.naz"))
        Script::LanguageStringsOffset = 4;
    if (File::IsFileValid("/fr_sounds.zip") || File::IsFileValid("/fr_sounds.naz"))
        Script::LanguageStringsOffset = 1;
    if (File::IsFileValid("/de_sounds.zip") || File::IsFileValid("/de_sounds.naz"))
        Script::LanguageStringsOffset = 3;
    if (File::IsFileValid("/it_sounds.zip") || File::IsFileValid("/it_sounds.naz"))
        Script::LanguageStringsOffset = 2;
    if (File::IsFileValid("/uk_sounds.zip") || File::IsFileValid("/uk_sounds.naz"))
        Script::LanguageStringsOffset = 0;

    if (m_ConfigurationVariables->IsVariableSet("language_mode"))
    {
        String countrycodestr;
        m_ConfigurationVariables->GetParamValueString(countrycodestr, "language_mode");
        Script::SetCountryCode(countrycodestr.m_Str);
    }

    Script::LanguageMode = Script::CountryCodes[Script::LanguageStringsOffset];

    g_Window = new Window(m_GameName.m_Str, 1, CONFIG_MENU_RESOURCEID, Script::Filesystem.m_Str, Script::IconResourceId ? Script::IconResourceId : iconResId);

    g_InputMouse = new Input::Mouse();
    g_InputKeyboard = new Input::Keyboard();
    g_InputGamepad[0] = new Input::Gamepad(0, 1);

    Input::Gamepad::GetGameControllerByIndex(0);

    // TODO: we probably want to control how many of save slots are available.
    MemoryCardInfo[SAVE_SLOT_0] = new MemoryCard(SAVE_SLOT_0);
    MemoryCardInfo[SAVE_SLOT_1] = new MemoryCard(SAVE_SLOT_1);
    MemoryCardInfo[SAVE_SLOT_8] = new MemoryCard(SAVE_SLOT_8);

    Script::SavePlatformPS2 = true;

    if (m_ConfigurationVariables->IsVariableSet("save_platform"))
    {
        String platform;
        m_ConfigurationVariables->GetParamValueString(platform, "save_platform");

        if (platform.Equal("PS2"))
        {
            char* memcardFolders[] =
            {
                "/savegames/memorycard0/",
                "/savegames/memorycard1/"
            };

            for (unsigned int i = 0; i < (sizeof(memcardFolders) / sizeof(char*)); i++)
            {
                MemoryCardInfo[i]->m_SaveFolderPath = memcardFolders[i];
                CreateDirectoriesRecursive(memcardFolders[i]);

                if (!MemoryCardInfo[i]->IsFormatted())
                    MemoryCardInfo[i]->FormatCard();
            }
        }

        MemoryCardInfo[SAVE_SLOT_8]->m_SaveFolderPath = "/savegames/harddisk/";
        CreateDirectoriesRecursive(MemoryCardInfo[SAVE_SLOT_8]->m_SaveFolderPath.m_Str);

        if (!MemoryCardInfo[SAVE_SLOT_8]->IsFormatted())
            MemoryCardInfo[SAVE_SLOT_8]->FormatCard();
    }
    else
    {
        Script::SavePlatformPS2 = false;
        Script::FileCheck = true;

        GetUserDocumentsDir(MemoryCardInfo[SAVE_SLOT_8]->m_SaveFolderPath);
        MemoryCardInfo[SAVE_SLOT_8]->m_SaveFolderPath.Append(CONFIG_SAVEDIR);
    }

    Script::CutsceneDisableAware = false;
    if (m_ConfigurationVariables->IsVariableSet("cutscene_disable_aware"))
        Script::CutsceneDisableAware = m_ConfigurationVariables->GetParamValueBool("cutscene_disable_aware");

    Script::CutsceneForceCompleteLodUpdates = false;
    if (m_ConfigurationVariables->IsVariableSet("cutscene_force_complete_lod_updates"))
        Script::CutsceneForceCompleteLodUpdates = m_ConfigurationVariables->GetParamValueBool("cutscene_force_complete_lod_updates");

    if (m_ConfigurationVariables->IsVariableSet("streamed_sound_ext"))
        m_ConfigurationVariables->GetParamValueString(Script::StreamedSoundExt, "streamed_sound_ext");
    else
        Script::StreamedSoundExt = "ogg";

    if (m_ConfigurationVariables->IsVariableSet("soundrenderer"))
    {
        String soundrenderer;
        m_ConfigurationVariables->GetParamValueString(soundrenderer, "soundrenderer");
        SoundSystemType rendererid = SOUND_SYSTEM_UNDEFINED;

        soundrenderer.ToLowerCase();

        if (soundrenderer.Equal("directsound"))
            rendererid = SOUND_SYSTEM_DSOUND;
        if (soundrenderer.Equal("dieselpower"))
            rendererid = SOUND_SYSTEM_DIESELPOWER;
        if (soundrenderer.Equal("auto"))
            rendererid = SOUND_SYSTEM_AUTOSELECT;

        if (rendererid != 0)
            StreamedSoundBuffers::RememberSoundRenderer(rendererid);
    }

    g_StreamedSoundBuffers = new StreamedSoundBuffers(1, 44100);

    if (m_ConfigurationVariables->IsVariableSet("sound_max_concurrent_sounds"))
        g_StreamedSoundBuffers->m_MaxConcurrentSounds = m_ConfigurationVariables->GetParamValueInt("sound_max_concurrent_sounds");

    if (m_ConfigurationVariables->IsVariableSet("change_sound_group_volume_scaling") && m_ConfigurationVariables->GetParamValueBool("change_sound_group_volume_scaling"))
    {
        if (m_ConfigurationVariables->IsVariableSet("default_fx_volume_var"))
            StreamedSoundBuffers::SetDefaultFxVolume(m_ConfigurationVariables->GetParamValueFloat("default_fx_volume_var"));

        if (m_ConfigurationVariables->IsVariableSet("default_ambience_volume_var"))
            StreamedSoundBuffers::SetDefaultAmbienceVolume(m_ConfigurationVariables->GetParamValueFloat("default_ambience_volume_var"));

        if (m_ConfigurationVariables->IsVariableSet("default_music_volume_var"))
            StreamedSoundBuffers::SetDefaultMusicVolume(m_ConfigurationVariables->GetParamValueFloat("default_music_volume_var"));

        if (m_ConfigurationVariables->IsVariableSet("default_speaks_volume_var"))
            StreamedSoundBuffers::SetDefaultSpeaksVolume(m_ConfigurationVariables->GetParamValueFloat("default_speaks_volume_var"));
    }

    g_StreamedSoundBuffers->m_Sound = !m_ConfigurationVariables->IsVariableSet("sound") ||
        m_ConfigurationVariables->GetParamValueBool("sound");

    Vector3<float> BackgroundSizeVec;
    if (m_ConfigurationVariables->IsVariableSet("background"))
        m_ConfigurationVariables->GetParamValueVector3(BackgroundSizeVec, "background", ',');

    m_Background.x = BackgroundSizeVec.x * (float)(1 / 255);
    m_Background.y = BackgroundSizeVec.y * (float)(1 / 255);
    m_Background.z = BackgroundSizeVec.z * (float)(1 / 255);
    m_Background.a = 1.f;

    ScreenResolution ScreenSize;
    if (m_ConfigurationVariables->IsVariableSet("screensize"))
        m_ConfigurationVariables->GetParamValueVector2i(ScreenSize, "screensize", ',');

    if (m_ConfigurationVariables->IsVariableSet("widescreen_emulation"))
    {
        String WideScreenType;
        m_ConfigurationVariables->GetParamValueString(WideScreenType, "widescreen_emulation");

        if (WideScreenType.Equal("pal"))
        {
            GfxInternal::WideScreen = true;
            ScreenSize.x = 720;
            ScreenSize.y = 576;

            LogDump::LogA("EMULATING Pal-Wide\n");
        }

        if (WideScreenType.Equal("ntsc"))
        {
            GfxInternal::WideScreen = true;
            ScreenSize.x = 720;
            ScreenSize.y = 480;

            LogDump::LogA("EMULATING Ntsc-Wide\n");
        }

        if (WideScreenType.Equal("hdtv"))
        {
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
    ScreenBuffers[11].x = ScreenBuffers[12].x = 2.f;

    if (Script::Fullscreen)
        g_GfxInternal = new GfxInternal(ScreenSize, 32, 16, 130, 31, 20, ScreenBuffers);
    else
        g_GfxInternal = new GfxInternal(ScreenSize, 32, 16, (GfxInternal::FSAA ? 512 : 0) | 128, 31, 20, ScreenBuffers);

    Script::Region = GfxInternal::GetRegion() ? "europe" : "usa";

    if (m_ConfigurationVariables->IsVariableSet("region"))
        m_ConfigurationVariables->GetParamValueString(Script::Region, "region");

    g_AssetManager->SetRegion(AssetManager::RegionIdByName(Script::Region));
    LogDump::LogA("Using region: %s\n", Script::Region.m_Str);

    if (m_ConfigurationVariables->IsVariableSet("virtual_hud_screensize"))
    {
        Vector2<float> VirtualHudSize;
        m_ConfigurationVariables->GetParamValueVector2f(VirtualHudSize, "virtual_hud_screensize", ',');
        VirtualHud::VirtualHudInstance.SetVirtualHudScreenSize(VirtualHudSize.x, VirtualHudSize.y, 1.0, 1.0);
    }

    if (m_ConfigurationVariables->IsVariableSet("screen_safe_area"))
        VirtualHud::VirtualHudInstance.SetSafeArea(m_ConfigurationVariables->GetParamValueFloat("screen_safe_area"));

    g_GfxInternal->SetClearColorForBufferIndex(*((ColorRGB*)&m_Background), -1);

    // TODO: what is this?
    FrameBuffer::_A08704[0] = 0;
    FrameBuffer::_A08704[1] = 0;
    FrameBuffer::_A08704[5] = 0;

    if (m_ConfigurationVariables->IsVariableSet("version_name"))
        m_ConfigurationVariables->GetParamValueString(Script::VersionName, "version_name");

    // NOTE: this is unused.
    Script::_A1B98D = 0;

    Font::MakeCharactersMap(Font::GlyphsInfo);
    new Font(Font::GlyphsInfo);

    // Load NAZ archives into memory.
    File::ReadZipDirectories(Script::Filesystem.m_Str);

    EnumMaterialsInCollmat();
    // Parse facecolmat file
    EnumFaceColMaterials();

    // Init random numbers generator.
    Random::Init((int)__rdtsc());

    // Load screen and progress class.
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

    // NOTE: this adjusts size for global entities lists.
    //(*(void(__cdecl*)(int, int))0x8C66E0)(8192, 512);

    LoadScripts();

    m_PropertiesLoadedChecksum = GetGlobalPropertyListChecksum();
    m_CommandsLoadedChecksum = GetGlobalCommandListChecksum();
    m_TypesLoadedChecksum = DataType::GetTypesListChecksum();

    // Instantiate scene.
    bool SceneSet = false;
    if (m_ConfigurationVariables->IsVariableSet("scenefile"))
    {
        String scenefile;
        m_ConfigurationVariables->GetParamValueString(scenefile, "scenefile");
        LogDump::LogA("Opening scene %s\n", scenefile.m_Str);

        SceneSet = OpenScene(scenefile.m_Str);
    }

    if (!SceneSet)
        if (!(SceneSet = OpenScene("/data/Overdose_THE_GAME/OverdoseIntro.scene")))
            tScene->CreateNode();

    if (m_ConfigurationVariables->IsVariableSet("fixedframerate"))
    {
        Scene::SceneInstance->m_FixedFramerate = true;
        Scene::SceneInstance->m_FixedFramerateVal = 1.0f / m_ConfigurationVariables->GetParamValueFloat("fixedframerate");
    }

    if (!Script::Fullscreen)
    {
        Node* scenechild = Scene::SceneInstance->m_FirstChild;
        if (scenechild)
        {
            do
            {
                EntityType* editorCamera = scenechild->m_ScriptEntity;
                if (editorCamera)
                {
                    bool noparent = false;
                    while (tEditorCamera != editorCamera)
                    {
                        editorCamera = editorCamera->m_Parent;
                        if (!editorCamera)
                        {
                            noparent = true;
                            break;
                        }
                    }

                    if (noparent)
                        continue;

                    Scene::SceneInstance->m_GameCamera = (Camera*)scenechild;
                    Scene::SceneInstance->StoreGameCamera();
                }
                scenechild = scenechild->m_NextSibling;
            } while (scenechild);
        }
    }

    if (m_ConfigurationVariables->IsVariableSet("frame_console_marker"))
        Script::FrameConsoleMarker = m_ConfigurationVariables->GetParamValueBool("frame_console_marker");

    Script::CheckDataSanity = false;
    Script::CheckDivisionByZero = false;

    if (profileVariables)
    {
        if (profileVariables->IsVariableSet("check_data_sanity"))
            Script::CheckDataSanity = profileVariables->GetParamValueBool("check_data_sanity");

        if (profileVariables->IsVariableSet("check_division_by_zero"))
            Script::CheckDivisionByZero = profileVariables->GetParamValueBool("check_division_by_zero");
    }

    // Start editor or game.
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

    if (m_ConfigurationVariables->IsVariableSet("simulate_release_build"))
    {
        Script::SimulateReleaseBuild = m_ConfigurationVariables->GetParamValueBool("simulate_release_build");
        LogDump::LogA("Script::SimulateReleaseBuild == %i\n", Script::SimulateReleaseBuild);
    }

#ifdef INCLUDE_FIXES
    g_Window->SetCursorReleased(true);
#else
    g_Window->SetCursorReleased(false);
#endif

    if (profileVariables)
        delete profileVariables;

#ifdef INCLUDE_FIXES
    debug("Game init complete! Took %i ms\n", clock() - timeStart);
#endif
}

void KapowEngineClass::InitEntitiesDatabase()
{
    Entity::Register();
    RegisterGlobalProperty("block_id:integer", true);
    Node::Register();
    NodeSpatial::Register();
    NodeLogical::Register();
    Camera::Register();
    Model::Register();
    Folder_::Register();
    Scene::Register();
    MockupBox::Register();
    EditorCamera::Register();
    Character::Register();
    Bone::Register();
    AnimSlot::Register();
    MotionAnimSlot::Register(); //  TODO: implement!
    AnimLayer::Register();  //  TODO: implement!
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
    CollisionProbe::Register(); //  TODO: implement!
    RigidBody::Register();
    OverdoseVehicle::Register();
    StretchModel::Register();   //  TODO: implement!
    SkyBox::Register();
    Fog::Register();
    Wind::Register();
    Cloth::Register();
    GFXEffect::Register();  //  TODO: implement!
    Shadow::Register();
    LensFlare::Register();  //  TODO: implement!
    LensFlareManager::Register();   //  TODO: implement!
    DecalManager::Register();
    SurroundGeometry::Register();   //  TODO: implement!
    DynamicSurroundGeometry::Register();    //  TODO: implement!
    Light::Register();  //  TODO: implement!
    Sprite::Register(); //  TODO: implement!
    TextBox::Register();    //  TODO: implement!
    TextSlot::Register();
    GuideBox::Register();
    Bullet::Register();
    SoundSlot::Register();  //  TODO: implement!
    SoundEmitter::Register();   //  TODO: implement!
    StaticLighting::Register();
    MoviePlayer::Register();
    ParticleSystem::Register(); //  TODO: implement!
    GEKeyFrame::Register();
    GeometryEffect::Register();
    ProfilerInput::Register();
    RealtimeFolder::Register();
    BuiltinType::Register();
    PlaceHolder::Register();
    CharacterPlaceHolder::Register();
    HavocPlaceHolder::Register();
    GoodiePlaceHolder::Register();
    WeaponPlaceHolder::Register();
    MemoryCards::Register();    //  TODO: implement!
    LoadScreenNode::Register();

    m_PropertiesBuiltinChecksum = GetGlobalPropertyListChecksum();
    m_CommandsBuiltinChecksum = GetGlobalCommandListChecksum();
    m_TypesBuiltinChecksum = DataType::GetTypesListChecksum();

    m_PropertiesTotal = GetGlobalPropertyListSize();
    m_CommandsTotal = GetGlobalCommandListSize();
    m_TypesTotal = DataType::GetTypesListSize();
}

void KapowEngineClass::UninitialiseGame()
{
    Scene::SceneInstance->Destroy();
    DataType::ClearScriptLists();
    Light::ClearLightsList();

    g_Window->SetCursorReleased(true);

    delete m_ConfigurationVariables;
    delete g_AssetManager;
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

bool KapowEngineClass::OpenScene(const char* scene)
{
    tScene->CreateNode();
    g_AssetManager->SetSceneName(scene);
    Scene::SceneInstance->Load(scene);
    Scene::SceneInstance->UpdateLoadedBlocks_Impl(0, nullptr);
    Scene::SceneInstance->m_StartTimeMs = Timer::GetMilliseconds();
    Scene::SceneInstance->InstantiateAllChildren();
    Scene::SceneInstance->FinishCreation("Scene instantiate all completed.");

    return true;
}

#pragma message(TODO_IMPLEMENTATION)
void KapowEngineClass::CreateUnknownMatricies()
{
}

bool KapowEngineClass::CheckAssetChecksum(File& file, const unsigned int propertyChecksum, const unsigned int commandChecksum)
{
    unsigned int chksum;
    if (file.Read(&chksum, sizeof(chksum)) != sizeof(chksum) || chksum != propertyChecksum)
        return false;

    if (file.Read(&chksum, sizeof(chksum)) != sizeof(chksum) || chksum != commandChecksum)
        return false;

    file.Read(&chksum, sizeof(chksum));
    return true;
}

bool KapowEngineClass::UpdateGame()
{
    if (!Scene::SceneInstance)
        return true;

    Scene::SceneInstance->m_FrameBuffer_1->Reset();
    Scene::SceneInstance->m_FrameBuffer_2->Reset();

    ((Defragmentator*)MemoryManager::AllocatorsList[DEFRAGMENTING]->m_Defragmentator)->DefragmentIfNecessary();

    DWORD64 timeBeforeRender = __rdtsc();
    Scene::SceneInstance->Update();
    g_GfxInternal->SetClearFlagsForBufferIndex(3, 0);

    bool notLoadingNow = false;
    if (!g_LoadScreenInfo->m_Enabled)
    {
        Scene::SceneInstance->Render();
        notLoadingNow = true;
    }

    Scene::TotalFrames++;
    Scene::SceneInstance->m_RenderTimeMs = (unsigned int)(__rdtsc() - timeBeforeRender);
#if defined VERBOSE_LOG && defined INCLUDE_FIXES
    LogDump::LogA("Scene render took %d ms\n", Scene::SceneInstance->m_RenderTimeMs);
#endif

    const Camera* sceneCamera = Scene::SceneInstance->m_ActiveCamera;
    if (g_StreamedSoundBuffers && sceneCamera)
    {
        Vector4f cameraPos;
        DirectX::XMMATRIX cameraMat;
        DirectX::XMVECTOR cameraRotVec;
        Orientation cameraOrient;

        sceneCamera->GetWorldPos(cameraPos);
        g_StreamedSoundBuffers->SetListener3DPos(cameraPos);
        sceneCamera->GetWorldMatrix(cameraMat);
        cameraRotVec = DirectX::XMQuaternionRotationMatrix(cameraMat);
        //  NOTE: I'm not sure if compiler will make good conversion out of 128 bit into 3 32 bit floating points, so here goes.
        cameraOrient.w = *(float*)&cameraMat;
        cameraOrient.x = *(float*)(&cameraMat + (int)1);
        cameraOrient.y = *(float*)(&cameraMat + (int)2);
        cameraOrient.z = 0;

        g_StreamedSoundBuffers->SetListener3DOrientation(cameraOrient);
        SoundSlot::UpdateSoundGroups(Scene::RealTimePassed);
        g_StreamedSoundBuffers->UpdateSoundBuffers();
    }

    if (notLoadingNow)
    {
        Scene::SceneInstance->m_FrameBuffer_1->_436BF0();
        Scene::SceneInstance->m_FrameBuffer_2->_436BF0();
        Scene::SceneInstance->m_FrameBuffer_1->_436040(19, 0);
        Scene::SceneInstance->m_FrameBuffer_2->_436040(29, 0);

        if (g_GfxInternal->m_RenderBufferEmpty)
            g_AssetManager->DestroySceneNodesFrameBuffers(1);
        else
            g_GfxInternal->Render(nullptr, true, -1, -1);
    }

    if (g_KapowEngine->m_SceneName.Empty())
    {
        Scene::SceneInstance->Reset();
        Scene::SceneInstance->Destroy();
        g_Progress->Enable();
        g_KapowEngine->OpenScene(g_KapowEngine->m_SceneName.m_Str);
        g_Progress->Disable();
        g_KapowEngine->m_SceneName.Clear();
        Scene::SceneInstance->Start();
    }

    return false;
}

void KapowEngineClass::_93CDA0(const char* const str)
{
    if (_A1B9F8.m_Length >= 10000)
        return;

    _A1B9F8.Append(str);
}

void ConfigVariables::LoadVariablesFile(const char* file, bool configvariables)
{
    LogDump::LogA("Loading variable file '%s'...\n", file);

    File file_(file, 1, true);
    ParseVariablesFile(&file_, configvariables);
}

#pragma message(TODO_IMPLEMENTATION)
void ConfigVariables::ParseVariablesFile(File* file, bool configvariables)
{
    file->WriteFromBuffer();
    const unsigned int filesize = file->GetPosition();
    file->_WriteBufferAndSetToStart();
    char* const buffer = (char*)MemoryManager::AllocatorsList[DEFAULT]->Allocate(filesize, NULL, NULL);
    memset(buffer, NULL, filesize);
    const int bytesread = file->Read(buffer, filesize);

    if (bytesread <= NULL)
    {
        LogDump::LogA("%d variables read.\n", 0);
        field_64 = configvariables;
        m_TotalVariables = 0;
        return;
    }

    unsigned int lineNumber = NULL;
    char* currline = strtok(buffer, "\n");
    while (currline)
    {
        // TODO: curly brackets are somehow used, haven't figured out how yet...

        if (!*currline)
            continue;

        char* eqpos = strchr(currline, '=');

        if (eqpos)
        {
            char keybuf[32] = {};
            char* quotmarkpos = strchr(eqpos + 1, '"');
            if (quotmarkpos)
                *strrchr(eqpos + 1, '"') = NULL;

            strncpy(keybuf, currline, eqpos - currline);

            m_KeyValueMap[keybuf] = quotmarkpos ? quotmarkpos + 1 : eqpos + 1;
            m_Keys[lineNumber] = keybuf;
            m_StringVariables[lineNumber] = quotmarkpos != nullptr;
        }
        else
        {
            if (strstr(currline, "#include") == currline)
            {
                char* quotmarkpos = strchr(currline, '"');

                if (quotmarkpos)
                {
                    char includename[32] = {};
                    strncpy(includename, quotmarkpos + 1, strrchr(currline, '"') - quotmarkpos);

                    LoadVariablesFile(includename, false);
                    field_64 = false;
                }
                else
                {
                    m_UnrecognizedKeys[lineNumber] = currline;
                }
            }
            else
            {
                m_UnrecognizedKeys[lineNumber] = currline;
            }
        }

        lineNumber++;
        currline = strtok(NULL, "\n");
    }

    LogDump::LogA("%d variables read.\n", lineNumber);
    field_64 = configvariables;
    m_TotalVariables = lineNumber;
}

ConfigVariables::ConfigVariables(int)
{
    MESSAGE_CLASS_CREATED(ConfigVariables);

    m_TotalVariables = NULL;
    field_64 = NULL;
}

ConfigVariables::ConfigVariables(const char* file, bool configvariables)
{
    MESSAGE_CLASS_CREATED(ConfigVariables);

    LoadVariablesFile(file, configvariables);
}

ConfigVariables::~ConfigVariables()
{
    MESSAGE_CLASS_DESTROYED(ConfigVariables);
}

bool ConfigVariables::IsVariableSet(const char* const variableName) const
{
    // FIXME: optimize this!
    const auto& s_ = m_KeyValueMap.find(variableName);
    const size_t varstrlen = strlen(variableName);

    if (s_ == m_KeyValueMap.end())
        return false;

    if (s_->first.m_Length != varstrlen)
        return false;
    else
        if (strncmp(s_->first.m_Str, variableName, varstrlen) == NULL)
            return true;
        else
            return false;
}

const bool ConfigVariables::GetParamValueBool(const char* const variableName) const
{
    if (IsVariableSet(variableName))
        return m_KeyValueMap[variableName].m_Str == "true" ? true : false;
    else
        return false;
}

const int ConfigVariables::GetParamValueInt(const char* const variableName) const
{
    if (IsVariableSet(variableName))
        return atol(m_KeyValueMap[variableName].m_Str);
    else
        return NULL;
}

const float ConfigVariables::GetParamValueFloat(const char* const variableName) const
{
    if (IsVariableSet(variableName))
        return (float)atof(m_KeyValueMap[variableName].m_Str);
    else
        return 0.f;
}

Vector2<int>& ConfigVariables::GetParamValueVector2i(Vector2<int>& outvec, const char* variableName, char delimiter) const
{
    if (!IsVariableSet(variableName))
        return outvec;

    char* const varval = m_KeyValueMap[variableName].m_Str;
    char* delimpos = nullptr;
    int* vecint = (int*)&outvec;

    // TODO: bounds check.
    while (delimpos = strtok(delimpos ? NULL : varval, (char*)&delimiter))
        *vecint++ = atoi(delimpos);

    return outvec;
}

Vector2<unsigned int>& ConfigVariables::GetParamValueVector2i(Vector2<unsigned int>& outvec, const char* variableName, char delimiter) const
{
    if (!IsVariableSet(variableName))
        return outvec;

    char* const varval = m_KeyValueMap[variableName].m_Str;
    char* delimpos = nullptr;
    int* vecint = (int*)&outvec;

    // TODO: bounds check.
    while (delimpos = strtok(delimpos ? NULL : varval, (char*)&delimiter))
        *vecint++ = atoi(delimpos);

    return outvec;
}

Vector2<float>& ConfigVariables::GetParamValueVector2f(Vector2<float>& outvec, const char* const variableName, const char delimiter) const
{
    if (!IsVariableSet(variableName))
        return outvec;

    char* const varval = m_KeyValueMap[variableName].m_Str;
    char* delimpos = nullptr;
    float* vecfl = (float*)&outvec;

    // TODO: bounds check.
    while (delimpos = strtok(delimpos ? NULL : varval, (char*)&delimiter))
        *vecfl++ = (float)atof(delimpos);

    return outvec;
}

Vector3<float>& ConfigVariables::GetParamValueVector3(Vector3<float>& outvec, const char* const variableName, const char delimiter) const
{
    if (!IsVariableSet(variableName))
        return outvec;

    char* const varval = m_KeyValueMap[variableName].m_Str;
    char* delimpos = nullptr;
    float* vecfl = (float*)&outvec;

    // TODO: bounds check.
    while (delimpos = strtok(delimpos ? NULL : varval, (char*)&delimiter))
        *vecfl++ = (float)atof(delimpos);

    return outvec;
}

Vector4f& ConfigVariables::GetParamValueVector4(Vector4f& outvec, const char* const variableName, const char delimiter) const
{
    if (!IsVariableSet(variableName))
        return outvec;

    char* const varval = m_KeyValueMap[variableName].m_Str;
    char* delimpos = nullptr;
    float* vecfl = (float*)&outvec;

    // TODO: bounds check.
    while (delimpos = strtok(delimpos ? NULL : varval, (char*)&delimiter))
        *vecfl++ = (float)atof(delimpos);

    return outvec;
}

String& ConfigVariables::GetParamValueString(String& outstr, const char* const variableName) const
{
    if (!IsVariableSet(variableName))
        return outstr;

    return (outstr = m_KeyValueMap[variableName], outstr);
}

void ConfigVariables::SetParamValue(const char* const variableName, const char* const value)
{
    if (IsVariableSet(variableName))
        m_KeyValueMap[variableName] = value;
}

void ConfigVariables::SetParamValueBool(const char* const variableName, const bool value)
{
    if (IsVariableSet(variableName))
        m_KeyValueMap[variableName] = value ? "true" : "false";
}

void InitialiseGame(LPSTR cmdline)
{
    g_KapowEngine = new KapowEngineClass();

    g_KapowEngine->Init(cmdline, NULL, nullptr, NULL);
    g_Window->Process(KapowEngineClass::UpdateGame);

    delete g_KapowEngine;
}

void EnumMaterialsInCollmat()
{
    String materialname;
    int materialproperties = NULL;
    char collmatFilename[] = "/CollMat.txt";

#ifdef INCLUDE_FIXES
    if (!File::FindFileEverywhere(collmatFilename))
        return;
#else
    // NOTE: original code doesn't check the result.
    File::FindFileEverywhere(collmatFilename);
#endif

    if (OpenCollMatFile(collmatFilename, materialname, materialproperties))
    {
        unsigned int totalMaterials = NULL;
        unsigned int deftype = 19;

        do
        {
            CollMatProperties[materialname] = materialproperties;

            materialname.ToLowerCase();

            FaceColl mattype;
            if (strstr(materialname.m_Str, "metal"))
                mattype = FACECOLL_METAL;
            else if (strstr(materialname.m_Str, "wood"))
                mattype = FACECOLL_WOOD;
            else if (strstr(materialname.m_Str, "dirt"))
                mattype = FACECOLL_DIRT;
            else if (strstr(materialname.m_Str, "stone"))
                mattype = FACECOLL_STONE;
            else if (strstr(materialname.m_Str, "bush"))
                mattype = FACECOLL_BUSH;
            else if (strstr(materialname.m_Str, "glass"))
                mattype = FACECOLL_GLASS;
            else if (strstr(materialname.m_Str, "wirefence"))
                mattype = FACECOLL_WIREFENCE;
            else if (strstr(materialname.m_Str, "grass"))
                mattype = FACECOLL_GRASS;
            else if (strstr(materialname.m_Str, "asphalt"))
                mattype = FACECOLL_ASPHALT;
            else if (strstr(materialname.m_Str, "water"))
                mattype = FACECOLL_WATER;
            else if (strstr(materialname.m_Str, "cloth"))
                mattype = FACECOLL_CLOTH;
            else if (strstr(materialname.m_Str, "allround"))
                mattype = FACECOLL_ALLROUND;
            else
                mattype = (FaceColl)deftype++;

            CollMatMaterialsTypes[materialname] = mattype;
            ++totalMaterials;

        } while (ReadAndParseCollMatMaterial(materialname, materialproperties));

        delete CollMatFile;
        LogDump::LogA("%d materials found in file\n", totalMaterials);
    }
}

void EnumFaceColMaterials()
{
    char filename[] = "/FaceColl.mat";
    if (FaceCollList.size() > 0 ||
        !File::FindFileEverywhere(filename))
        return;

    File faceColFile(filename, 1, true);
    if (!faceColFile.IsFileOpen())
        return;

    String buffer;
    while (faceColFile.ReadString(buffer))
    {
        buffer.ToLowerCase();
        FaceCollList.push_back(buffer);
    }
}

void CreateDirectoriesRecursive(char* dir)
{
    size_t slen = strlen(dir);
    if (!slen)
        return;

    if (File::IsDirectoryValid(dir))
        return;

    char* lastslash = strrchr(dir, '/');
    char* lastslashr = strrchr(dir, '\\');

    if (lastslash)
    {
        *lastslash = NULL;
        slen--;
    }

    if (lastslashr)
    {
        *lastslashr = NULL;
        slen--;
    }

    if (slen - 1 >= 0)
    {
        while (true)
        {
            if (dir[slen] == '/' || dir[slen] == '\\')
                break;

            if (--slen < 0)
            {
                File::CreateNewDirectory(dir);
                return;
            }
        }

        char tmp[MAX_PATH] = {};
        strncpy(tmp, dir, slen + 1);

        if (slen >= 0)
            CreateDirectoriesRecursive(tmp);
    }

    File::CreateNewDirectory(dir);
}

void GetInternalGameName(String& outStr)
{
#ifdef INCLUDE_FIXES
    const char devpath[] = __FILE__;
    String::ConvertBackslashes((char*)devpath);
#else
    const char devpath[] = "E:/Develop/KapowSystems/TOD1/Libs/Toolbox/Functions.cpp";
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

bool OpenCollMatFile(const char* const fileName, String& materialName, int& materialProperties)
{
    CollMatFile = new File(fileName, 1, true);

    if (CollMatFile->IsFileOpen())
        return ReadAndParseCollMatMaterial(materialName, materialProperties);
    else
        return false;
}

bool ReadAndParseCollMatMaterial(String& materialName, int& outMaterialProperties)
{
    if (!CollMatFile)
        return false;

    String buf;
    if (!CollMatFile->ReadString(buf))
        return false;

    outMaterialProperties = NULL;
    char* tok = strtok(buf.m_Str, ": \t");

    if (tok)
    {
        materialName = tok;
        char* matparams = strtok(NULL, ": \t");

        if (matparams)
        {
            while (true)
            {
                if (strcmp(matparams, "SOLID") == NULL)
                    outMaterialProperties = outMaterialProperties | 1;
                if (strcmp(matparams, "BULLETPROOF") == NULL)
                    outMaterialProperties = outMaterialProperties | 2;
                if (strcmp(matparams, "OPAQUE") == NULL)
                    outMaterialProperties = outMaterialProperties | 4;
                if (strcmp(matparams, "CAMERABLOCK") == NULL)
                    outMaterialProperties = outMaterialProperties | 8;
                if (strcmp(matparams, "TRIGGER") == NULL)
                    outMaterialProperties = outMaterialProperties | 16;
                if (strcmp(matparams, "NAVIGATIONBLOCK") == NULL)
                    outMaterialProperties = outMaterialProperties | 32;
                if (strcmp(matparams, "PHYSICS") == NULL)
                    outMaterialProperties = outMaterialProperties | 64;

                matparams = strtok(NULL, ": \t");
                if (!matparams)
                    break;
            }
        }
    }

    return true;
}

int GetCollmatMaterialId(const char* materialname)
{
    if (!materialname || !*materialname)
        return -1;

    if (!FaceCollList.size())
        return -1;

    int matid = 0;
    for (std::list<String>::const_iterator it = FaceCollList.cbegin(); it != FaceCollList.cend(); ++it, ++matid)
        if (strncmp(it->m_Str, materialname, it->m_Length) == NULL)
            return matid;

    return -1;
}

void ConfigCallback::UninitialiseGameCallback(int)
{
#ifdef INCLUDE_FIXES
    LogDump::LogA("Uninitialise game callback called!\n");
#endif
}