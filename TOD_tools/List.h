#pragma once

#include "StringsPool.h"

#define LIST_CLASS_SIZE 16

template <typename T>
class List
{
public:
	T** m_pElements;
	unsigned int m_nCurrIndex;
	unsigned int m_nCapacity;
	unsigned int m_nFlags;

public:
	List()
		:m_pElements(nullptr), m_nCurrIndex(NULL), m_nCapacity(NULL), m_nFlags(NULL)
	{}

	explicit inline List(unsigned int flags)
	{
		m_pElements = nullptr;
		m_nCurrIndex = m_nCapacity = NULL;
		m_nFlags = NULL & ((flags & 0x000FFF00) | flags);
	}

	void	SetCapacity(unsigned int _size)	//	@851EE0
	{
		if (_size < m_nCurrIndex)
		{
			if (m_nFlags & 0x200)	//	LIST_FLAG_CLEAR_ELEMENTS
			{
				for (unsigned int ind = 0; ind < m_nCurrIndex; ind++)
					if (m_pElements[ind])
						delete m_pElements[ind];
			}

			m_nCurrIndex = _size;
			return;
		}

		if (_size > m_nCapacity)
		{
			m_nCapacity = _size;
			AdjustCapacity();
		}

		if (m_pElements[_size] == m_pElements[m_nCurrIndex])
		{
			m_nCurrIndex = _size;
			return;
		}

		for (unsigned int ind = m_nCurrIndex; ind < _size; ind++)
			if (m_pElements[ind])
				m_pElements[ind] = new T();

		m_nCurrIndex = _size;
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
			for (unsigned int ind = 0; ind < m_nCapacity; ind++)
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

	void	Add(String* str)	//	@853B20
	{
		if (m_nCurrIndex >= m_nCapacity)
		{
			if (m_nCurrIndex + 1 > m_nCapacity)
			{
				m_nCapacity = (m_nCurrIndex + 1) + ((m_nCurrIndex + 1) >> 1);
				AdjustCapacity();
			}
		}

		if (m_pElements[m_nCurrIndex])
			m_pElements[m_nCurrIndex]->Set(str->m_szString);

		m_nCurrIndex++;
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