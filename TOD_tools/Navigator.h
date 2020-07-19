#pragma once

#include "Node.h"

#define NAVIGATOR_CLASS_SIZE 220

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
	List<int>					m_PathList;
	int*						field_98;
	List<int>					m_Unknown_2;
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

static_assert(sizeof(Navigator) == NAVIGATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Navigator));