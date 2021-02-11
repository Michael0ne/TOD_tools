#pragma once

template <typename T>
struct KeyValueListElement
{
	T				m_Element;

	KeyValueListElement* m_Next;
	KeyValueListElement* m_Previous;
	unsigned int			m_ElementHash;	//	TODO: i guess?
};

template <typename T, typename K>
class KeyValueList
{
protected:
	int					m_TotalEntries = NULL;
	unsigned int		m_Flags = 0xFF800000;

	T*					m_Keys;
	K*					m_Values;

public:
	KeyValueList()
	{};

	explicit KeyValueList(unsigned int elemcount)
		:	m_TotalEntries(NULL),
			m_Flags(0xFF800000 | elemcount)
	{};
};