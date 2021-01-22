#include "MemoryCards.h"

ScriptType_Entity* tMemoryCards = nullptr;

MemoryCards::MemoryCards() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(MemoryCards);

	m_UniqueId_0.Set();
	m_UniqueId_1.Set();
	m_UniqueId_2.Set();
	m_UniqueId_3.Set();

	m_Ps2SlesLicense = String("00000");
	m_Ps2SlusLicense = String("00000");
	m_GameName = String("Game");

	m_SaveFileSize = 0;
}

#pragma message(TODO_IMPLEMENTATION)
void UniqueId::Set()
{
	//	TODO: simplify this?
	FILETIME filetime;
	GetSystemTimeAsFileTime(&filetime);

	m_Time = (filetime.dwLowDateTime + 0x2AC18000) / (filetime.dwHighDateTime + 0xFE624E21);
	m_Rdtsc = (int)__rdtsc();
}