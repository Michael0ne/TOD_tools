#include "Navigator.h"
#include "ScriptDatabase.h"

#pragma message(TODO_IMPLEMENTATION)
Navigator::Navigator() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(Navigator);

	//m_PathList = List<int>(0x24300);
	//m_Unknown_2 = List<int>(0x173000);

	m_TargetPoint = Vector4f();

	m_f70 = 0.60000002f;
	m_f74 = 0.5f;
	m_PathFraction = 0.0f;
	m_MoveCtrl = 0;
	m_MoveCtrlCommand = GetCommandByName("command_run");
	m_Flags = m_Flags & 0xFFFFFFFC;
	m_ObstacleFound = 0;
	m_NextObstacleCheckTime = 0;
	m_MaxLookAhead = 4.0f;
	m_f84 = 4.0f;
	m_ObstacleCheckInterval = 500;
	m_PathFinishedEvent = -1;
}