#include "DynamicSurroundGeometry.h"

unsigned int DynamicSurroundGeometry::TotalCreated;

DynamicSurroundGeometry::DynamicSurroundGeometry() : Node(NODE_MASK_QUADTREE)
{
 MESSAGE_CLASS_CREATED(DynamicSurroundGeometry);

 m_Texture_1 = m_Texture_2 = m_Texture_3 = m_Texture_4 = nullptr;

 //m_List_1 = List<int>(0x1FB00);
 //m_List_2 = List<int>(0x1FB00);
 //m_List_3 = List<int>(0x26300);

 m_ObjectDensity = 0.02f;
 m_InitialHeight = 5.0f;
 m_WindLiftHeight = 10.0f;
 m_Radius = 25.0f;
 m_SpawnRate = 25.0f;
 m_AirResistanceXZ = 0.009f;
 m_RotResistanceXZ = 0.009f;
 field_CC = 0;
 m_Category = 0;
 m_MaxCount = 32;
 m_Size = 1.0f;
 m_Mass = 1.0f;
 m_MassSize = 1.0f;
 m_WindFactor = 1.0f;
 m_WindLift = 0.0f;
 m_AirResistanceY = 1.0f;
 m_RotResistanceY = 1.0f;
 m_ForceOffset = 1.0f;
 m_ForceRotate = 0.1f;

 m_fAC = 1.0f;
 m_fB0 = m_fB4 = m_fB8 = m_fC8 = m_f110 = 0.0f;

 ++TotalCreated;
}