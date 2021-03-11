#pragma once
#include "Node.h"

class Cloth : public Node
{
protected:
	Vector4f m_BoundingRadius;
	class Scene_Buffer* m_SceneBuffer;
	int* m_ResourceInfo;
	int field_68;
	int* field_6C;
	int* field_70;
	float m_Weight;
	float m_Damping;
	float m_WindInfluence;
	float m_Opacity;
	unsigned int m_Flags;
	int m_List_1[4];
	int m_List_2[4];
	int field_A8;
	int field_AC;
	int field_B0;

public:
	Cloth();	//	@92A7E0

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

ASSERT_CLASS_SIZE(Cloth, 180);