#include "StringsPool.h"

String::String(const char* const str)
{
    m_Length = strlen(str);
    m_Flags = (STRING_BITMASK_DEFAULT ^ (m_Length + (m_Length >> 2))) & STRING_BITMASK_ONLY_SIZE ^ STRING_BITMASK_DEFAULT;
    m_Str = &m_ShortStr;
    m_ShortStr = NULL;

    if (m_Length == NULL)
        return;

    if (m_Length >= 4)
    {
        m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
        strcpy_s(m_Str, m_Length + 1, str);
    }
    else
        strcpy_s(&m_ShortStr, m_Length + 1, str);
}

String::String(const String& rhs)
{
    m_Length = rhs.m_Length;
    m_Flags = rhs.m_Flags;
    m_Str = &m_ShortStr;
    m_ShortStr = NULL;

    if (m_Length == NULL)
        return;

    if (m_Length >= 4)
    {
        m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
        strcpy_s(m_Str, m_Length + 1, rhs.m_Str);
    }
    else
        strcpy_s(&m_ShortStr, m_Length + 1, &(rhs.m_ShortStr));
}

String::String(String& rhs)
{
    m_Length = rhs.m_Length;
    m_Flags = rhs.m_Flags;
    m_Str = &m_ShortStr;
    m_ShortStr = NULL;

    if (m_Length == NULL)
        return;

    if (m_Length >= 4)
    {
        m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
        strcpy_s(m_Str, m_Length + 1, rhs.m_Str);
    }
    else
        strcpy_s(&m_ShortStr, m_Length + 1, &(rhs.m_ShortStr));
}

String& String::operator=(const String& _r)
{
    if (this == &_r)
        return *this;

    if (m_Str != &m_ShortStr && (m_Flags & STRING_BITMASK_DEFAULT) != NULL)
        MemoryManager::ReleaseMemory(m_Str, 0);

    m_Length = _r.m_Length;
    m_Flags = _r.m_Flags;
    m_Str = &m_ShortStr;
    m_ShortStr = NULL;

    if (m_Length == NULL)
        return *this;

    if (m_Length >= 4)
    {
        m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
        strcpy_s(m_Str, m_Length + 1, _r.m_Str);
    }
    else
        strcpy_s(&m_ShortStr, m_Length + 1, _r.m_Str);

    return *this;
}

String* String::Substring(String* outStr, unsigned int posStart, unsigned int length)
{
    if (posStart > m_Length) {
        outStr->m_Length = 0;
        outStr->m_Str = &outStr->m_ShortStr;
        outStr->m_Flags = STRING_BITMASK_SHORT;
        outStr->m_ShortStr = NULL;

        return outStr;
    }

    if (posStart + length <= m_Length) {
        outStr->m_Length = length;
        outStr->m_Flags = (STRING_BITMASK_DEFAULT ^ (length + (length >> 2))) & STRING_BITMASK_ONLY_SIZE ^ STRING_BITMASK_DEFAULT;
        outStr->m_ShortStr = NULL;
        outStr->m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(outStr->m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
        strncpy(outStr->m_Str, &m_Str[posStart], length);
    }
    else
    {
        outStr->m_Length = m_Length - posStart;
        outStr->m_Flags = (STRING_BITMASK_DEFAULT ^ ((m_Length - posStart) + ((m_Length - posStart) >> 2))) & STRING_BITMASK_ONLY_SIZE ^ STRING_BITMASK_DEFAULT;
        outStr->m_ShortStr = NULL;
        outStr->m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(outStr->m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
        strncpy(outStr->m_Str, &m_Str[posStart], m_Length - posStart);
    }

    return outStr;
}

void String::Append(const char* str)
{
    int _len = m_Length;
    int _len_str = strlen(str);

    m_Length = _len + _len_str;

    AdjustBufferSize();

    memcpy(&m_Str[_len], str, _len_str + 1);
}

bool String::Equal(const char* const _str) const
{
    if (m_Str == &m_ShortStr && m_Flags < 0)
        return false;

    if (m_Str == _str)
        return true;

    return strncmp(_str, m_Str, m_Length) == NULL;
}

bool String::EqualIgnoreCase(const char* str1, const char* str2, unsigned int len)
{
    return strncmp(str1, str2, len) == NULL;
}

void String::AllocateSpaceForString()
{
    if (m_Length >= 4)
    {
        m_Flags = (m_Flags ^ (m_Length + (m_Length >> 2))) & STRING_BITMASK_ONLY_SIZE ^ m_Flags;
        m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
    }
    else
    {
        m_Flags = m_Flags & STRING_BITMASK_SHORT | 4;
        m_Str = &m_ShortStr;
    }
}

void String::AdjustBufferSize()
{
    if (m_Length >= (m_Flags & STRING_BITMASK_ONLY_SIZE))
    {
        int lenOld = m_Flags & STRING_BITMASK_ONLY_SIZE;
        m_Flags = m_Flags ^ (m_Flags ^ (m_Length + (m_Length >> 1))) & STRING_BITMASK_ONLY_SIZE;

        if (m_Str == &m_ShortStr)
        {
            m_Str = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
            memcpy(m_Str, &m_ShortStr, sizeof(m_ShortStr));
        }
        else
        {
            char* str_ = (char*)MemoryManager::AllocatorsList[TEMP]->Allocate(m_Flags & STRING_BITMASK_ONLY_SIZE, NULL, NULL);
            memcpy(str_, m_Str, lenOld);

            if (m_Flags & STRING_BITMASK_DEFAULT)
                MemoryManager::ReleaseMemory(m_Str, false);

            m_Str = str_;
            m_Flags = m_Flags | STRING_BITMASK_DEFAULT;
        }
    }
}

StringTuple::StringTuple(const char* str1, const char* str2)
{
    MESSAGE_CLASS_CREATED(StringTuple);

    m_String_1 = str1;
    m_String_2 = str2;
}