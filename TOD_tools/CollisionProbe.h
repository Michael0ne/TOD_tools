#pragma once

#include "Node.h"

#define COLLISIONPROBE_CLASS_SIZE 292

class CollisionProbe : public Node
{
protected:
	Vector4f m_ResolvedPos;
	Vector4f m_ContactPos;
	Vector4f m_ClosestNormal;
	Node* m_ClosestNode;
	Node* m_RealNode;
	int m_ClosestCollisionVolume;
	float m_MinDistance;
	class ScriptType_Entity* m_TouchingNodes;
	List<int> m_List_1;
	int m_SurfaceID;
	int m_MaterialID;
	int field_AC;
	float m_Angle;
	float m_Radius;
	List<Node> m_IgnoredNodes;
	int m_DynamicMask;
	int m_UserMask;
	int* field_D0;
	class ScriptType_Entity* m_Nodes;
	List<int> m_List_3;
	int m_CollisionMask;
	int m_LineMode;
	float m_LineThickness;
	int m_LineWidth;
	int m_LineHeight;
	char field_FC;
	Vector4f m_Unknown_1;
	int field_110;
	int field_114;
	Node* m_HintNode;
	int m_HintCollisionVolume;
	char m_OptimisticMode;

public:
	CollisionProbe(int unk1, float unk2);	//	@8BA600

	void		Reset();	//	@8B61D0
	
	static List<Node>&	ms_List;	//	@A3DD4C
};

static_assert(sizeof(CollisionProbe) == COLLISIONPROBE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(CollisionProbe));