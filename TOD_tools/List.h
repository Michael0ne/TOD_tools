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
		:m_Elements(nullptr), m_CurrIndex(NULL), m_Capacity(NULL), m_Flags(NULL)
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
			AdjustStringListCapacity();
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

		if ((m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL) == NULL)
		{
			m_Flags |= LIST_FLAGS_CLEAR_ELEMENTS_ALL;
			m_Elements = nullptr;
		}

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
			for (unsigned int ind = 0; ind < m_Capacity; ind++)
				if (m_Elements[ind] != nullptr)
					delete m_Elements[ind];

		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL)
			delete m_Elements;
	}

	void AddString(String* str)	//	@853B20
	{
		if (m_CurrIndex >= m_Capacity)
		{
			if (m_CurrIndex + 1 > m_Capacity)
			{
				m_Capacity = (m_CurrIndex + 1) + ((m_CurrIndex + 1) >> 1);
				AdjustStringListCapacity();
			}
		}

		if (m_Elements[m_CurrIndex])
			m_Elements[m_CurrIndex]->Set(str->m_szString);

		m_CurrIndex++;
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
	void AdjustStringListCapacity()	//	@405E80
	{
		void* _newspace = (T**)Allocators::AllocateByType(m_Flags & 255, 16 * m_Capacity);

		if (!_newspace)
		{
			m_Flags |= LIST_FLAGS_NOT_ALLOCATED;
			return;
		}

		if (m_Elements != m_Elements[m_CurrIndex])
		{
			for (; m_Elements != m_Elements[m_CurrIndex]; m_Elements++)
			{
				if (_newspace)
					((String**)m_Elements) = ((String*)_newspace)++;

				if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS)
					delete m_Elements;
			}
		}

		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL)
			delete m_Elements;

		m_Flags |= LIST_FLAGS_CLEAR_ELEMENTS_ALL;
		m_Elements = (T**)_newspace;
	}

	void AdjustListCapacity()	//	@889F70
	{
		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL)
		{
			if (m_Elements)
				//m_Elements = (T**)Allocators::Realloc(m_Elements, 4 * m_Capacity, false);
				m_Elements = (T**)Allocators::AllocatorsList[DEFAULT]->Realloc(m_Elements, 4 * m_Capacity, NULL, NULL);	//	FIXME: use the one above, but it needs to be fixed first!
			else
			{
				m_Elements = (T**)Allocators::AllocateByType((unsigned char)m_Flags, 4 * m_Capacity);
				if (!m_Elements)
				{
					m_Flags |= LIST_FLAGS_NOT_ALLOCATED;
					m_CurrIndex = m_Capacity = NULL;
				}
			}
		}else{
			m_Flags |= LIST_FLAGS_CLEAR_ELEMENTS_ALL;
			void* newAllocatedSpace = Allocators::AllocateByType((unsigned char)m_Flags, 4 * m_Capacity);

			if (newAllocatedSpace)
			{
				memcpy(newAllocatedSpace, m_Elements, 4 * m_CurrIndex);
				m_Elements = (T**)newAllocatedSpace;
			}else{
				m_Flags |= LIST_FLAGS_NOT_ALLOCATED;
				m_Elements = nullptr;
				m_CurrIndex = m_Capacity = NULL;
			}
		}
	}
};

static_assert(sizeof(List<String>) == LIST_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(List));