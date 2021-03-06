#pragma once

#include "ResourcesTypes.h"

#define FRAGMENT_CLASS_SIZE 24

class Fragment
{
	friend class Node;
protected:
	class Node	*m_Owner;
	ResType::Fragment* m_FragmentRes;
	int				field_8;
	char*			m_Name;
	UINT64			m_UniqueId;

public:
	Fragment(class Node* owner);	//	@87F1E0

	void			SetFragmentName(const char* fragmentname);	//	@87F090
	void			LoadResourceFile(const char* fname);	//	@87F130
	void			ApplyFragment() const;	//	@87F190
};

static_assert(sizeof(Fragment) == FRAGMENT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Fragment));