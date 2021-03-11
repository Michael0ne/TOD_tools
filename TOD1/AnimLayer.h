#pragma once
#include "Node.h"

class AnimLayer : public Node
{
protected:
	int							field_50;
	int							field_54;
	int							field_58;
	int							field_5C;
	Orientation					m_Orient_1;
	Vector4f					m_Pos_1;
	Node*						m_TargetAnim_Entity;
	class ScriptType_Entity*	m_TargetAnim_Entity_2;
	unsigned int				m_Flags;
	float						m_SpeedMultiplier;
	float						m_CrossBlendFactor;
	float						m_CrossBlendSpeed;
	int							m_TargetAnim;
	float						m_Weight;
	class ScriptType_Entity*	m_MotionLayer;
	int							m_Event;
	int							m_BlendFinishedEvent;
	int							m_OverrideTarget;
	Vector4f					m_GamePivotPos;
	Vector4f					m_GamePivotPos_2;
	int							m_ClearOverrideTarget_1;
	int							field_D4;
	int							field_D8;
	int							field_DC;
	int							m_List_1[4];
	float						m_PlayPos_1;
	int							field_F4;
	int							field_F8;
	int							field_FC;
	int							field_100;
	int							field_104;
	int							m_List_2[4];
	float						m_PlayPos_2;
	int							field_11C;
	int							field_120;
	int							m_List_3[4];
	float						m_f134;
	int							field_138;
	int							field_13C;
	float						m_f140;
public:
	AnimLayer();	//	@902F90
};

ASSERT_CLASS_SIZE(AnimLayer, 324);