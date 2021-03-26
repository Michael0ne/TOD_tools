#pragma once
#include "EntityType.h"
#include "ResourcesTypes.h"

class Entity
{
	friend class Position;
	friend class Blocks;
	friend class Fragment;
protected:
	EntityType* m_ScriptEntity;
	unsigned char	field_8[10];
	short			m_Order;
	//	NOTE: m_Id's highest bit contains block number associated with this entity (0-6).
	int				m_Id;		//	NOTE: actual id is m_Id >> 8 - lower 3 bytes.
	int*			m_Parameters;	//	NOTE: raw array of entity parameters, seems like it.
	int				field_1C;
	int*			field_20;	//	NOTE: at field_20[1] is a pointer to ScriptThread. Maybe script thread that's attached to this entity (?)

	unsigned char	SaveScriptDataToFile_Impl(EntityType*, int, int, const char*);	//	@86B650
	unsigned char	LoadScriptDataFromFile_Impl(EntityType*, int, int);	//	@86B8B0

public:
	virtual			~Entity();	//	@86C010
	virtual void	Destroy();	//	@86C010

	Entity();	//	@86A1D0

	void* operator new (size_t size)
	{
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}

	int				GetId() const;	//	@489770

	int				GetScriptPriority() const;	//	@86C100
	void			SetScriptPriority();	//	@4A0C40

	void			SaveScriptDataToFile(int* params);	//	@86BBC0
	void			LoadScriptDataFromFile(int* params);	//	@86BC20

	void			SetScript(const EntityType*);	//	@869E20

	static void		Register();	//	@86BC70
	static Entity*	Create(AllocatorIndex);	//	@86C130
};

extern EntityType* tEntity;	//	@A3CEE0

ASSERT_CLASS_SIZE(Entity, 36);