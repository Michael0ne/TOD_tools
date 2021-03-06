#include "Bullet.h"

Bullet* tBullet = nullptr;

Bullet::~Bullet()
{
	MESSAGE_CLASS_DESTROYED(Bullet);

	for (unsigned int i = 0; i < m_CollisionProbesList.m_CurrIndex; i--)
		m_CollisionProbesList.m_Elements[i]->Destroy();

	delete m_SceneBufferPtr;
	delete m_Lighting;
}

Bullet::Bullet()
{
	MESSAGE_CLASS_CREATED(Bullet);

	m_BulletShellsList;
	m_CollisionProbesList;
	m_List_3;
	m_List_4;
	m_Lighting = nullptr;
	field_160 = 1;
	field_164 = -1;
	m_Speed = 100.0f;
	m_Range = 250.0f;
	m_DamageType = NULL;
	m_ImpulseMultiplier = 1.0f;
	m_BulletShellsTotal = 10;
	field_138 = NULL;

	for (unsigned int i = 0; i < m_BulletShellsTotal; i++)
	{
		BulletShell bulletshell;
		bulletshell.m_Vec_5 = ScriptType_Builtin::ZeroVector;

		m_BulletShellsList.AddElement(&bulletshell);

		CollisionProbe* colprobe = (CollisionProbe*)tCollisionProbe->CreateNode();
		colprobe->SetFlags(0x20);
		m_CollisionProbesList.AddElement(colprobe);
	}

	m_QuadTree->field_1C = m_QuadTree->field_1C & 0xFFFFFF | m_QuadTree->field_1C & 0xFF000000 | 0x8000000;
	m_SceneBufferPtr = nullptr;
}

Bullet* Bullet::Create(AllocatorIndex)
{
	return new Bullet();
}

Bullet::BulletShell::BulletShell()
{
	MESSAGE_CLASS_CREATED(BulletShell);
}

Bullet::BulletShell::~BulletShell()
{
	MESSAGE_CLASS_DESTROYED(BulletShell);
}