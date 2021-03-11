#pragma once
#include "Node.h"

class SignPost : public Node
{
protected:
	ScriptType_List* m_ConnectionsList;
	int	m_Unknown_1[4];
	int	m_Unknown_2[4];
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;

public:
	SignPost();	//	@920EA0

	static std::vector<SignPost*>	SignPostList;	//	@A133F0
};

ASSERT_CLASS_SIZE(SignPost, 136);