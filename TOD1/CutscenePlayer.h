#pragma once
#include "Node.h"

class CutscenePlayer : public Node
{
	enum CutscenePlayerFlags
	{
		FLAG_IS_PLAYING = 0x000000FF,
		FLAG_IS_LOOPED = 0xF3000000,
		FLAG_IS_CURRENT_CAMERA_OVERRIDEN = 0xF1FFFFFF,
		FLAG_IS_DEFAULT_INHERIT = 0xFF000000
	};

protected:
	int field_50;
	int field_54;
	int m_RegisteredRootObjects[4];
	int m_List_2[4];
	float m_Speed;
	int field_7C;
	float field_80;
	float m_CamNearclip;
	unsigned int m_Flags;
	String m_StopEvent;
	float m_Timepos;
	String m_ActiveCameraAnim;
	String m_BeforeStartEvent;
	String m_AfterStopEvent;
	int field_D0;
	int field_D4;
	float m_CamDynlightsCullrange;
	String m_String_5;
	int field_EC;
	int field_F0;
public:
	CutscenePlayer();	//	@9170D0
};

ASSERT_CLASS_SIZE(CutscenePlayer, 244);