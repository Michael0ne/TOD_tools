#include "AnimLayer.h"

AnimLayer::AnimLayer() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(AnimLayer);

	m_Orient_1 = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
	field_D4 = 0;
	field_D8 = 1;
	m_PlayPos_1 = 0.0f;
	field_FC = 0;
	field_100 = 1;
	m_PlayPos_2 = 0.0f;
	m_TargetAnim = 1;
	m_BlendFinishedEvent = -1;
	m_Event = -1;
	m_Flags = m_Flags & 0xFFFFFFF8 | 8;
	m_TargetAnim_Entity = nullptr;
	m_TargetAnim_Entity_2 = nullptr;
	m_MotionLayer = 0;
	m_Pos_1 = Vector4f();
	m_Weight = 0.0f;
	m_SpeedMultiplier = 1.0f;
	m_CrossBlendFactor = 0.0f;
	m_OverrideTarget = m_OverrideTarget & 0xFFFFFFFC;
	m_GamePivotPos = Vector4f();
	m_GamePivotPos_2 = Vector4f();
	m_ClearOverrideTarget_1 = m_ClearOverrideTarget_1 & 0xFFFFFFFC;
}