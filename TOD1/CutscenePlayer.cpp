#include "CutscenePlayer.h"

EntityType* tCutscenePlayer;

CutscenePlayer::CutscenePlayer() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(CutscenePlayer);

    field_50 = 0;
    field_54 = 1;

    m_StopEvent = {};
    m_ActiveCameraAnim = {};
    m_BeforeStartEvent = {};
    m_AfterStopEvent = {};
    m_String_5 = {};
}

CutscenePlayer::~CutscenePlayer()
{
    MESSAGE_CLASS_DESTROYED(CutscenePlayer);
}

void CutscenePlayer::Pause(const int)
{
    m_Flags.Paused = !m_Flags.Paused;
}