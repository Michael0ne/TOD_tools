#include "MoviePlayer.h"
#include "Scene.h"
#include "AssetManager.h"
#include "Camera.h"
#include "StringType.h"
#include "IntegerType.h"
#include "NumberType.h"

EntityType* tMoviePlayer;
String MoviePlayer::MovieName;
MoviePlayer* MoviePlayer::Instance;
bool MoviePlayer::MovieOpen = false;
HANDLE MoviePlayer::BinkHandle = NULL;
int MoviePlayer::StopPressedCommand = -1;
int MoviePlayer::PlayPressedCommand = -1;

HANDLE BinkOpen(HANDLE binkHandle, const int binkOpenFlags)
{
    debug("BinkOpen not implemented!");
    return NULL;
}

int BinkGetError()
{
    debug("BinkGetError not implemented!");
    return NULL;
}

void BinkClose(const HANDLE binkHandle)
{
    debug("BinkClose not implemented!");
}

String* MoviePlayer::GetResourceName(String* resname)
{
    *resname = GetMovie();
    return resname;
}

const char* MoviePlayer::GetMovie() const
{
    static String MovieName;

    MovieName = m_FrameInfo.m_MovieName;
    g_AssetManager->GetResourcePathSceneRelative(MovieName.m_Str);

    return MovieName.m_Str;
}

void MoviePlayer::SetMovie(const char* const moviename)
{
    char movieDir[1024] = {}, movieFName[512] = {}, movieExt[16] = {};
    FileBuffer::ExtractFilePath(moviename, movieDir, movieFName, movieExt);

    String movieFilePathNoExt(movieDir);
    movieFilePathNoExt.Append(movieFName);

    char fullMovieDataPath[1024] = {};
    g_AssetManager->GetPlatformSpecificPath(fullMovieDataPath, movieFilePathNoExt.m_Str, nullptr, AssetManager::PlatformId::PC);
    strcat(fullMovieDataPath, ".bik");

    g_AssetManager->GetResourcePath(m_FrameInfo.m_MovieName, fullMovieDataPath);
}

void MoviePlayer::SetRenderSubtitleNode(int* args)
{
    m_SubtitleNode = (Node*)args[0];
}

void MoviePlayer::DeallocateStream(int* args)
{
    m_FrameInfo.Stop();
    DeallocateStream_Impl();
}

void MoviePlayer::AllocateStream(int* args)
{
    if (args)
    {
        Instance = this;
        m_StreamScriptId = args[0];
        m_Flags.MovieClosed = true;
    }
    else
    {
        Instance = this;
        m_StreamScriptId = -1;
        m_Flags.MovieClosed = true;
    }
}

void MoviePlayer::PlayWithStopEventAllocated(int* args)
{
    m_StopEventScriptId = args[0];
    m_FrameInfo.PlayAllocated();
}

void MoviePlayer::PlayWithStopEvent(int* args)
{
    Instance = this;
    m_Flags.MovieClosed = true;
    m_StreamScriptId = -1;
    m_StopEventScriptId = args[0];

    m_FrameInfo.PlayAllocated();
}

void MoviePlayer::Stop(int* args)
{
    if (Scene::SceneInstance->m_PlayMode == Scene::MODE_STOP)
        TriggerGlobalScript(StopPressedCommand, nullptr);

    m_FrameInfo.Stop();
}

void MoviePlayer::PlayAllocated(int* args)
{
    m_FrameInfo.PlayAllocated();
}

const float MoviePlayer::GetVolume() const
{
    return m_Volume;
}

void MoviePlayer::SetVolume(const float volume)
{
    m_Volume = volume;
}

const float MoviePlayer::GetScaleX() const
{
    return m_ScaleX;
}

void MoviePlayer::SetScaleX(const float scalex)
{
    m_ScaleX = scalex;
}

const float MoviePlayer::GetScaleY() const
{
    return m_ScaleY;
}

void MoviePlayer::SetScaleY(const float scaley)
{
    m_ScaleY = scaley;
}

const short MoviePlayer::GetDisplacementX() const
{
    return m_DisplacementX;
}

void MoviePlayer::SetDisplacementX(const short dispx)
{
    m_DisplacementX = dispx;
}

