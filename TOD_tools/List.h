#pragma once

#include "StringsPool.h"

#define LIST_CLASS_SIZE 16
#define LIST_FLAGS_CLEAR_ELEMENTS_ALL 0x100	//	NOTE: when used this de-allocates Elements pointer (doesn't de-allocate elements pointers itself).
#define LIST_FLAGS_CLEAR_ELEMENTS 0x200	//	NOTE: when used this de-allocates EACH element.
#define LIST_FLAGS_NOT_ALLOCATED 0x400	//	NOTE: when allocation for elements have failed.
#define LIST_FLAGS_DEFAULT 0x000FFF00

template <typename T>
class List
{
public:
	T**				m_Elements;
	unsigned int	m_CurrIndex;
	unsigned int	m_Capacity;
	unsigned int	m_Flags;	//	NOTE: looks like lower byte is used for allocator index when creating new elements.

public:
	List<T>()
		: m_Elements(nullptr), m_CurrIndex(NULL), m_Capacity(NULL), m_Flags(NULL)
	{}

	explicit inline List<T>(unsigned int flags, unsigned char cap = NULL)
	{
		m_Elements = nullptr;
		m_CurrIndex = m_Capacity = NULL;
		*(unsigned char*)&m_Flags = cap;
		m_Flags = m_Flags & (flags | LIST_FLAGS_DEFAULT) | flags;
	}

	void SetCapacity(unsigned int _size)	//	@851EE0
	{
		if (_size < m_CurrIndex)
		{
			if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS)
			{
				for (unsigned int ind = 0; ind < m_CurrIndex; ind++)
					if (m_Elements[ind])
						delete m_Elements[ind];
			}

			m_CurrIndex = _size;
			return;
		}

		if (_size > m_Capacity)
		{
			m_Capacity = _size;
			AdjustListCapacity();
		}

		if (m_Elements[_size] == m_Elements[m_CurrIndex])
		{
			m_CurrIndex = _size;
			return;
		}

		for (unsigned int ind = m_CurrIndex; ind < _size; ind++)
			if (m_Elements[ind])
				m_Elements[ind] = new T();

		m_CurrIndex = _size;
	}

	inline void SetCapacityAndErase(unsigned int capacity)
	{
		if (m_Capacity >= capacity)
			return;

		m_Capacity = capacity;
		m_Elements = nullptr;

		AdjustListCapacity();
	}

	inline void AddElement(T* element)
	{
		if (m_CurrIndex >= m_Capacity)
			if (m_CurrIndex + 1 > m_Capacity)
			{
				m_Capacity = (m_CurrIndex + 1) + ((m_CurrIndex + 1) >> 1);
				AdjustListCapacity();
			}

		if (m_Elements[m_CurrIndex])
			m_Elements[m_CurrIndex] = element;

		++m_CurrIndex;
	}

	void Erase()	//	@4054C0
	{
		if (!m_Elements)
			return;

		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS)
			for (unsigned int ind = NULL; ind != m_Capacity; ind++)
				if (m_Elements[ind])
					delete m_Elements[ind];

		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL)
			delete m_Elements;
	}

	void Empty()	//	@4395E0
	{
		if ((m_Flags & LIST_FLAGS_CLEAR_ELEMENTS) == NULL)
		{
			m_CurrIndex = NULL;
			return;
		}

		if (!m_Elements)
		{
			m_CurrIndex = NULL;
			return;
		}

		if (m_Elements != m_Elements[m_CurrIndex])
		{
			m_CurrIndex = NULL;
			return;
		}

		for (unsigned int ind = NULL; m_Elements[ind]; ind++)
			delete m_Elements[ind];

		m_CurrIndex = NULL;
	}

private:
	void AdjustListCapacity()	//	@889F70
	{
		void** elements = (void**)Allocators::AllocateByType((unsigned char)m_Flags, sizeof(unsigned int) * m_Capacity);
		if (!elements)
		{
			m_Flags |= LIST_FLAGS_NOT_ALLOCATED;
			return;
		}

		if (m_Elements != &m_Elements[m_CurrIndex])
		{
			for (unsigned int i = NULL; i != m_Capacity; i++)
				if (m_Elements[i])
					elements[i] = new T(*m_Elements[i]);
				else
					elements[i] = new T();
		}

		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS && m_Elements)
			for (unsigned int i = NULL; i != m_Capacity; i++)
				if (m_Elements[i])
					delete m_Elements[i];

		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL)
			delete m_Elements;

		m_Flags |= LIST_FLAGS_CLEAR_ELEMENTS_ALL;
		m_Elements = (T**)elements;
	}
};

static_assert(sizeof(List<String>) == LIST_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(List));