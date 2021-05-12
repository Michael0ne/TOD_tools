#include "MoviePlayer.h"
#include "Scene.h"
#include "AssetManager.h"
#include "Camera.h"

EntityType* tMoviePlayer;
String MoviePlayer::MovieName;
MoviePlayer* MoviePlayer::Instance;
bool MoviePlayer::MovieOpen = false;
BINK* MoviePlayer::BinkHandle = nullptr;
int MoviePlayer::StopPressedCommand = -1;
int MoviePlayer::PlayPressedCommand = -1;

String* MoviePlayer::GetResourceName(String* resname)
{
	*resname = GetMovie();
	return resname;
}

const char* MoviePlayer::GetMovie() const
{
	static String MovieName;
	
	MovieName = m_FrameInfo.m_MovieName;
	MovieName = g_AssetManager->GetResourcePathSceneRelative(MovieName.m_szString);

	return MovieName.m_szString;
}

void MoviePlayer::Play(int args)
{
	Instance = this;
	m_StreamScriptId = -1;
	m_Flags.m_FlagsBits.MovieClosed = true;

	if (Scene::SceneInstance->m_PlayMode == Scene::PlayMode::MODE_UNKNOWN_1)
		TriggerGlobalScript(PlayPressedCommand, nullptr);

	m_FrameInfo.PlayAllocated();
}

MoviePlayer::MoviePlayer() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(MoviePlayer);

	m_StreamScriptId = -1;
	m_StopEventScriptId = -1;
	m_DisplacementX = 0;
	m_ScaleX = 1;
	m_ScaleY = 1;
	m_DisplacementY = 0;
	m_Opacity = 1;
	m_FrameBuffer = nullptr;
	m_SubtitleNode = nullptr;
	m_Flags.m_FlagsBits.HasFrameBuffer = 0;
	m_Flags.m_FlagsBits.MovieClosed = 1;
	m_Volume = 0;
}

MoviePlayer::~MoviePlayer()
{
	MESSAGE_CLASS_DESTROYED(MoviePlayer);

	if (Instance == this)
	{
		LogDump::LogA("Deallocating movie\n");
		Instance = nullptr;
		MovieOpen = false;
		Scene::SceneInstance->_896BA0();
		Scene::SceneInstance->AllocateRewindBuffer();
	}

	delete m_FrameBuffer;
}

void MoviePlayer::Update()
{
	if (m_Flags.m_FlagsBits.MovieClosed)
	{
		Scene::SceneInstance->FreeRewindBuffer();
		Scene::SceneInstance->ReleaseQuadTreeAndRenderlist();

		MovieOpen = true;
		m_FrameInfo.OpenMovie();
		m_Flags.m_FlagsBits.MovieClosed = false;
	}

	if (!m_Flags.m_FlagsBits.HasFrameBuffer)
		delete m_FrameBuffer;

	m_Flags.m_FlagsBits.HasFrameBuffer = false;

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
		int streamscriptid = m_StreamScriptId;
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
		m_Flags.m_FlagsBits.HasFrameBuffer = 1;
		m_FrameInfo.ProcessFrame(m_FrameBuffer);
		m_FrameBuffer->_436BF0();
		m_FrameBuffer->_436040(23, 0);
	}

	if (m_SubtitleNode)
	{
		CameraMatrix* cammat = new CameraMatrix;
		cammat->m_Vec_1 = Camera::ActiveCameraPosition;
		cammat->m_Vec_2 = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec_1;
		cammat->m_Vec_3 = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec_2;
		cammat->m_Vec_4 = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec_3;
		cammat->m_Vec_5 = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec_4;
		cammat->m_Pos = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec_5;

		m_SubtitleNode->_88C310(cammat);
		delete cammat;
	}
}

MoviePlayer* MoviePlayer::Create(AllocatorIndex)
{
	return new MoviePlayer;
}

#pragma message(TODO_IMPLEMENTATION)
void MoviePlayer::Register()
{
	tMoviePlayer = new EntityType("MoviePlayer");
	tMoviePlayer->InheritFrom(tNode);
	tMoviePlayer->SetCreator((EntityType::CREATOR)Create);

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
		BinkHandle = nullptr;
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
	m_MovieFile = new File(m_MovieName.m_szString, 0x21, true);
	HANDLE filehnd = m_MovieFile->GetFileHandle();

	if (filehnd)
	{
		BinkHandle = BinkOpen((const char*)filehnd, BINKNOFILLIOBUF | BINKFILEHANDLE);
		if (!BinkHandle)
			BinkGetError();
	}

	m_CurrentFrameTexture = new GfxInternal_Dx9_Texture({ BinkHandle->Width, BinkHandle->Height }, 1, 4);
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