#pragma once
#include "MemoryManager.h"

#define STRING_BITMASK_DEFAULT	0x80000000
#define STRING_BITMASK_SHORT	0x80000004
#define STRING_BITMASK_ONLY_SIZE 0x7FFFFFFF

class String
{
public:
	unsigned int	m_nLength;
	char*			m_szString;
	unsigned int	m_nBitMask;
	char			m_pEmpty;

	inline String() :
		m_nLength(0), m_szString(&m_pEmpty), m_nBitMask(STRING_BITMASK_SHORT), m_pEmpty(NULL)
	{}

	String(const char* const str);
	String(const String& rhs);
	String(String& rhs);
	inline ~String()
	{
		if (m_szString != &m_pEmpty && (m_nBitMask & STRING_BITMASK_DEFAULT) != NULL)
			MemoryManager::ReleaseMemory(m_szString, 0);
	}

	String& operator=(const String& _r);

	bool operator<(const String& _r) const
	{
		return m_nLength < _r.m_nLength;
	}

	bool operator<(const char* const _r) const
	{
		return m_szString < _r;
	}

	bool operator==(const String& _r) const
	{
		return strncmp(m_szString, _r.m_szString, m_nLength) == 0;
	}

	bool operator==(const char* const _r) const
	{
		return m_szString == _r;
	}

	void			Append(const char* str);
	bool			Equal(const char* const _str) const;	//	@40FE30
	String*			Substring(String* outStr, unsigned int posStart, unsigned int length);	//	@409E90
	inline bool		Empty() const
	{
		return (m_szString != &m_pEmpty ? (*m_szString != NULL && m_szString != nullptr) : (m_pEmpty != NULL));
	}
	inline void		ConvertBackslashes()
	{
		Replace('\\', '/');
	}
	inline void		ToLowerCase()
	{
		char* s_ = m_szString;
		do
			*s_ = tolower(*s_);
		while (*(s_++));
	}
	inline void		Replace(char oldChar, char newChar)
	{
		for (int ind = NULL; ind != m_nLength; ind++)
			if (m_szString[ind] == oldChar)
				m_szString[ind] = newChar;
	}

	static bool		EqualIgnoreCase(const char* str1, const char* str2, unsigned int len);	//	@4177C0
	static inline void ConvertBackslashes(char* str)
	{
		for (int ind = NULL; ind != strlen(str); ind++)
			if (str[ind] == '\\')
				str[ind] = '/';
	}
	static inline void ToLowerCase(char* str)
	{
		char* s_ = str;
		do
			*s_ = tolower(*s_);
		while (*(s_++));
	}

	inline void		Concatenate(const String& baseStr, const String& appendStr)	//	@419C50
	{
		m_nBitMask |= STRING_BITMASK_DEFAULT;
		m_nLength = baseStr.m_nLength + appendStr.m_nLength;
		
		AdjustBufferSize();
		
		strcpy_s(m_szString, m_nLength, baseStr.m_szString);
		strcat_s(m_szString, m_nLength, appendStr.m_szString);
	}

	inline bool		EndsWith(const char ch) const
	{
		return m_nLength == 0 || m_szString == nullptr || (m_szString != nullptr && *m_szString == NULL) ? false : m_szString[m_nLength - 1] == ch;
	}

	inline bool		StartsWith(const char ch) const
	{
		return m_nLength == 0 || m_szString == nullptr || (m_szString != nullptr && *m_szString == NULL) ? false : m_szString[0] == ch;
	}

	inline void		Clear()
	{
		if (!m_nLength)
			return;

		m_nLength = NULL;
		m_nBitMask = STRING_BITMASK_DEFAULT;
		delete m_szString;
		m_szString = &m_pEmpty;
		memset(&m_pEmpty, 0, sizeof(m_pEmpty));
	}
private:
	void			AllocateSpaceForString();	//	@4056E0
	void			AdjustBufferSize();	//	@405610
};

class StringTuple
{
public:
	String			m_String_1;
	String			m_String_2;

public:
	StringTuple()
		: m_String_1(), m_String_2()
	{};

	StringTuple(const char* str1, const char* str2);
};