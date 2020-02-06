#pragma once

#include "stdafx.h"
#include "KapowStringsPool.h"

class KapowConfig
{
private:
	BYTE m_bInitialised;
	BYTE m_nUnkInt1a[3];
	String m_sGameName;
	String m_sGameDir;
	String *m_sCurrentString;
	String *m_sSessionVariable;
	int m_nUnkInt12;
	int m_nUnkInt13;
	int m_nUnkInt14;
	BYTE m_nUnkInt15;
	BYTE m_nUnkInt15a[3];
	int m_nUnkInt16;
	int m_nUnkInt17;
	int m_nUnkInt18;
	BYTE m_nUnkInt19;
	BYTE m_nUnkInt19a[3];
	int m_nUnkInt20;
	BYTE m_bStartEditor;
	int m_nUnkInt21a;
	String m_sSceneName;
	int m_nUnkInt27;
	int m_nUnkInt28;
	int m_nUnkInt29;
	int m_nUnkInt30;
	BYTE _pad[126];
};

