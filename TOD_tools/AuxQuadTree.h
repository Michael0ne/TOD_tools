#pragma once

#include "stdafx.h"
#include "Types.h"

class AuxQuadTree
{
public:
	class Entity*	m_Owner;
	int				field_4;
	int				field_8;
	AuxQuadTree*	m_FirstSibling;
	short			field_10[4];
	int				field_18;
	int				field_1C;
	int				field_20;
	int				field_24;
	int				field_28;
	int				field_2C;
	int				field_30;
	int*			m_ContactNode;
	int*			field_38;
	char			field_3C;
	char			field_3D;
	short			m_TraverseDistance;
	Vector4f		m_Position;
	char			field_4D;
	char			m_Lod;
	char			m_LodFade;
	char			m_ContactFilter;
	int				m_LodDistance;

public:
	AuxQuadTree(class Entity* owner);	//	@89F430
	~AuxQuadTree();

	void			CalculateLodForAllChildren();	//	@8A3820
	void			CalculateLod();		//	@8A24F0
	void			_8A3810(void* ptr);	//	@8A3810
	void			Refresh();	//	@8A2EE0;
};

ASSERT_CLASS_SIZE(AuxQuadTree, 88);