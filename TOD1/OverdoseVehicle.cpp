#include "OverdoseVehicle.h"

EntityType* tOverdoseVehicle;

#pragma message(TODO_IMPLEMENTATION)
OverdoseVehicle::OverdoseVehicle() : RigidBody()
{
	MESSAGE_CLASS_CREATED(OverdoseVehicle);

	//m_List_8 = List<int>(0x25300);
	//m_LastNormalList = List<Vector4f>(0x25300);
	//m_NormalList = List<Vector4f>(0x25300);
	//m_List_5 = List<int>(0x17300);
	//m_List_4 = List<int>(0x17300);
	//m_List_3 = List<int>(0x25300);
	//m_List_2 = List<int>(0x25300);
	//m_List_1 = List<int>(0x25300);

	m_Vec_8 = Vector4f(0.0f, 1.0f, 0.0f, 0.0f);
	m_Vec_7 = Vector4f(0.0f, 0.0f, 1.0f, 0.0f);
	m_OldUpVec = Vector4f(0.0f, 1.0f, 0.0f, 0.0f);

	m_GroundProbe = nullptr;
	m_RealWheelLF = m_RealWheelLR = m_RealWheelRF = m_RealWheelRR = nullptr;
	m_Realmainbody = nullptr;
	m_DamageCtrl = nullptr;

	m_LFSpring = m_RFSpring = m_LRSpring = m_RRSpring = 0.0f;
	m_f254 = m_f258 = m_f25C = m_f260 = 0.0f;

	m_CurrentVelocity = {};

	m_SpringKoefficient = 0.0f;
	m_IsBike = false;
	m_SpringDamping = 0.0f;
	m_Stiffness = 0.0f;
	m_RealmainbodyDefaultY = 0.0f;

	m_FrontPointVelocity = {};
	m_RearPointVelocity = {};
	m_FrontPointT = {};
	m_RearPointT = {};

	m_RollAngleT = 0.0f;
	m_TiltAngleSpeed = 0.0f;

	//m_List_8.m_CurrIndex = 0;
	//m_LastNormalList.m_CurrIndex = 0;
	//m_NormalList.m_CurrIndex = 0;
	//m_List_3.m_CurrIndex = 0;
	//m_List_2.m_CurrIndex = 0;
	//m_List_1.m_CurrIndex = 0;

	m_WheelRadius = 0.0f;

	m_IsOutOfBounds = false;
	m_IsDriveableGround = false;
	m_IsCppPropertiesInited = false;

	m_Tilt = 0.0f;
	m_AxleHeight = 0.0f;
	m_Width = 0.0f;
}