#include "Blocks.h"

Blocks* g_Blocks = NULL;

const char* Blocks::g_szBlockTypes[7] = //	@A11B64
{
	(const char*)0xC,
	"map",
	"submap",
	"mission",
	"cutscene",
	"playerdata",
	"",
};

void Blocks::SetSceneName(const char* szSceneName)
{
	(*(void(__thiscall*)(Blocks*, const char*))0x877F40)(this, szSceneName);
}

int Blocks::GetFreeResourceTypeListItem(unsigned int index)	//	@875540
{
	if (index + 1 >= m_ResourceTypesList.m_nCurrIndex)
		return 0;

	TypeInfo** restype = &m_ResourceTypesList.m_pElements[index + 1];
	int freeind = index + 1;

	while (!*restype) {
		restype++;
		freeind++;

		if (freeind >= m_ResourceTypesList.m_nCurrIndex)
			return 0;
	}

	return freeind;
}

void Blocks::Init(bool unk)
{
	patch(0xA3D7C4, this, 4);

	void(__thiscall * _Init)(Blocks * _this, bool _unk) = (void(__thiscall*)(Blocks*, bool))0x876E20;

	_Init(this, unk);
}