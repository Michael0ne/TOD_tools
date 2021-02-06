#pragma once

template <typename T, typename K>
class KeyValueList
{
	struct KeyValueListElement
	{
		T				m_Element;

		KeyValueListElement*	m_Next;
		KeyValueListElement*	m_Previous;
		unsigned int			m_ElementHash;	//	TODO: i guess?
	};
protected:
	int					m_TotalEntries;
	unsigned int		m_Flags;

	T*					m_Keys;
	K*					m_Values;

public:
	KeyValueList() {};
};