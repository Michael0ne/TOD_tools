#pragma once
#include "Node.h"

class Navigator : public Node
{
protected:
	Vector4f					m_TargetPoint;
	Vector4f					m_NextTargetPoint;
	float						m_f70;
	float						m_f74;
	class ScriptType_Entity*	m_MoveCtrl;
	unsigned int				m_Flags;
	float						m_MaxLookAhead;
	float						m_f84;
	int							m_PathList[4];
	int*						field_98;
	int							m_Unknown_2[4];
	int							m_PathPoint;
	float						m_PathFraction;
	int							m_ObstacleCheckInterval;
	int							m_NextObstacleCheckTime;
	char						m_ObstacleFound;
	Vector4f					m_ObstacleNormal;
	float						m_ObstacleDist;
	int							m_PathFinishedEvent;
	int							m_MoveCtrlCommand;
public:
	Navigator();	//	@91DB30
};

ASSERT_CLASS_SIZE(Navigator, 220);