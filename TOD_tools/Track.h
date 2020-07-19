#pragma once

#include "Node.h"

#define TRACK_CLASS_SIZE 188

class Track : public Node
{
protected:
	Vector4f	m_LatestPos;
	Orientation	m_LatestOrient;
	int			field_70;
	int			field_74;
	int			field_78;
	int			field_7C;
	int			field_80;
	int			field_84;
	int			field_88;
	Orientation	m_Orient_2;
	Vector4f	m_LatestTangent;
	unsigned int m_Flags;
	float		m_Framerate;
	Node*		m_TrackPoint;
	float		m_LatestTime;

public:
	Track() : Node(NODE_MASK_POSITION)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(Track);

		m_LatestOrient = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
		m_Orient_2 = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
		m_Flags = m_Flags & 0xFFFFFFBD | 0x3D;
		field_7C = field_80 = field_84 = field_88 = field_78 = field_74 = 0;
		field_70 = ((m_Flags & 0x40) != 0) ? -1 : 1;
		m_Framerate = 1.0f;
		m_LatestTangent = Vector4f();
	}
};

static_assert(sizeof(Track) == TRACK_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Track));