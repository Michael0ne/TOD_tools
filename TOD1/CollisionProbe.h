#pragma once
#include "Node.h"
#include <vector>

class CollisionProbe : public Node
{
protected:
	Vector4f	m_ResolvedPos;
	Vector4f	m_ContactPos;
	Vector4f	m_ClosestNormal;
	Node*		m_ClosestNode;
	Node*		m_RealNode;
	int			m_ClosestCollisionVolume;
	float		m_MinDistance;
	Node*		m_TouchingNodes;
	std::vector<int>	m_List_1;
	int			m_SurfaceID;
	int			m_MaterialID;
	int			field_AC;
	float		m_Angle;
	float		m_Radius;
	std::vector<Node*> m_IgnoredNodes;
	int			m_DynamicMask;
	int			m_UserMask;
	int*		field_D0;
	class EntityType* m_Nodes;
	std::vector<int>	m_List_3;
	int			m_CollisionMask;
	int			m_LineMode;
	float		m_LineThickness;
	int			m_LineWidth;
	int			m_LineHeight;
	char		field_FC;
	Vector4f	m_Unknown_1;
	int			field_110;
	int			field_114;
	Node*		m_HintNode;
	int			m_HintCollisionVolume;
	char		m_OptimisticMode;

public:
	CollisionProbe(int, float);	//	@8BA600
	CollisionProbe() {};

	void		Reset();	//	@8B61D0
	void		ClearCache();	//	@8B6130
	
	static std::vector<CollisionProbe*>	ProbesList;	//	@A3DD4C
	static unsigned int	CachedProbes;	//	@A3DE48

	static void				Register();	//	@8BA830
	static CollisionProbe*	Create();	//	@8BDEA0
};

extern EntityType* tCollisionProbe;	//	@A3DE38

ASSERT_CLASS_SIZE(CollisionProbe, 292);