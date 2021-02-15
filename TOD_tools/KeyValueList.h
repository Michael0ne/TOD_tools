#pragma once

template <typename T>
struct KeyValueListElement
{
	T					m_Element;

	KeyValueListElement* m_Next;
	KeyValueListElement* m_Previous;
	unsigned int		m_ElementHash;	//	TODO: i guess?
};

template <typename K, typename V>
class KeyValueList
{
protected:
	int					m_TotalEntries = NULL;
	unsigned int		m_Flags = 0xFF800000;

	K*					m_Keys;
	V*					m_Values;

public:
	KeyValueList();
	~KeyValueList();

	KeyValueList(unsigned int elemcount)
		:	m_TotalEntries(NULL),
			m_Flags(0xFF800000 | elemcount)
	{};

	bool				HasKey() const;
	const V&			GetValue(const K&) const;
};