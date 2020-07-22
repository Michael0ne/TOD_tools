#pragma once

#include "Node.h"

#define MEMORYCARDS_CLASS_SIZE 164

struct UniqueId
{
	int		m_Time;
	int		m_Rdtsc;

	void	Set();	//	@40FEA0
};

class MemoryCards : public Node
{
protected:
	UniqueId m_UniqueId_0;
	UniqueId m_UniqueId_1;
	UniqueId m_UniqueId_2;
	UniqueId m_UniqueId_3;
	String m_Ps2SlesLicense;
	String m_Ps2SlusLicense;
	String m_GameName;
	int m_SaveFileSize;

public:
	MemoryCards();	//	@9263B0
};

static_assert(sizeof(MemoryCards) == MEMORYCARDS_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(MemoryCards));