#pragma once

#include "StringsPool.h"

#define LIST_CLASS_SIZE 16

template <typename T>
class List
{
public:
	T** m_pElements;
	int m_nCurrIndex;
	int m_nCapacity;
	int m_nFlags;

public:
	List()
	{
		m_pElements = nullptr;
		m_nCurrIndex = m_nCapacity = m_nFlags = 0;
	}

	explicit inline List(unsigned int flags)
	{
		m_pElements = nullptr;
		m_nCurrIndex = m_nCapacity = 0;
		*(unsigned char*)&m_nFlags = (unsigned char)0;
		m_nFlags = m_nFlags & ((flags & 0x000FFF00) | flags);
	}

	void	AddElement(T* _el)
	{
		if (m_nCurrIndex >= m_nCapacity &&
			m_nCurrIndex + 1 > m_nCapacity) {
			m_nCapacity = m_nCurrIndex + 1 + ((m_nCurrIndex + 1) >> 1);
			(*(void(__thiscall*)(List*))0x889F70)(this);	//	::Adjust
		}

		T** _t = &m_pElements[m_nCurrIndex];
		*_t = _el;

		m_nCurrIndex++;
	}

	void	Erase()	//	@4054C0
	{
		if (!m_pElements)
			return;

		if (m_nFlags & 0x200)
			for (int ind = 0; ind < m_nCapacity; ind++)
				if (m_pElements[ind] != nullptr)
					delete m_pElements[ind];

		if (m_nFlags & 0x100)
			delete m_pElements;
	}

	void	AdjustCapacity()	//	@405E80
	{
		void(__thiscall * _AdjustCapacity)(List * _this) = (void(__thiscall*)(List*))0x405E80;

		_AdjustCapacity(this);
	}

	void	Insert(signed int position, T* el)	//	@4061E0
	{
		void(__thiscall * _Insert)(List * _this, signed int _pos, T * _el) = (void(__thiscall*)(List*, signed int, T*))0x4061E0;

		_Insert(this, position, el);
	}

	void	AdjustCapacityA()	//	@409DA0
	{
		void(__thiscall * _AdjustCapacity)(List*) = (void(__thiscall*)(List*))0x409DA0;

		_AdjustCapacity(this);
	}

	signed int	Add(String* str)	//	@853B20
	{
		signed int(__thiscall * _Add)(List* _this, String* _str) = (signed int(__thiscall*)(List*, String*))0x853B20;

		return _Add(this, str);
	}

	void	Empty()	//	@4395E0
	{
		if (m_nFlags & 0x200) {
			if (m_pElements) {
				if (*m_pElements == m_pElements[m_nCurrIndex]) {
					m_nCurrIndex = 0;
				}else{
					do {
						delete *m_pElements;

					} while (*m_pElements != m_pElements[m_nCurrIndex]);
					m_nCurrIndex = 0;
				}
			}else{
				m_nCurrIndex = 0;
			}
		}else{
			m_nCurrIndex = 0;
		}
	}
};

static_assert(sizeof(List<String>) == LIST_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(List));