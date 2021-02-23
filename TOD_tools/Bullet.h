#pragma once

#include "Model.h"
#include "CollisionProbe.h"
#include "Light.h"

class Bullet : public Model
{
	struct BulletShell
	{
	protected:
		Vector4f			m_RightVec;
		Vector4f			m_UpVec;
		Vector4f			m_InVec;
		Vector4f			m_OrientVec;
		Vector4f			m_Vec_5;
		float				field_50;
		float				field_54;
		ScriptType_Entity*	m_IgnoredEntity;
		char				field_5C;
		char				field_5D;
		char				field_5E;
		char				field_5F;
		int					field_60;
		int					field_64;

	public:
		BulletShell();
		BulletShell(const BulletShell& rhs);	//	@8CAFA0
	};
protected:
	float					m_Speed;
	float					m_Range;
	int						m_DamageType;
	float					m_ImpulseMultiplier;
	unsigned int			m_BulletShellsTotal;
	int*					m_SceneBufferPtr;
	List<BulletShell>		m_BulletShellsList;
	List<CollisionProbe>	m_CollisionProbesList;
	int						field_138;
	List<int>				m_List_3;
	List<int>				m_List_4;
	Light_Properties*		m_LightingProperties;
	int						field_160;
	int						field_164;

public:
	virtual ~Bullet();	//	@8C9290
	Bullet();	//	@8C9ED0

	static Bullet*			Create(AllocatorIndex);	//	@8CB280
	static void				Register();	//	@8CADB0
};

extern Bullet* tBullet;	//	@A3DFB4

ASSERT_CLASS_SIZE(Bullet, 360);