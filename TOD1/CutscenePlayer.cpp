#include "CutscenePlayer.h"

CutscenePlayer::CutscenePlayer() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(CutscenePlayer);

	field_50 = 0;
	field_54 = 1;

	m_RegisteredRootObjects = List<int>(0x23B00);
	m_List_2 = List<int>(0x23B00);

	m_StopEvent = String();
	m_ActiveCameraAnim = String();
	m_BeforeStartEvent = String();
	m_AfterStopEvent = String();
	m_String_5 = String();
}