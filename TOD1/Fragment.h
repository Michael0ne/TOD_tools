#pragma once
#include "FragmentAsset.h"

class Fragment
{
	friend class Node;
	friend class Folder_;
protected:
	Node           *m_Owner;
	FragmentAsset  *m_FragmentRes;
	int             field_8;
	char*           m_Name;
	UINT64          m_UniqueId;

public:
	Fragment(class Node* owner);	//	@87F1E0

	void			SetFragmentName(const char* fragmentname);	//	@87F090
	void			LoadResourceFile(const char* fname);	//	@87F130
	void			ApplyFragment() const;	//	@87F190
};

ASSERT_CLASS_SIZE(Fragment, 24);