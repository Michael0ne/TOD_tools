#include "StringsPool.h"

String::String(const char* const str)
{
	m_nLength = strlen(str);
	m_nBitMask = (STRING_BITMASK_DEFAULT ^ (m_nLength + (m_nLength >> 2))) & STRING_BITMASK_ONLY_SIZE ^ STRING_BITMASK_DEFAULT;
	m_szString = &m_pEmpty;
	m_pEmpty = NULL;

	if (m_nLength == NULL)
		return;

	if (m_nLength >= 4)
	{
		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);

		strcpy_s(m_szString, m_nLength + 1, str);
	}
	else
		strcpy_s(&m_pEmpty, m_nLength + 1, str);
}

String::String(const String& rhs)
{
	m_nLength = rhs.m_nLength;
	m_nBitMask = rhs.m_nBitMask;
	m_szString = &m_pEmpty;
	m_pEmpty = NULL;

	if (m_nLength == NULL)
		return;

	if (m_nLength >= 4)
	{
		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
		
		strcpy_s(m_szString, m_nLength + 1, rhs.m_szString);
	}
	else
		strcpy_s(&m_pEmpty, m_nLength + 1, &(rhs.m_pEmpty));
}

String& String::operator=(const String& _r)
{
	if (m_szString != &m_pEmpty && (m_nBitMask & STRING_BITMASK_DEFAULT) != NULL)
		Allocators::ReleaseMemory(m_szString, 0);

	m_nLength = _r.m_nLength;
	m_nBitMask = _r.m_nBitMask;
	m_szString = &m_pEmpty;
	m_pEmpty = NULL;

	if (m_nLength == NULL)
		return *this;
	
	if (m_nLength >= 4)
	{
		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);

		strcpy_s(m_szString, m_nLength + 1, _r.m_szString);
	}
	else
		strcpy_s(&m_pEmpty, m_nLength + 1, _r.m_szString);

	return *this;
}

String* String::Substring(String* outStr, unsigned int posStart, unsigned int length)
{
	if (posStart > m_nLength) {
		outStr->m_nLength = 0;
		outStr->m_szString = &outStr->m_pEmpty;
		outStr->m_nBitMask = STRING_BITMASK_SHORT;
		outStr->m_pEmpty = NULL;

		return outStr;
	}

	if (posStart + length <= m_nLength) {
		outStr->m_nLength = length;
		outStr->m_nBitMask = (STRING_BITMASK_DEFAULT ^ (length + (length >> 2))) & STRING_BITMASK_ONLY_SIZE ^ STRING_BITMASK_DEFAULT;
		outStr->m_pEmpty = NULL;
		outStr->m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(outStr->m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
		strncpy(outStr->m_szString, &m_szString[posStart], length);
	}else{
		outStr->m_nLength = m_nLength - posStart;
		outStr->m_nBitMask = (STRING_BITMASK_DEFAULT ^ ((m_nLength - posStart) + ((m_nLength - posStart) >> 2))) & STRING_BITMASK_ONLY_SIZE ^ STRING_BITMASK_DEFAULT;
		outStr->m_pEmpty = NULL;
		outStr->m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(outStr->m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
		strncpy(outStr->m_szString, &m_szString[posStart], m_nLength - posStart);
	}

	return outStr;
}

void String::Append(const char* str)
{
	int _len = m_nLength;
	int _len_str = strlen(str);

	m_nLength = _len + _len_str;

	AdjustBufferSize();

	memcpy(&m_szString[_len], str, _len_str + 1);
}

bool String::Equal(const char* const _str) const
{
	//	If base string is empty.
	if (m_szString == &m_pEmpty && m_nBitMask < 0)
		return false;

	//	If strings have same memory address.
	if (m_szString == _str)
		return true;

	return strncmp(_str, m_szString, m_nLength) == NULL;
}

bool String::EqualIgnoreCase(const char* str1, const char* str2, unsigned int len)
{
	return strncmp(str1, str2, len) == NULL;
}

void String::AllocateSpaceForString()
{
	if (m_nLength >= 4)
	{
		m_nBitMask = (m_nBitMask ^ (m_nLength + (m_nLength >> 2))) & STRING_BITMASK_ONLY_SIZE ^ m_nBitMask;
		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
	}
	else
	{
		m_nBitMask = m_nBitMask & STRING_BITMASK_SHORT | 4;
		m_szString = &m_pEmpty;
	}
}

void String::AdjustBufferSize()
{
	if (m_nLength <= 3)
	{
		m_nBitMask |= STRING_BITMASK_SHORT;
		return;
	}

	int oldSize = m_nBitMask & STRING_BITMASK_ONLY_SIZE;
	m_nBitMask = m_nBitMask ^ (m_nBitMask ^ (m_nBitMask + (m_nLength >> 1))) & STRING_BITMASK_ONLY_SIZE;

	if (m_szString == &m_pEmpty)
	{
		if (Allocators::Released)
			m_szString = nullptr;
		else
			m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);

		if (m_pEmpty != NULL)
			memcpy(m_szString, &m_pEmpty, strlen((const char*)&m_pEmpty));
	}else{
		char* buf_;
		if (Allocators::Released)
			buf_ = nullptr;
		else
			buf_ = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
		memcpy(buf_, m_szString, oldSize);
		if ((m_nBitMask & STRING_BITMASK_DEFAULT) != NULL)
			Allocators::ReleaseMemory(m_szString, NULL);
		m_szString = buf_;
		m_nBitMask = m_nBitMask | STRING_BITMASK_DEFAULT;
	}
}

StringTuple::StringTuple(const char* str1, const char* str2)
{
	MESSAGE_CLASS_CREATED(StringTuple);

	m_String_1 = str1;
	m_String_2 = str2;
}