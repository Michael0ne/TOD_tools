#pragma once

#include "RigidBody.h"

#define OVERDOSE_VEHICLE_CLASS_SIZE 980

class OverdoseVehicle : public RigidBody
{
protected:
	float m_LFSpring;
	float m_RFSpring;
	float m_LRSpring;
	float m_RRSpring;
	float m_f254;
	float m_f258;
	float m_f25C;
	float m_f260;
	Vector4f m_CurrentVelocity;
	float m_SpringKoefficient;
	float m_SpringDamping;
	float m_Stiffness;
	float m_RealmainbodyDefaultY;
	char m_IsBike;
	Vector4f m_FrontPointVelocity;
	Vector4f m_RearPointVelocity;
	Vector4f m_FrontPointT;
	Vector4f m_RearPointT;
	float m_RollAngleT;
	float m_TiltAngleSpeed;
	class CollisionProbe* m_GroundProbe;
	List<int>* m_HintNodeList;
	List<int> m_List_8;
	List<Vector4f> m_LastNormalList;
	List<Vector4f> m_NormalList;
	int field_308;
	List<int> m_List_5;
	int field_31C;
	List<int> m_List_4;
	float m_WheelRadius;
	int* m_DistList;
	List<int> m_List_3;
	int* m_CVList;
	List<int> m_List_2;
	int* m_CarContactList;
	List<int> m_List_1;
	float m_Tilt;
	char m_IsOutOfBounds;
	char m_IsDriveableGround;
	__int16 field_376;
	float m_AxleHeight;
	float m_Width;
	int m_LeftRearWheelMaterial;
	int field_384;
	char m_IsCppPropertiesInited;
	Node* m_RealWheelLF;
	Node* m_RealWheelLR;
	Node* m_RealWheelRF;
	Node* m_RealWheelRR;
	Node* m_Realmainbody;
	Node* m_DamageCtrl;
	Vector4f m_Vec_8;
	Vector4f m_Vec_7;
	Vector4f m_OldUpVec;
public:
	OverdoseVehicle();	//	@92D3B0
};

extern ScriptType_Entity*	tOverdoseVehicle;	//	@A3E184

static_assert(sizeof(OverdoseVehicle) == OVERDOSE_VEHICLE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(OverdoseVehicle));