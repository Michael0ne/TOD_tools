#pragma once

template <typename T>
struct DictionaryNode
{
protected:
	T		m_Storage;

	DictionaryNode*	m_Next;
	DictionaryNode*	m_Previous;
	int*			m_Unknown;
};

template <typename T, typename K>
class Dictionary
{
protected:
	int				m_TotalEntries;
	unsigned int	m_Flags;

	DictionaryNode<T>*	m_Keys;
	DictionaryNode<T>	m_Values;

public:
	Dictionary();
};