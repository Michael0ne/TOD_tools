#pragma once

#include "stdafx.h"
#include "KapowStringsPool.h"

template <typename T>
class List
{
	T *m_pElements;
	int m_nCurrIndex;
	int m_nCapacity;
	unsigned int m_nFlags;
public:
	//	>> 4054C0
	void			Erase();
	//	>> 405E80
	void			AdjustCapacity();
	//	>> 4061E0
	void			Insert(signed int position, T* el);
	//	>> 409DA0
	void			AdjustCapacityA();
	//	>> 409F00
	signed int		Add(String* str);
	//	>> 4395E0
	void			Empty();
};

extern List<String> * g_pList;	//	>> A3D7EC

static_assert(sizeof(List<String>) == 0x10, MESSAGE_WRONG_CLASS_SIZE("List"));