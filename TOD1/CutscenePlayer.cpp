#include "CutscenePlayer.h"

CutscenePlayer::CutscenePlayer() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(CutscenePlayer);

	field_50 = 0;
	field_54 = 1;

	//m_RegisteredRootObjects = List<int>(0x23B00);
	//m_List_2 = List<int>(0x23B00);

	m_StopEvent = {};
	m_ActiveCameraAnim = {};
	m_BeforeStartEvent = {};
	m_AfterStopEvent = {};
	m_String_5 = {};
}