const short MoviePlayer::GetDisplacementY() const
{
    return m_DisplacementY;
}

void MoviePlayer::SetDisplacementY(const short dispy)
{
    m_DisplacementY = dispy;
}

const float MoviePlayer::GetOpacity() const
{
    return m_Opacity;
}

void MoviePlayer::SetOpacity(const float opacity)
{
    m_Opacity = opacity;
}

void MoviePlayer::Play(int args)
{
    Instance = this;
    m_StreamScriptId = -1;
    m_Flags.MovieClosed = true;

    if (Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_STOP)
        TriggerGlobalScript(PlayPressedCommand, nullptr);

    m_FrameInfo.PlayAllocated();
}

MoviePlayer::~MoviePlayer()
{
    MESSAGE_CLASS_DESTROYED(MoviePlayer);

    if (Instance == this)
        DeallocateStream_Impl();

    delete m_FrameBuffer;
}

void MoviePlayer::Update()
{
    if (m_Flags.MovieClosed)
    {
        Scene::SceneInstance->FreeRewindBuffer();
        Scene::SceneInstance->ReleaseQuadTreeAndRenderlist();

        MovieOpen = true;
        m_FrameInfo.OpenMovie();
        m_Flags.MovieClosed = false;
    }

    if (!m_Flags.HasFrameBuffer)
        delete m_FrameBuffer;

    m_Flags.HasFrameBuffer = false;

    if (m_FrameInfo.m_PlayingMovie)
    {
        m_FrameInfo.m_ScaleX = m_ScaleX;
        m_FrameInfo.m_ScaleY = m_ScaleY;
        m_FrameInfo.m_DisplacementX = m_DisplacementX;
        m_FrameInfo.m_DisplacementY = m_DisplacementY;
        m_FrameInfo.m_Opacity = m_Opacity;
    }
    else
    {
        int stopeventid = m_StopEventScriptId;
        if (stopeventid >= 0)
        {
            m_StopEventScriptId = -1;
            TriggerGlobalScript(stopeventid, nullptr);
        }
    }

    if (m_FrameInfo._442A70())
    {
        const int streamscriptid = m_StreamScriptId;
        if (streamscriptid >= 0)
        {
            m_StreamScriptId = -1;
            TriggerGlobalScript(streamscriptid, nullptr);
        }
    }
}

void MoviePlayer::Render()
{
    if (m_FrameInfo.m_PlayingMovie)
    {
        if (!m_FrameBuffer)
            m_FrameBuffer = new FrameBuffer(50, 6, 2);

        m_FrameBuffer->Reset();
        m_Flags.HasFrameBuffer = 1;
        m_FrameInfo.ProcessFrame(m_FrameBuffer);
        m_FrameBuffer->_436BF0();
        m_FrameBuffer->_436040(23, 0);
    }

    if (m_SubtitleNode)
    {
        CameraMatrix* cammat = new CameraMatrix;
        cammat->m_Vec[0] = Camera::ActiveCameraPosition;
        cammat->m_Vec[1] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[0];
        cammat->m_Vec[2] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[1];
        cammat->m_Vec[3] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[2];
        cammat->m_Vec[4] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[3];
        cammat->m_Vec[5] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[4];

        m_SubtitleNode->_88C310(cammat);
        delete cammat;
    }
}

MoviePlayer* MoviePlayer::Create(AllocatorIndex)
{
    return new MoviePlayer;
}

