#include "StringsPool.h"

String::String(const char* str)
{
	m_nLength = strlen(str);
	m_nBitMask = (STRING_BITMASK_DEFAULT ^ (m_nLength + (m_nLength >> 2))) & STRING_BITMASK_ONLY_SIZE ^ STRING_BITMASK_DEFAULT;
	m_szString = &m_pEmpty;
	m_pEmpty = NULL;

	if (m_nLength == NULL)
		return;

	if (m_nLength > 4)
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

	if (m_nLength > 4)
	{
		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate_A(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
		
		strcpy_s(m_szString, m_nLength + 1, rhs.m_szString);
	}
	else
		strcpy_s(&m_pEmpty, m_nLength + 1, &(rhs.m_pEmpty));
}

void String::operator=(const String& _r)
{
	if (m_szString != &m_pEmpty && (m_nBitMask & STRING_BITMASK_DEFAULT) != NULL)
		Allocators::ReleaseMemory(m_szString, 0);

	m_nLength = _r.m_nLength;
	m_nBitMask = _r.m_nBitMask;
	m_szString = &m_pEmpty;
	m_pEmpty = NULL;

	if (m_nLength == NULL)
		return;
	
	if (m_nLength > 4)
	{
		m_szString = (char*)Allocators::AllocatorsList[DEFAULT]->Allocate(m_nBitMask & STRING_BITMASK_ONLY_SIZE, NULL, NULL);

		strcpy_s(m_szString, m_nLength + 1, _r.m_szString);
	}
	else
		strcpy_s(&m_pEmpty, m_nLength + 1, _r.m_szString);
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

bool String::Equal(const char* _str)
{
	//	If base string is empty.
	if (m_szString == &m_pEmpty && m_nBitMask < 0)
		return false;

	//	If strings have same memory address.
	if (m_szString == _str)
		return true;

	//	If first character is null.
	if (!*m_szString)
		if (!*_str)
			return true;
		else
			return false;

	char* _base = m_szString;
	char _char = *m_szString;
	while (_char == *_str)
	{
		_char = (_base++)[1];
		_str++;

		if (!_char)
			if (!*_str)
				return true;
			else
				return false;
	}

	if (*m_szString)
		return false;

	if (!*_str)
		return true;

	return false;
}

bool String::EqualIgnoreCase(const char* str1, const char* str2, unsigned int len)
{
	if (!len)
		return true;

	char* str1_ = (char*)str1;
	char* str2_ = (char*)str2;
	unsigned int ind = 0;

	while (true)
	{
		char str1_char = *str1_;
		char str2_char = *str2_;

		++str1_;
		++str2_;
		++ind;

		if (!str1_char)
			return str2_char == 0;

		if (!str2_char || (str2_char & str1_char) & 0xDF)
			break;

		if (ind == len)
			return true;
	}

	return false;
}

void String::AllocateSpaceForString()
{
	if (m_nLength < 4)
	{
		m_nBitMask = m_nBitMask & STRING_BITMASK_SHORT | 4;
		m_szString = &m_pEmpty;
	}else{
		m_nBitMask = (m_nBitMask ^ (m_nLength + (m_nLength >> 2))) & 0x7FFFFFFF ^ m_nBitMask;

		if (Allocators::Released)
			m_szString = nullptr;
		else
			m_szString = (char*)Allocators::AllocatorsList[TEMP]->Allocate_A(m_nBitMask & 0x7FFFFFFF, NULL, NULL);
	}
}

void String::AdjustBufferSize()
{
	if (m_nLength < (int)(m_nBitMask & STRING_BITMASK_ONLY_SIZE) ||
		m_nLength < 4)
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