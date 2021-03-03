#include "KeyValueList.h"
#include "StringsPool.h"

template <>
bool KeyValueList<KeyValueListElement<StringTuple>, KeyValueListElement<StringTuple>>::GetValue(const char* const keyname, KeyValueListElement<StringTuple>& ret) const
{
	if (!m_TotalEntries || !keyname || !*keyname)
		return false;

	const size_t keynamelen = strlen(keyname);

	for (KeyValueListElement<StringTuple>* it = m_Keys; it; it++)
		if (strncmp(it->m_Element.m_String_1.m_szString, keyname, keynamelen) == NULL)
			return (ret = *it, true);

	return false;
}

template <>
void KeyValueList<KeyValueListElement<StringTuple>, KeyValueListElement<StringTuple>>::SetValue(const char* const keyname, const KeyValueListElement<StringTuple>& value)
{
	if (!m_TotalEntries || !keyname || !*keyname)
		return;

	const size_t keynamelen = strlen(keyname);

	for (KeyValueListElement<StringTuple>* it = m_Keys; it; it++)
		if (strncmp(it->m_Element.m_String_1.m_szString, keyname, keynamelen) == NULL)
			it->m_Element.m_String_2 = value.m_Element.m_String_2;
}

bool KeyValueList<KeyValueListElement<StringTuple>, KeyValueListElement<StringTuple>>::HasKey(const char* const keyname) const
{
	if (!m_TotalEntries || !keyname || !*keyname)
		return false;

	const size_t keynamelen = strlen(keyname);

	for (KeyValueListElement<StringTuple>* it = m_Keys; it; it++)
		if (strncmp(it->m_Element.m_String_1.m_szString, keyname, keynamelen) == NULL)
			return true;

	return false;
}

bool KeyValueList<String, String>::HasKey(const char* const keyname) const
{
	if (!m_TotalEntries || !keyname || !*keyname)
		return false;

	const size_t keynamelen = strlen(keyname);

	for (String* it = m_Keys; it; it++)
		if (strncmp(it->m_szString, keyname, keynamelen) == NULL)
			return true;

	return false;
}