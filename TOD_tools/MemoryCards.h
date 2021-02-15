#pragma once

#include "Node.h"

struct UniqueId
{
	int		m_Time;
	int		m_Rdtsc;

	void	Set();	//	@40FEA0
};

#define MEMCARD_DEFAULT_SAVE_DIR "savegames"

class MemoryCards : public Node
{
protected:
	UniqueId		m_UniqueId_0;
	UniqueId		m_UniqueId_1;
	UniqueId		m_UniqueId_2;
	UniqueId		m_UniqueId_3;
	String			m_Ps2SlesLicense;
	String			m_Ps2SlusLicense;
	String			m_GameName;
	unsigned int	m_SaveFileSize;

	String&			MakeSaveSlotString(String& outStr, unsigned int slotind) const;	//	@926540

	static void		Register();	//	@9281B0

private:
	static MemoryCards* Create(AllocatorIndex allocInd);	//	@928F90

	unsigned int	GetSavePointSize(unsigned int memcardind, unsigned int slotind) const;	//	@926F00
	void			HasCardChanged(int* args) const;	//	@87AA10
	unsigned int	GetCardFreeSpace(unsigned int memcardind) const;	//	@928FD0
	void			IsFormatted(int* args) const;	//	@9289A0
	void			IsPresent(int* args) const;	//	@9287E0
	bool			IsPrepared(unsigned int memcardind) const;	//	@926F90
	void			LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, ScriptType_Entity* summarynode) const;	//	@926D40
	void			RestoreSavePoint(unsigned int memcardind, unsigned int slotind, ScriptType_Entity* summarynode) const;	//	@926CE0
	void			CreateSavePoint(unsigned int memcardind, unsigned int slotind, ScriptType_Entity* textbox, unsigned int, const char* const savedirectory, ScriptType_Entity* summarynode) const;	//	@926C70

public:
	MemoryCards();	//	@9263B0
	virtual ~MemoryCards();

	unsigned int	GetLastModifiedTimeAsNumber(unsigned int memcardind, unsigned int slotind) const;	//	@928090
};

extern ScriptType_Entity* tMemoryCards;	//	@A3E178

ASSERT_CLASS_SIZE(MemoryCards, 164);