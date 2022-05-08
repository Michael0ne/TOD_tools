#pragma once
#include "MemoryManager.h"

#define STRING_BITMASK_DEFAULT 0x80000000
#define STRING_BITMASK_SHORT 0x80000004
#define STRING_BITMASK_ONLY_SIZE 0x7FFFFFFF

// NOTE: original file name "/Libs/Toolbox/Str.h".
class String
{
public:
    unsigned int    m_Length;
    char           *m_Str;
    unsigned int    m_Flags;
    char            m_ShortStr;

    inline String() :
        m_Length(0), m_Str(&m_ShortStr), m_Flags(STRING_BITMASK_SHORT), m_ShortStr(NULL)
    {}

    String(const char* const str);
    String(const String& rhs);
    String(String& rhs);
    inline ~String()
    {
        if (m_Str != &m_ShortStr && (m_Flags & STRING_BITMASK_DEFAULT) != NULL)
            MemoryManager::ReleaseMemory(m_Str, 0);
    }

    String& operator=(const String& _r);

    bool operator<(const String& _r) const
    {
        return operator<(_r.m_Str);
    }

    bool operator<(const char* const _r) const
    {
        //return m_Str < _r;
        return strcmp(m_Str, _r) < 0;
    }

    bool operator==(const String& _r) const
    {
        return operator==(_r.m_Str);
    }

    bool operator!=(const String& _r) const
    {
        return (_r == m_Str) == false;
    }

    bool operator==(const char* const _r) const
    {
        return strncmp(m_Str, _r, m_Length) == 0;
    }

    void   Append(const char* str);
    bool   Equal(const char* const _str) const; // @40FE30
    String* Substring(String* outStr, unsigned int posStart, unsigned int length); // @409E90
    inline bool  Empty() const
    {
        return (m_Str != &m_ShortStr ? (*m_Str != NULL && m_Str != nullptr) : (m_ShortStr != NULL));
    }
    inline void  ConvertBackslashes()
    {
        Replace('\\', '/');
    }
    inline void  ToLowerCase()
    {
        char* s_ = m_Str;
        do
            *s_ = tolower(*s_);
        while (*(s_++));
    }
    inline void  Replace(char oldChar, char newChar)
    {
        for (int ind = NULL; ind != m_Length; ind++)
            if (m_Str[ind] == oldChar)
                m_Str[ind] = newChar;
    }

    static bool  EqualIgnoreCase(const char* str1, const char* str2, unsigned int len); // @4177C0
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

    inline void  Concatenate(const String& baseStr, const String& appendStr) // @419C50
    {
        m_Flags |= STRING_BITMASK_DEFAULT;
        m_Length = baseStr.m_Length + appendStr.m_Length;

        AdjustBufferSize();

        strcpy_s(m_Str, m_Length, baseStr.m_Str);
        strcat_s(m_Str, m_Length, appendStr.m_Str);
    }

    inline bool  EndsWith(const char ch) const
    {
        return m_Length == 0 || m_Str == nullptr || (m_Str != nullptr && *m_Str == NULL) ? false : m_Str[m_Length - 1] == ch;
    }

    inline bool  StartsWith(const char ch) const
    {
        return m_Length == 0 || m_Str == nullptr || (m_Str != nullptr && *m_Str == NULL) ? false : m_Str[0] == ch;
    }

    inline void  Clear()
    {
        if (!m_Length)
            return;

        m_Length = NULL;
        m_Flags = STRING_BITMASK_DEFAULT;
        delete m_Str;
        m_Str = &m_ShortStr;
        memset(&m_ShortStr, 0, sizeof(m_ShortStr));
    }
private:
    void   AllocateSpaceForString(); // @4056E0
    void   AdjustBufferSize(); // @405610
};

class StringTuple
{
public:
    String   m_String_1;
    String   m_String_2;

public:
    StringTuple()
        : m_String_1(), m_String_2()
    {};

    StringTuple(const char* str1, const char* str2);
};