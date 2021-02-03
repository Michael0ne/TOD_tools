#pragma once

#include "StringsPool.h"

#define LIST_CLASS_SIZE 16
#define LIST_FLAGS_CLEAR_ELEMENTS_ALL 0x100	//	NOTE: when used this de-allocates Elements pointer (doesn't de-allocate elements pointers itself).
#define LIST_FLAGS_CLEAR_ELEMENTS 0x200	//	NOTE: when used this de-allocates EACH element.
#define LIST_FLAGS_NOT_ALLOCATED 0x400	//	NOTE: when allocation for elements have failed.
#define LIST_FLAGS_DEFAULT 0x000FFF00

//	NOTE: there are 2 types of 'lists' used in the game. 1st - looks like STL array (elements = pointer to contiguous buffer where elements follow each other);
//		2nd type - could be STL vector (elements = array of pointers, each pointer is pointer to actual element).
template <typename T>
class List
{
public:
	T** m_Elements;
	unsigned int	m_CurrIndex;
	unsigned int	m_Capacity;
	unsigned int	m_Flags;	//	NOTE: looks like lower byte is used for allocator index when creating new elements.
	/*union
	{
		struct FlagBits
		{
			unsigned int	m_AllocatorIndex : 8;
			unsigned int	m_ClearAllElements : 1;
			unsigned int	m_ClearEachElement : 1;
			unsigned int	m_AllocationFailed : 1;
			unsigned int	m_Unused : 1;
			unsigned int	m_Unused_1 : 4;
			unsigned int	m_Unused_2 : 4;
		} m_FlagBits;
		unsigned int m_Flags;
	} m_Flags;*/

public:
	List<T>()
		: m_Elements(nullptr), m_CurrIndex(NULL), m_Capacity(NULL), m_Flags(NULL)
	{}

	explicit inline List<T>(unsigned int flags, unsigned char cap = NULL)
	{
		m_Elements = nullptr;
		m_CurrIndex = m_Capacity = NULL;
		m_Flags = (unsigned int)cap;
		m_Flags = m_Flags & (flags | LIST_FLAGS_DEFAULT) | flags;
	}

	//	NOTE: erases all elements, only to be used when initializing list first time.
	inline void SetCapacityAndErase(unsigned int capacity)
	{
		if (m_Capacity >= capacity)
			return;

		m_Capacity = capacity;
		delete[] m_Elements;

		AdjustListCapacity();
	}

	//	NOTE: always inlined.
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
			delete[] m_Elements;
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
		if (m_Flags & LIST_FLAGS_CLEAR_ELEMENTS_ALL)
		{
			if (m_Elements)
			{
				void** elements = (void**)Allocators::Realloc(m_Elements, m_Capacity * sizeof(int), false);
				m_Elements = (T**)elements;

				if (m_Capacity)
				{
					for (unsigned int i = m_CurrIndex; i < m_Capacity; i++)
						m_Elements[i] = new T();
				}
			}
			else
			{
				m_Elements = (T**)new unsigned int[m_Capacity * sizeof(int)];

				if (m_CurrIndex == NULL)
					*m_Elements = new T();
				else
					for (unsigned int i = NULL; i < m_Capacity; i++)
						m_Elements[i] = new T();
			}
		}
		else
		{
			m_Flags |= LIST_FLAGS_CLEAR_ELEMENTS_ALL;
			void** elements = (void**)new unsigned int[m_Capacity * sizeof(int)];

			if (m_CurrIndex == NULL)
				*elements = new T();
			else
				for (unsigned int i = NULL; i < m_CurrIndex; i++)
					if (m_Elements[i])
						elements[i] = new T(*m_Elements[i]);
					else
						elements[i] = new T();

			m_Elements = (T**)elements;
		}
	}
};

static_assert(sizeof(List<String>) == LIST_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(List));