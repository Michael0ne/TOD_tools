#pragma once

#include "ScriptTypes.h"

#define ENTITY_CLASS_SIZE 36

class Entity
{
protected:
	ScriptTypes::ScriptType_Entity* m_ScriptEntity;
	int field_8;
	char* m_FragmentPath;
	short field_10;
	short m_Order;
	int m_Id;
	int* field_18;
	int field_1C;
	int* field_20;

	virtual void	scalar_destructor(bool freeMemory);	//	@86C6D0

public:
	Entity();	//	@86A1D0
	~Entity();	//	@86C010

	static int		GetPropertyId(const char* prop);	//	@8732C0
	static int*		GetMessageId_A(int*, int*, const char*);	//	@8729F0
};

static_assert(sizeof(Entity) == ENTITY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Entity));