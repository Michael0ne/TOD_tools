#include "Bullet.h"

#pragma message(TODO_IMPLEMENTATION)
Bullet::~Bullet()
{
	MESSAGE_CLASS_DESTROYED(Bullet);
}

#pragma message(TODO_IMPLEMENTATION)
Bullet::Bullet() : Model()
{
	MESSAGE_CLASS_CREATED(Bullet);

	m_List_1 = List<int>(0x21B00);
	m_List_2 = List<int>(0x21B00);
	m_List_3 = List<int>(0x1FB00);
	m_List_4 = List<int>(0x1FB00);

	field_15C = 0;
	field_160 = 1;
	field_164 = -1;
	m_Speed = 100.0f;
	m_Range = 250.0f;
	m_DamageType = 0;
	m_ImpulseMultiplier = 1.0f;
	field_110 = 10;
	field_138 = 0;
	field_114 = 0;

	//TODO: fill in List_1 and List_2 with nodes.

	m_QuadTree->m_nUserType = m_QuadTree->m_nUserType & 0xFFFFFF | m_QuadTree->m_nUserType & 0xFF000000 | 0x8000000;
}