void MoviePlayer::Register()
{
    tMoviePlayer = new EntityType("MoviePlayer");
    tMoviePlayer->InheritFrom(tNode);
    tMoviePlayer->SetCreator((CREATOR)Create);

    tMoviePlayer->RegisterProperty(tSTRING, "movie", (EntityGetterFunction)&GetMovie, (EntitySetterFunction)&SetMovie, "control=resource|type=*.*");
    tMoviePlayer->RegisterProperty(tNUMBER, "volume", (EntityGetterFunction)&GetVolume, (EntitySetterFunction)&SetVolume, "control=slider|min=0|max=1");
    tMoviePlayer->RegisterProperty(tNUMBER, "scaleX", (EntityGetterFunction)&GetScaleX, (EntitySetterFunction)&SetScaleX, "control=slider|min=0|max=1");
    tMoviePlayer->RegisterProperty(tNUMBER, "scaleY", (EntityGetterFunction)&GetScaleY, (EntitySetterFunction)&SetScaleY, "control=slider|min=0|max=1");
    tMoviePlayer->RegisterProperty(tINTEGER, "displacementX", (EntityGetterFunction)&GetDisplacementX, (EntitySetterFunction)&SetDisplacementX, "control=slider|min=-1024|max=1024");
    tMoviePlayer->RegisterProperty(tINTEGER, "displacementY", (EntityGetterFunction)&GetDisplacementY, (EntitySetterFunction)&SetDisplacementY, "control=slider|min=-1024|max=1024");
    tMoviePlayer->RegisterProperty(tNUMBER, "opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1");

    tMoviePlayer->RegisterScript("play", (EntityFunctionMember)&Play, 0, 0, 0, "control=button|text=play");
    tMoviePlayer->RegisterScript("play_allocated", (EntityFunctionMember)&PlayAllocated, 0, 0, 0, "control=button|text=play_allocated");
    tMoviePlayer->RegisterScript("stop", (EntityFunctionMember)&Stop, 0, 0, 0, "control=button|text=stop");
    tMoviePlayer->RegisterScript("playwithstopevent(integer)", (EntityFunctionMember)&PlayWithStopEvent);
    tMoviePlayer->RegisterScript("playwithstopevent_allocated(integer)", (EntityFunctionMember)&PlayWithStopEventAllocated);
    tMoviePlayer->RegisterScript("allocate_stream(integer)", (EntityFunctionMember)&AllocateStream, 0, 0, 0, "control=button|text=allocate_stream");
    tMoviePlayer->RegisterScript("deallocate_stream", (EntityFunctionMember)&DeallocateStream);
    tMoviePlayer->RegisterScript("setrendersubtitlenode(entity)", (EntityFunctionMember)&SetRenderSubtitleNode);

    tMoviePlayer->PropagateProperties();

    StopPressedCommand = GetMessage("stop_pressed", true);
    PlayPressedCommand = GetMessage("play_command", true);
}

MoviePlayer::FrameInfo::FrameInfo()
{
    MESSAGE_CLASS_CREATED(FrameInfo);

    m_MovieStopped = true;
    m_ScaleX = 1;
    m_ScaleY = 1;
    m_DisplacementX = 0;
    m_DisplacementY = 0;
    m_Opacity = 1;
    m_PlayingMovie = false;
    m_CurrentFrameTexture = nullptr;
    m_MovieFile = nullptr;
}

MoviePlayer::FrameInfo::~FrameInfo()
{
    MESSAGE_CLASS_DESTROYED(FrameInfo);

    Stop();

    if (BinkHandle)
    {
        BinkClose(BinkHandle);
        BinkHandle = NULL;
    }
}

void MoviePlayer::FrameInfo::Stop()
{
    if (!m_MovieStopped)
    {
        m_MovieStopped = true;
        m_PlayingMovie = false;
    }

    delete m_CurrentFrameTexture;
    delete m_MovieFile;
}

#pragma message(TODO_IMPLEMENTATION)
void MoviePlayer::FrameInfo::ProcessFrame(FrameBuffer* fb)
{
    if (m_MovieStopped || !m_PlayingMovie)
        return;
}

void MoviePlayer::FrameInfo::OpenMovie()
{
    m_MovieFile = new FileBuffer(m_MovieName.m_Str, 0x21, true);
    HANDLE filehnd = m_MovieFile->GetFileHandle();

    if (filehnd)
    {
        BinkHandle = BinkOpen(filehnd, 0xA00000);
        if (!BinkHandle)
            BinkGetError();
    }

    m_CurrentFrameTexture = new Texture({ *(unsigned int*)BinkHandle, *(unsigned int*)((int)BinkHandle + 4) }, 1, 4);
}

bool MoviePlayer::FrameInfo::_442A70() const
{
    return true;
}

void MoviePlayer::FrameInfo::PlayAllocated()
{
    if (m_MovieStopped)
    {
        m_PlayingMovie = true;
        m_MovieStopped = 0;
    }
}