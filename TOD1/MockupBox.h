#pragma once
#include "Node.h"

class MockupBox : public Node
{
private:
	float			m_Left;
	float			m_Right;
	float			m_Front;
	float			m_Back;
	float			m_Height;
	int*			m_ResourceInfo;	//	TODO: this is a class/struct pointer.
	int				field_68;
	float			m_Opacity;
	int				field_70;
	int				field_74;
	char			m_MetricTextureMode;
	float			m_MetricTextureMultiplier;
	int*			field_80;
	unsigned int	m_Flags;
	float			m_f88;
	float			m_f8C;
	float			m_f90;
	float			m_f94;
	int				m_List_1[4];
	int				m_List_2[4];
	int				field_B8;
	int				field_BC;
	int				field_C0;
public:
	MockupBox();	//	@8CF870

	static unsigned int TotalCreated;	//	@A3DFD8
};

ASSERT_CLASS_SIZE(MockupBox, 196);