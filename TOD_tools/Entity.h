#pragma once

#include "ScriptTypes.h"

#define ENTITY_CLASS_SIZE 36

struct Entity_fld20
{
	int				field_0;
	ScriptTypes::ScriptType_Entity* m_ScriptEntity;
};

class Entity
{
protected:
	ScriptTypes::ScriptType_Entity* m_ScriptEntity;
	int				field_8;
	char*			m_FragmentPath;
	short			field_10;
	short			m_Order;
	int				m_Id;
	int*			field_18;
	int				field_1C;
	Entity_fld20*	field_20;

	unsigned char	SaveScriptDataToFile_Impl(MemoryCards*, int, int, const char*);	//	@86B650
	unsigned char	LoadScriptDataFromFile_Impl(MemoryCards*, int, int);	//	@86B8B0

public:
	virtual void	scalar_destructor(bool freeMemory);	//	@86C6D0
	virtual void	Destroy();	//	@86C010

	Entity();	//	@86A1D0
	~Entity();	//	@86C010

	int				GetId();	//	@489770

	int				GetScriptPriority();	//	@86C100
	void			SetScriptPriority();	//	@4A0C40

	void			SaveScriptDataToFile(int* params);	//	@86BBC0
	void			LoadScriptDataFromFile(int* params);	//	@86BC20

	static int		GetPropertyId(const char* prop);	//	@8732C0
	static int*		GetMessageId_A(int*, int*, const char*);	//	@8729F0
};

static_assert(sizeof(Entity) == ENTITY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Entity));