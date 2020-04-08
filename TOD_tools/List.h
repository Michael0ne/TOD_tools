#pragma once

#include "stdafx.h"
#include "StringsPool.h"

template <typename T>
class List
{
	T* m_pElements;
	int m_nCurrIndex;
	int m_nCapacity;
	unsigned int m_nFlags;
public:
	void			Erase()	//	@4054C0
	{
		void(__thiscall * _Erase)(List * _this) = (void(__thiscall*)(List*))0x405E80;

		_Erase(this);
	}

	void			AdjustCapacity()	//	@405E80
	{
		void(__thiscall * _AdjustCapacity)(List * _this) = (void(__thiscall*)(List*))0x405E80;

		_AdjustCapacity(this);
	}

	void			Insert(signed int position, T* el)	//	@4061E0
	{
		void(__thiscall * _Insert)(List * _this, signed int _pos, T * _el) = (void(__thiscall*)(List*, signed int, T*))0x4061E0;

		_Insert(this, position, el);
	}

	void			AdjustCapacityA()	//	@409DA0
	{
		void(__thiscall * _AdjustCapacity)(List*) = (void(__thiscall*)(List*))0x409DA0;

		_AdjustCapacity(this);
	}

	signed int		Add(String* str)	//	@853B20
	{
		signed int(__thiscall * _Add)(List* _this, String* _str) = (signed int(__thiscall*)(List*, String*))0x853B20;

		return _Add(this, str);
	}

	void			Empty()	//	@4395E0
	{
		void(__thiscall * _Empty)(List * _this) = (void(__thiscall*)(List*))0x4395E0;

		_Empty(this);
	}
};

extern List<String> * g_pList;	//	@A3D7EC

static_assert(sizeof(List<String>) == 0x10, MESSAGE_WRONG_CLASS_SIZE("List"));