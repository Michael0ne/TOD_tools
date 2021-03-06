#pragma once

#include "Node.h"

#define SIGNPOST_CLASS_SIZE 136

class SignPost : public Node
{
protected:
	ScriptType_List* m_ConnectionsList;
	List<int> m_Unknown_1;
	List<int> m_Unknown_2;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;

public:
	SignPost();	//	@920EA0

	static List<SignPost>&	ms_List;	//	@A133F0
};

static_assert(sizeof(SignPost) == SIGNPOST_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SignPost));