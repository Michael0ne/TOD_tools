#include "Fragment.h"

Fragment::Fragment(const Entity* owner)
{
	MESSAGE_CLASS_CREATED(Fragment);

	m_ResourceInfo = nullptr;
	field_8 = 1;
	m_pOwner = (Entity*)owner;
	(*(__int64* (__thiscall*)(__int64*))0x40FEA0)(&m_nUniqueId);
	m_szName = nullptr;
}

const char* Fragment::_GetResourcePath()
{
	return (*(const char* (__thiscall*)(void*))0x851720)(m_ResourceInfo);
}
